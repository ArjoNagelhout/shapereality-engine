//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_PIPELINE_H
#define BORED_ENGINE_PIPELINE_H

#include "renderer.h"

namespace renderer
{
	class Pipeline;

	class PipelineImplementation
	{
	public:
		explicit PipelineImplementation(Pipeline* pipeline);
		virtual ~PipelineImplementation();

	protected:
		Pipeline* pPipeline;
	};

	class Pipeline : public RendererObject
	{
	public:
		explicit Pipeline();
		~Pipeline() override;

		void onRendererBackendChanged(RendererBackendType const& rendererBackendType) override;

	private:
		std::unique_ptr<PipelineImplementation> pImplementation;
	};
}

#endif //BORED_ENGINE_PIPELINE_H
