#pragma once
#include "../common.h"
#include "RenderDevice.h"
#include "../Utils/imageLoader.h"

namespace Angine {


		class Texture
		{
		public:
			Texture() {};
			~Texture() {};
		
			uint32 width,height;
			uint32 texId;
			
		};
	
}