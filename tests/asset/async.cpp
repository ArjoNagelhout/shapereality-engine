//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <chrono>
#include <iostream>

namespace async_testing
{
    struct AssetId
    {
        size_t inputFile{};
        size_t artifact{};
    };

    [[nodiscard]] bool operator==(AssetId const& lhs, AssetId const& rhs)
    {
        return lhs.inputFile == rhs.inputFile && lhs.artifact == rhs.artifact;
    }

    std::ostream& operator<<(std::ostream& ostream, AssetId const& assetId)
    {
        ostream << "{ inputFile: " << assetId.inputFile << ", artifact: " << assetId.artifact << " }";
        return ostream;
    }
}

template<>
struct std::hash<async_testing::AssetId>
{
    [[nodiscard]] size_t operator()(async_testing::AssetId const& id) const
    {
        return id.inputFile ^ id.artifact;
    }
};

namespace async_testing
{
    using ImportFunction = std::function<std::vector<AssetId>()>;

    std::vector<AssetId> importFbx(size_t inputFile)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::vector<AssetId> result;
        for (size_t i = 0; i < 20; i++)
        {
            result.emplace_back(AssetId{.inputFile = inputFile, .artifact = i});
        }
        return result;
    }

    enum class AssetType
    {
        None = 0,
        Texture,
        Mesh
    };

    class AssetHandle
    {
    public:
        enum class Status
        {
            None,
            Loading,
            Success,
            Error
        };

        AssetId assetId{};
        std::atomic<Status> status = Status::None; // because status gets set by a different thread, it needs to be atomic.
        AssetType type = AssetType::None;
    };

    using Asset = std::shared_ptr<AssetHandle>;

    // when an asset handle has been created / exists inside the assets unordered_map,
    // this means that it is being loaded or loaded, so we don't need to perform any
    // additional operations, except for subscribing to its onComplete event.
    class AssetDatabase
    {
    public:
        // we don't call it load because it won't always have to load, sometimes it already exists in memory
        [[nodiscard]] std::shared_ptr<AssetHandle> get(AssetId assetId)
        {
            std::weak_ptr<AssetHandle>& handle = handles[assetId];
            if (!handle.expired())
            {
                std::cout << "not expired: " << assetId << std::endl;
                return handle.lock();
            }

            std::cout << "new handle for: " << assetId << std::endl;
            std::shared_ptr<AssetHandle> result = std::make_shared<AssetHandle>();
            handle = result;
            importFile(assetId.inputFile);
            return result;
        }

        void importFile(size_t inputFile)
        {
            // check if already importing:
            if (imports.contains(inputFile))
            {
                // we don't need to do anything
                std::cout << "already importing: " << inputFile << std::endl;
                return;
            }

            // start async import if asset is currently loading
            std::cout << "import input file: " << inputFile << std::endl;
            std::future<std::vector<AssetId>> future = std::async(std::launch::async, importFbx, inputFile);
            imports.emplace(inputFile, std::move(future));
        }

    private:
        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> handles;
        std::unordered_map<size_t, std::future<std::vector<AssetId>>> imports;
    };

    class Scene
    {
    public:
        void render()
        {
            for (auto& asset: assets)
            {
                if (asset->status == AssetHandle::Status::Success)
                {

                }
            }
        }

    private:
        std::vector<Asset> assets; // assets to render
    };

    TEST(Asset, AsyncTesting)
    {
        AssetDatabase db;

        Asset a = db.get({0, 0});
        Asset b = db.get({0, 1});
        Asset c = db.get({0, 2});
        Asset d = db.get({0, 2});
    }
}