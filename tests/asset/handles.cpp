//
// Created by Arjo Nagelhout on 08/04/2024.
//


#include <gtest/gtest.h>

#include <renderer/mesh.h>
#include <scene/scene.h>

namespace handles_test
{
    template<typename Type>
    struct Provider
    {
        static constexpr std::string_view fileExtension = "asset";
    };

    template<>
    struct Provider<scene::Scene>
    {
        static constexpr std::string_view fileExtension = "scene";
    };

    TEST(Asset, Handles)
    {
        std::cout << "Mesh extension: " << Provider<renderer::Mesh_>::fileExtension << std::endl;
        std::cout << "Scene extension: " << Provider<scene::Scene>::fileExtension << std::endl;
    }
}