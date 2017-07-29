#pragma once
#include "Loader.h"
#include "../common.h"
#include "Mesh.h"
#include "../renderer/VertexData.h"
#include "../renderer/Mesh.h"
#include "../Utils/Tiny_obj_loader.h"
#include "Texture2D.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TextureManager.h"

namespace Angine {
	namespace Renderer {

		
		class Loader
		{
		public:

			Loader()
			{
			};
			~Loader();
		public:
			Model* loadModelFromFile(const std::string& fileloc);
			Mesh * loadToVao(const std::vector<VertexData>& data, const std::vector<unsigned>& indicies);

			void processNode(aiNode *node, const aiScene *scene, Model* model);
			Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Texture2D> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
				TextureType typeName);
		private:
			GLuint createVao();
			void Loader::createVertexBuffer(const std::vector<VertexData>& data);
			GLuint createIndicesBuffer(const std::vector < unsigned int>& indices);
			std::vector<GLuint> m_vaos;
			std::vector<GLuint> m_vbos;
			std::string	directory;

			void unbindVao();
		};
	}
}