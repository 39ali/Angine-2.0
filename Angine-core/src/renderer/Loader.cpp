#include "Loader.h"

namespace Angine {
	namespace Renderer {

		std::vector<GLuint> Loader::m_vaos;
		std::vector<GLuint> Loader::m_vbos;
		std::string	Loader::directory;



		Mesh* Loader::loadToVao(const std::vector<VertexData>& data, const std::vector<unsigned>& indices)
		{
			GLuint vaoID = createVao();
			m_vaos.push_back(vaoID);

			createVertexBuffer(data);
			GLuint vio = createIndicesBuffer(indices);
			m_vbos.push_back(vio);
			unbindVao();
			return new Mesh(vaoID, indices.size(), vio);
		}


		Model* Loader::loadModelFromVertices(const std::vector<VertexData>& data, const std::vector<unsigned>& indices)
		{
			Model * model = new Model();

			Mesh* mesh = loadToVao(data, indices);
			model->m_meshes.push_back(mesh);
			return model;
		}

		Model* Loader::loadModelFromFile(const std::string& fileloc)
		{
			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(fileloc, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return nullptr;
			}
			directory = fileloc.substr(0, fileloc.find_last_of('/'));
			directory += "/";
			Model * model = new Model();

			processNode(scene->mRootNode, scene, model);
			return model;
		}

		void Loader::processNode(aiNode *node, const aiScene *scene, Model* model)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				model->m_meshes.push_back(processMesh(mesh, scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene, model);
			}
		}
		Mesh* Loader::processMesh(aiMesh *mesh, const aiScene *scene)
		{
			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;
			std::vector<Texture2D*>* textures = new std::vector<Texture2D*>();

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				VertexData vertex;
				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;

				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;


				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					vertex.uv.x = mesh->mTextureCoords[0][i].x;
					vertex.uv.y = mesh->mTextureCoords[0][i].y;
				}
				else
					vertex.uv = glm::vec2(0.0f, 0.0f);

				vertices.push_back(vertex);
			}


			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			Mesh * mMesh = loadToVao(vertices, indices);

			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

				std::vector<Texture2D*>* diffuseMaps = loadMaterialTextures(material,
					aiTextureType_DIFFUSE, TextureType::Diffuse);
				textures->insert(textures->end(), diffuseMaps->begin(), diffuseMaps->end());

				std::vector<Texture2D*>* specularMaps = loadMaterialTextures(material,
					aiTextureType_SPECULAR, TextureType::Specular);
				textures->insert(textures->end(), specularMaps->begin(), specularMaps->end());
			}
			if (!textures->empty()) {
				mMesh->m_textures = textures;
				mMesh->hasTextures = true;
			}

			return mMesh;
		}
		///TODO :load for every mesh it's diffuse and specular maps 
		std::vector<Texture2D*>* Loader::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
			TextureType typeName)
		{
			std::vector<Texture2D*>* textures = new std::vector<Texture2D*>();
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				std::string strr(str.C_Str());
				Texture2D* texture = TextureManager::LoadTexture((directory + "textures/" + strr).c_str());
				texture->m_type = typeName;
				textures->push_back(texture);
			}
			return textures;

		}


		GLuint Loader::createVao()
		{
			GLuint  vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			return vao;
		}
		void Loader::createVertexBuffer(const std::vector<VertexData>& data)
		{
			GLuint buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexData), data.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, uv));

			m_vbos.push_back(buffer);
		}

		GLuint Loader::createIndicesBuffer(const std::vector < unsigned int>& indices)
		{
			GLuint indicesBufferID;
			glGenBuffers(1, &indicesBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
			return  indicesBufferID;
		}

		void Loader::unbindVao()
		{
			glBindVertexArray(0);
		}


		void Loader::clean()
		{
			for each (const GLuint var in m_vbos)
			{
				glDeleteBuffers(1, &var);
			}

			for each (const GLuint var in m_vaos)
			{
				glDeleteVertexArrays(1, &var);
			}
			m_vbos.clear();
			m_vaos.clear();
		}

		Loader::~Loader()
		{

		};


	}
}