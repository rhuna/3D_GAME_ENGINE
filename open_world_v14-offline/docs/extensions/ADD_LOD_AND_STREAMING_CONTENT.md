# Add LOD + Streaming Content

## For a large detailed prop
1. Export the high-detail Blender model
2. Export at least one cheaper LOD model
3. Put the models under a stable asset path
4. Register those paths in the prefab or content entry
5. Assign a material budget tier
6. Place the prefab inside a scene chunk
7. Test culling and chunk loading behavior

## For a chunked region
1. Define chunk ids for the region
2. Assign scene placements to chunk cells
3. Add travel anchors between major chunk boundaries if needed
4. Validate chunk density and budget usage
5. Test load, unload, and restore
