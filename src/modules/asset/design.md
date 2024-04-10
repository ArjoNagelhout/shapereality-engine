# Asset system design

In order to determine the design of the asset system,
we should identify the way it gets used in practice.

While considering all the edge cases and workflows up-front
will be impossible, identifying some sort of usage patterns
will reduce development time. 

## Loading assetHandles

There are two ways how an asset can be loaded in the engine.
1. explicitly by the user in a call to some function `ImportResult loadAsset(std::string const& identifier)`
2. by the engine itself through deserializing a Scene file (Scenes also function as Prefabs)
   and importing any of the asset identifiers it has referenced.

## Stages

There are two stages for getting assetHandles into the engine
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
      "MeshRendererComponent"
   ],
   "data": "example_scene.scene.bin"
}
```

`size` = amount of entities in the scene

`components` = which components have been serialized

## MeshRenderer

```c++
// asset identifier. Could be a hash or a string to the path
using asset_id = std::string;

struct MeshRendererComponent
{
    asset_id meshAssetId; // reference to which mesh should be loaded
    Asset<Mesh> mesh; // the asset handle to the actual mesh, not serialized, only used on runtime
};
```

## Loading and unloading of assetHandles
```c++

// called on loading the component (i.e. added to the entity registry)
void load(MeshRendererComponent& component)
{
    // we should instruct the asset database to load the asset (increase the use count to one)
    // the asset database will return a handle Asset<T>, which contains a pointer to the data
    // and whether it has been loaded yet
    component.mesh = AssetDatabase::get<Mesh>(component.meshAssetId);
}

// called on unloading the component (i.e. removed from the entity registry)
void unload(MeshRendererComponent& component)
{
    component.mesh.release(); // releases the handle and reduces use count similar to shared_ptr
}

void render(MeshRenderComponent& component)
{
    if (component.mesh.isLoaded())
   {
        // use mesh data
   }
}
```

We could use a `std::shared_ptr<AssetHandle<T>>` that points to whether the asset is loaded.

That way, we don't have to reinvent the behaviour of a shared pointer. Whenever the last shared pointer is deleted, we 
deallocate the asset data. 

Because `Mesh`, `Texture` and other engine types adheres to RAII (resource acquisition is initialization), we don't have 
to manually clean up anything when the last `std::shared_ptr` goes out of scope or its `.release()` method gets called.

One important detail is that resource loading is done asynchronously, and this asynchronous operation should be 
cancellable, so we should not directly store a `Mesh` or equivalent type inside the `std::shared_ptr`.   

Therefore, we introduce the type `AssetHandle`, which gets stored inside the `std::shared_ptr`:

```c++

// AssetHandle contains whether a given type is instantiated
template<typename Type>
class AssetHandle final
{
public:
    // starts loading
    explicit AssetHandle(asset_id assetId, std::function<void()> onLoaded)
    {
        // begin async loading of the asset
    }
    
    // stops loading
    ~AssetHandle()
    {
        asset.release(); // not necessary, but more explicit
    }
    
    enum class State
    {
        Uninitialized,
        Loading,
        Error,
        Success,
    };

    [[nodiscard]] State getState() const
    {
        return state;
    }
    
    [[nodiscard]] std::string getError() const
    {
        assert(state == Error && "why are you reading an error if there is no error silly");
        return error;
    }
    
    Type& get()
    {
        assert(state == Success && "check before you get, i.e. check the date on a milk carton, otherwise you might drink spoiled milk");
        return *asset;
    }
    
    Type* tryGet()
    {
        if (state == Success)
        {
            return asset.get();           
        }
        
        return nullptr;
    }

private:
    asset_id assetId;
    State state;
    std::unique_ptr<Type> asset;
    std::string error; // change into proper error format
};

template<typename Type>
using Asset = std::shared_ptr<AssetHandle<T>>;

```

Alternatively, if we don't want to store handles, we manually increase and decrease use count. 
This makes it possible to not have to store the asset handle inside an engine struct such as `MeshRendererComponent`.

Otherwise, the engine's renderer needs to know about the `asset` module. Whereas we want to have the dependency be the 
other way around. The renderer is simply a backend for the scene. 

But Asset<Mesh> requires knowing the renderer specific type. 

# Design

## `AssetId`

Importing an `Input file` generates a list of `Artifacts`.  
An `Artifact` can be referenced via a unique `AssetId`, which has the following structure:

`sub_directory/another_sub_directory/input_file_name.fbx#artifact_name.artifact_extension`

### Hashing
In order to support an arbitrarily deeply nested directory structure, `AssetId` is hashed in the following manner:

`AssetId` consists of two parts that get hashed separately, so that the original input directory path can be
resolved from the hash. 

1. `Input file path` (relative to any `Input directory`)
2. `Artifact` name and file extension

## `Input directory` and `Load directory`

Importing an `Input file` can be achieved by placing it inside a `Input directory`.

The produces artifacts are stored inside a `Load directory`.

When the user calls `getAsset(AssetId id)`, the artifact will be loaded from this `Load directory`. If it does
not exist there, or is outdated, it will be imported from the relevant `Import directory`.

```c++
AssetHandle getAsset(AssetId id);
```

To determine whether an `Input file` is outdated, either a filewatcher can be used, or file hashes can be compared. 

## `AssetHandle`

When calling `getAsset(AssetId id)`, we return an `AssetHandle`. This acts as a `std::shared_ptr`, meaning that
when all instances of the asset are destroyed, the asset gets unloaded from disk.

We want to support preloading assetHandles before they are used inside a scene. This could be achieved through two ways:

1. Use reference counting and simply increase the use count
2. Load assetHandles inside the main scene, and put them to `not visible`. 
   - downsides:
     - we might want to unload assetHandles that are not visible anymore. 

# Auxiliary files and deduplication

Importers for certain input file types can import auxiliary files
with it.

For example: a .obj file might reference a .mtl file.
Certain auxiliary files (such as an .mtl file) can also be imported
individually.

Do we want to deduplicate beforehand? Or do we want to calculate a file hash?
I think the latter is cleanest