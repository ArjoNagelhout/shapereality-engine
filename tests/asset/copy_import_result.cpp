//
// Created by Arjo Nagelhout on 10/04/2024.
//

#include <gtest/gtest.h>

#include <asset/import_registry.h>

using namespace asset;

namespace copy_import_result
{
    struct ImportResultData1
    {
        std::vector<std::filesystem::path> paths;
    };

    using ImportResult1 = common::ValueResult<ImportResultData1>;

//    void copy(ImportResultData result)
//    {
//
//    }

    void copy(ImportResult1 result)
    {
        for (auto& path: result.get().paths)
        {
            std::cout << path << std::endl;
        }
    }

    [[nodiscard]] ImportResult1 get()
    {
        ImportResultData1 a{};
        {
            std::filesystem::path a1 = "path1_get_something";
            std::filesystem::path a2 = "path2_get_another_path";
            std::filesystem::path a3 = "path3_get_this_string_is_longer";
            a.paths.emplace_back(a1);
            a.paths.emplace_back(a2);
            a.paths.emplace_back(a3);
        }
        //ImportResult1 result =
        return ImportResult1::makeSuccess(std::move(a));
    }

    // apparently copying an import result gives a segfault.
    // it crashes on the destructor of a filesystem::path
    TEST(Asset, CopyImportResult)
    {
        //ImportResult result = ImportResult::makeSuccess(ImportResultData{});
        //copy(result);

        ImportResultData1 a{
            .paths = {
                "path1",
                "path2",
                "path3"
            }
        };
        ImportResult1 result = ImportResult1::makeSuccess(std::move(a));
        copy(result);

        ImportResult1 result2 = get();
        copy(result2);

        std::cout << "do something" << std::endl;
    }
}