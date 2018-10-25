#pragma once
#include "RenderDevice.h"
namespace Angine {
	namespace Renderer {
		class VertexArray {

		public:
			inline VertexArray(RenderDevice& _device , RenderDevice::BufferUse use) 
				:device(&_device),
				vaID()
			{
			//device->createVertexArray()
			};

		private:
			RenderDevice *device;
			uint32 vaID;
			

		};
	}
}