#pragma once
#include "RenderDevice.h"
#include "Shader.h"
#include "VertexArray.h"
namespace Angine {
	using namespace Renderer;
	class RenderContext {
	public:
		inline RenderContext(RenderDevice& _device):device(&_device) {}
		inline void clear(bool clearColor, bool clearDepth);
		inline void clear(const Color& color , bool clearDepth);
		inline void draw(Shader& shader, VertexArray& vertexArray,
			const RenderDevice::DrawInfo& drawParams, uint32 numInstances = 1);
		inline void draw(Shader& shader, VertexArray& vertexArray,
			const RenderDevice::DrawInfo& drawParams, uint32 numInstances,
			uint32 numIndices);

		inline void draw(const Shader& shader , Model& model);
	private :
		RenderDevice * device;
};

	inline void RenderContext::draw( const Shader& shader , Model& model) {
	
		device->draw(shader.getId() , model);
	}


}  // namespace Angine
