//
// Created by Arjo Nagelhout on 30/01/2024.
//

#ifndef SHAPEREALITY_RENDER_GRAPH_H
#define SHAPEREALITY_RENDER_GRAPH_H

#include <vector>
#include <string>

#include "graphics/device.h"

namespace rendering
{
    struct RenderPassNode
    {
        std::vector<RenderPassNode> dependencies;

        // output is specified in a render pass descriptor?
    };

    using RenderPassId = std::string;

    // a render graph is responsible for executing render passes
    // and optimally creating and destroying resources + synchronisation
    // primitives for the GPU.
    class RenderGraph
    {
    public:
        explicit RenderGraph(graphics::IDevice* device);

        ~RenderGraph();

        // step 1: set up all render passes
        void addRenderPass(RenderPassId id);

        // step 2: compile the render passes
        // culls unused render passes
        // sort based on required order (using topological sort)
        // calculates lifetimes of resources
        void compile();

        // step 3: executes the graph in
        // order determined by step 2,
        // creates and destroys resources when needed
        void execute();

    private:
        std::vector<RenderPassNode> nodes;

        graphics::IDevice* device;
    };
}

#endif //SHAPEREALITY_RENDER_GRAPH_H
