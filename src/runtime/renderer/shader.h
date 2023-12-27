//
// Created by Arjo Nagelhout on 27/12/2023.
//

#ifndef SHAPEREALITY_RENDERER_SHADER_H
#define SHAPEREALITY_RENDERER_SHADER_H

#include "graphics/shader.h"
#include "graphics/render_pipeline_state.h"
#include <memory>

namespace renderer
{
	class Shader
	{
	public:
		explicit Shader(graphics::IDevice* pDevice, graphics::IShaderLibrary* pShaderLibrary, std::string const& vertexEntryPoint, std::string const& fragmentEntryPoint);
		~Shader();

		//
		[[nodiscard]] graphics::IRenderPipelineState* getRenderPipelineState() const;

	private:
		std::unique_ptr<graphics::IRenderPipelineState> pRenderPipelineState;
	};
}

#endif //SHAPEREALITY_RENDERER_SHADER_H
