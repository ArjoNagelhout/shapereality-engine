//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_PIPELINE_H
#define BORED_ENGINE_PIPELINE_H

#include "graphics.h"

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

	class Pipeline : public GraphicsObject
	{
	public:
		explicit Pipeline();
		~Pipeline() override;

		void onGraphicsBackendChanged(GraphicsBackendType const& rendererBackendType) override;

	private:
		std::unique_ptr<PipelineImplementation> pImplementation;
	};
}

#endif //BORED_ENGINE_PIPELINE_H
