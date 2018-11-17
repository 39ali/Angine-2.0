#pragma once
#include "Model.h"
#include <assimp\Importer.hpp>     
#include <assimp\scene.h>      
#include <assimp\postprocess.h>
#include "RenderDevice.h"
namespace Angine
{
		class ModelFactory
		{
		public:
			ModelFactory() = delete;
			~ModelFactory() = delete;
			static	Model* ModelFactory::createSphere(float radius, unsigned int rings, unsigned int sectors);
			static Model* ModelFactory::createPlane();
			static bool loadModel( RenderDevice& renderDevice, const std::string& filename , Model& model, std::string& textureloc=std::string());
		
		private:
		
		};

}