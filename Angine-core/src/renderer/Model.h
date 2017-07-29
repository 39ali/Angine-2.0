#pragma once
#include "../common.h"
#include "../renderer/Mesh.h"
namespace Angine {
	namespace Renderer {
		class Model
		{
		public:
			Model() {};
			~Model() {};
			//void addMesh(Mesh * mesh) { m_meshes.push_back(mesh); };
			std::vector<Mesh*> m_meshes;
		private:
			std::string m_directory;




		};
	}
}