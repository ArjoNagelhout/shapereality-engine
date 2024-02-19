# Asset system design

In order to determine the design of the asset system,
we should identify the way it gets used in practice.

While considering all the edge cases and workflows up-front
will be impossible, identifying some sort of usage patterns
will reduce development time. 

## Loading assets

There are two ways how an asset can be loaded in the engine.
1. explicitly by the user in a call to some function `ImportResult loadAsset(std::string const& identifier)`
2. by the engine itself through deserializing a Scene file (Scenes also function as Prefabs)
   and importing any of the asset identifiers it has referenced

## Stages

There are two stages for getting assets into the engine
1. **Import**. Takes a source file or source directory in combination with import settings and converts it 
   into the engine's native format. This engine's native format comprises binary data and .json metadata. 
   This native format can be cached on disk for later retrieval. 
2. **Load**. Imports the engine's native format

## Asset identifiers

Assets are identified by a hash, calculated from their path.

## Example of import settings

## Example of scene file

`example_scene.scene`
`example_scene.scene.bin`

```json
{
   "name": "Example Scene",
   "size": 100,
   "components": [
      "HierarchyComponent",
      "TransformComponent",
      "LocalToWorldComponent",
      "MeshRenderer"
   ],
   "data": "example_scene.scene.bin"
}
```

`size` = amount of entities in the scene

`components` = which components have been serialized