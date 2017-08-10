#pragma once
#include "Model.h"

namespace Angine
{
	namespace Renderer {
		class ModelFactory
		{
		public:
			ModelFactory() = delete;
			~ModelFactory() = delete;
			static	Model* ModelFactory::createSphere(float radius, unsigned int rings, unsigned int sectors);



		private:

		};


	}
}