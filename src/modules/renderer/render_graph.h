//
// Created by Arjo Nagelhout on 30/01/2024.
//

#ifndef SHAPEREALITY_RENDER_GRAPH_H
#define SHAPEREALITY_RENDER_GRAPH_H

#include <vector>

namespace renderer
{
    struct RenderPassNode
    {
        std::vector<RenderPassNode> dependencies;

        // output is specified in a render pass descriptor?
    };

    // a render graph is responsible for
    class RenderGraph
    {
    public:
        explicit RenderGraph();

        ~RenderGraph();

        // step 1: set up all render passes
        void addRenderPass();

        // step 2: compile the render passes
        // culls unused render passes
        // sort based on required order
        // calculates lifetimes of resources
        void compile();

        // step 3: executes the graph in
        // order determined by step 2,
        // creates and destroys resources when needed
        void execute();

    private:
        std::vector<RenderPassNode> nodes;
    };
}

#endif //SHAPEREALITY_RENDER_GRAPH_H
