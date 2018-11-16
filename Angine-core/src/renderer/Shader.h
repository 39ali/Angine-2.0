#pragma once
#include "RenderDevice.h"

namespace Angine {



		class Shader
		{
		public:
			inline Shader(RenderDevice& _device, const std::string& filepath) :
				device(&_device), programID(device->createShaderProgram(filepath))
			{};

			inline ~Shader() {
				device->deleteShaderProgram(programID);
			}
			//template<typename T>
			//inline void setUniform<T>(void* data)
			//{
			//	//TODO : implement this 
			//}
			//TODO set sampler
			inline uint32 getId()const ;
		private:
			RenderDevice * device;
			uint32 programID;
		};


		inline uint32 Shader::getId()const
		{
			return programID;
			
		}
		



	
}