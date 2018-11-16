#pragma once
#include "Model.h"

namespace Angine
{
		class ModelFactory
		{
		public:
			ModelFactory() = delete;
			~ModelFactory() = delete;
			static	Model* ModelFactory::createSphere(float radius, unsigned int rings, unsigned int sectors);
			static Model* ModelFactory::createPlane();


		private:

		};

}