//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_set>

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
    using ImportResult = std::vector<AssetId>;

    class AssetDatabase;

    ImportResult importFbx(AssetDatabase& db, size_t inputFile);

    enum class AssetType
    {
        None = 0,
        Texture,
        Mesh
    };

    class AssetHandle
    {
    public:
        explicit AssetHandle(AssetId assetId_) : assetId(assetId_)
        {
        }

        enum class Status
        {
            None,
            Loading,
            Success,
            Error
        };

        AssetId assetId;
        std::atomic<Status> status = Status::None; // because status gets set by a different thread, it needs to be atomic.
        AssetType type = AssetType::None;
    };

    struct ImportHandle
    {
        std::future<ImportResult> future;
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
            // if a handle already has been created, we can assume its import function has already been called,
            // so we can safely return the handle
            std::weak_ptr<AssetHandle>& weakHandle = handles[assetId];
            if (!weakHandle.expired())
            {
                std::cout << "not expired: " << assetId << std::endl;
                return weakHandle.lock();
            }

            std::cout << "new handle for: " << assetId << std::endl;
            std::shared_ptr<AssetHandle> handle = std::make_shared<AssetHandle>(assetId);
            weakHandle = handle; // update weak handle

            importFor(assetId);

            return handle;
        }

        void onImportComplete(size_t inputFile)
        {
            std::lock_guard<std::mutex> guard(importsMutex);
            imports.erase(inputFile);
        }

    private:
        void importFor(AssetId assetId)
        {
            std::lock_guard<std::mutex> guard(importsMutex);

            size_t inputFile = assetId.inputFile;
            if (imports.contains(inputFile))
            {
                std::cout << "already importing: " << inputFile << std::endl;
                return;
            }

            std::cout << "import input file: " << inputFile << std::endl;

            std::future<ImportResult> future = std::async(std::launch::async, importFbx, std::ref(*this), inputFile);
            imports.emplace(inputFile, ImportHandle{std::move(future)});
        }

        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> handles;
        std::unordered_map<size_t, ImportHandle> imports;
        std::mutex importsMutex;
    };

    struct Cleaner
    {
        explicit Cleaner(AssetDatabase& db_, size_t inputFile_) : db(db_), inputFile(inputFile_) {}

        ~Cleaner()
        {
            //db.onImportComplete(inputFile);
        }

        AssetDatabase& db;
        size_t inputFile;
    };

    ImportResult importFbx(AssetDatabase& db, size_t inputFile)
    {
        Cleaner cleaner(db, inputFile);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ImportResult result;
        for (size_t i = 0; i < 5; i++)
        {
            result.emplace_back(AssetId{.inputFile = inputFile, .artifact = i});
        }
        return result;
    }

    TEST(Asset, AsyncTesting)
    {
        AssetDatabase db;

        Asset a = db.get({0, 0});
        Asset b = db.get({0, 1});
        Asset c = db.get({0, 2});
        Asset d = db.get({0, 2});
    }
}