#include "editor/serialization/ExportPipeline.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <ctime>

#include "core/FileSystem.h"
#include "editor/serialization/SceneExporter.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "ecs/World.h"

#ifdef _WIN32
extern "C" __declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(
    void *hModule,
    char *lpFilename,
    unsigned long nSize);
static constexpr unsigned long FW_MAX_PATH = 260;
#endif

namespace fs = std::filesystem;

namespace fw {
namespace {

std::string SanitizeFileName(std::string text) {
    for (char& c : text) {
        const bool ok = (c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') ||
                        c == '_' || c == '-';
        if (!ok) c = '_';
    }
    return text;
}

bool CopyDirectoryContents(const fs::path& source, const fs::path& destination) {
    std::error_code ec;
    if (!fs::exists(source, ec) || !fs::is_directory(source, ec)) {
        return false;
    }

    fs::create_directories(destination, ec);
    if (ec) return false;

    for (fs::recursive_directory_iterator it(source, ec), end; it != end; it.increment(ec)) {
        if (ec) return false;

        const fs::path relative = fs::relative(it->path(), source, ec);
        if (ec) return false;
        const fs::path target = destination / relative;

        if (it->is_directory()) {
            fs::create_directories(target, ec);
            if (ec) return false;
            continue;
        }
        if (!it->is_regular_file()) {
            continue;
        }

        fs::create_directories(target.parent_path(), ec);
        if (ec) return false;
        fs::copy_file(it->path(), target, fs::copy_options::overwrite_existing, ec);
        if (ec) return false;
    }

    return true;
}

bool CopyFileIfExists(const fs::path& source, const fs::path& destination) {
    std::error_code ec;
    if (!fs::exists(source, ec) || !fs::is_regular_file(source, ec)) {
        return false;
    }
    fs::create_directories(destination.parent_path(), ec);
    if (ec) return false;
    fs::copy_file(source, destination, fs::copy_options::overwrite_existing, ec);
    return !ec;
}

std::string CurrentTimestampText() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tmValue {};
#ifdef _WIN32
    localtime_s(&tmValue, &time);
#else
    localtime_r(&time, &tmValue);
#endif
    char buffer[64] {};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tmValue);
    return buffer;
}

std::string ResolveExecutablePath()
{
#ifdef _WIN32
    char buffer[FW_MAX_PATH]{};
    const unsigned long length = GetModuleFileNameA(nullptr, buffer, FW_MAX_PATH);
    if (length > 0 && length < FW_MAX_PATH)
    {
        return std::string(buffer, length);
    }
#elif defined(__linux__)
    std::error_code ec;
    const fs::path resolved = fs::read_symlink("/proc/self/exe", ec);
    if (!ec && !resolved.empty())
    {
        return resolved.string();
    }
#endif
    return {};
}

std::vector<std::string> AssetRootsToCopy(bool includeSaves) {
    std::vector<std::string> roots {
        "ambience", "builds", "debug", "dialogue", "editor", "encountergraphs", "encounters",
        "enemies", "exports", "factions", "foundation", "game", "hud", "interiors", "itemgraphs",
        "items", "links", "loot", "menus", "models", "music", "npcs", "prefab_variants", "prefabs",
        "profiles", "projects", "questgraphs", "quests", "regions", "registry", "reports", "routines",
        "runtime", "scenes", "schedules", "shaders", "ship", "template", "templates", "ui", "world"
    };
    if (includeSaves) {
        roots.push_back("saves");
    }
    return roots;
}

} // namespace

ExportBundleResult ExportPipeline::StageBundle(const ExportBundleSettings& settings, const World& world, const PrefabLibrary& prefabs) {
    ExportBundleResult result {};
    result.outputFolder = settings.outputFolder;

    if (settings.buildId.empty() || settings.outputFolder.empty()) {
        result.message = "Export staging requires both a buildId and an outputFolder.";
        return result;
    }

    const fs::path root = fs::path(settings.outputFolder);
    const fs::path normalizedRoot = root.lexically_normal();
    const std::string normalizedRootText = normalizedRoot.generic_string();
    if (normalizedRootText == "assets" || normalizedRootText.rfind("assets/", 0) == 0) {
        result.message = "Choose an outputFolder outside the assets tree, such as exports/my_build.";
        return result;
    }
    const fs::path assetsOut = root / "assets";
    const fs::path buildOut = root / "build";
    const fs::path binOut = root / "bin";
    const fs::path docsOut = root / "docs";

    if (!FileSystem::EnsureDirectory(root.string()) ||
        !FileSystem::EnsureDirectory(assetsOut.string()) ||
        !FileSystem::EnsureDirectory(buildOut.string()) ||
        !FileSystem::EnsureDirectory(binOut.string()) ||
        !FileSystem::EnsureDirectory(docsOut.string())) {
        result.message = "Failed to create export staging folders.";
        return result;
    }

    for (const std::string& assetRoot : AssetRootsToCopy(settings.includeSaves)) {
        const fs::path source = fs::path("assets") / assetRoot;
        if (!fs::exists(source)) {
            continue;
        }
        const fs::path destination = assetsOut / assetRoot;
        if (!CopyDirectoryContents(source, destination)) {
            result.message = "Failed to copy asset folder: " + source.string();
            return result;
        }
        result.copiedRoots.push_back(assetRoot);
    }

    CopyFileIfExists("assets/engine.cfg", assetsOut / "engine.cfg");
    CopyFileIfExists("assets/game.project", assetsOut / "game.project");

    const fs::path runtimeScenePath = assetsOut / "scenes" / (settings.buildId + std::string{"_runtime_export.scene"});
    if (!SceneExporter::ExportWorldAsScene(world, prefabs, settings.startScene.empty() ? settings.buildId : settings.startScene, runtimeScenePath.string())) {
        result.message = "Failed to export the current runtime scene into the staged bundle.";
        return result;
    }
    result.exportedScenePath = runtimeScenePath.string();

    std::ostringstream launch;
    launch << "buildId=" << settings.buildId << "\n";
    launch << "gameTitle=" << settings.gameTitle << "\n";
    launch << "version=" << settings.version << "\n";
    launch << "startScene=" << settings.startScene << "\n";
    launch << "startRegion=" << settings.startRegion << "\n";
    launch << "profileId=" << settings.profileId << "\n";
    launch << "menuId=" << settings.menuId << "\n";
    launch << "hudId=" << settings.hudId << "\n";
    launch << "runtimeExportScene=assets/scenes/" << settings.buildId << "_runtime_export.scene\n";
    launch << "includeSaves=" << (settings.includeSaves ? "true" : "false") << "\n";
    launch << "compressExport=" << (settings.compressExport ? "true" : "false") << "\n";
    launch << "generatedAt=" << CurrentTimestampText() << "\n";
    FileSystem::WriteTextFile((buildOut / (settings.buildId + std::string{".launchcfg"})).string(), launch.str());

    std::ostringstream manifest;
    manifest << "V114 Export Bundle\n";
    manifest << "Build Id: " << settings.buildId << "\n";
    manifest << "Game Title: " << settings.gameTitle << "\n";
    manifest << "Version: " << settings.version << "\n";
    manifest << "Start Scene: " << settings.startScene << "\n";
    manifest << "Start Region: " << settings.startRegion << "\n";
    manifest << "Profile: " << settings.profileId << "\n";
    manifest << "Menu: " << settings.menuId << "\n";
    manifest << "HUD: " << settings.hudId << "\n";
    manifest << "Runtime Scene Export: " << result.exportedScenePath << "\n";
    manifest << "Included Asset Roots: ";
    for (std::size_t i = 0; i < result.copiedRoots.size(); ++i) {
        manifest << result.copiedRoots[i];
        if (i + 1 < result.copiedRoots.size()) manifest << ", ";
    }
    manifest << "\n";
    manifest << "Generated: " << CurrentTimestampText() << "\n";
    result.manifestPath = (buildOut / (settings.buildId + std::string{".manifest.txt"})).string();
    FileSystem::WriteTextFile(result.manifestPath, manifest.str());

    const std::string exePath = ResolveExecutablePath();
    if (!exePath.empty()) {
        const fs::path exeSource(exePath);
        const fs::path exeDest = binOut / exeSource.filename();
        if (CopyFileIfExists(exeSource, exeDest)) {
            result.executablePath = exeDest.string();
        }
    }

    const std::string launcherName = "Launch_" + SanitizeFileName(settings.gameTitle.empty() ? settings.buildId : settings.gameTitle) + ".bat";
    result.launcherPath = (root / launcherName).string();
    std::ostringstream launcher;
    launcher << "@echo off\n";
    launcher << "setlocal\n";
    launcher << "set ROOT=%~dp0\n";
    launcher << "set EXE=%ROOT%bin\\3D_GAME_ENGINE.exe\n";
    launcher << "if not exist \"%EXE%\" (\n";
    launcher << "  echo Could not find the exported executable in the bin folder.\n";
    launcher << "  echo Put your built 3D_GAME_ENGINE.exe in %ROOT%bin and run this launcher again.\n";
    launcher << "  pause\n";
    launcher << "  exit /b 1\n";
    launcher << ")\n";
    launcher << "pushd %ROOT%\n";
    launcher << "start \"" << settings.gameTitle << "\" \"%EXE%\"\n";
    launcher << "popd\n";
    FileSystem::WriteTextFile(result.launcherPath, launcher.str());

    result.packageScriptPath = (root / "Package_Export.bat").string();
    std::ostringstream pack;
    pack << "@echo off\n";
    pack << "setlocal\n";
    pack << "set ROOT=%~dp0\n";
    pack << "set ZIP_NAME=" << SanitizeFileName(settings.buildId) << "_package.zip\n";
    pack << "powershell -NoProfile -ExecutionPolicy Bypass -Command \"Compress-Archive -Path '%ROOT%assets','%ROOT%bin','%ROOT%build','%ROOT%docs','%ROOT%" << launcherName << "' -DestinationPath '%ROOT%" << SanitizeFileName(settings.buildId) << "_package.zip' -Force\"\n";
    pack << "if errorlevel 1 (\n";
    pack << "  echo Packaging failed.\n";
    pack << "  pause\n";
    pack << "  exit /b 1\n";
    pack << ")\n";
    pack << "echo Wrote %ROOT%" << SanitizeFileName(settings.buildId) << "_package.zip\n";
    FileSystem::WriteTextFile(result.packageScriptPath, pack.str());

    std::ostringstream readme;
    readme << "V114 Export Bundle\n\n";
    readme << "1. Copy or build 3D_GAME_ENGINE.exe into the bin folder if it is not already there.\n";
    readme << "2. Launch the game with " << launcherName << ".\n";
    readme << "3. Run Package_Export.bat to zip the staged export on Windows.\n";
    readme << "4. The launch config is in build/" << settings.buildId << ".launchcfg\n";
    readme << "5. The exported runtime scene snapshot is in assets/scenes/" << settings.buildId << "_runtime_export.scene\n";
    FileSystem::WriteTextFile((docsOut / "EXPORT_README.txt").string(), readme.str());

    result.success = true;
    result.message = "Export bundle staged at " + root.string();
    return result;
}

} // namespace fw
