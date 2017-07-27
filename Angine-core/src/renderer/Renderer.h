#pragma once
#include "RawModel.h"
namespace Angine {
	namespace Renderer {

		class Renderer
		{
		public:
			Renderer() {};
			~Renderer() {};

			void render(const RawModel* model)const;


		};





	}
}
