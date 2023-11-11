//
// Created by Arjo Nagelhout on 07/11/2023.
//

#ifndef BORED_ENGINE_ASSETS_H
#define BORED_ENGINE_ASSETS_H

#include <filesystem>
#include <vector>

std::string const METADATA_FILE_EXTENSION = ".meta";

// there's two types of assets:
// during game creation time
// and after creating the game binary.

// a base class all asset types can implement (meshes, shaders)
// an asset is a file inside the project with additional metadata
//
// this additional metadata is stored alongside the source file in a .meta file
// this .meta file can contain import settings (e.g. resolution for images or which axis orientation to use)
class Asset
{
public:
    Asset();
    virtual ~Asset();

    // path of the source file, without the .meta file extension
    std::filesystem::path sourcePath;

    

    [[nodiscard]] std::filesystem::path getMetadataPath() const;

    [[nodiscard]] bool isLoaded() const;

//    [[nodiscard]] bool isLoading() const;

    // synchronous operations, these load the assets into memory in the native engine formats
    void load();
    void unload();

    // asynchronous operations
//    void loadAsync(std::function<void()> const& onComplete); // loads the asset into memory
//    void unloadAsync(std::function<void()> const& onComplete);

    // thumbnail
    int getThumbnail();

    // metadata (serializes)
    void saveMetadata();

private:

    // generates a preview image and caches it on disk
    int generateThumbnail();
};

// a base class that is able to import a specific file format and convert it to a format that is used
// by the engine
// this engine format is cached on disk and loaded into memory whenever placed in the scene
// or when previewing
class Importer
{

};

class Mesh : Asset
{

};

class Texture : Asset
{

};

class Shader : Asset
{

};

class Animation : Asset
{

};

class Scene : Asset
{

};

class Prefab : Asset
{

};

// manages all assets of a project
class Project
{
public:
    Project(std::filesystem::path const& projectDirectory); // open project, loads all assets in that project
    ~Project(); // close project, removes all assets from working memory

    // checks if any files have been added to the project directory and loads them
    // .meta files get added automatically
    void refresh();

    void clearCache();

private:

    // filepaths
    std::filesystem::path const& projectDirectory;
    std::filesystem::path const& assetsDirectory;
    std::filesystem::path const& cacheDirectory; // cache is stored inside the project folder in a hidden

    // contains loaded assets
    std::vector<Asset> assets;
};

#endif //BORED_ENGINE_ASSETS_H
