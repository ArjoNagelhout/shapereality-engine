//
// Created by Arjo Nagelhout on 30/01/2024.
//

#include "render_graph.h"

namespace renderer
{
    RenderGraph::RenderGraph(graphics::IDevice* device)
        : device(device)
    {

    }

    RenderGraph::~RenderGraph() = default;

    void RenderGraph::addRenderPass(RenderPassId id)
    {

    }

    void RenderGraph::compile()
    {
        // topological sort
    }

    void RenderGraph::execute()
    {
        // submit to device
    }
}