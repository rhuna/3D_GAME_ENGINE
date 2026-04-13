# V58 Asset Pipeline Upgrade

V58 adds a central asset registry so prefab and scene content can reference assets by stable IDs instead of raw file paths.

## New content pattern

```ini
useModel=true
mesh=starter_house
lod0=starter_house
lod1=starter_house_low
lod1Distance=25
lod2Distance=60
```

## Registry pattern

```ini
asset=starter_house;type=model;path=assets/models/buildings/starter_house.glb
asset=starter_house_low;type=model;path=assets/models/buildings/starter_house_low.glb
asset=starter_house_albedo;type=texture;path=assets/textures/buildings/starter_house.png
```

## Benefits

- Simpler asset authoring
- Safer folder reorganization
- Better validation before runtime
- Cleaner caching
- LOD-ready render data
- Backward compatibility with `modelPath=`
