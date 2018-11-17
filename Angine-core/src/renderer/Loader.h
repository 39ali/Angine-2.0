//#pragma once
//#include "Loader.h"
//#include "../common.h"
//#include "Mesh.h"
//#include "../renderer/VertexData.h"
//#include "../renderer/Mesh.h"
//#include "../Utils/Tiny_obj_loader.h"
//#include "Texture2D.h"
//#include "Model.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include "TextureManager.h"
//
//namespace Angine {
//	namespace Renderer {
//
//
//		class Loader
//		{
//
//		public:
//			static Model* loadModelFromFile(const std::string& fileloc);
//			static Model* loadModelFromVertices(const std::vector<VertexData>& data, const std::vector<unsigned>& indices);
//			static void clean();
//		private:
//			static	Mesh * loadToVao(const std::vector<VertexData>& data, const std::vector<unsigned>& indicies);
//			static void processNode(aiNode *node, const aiScene *scene, Model* model);
//			static Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
//			static std::vector<Texture2D*>* loadMaterialTextures(aiMaterial *mat, aiTextureType type,
//				TextureType typeName);
//
//			Loader()
//			{
//			};
//			~Loader();
//			static	GLuint createVao();
//			static void Loader::createVertexBuffer(const std::vector<VertexData>& data);
//			static GLuint createIndicesBuffer(const std::vector < unsigned int>& indices);
//			static std::vector<GLuint> m_vaos;
//			static std::vector<GLuint> m_vbos;
//			static std::string	directory;
//
//			static void unbindVao();
//		};
//	}
//}