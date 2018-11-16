#pragma once
#include "../common.h"
#include "../renderer/Mesh.h"
namespace Angine {
	
		class Model
		{
		public:
			Model() {};
			~Model() {};
			//void addMesh(Mesh * mesh) { m_meshes.push_back(mesh); };
			/// TODO : delete meshes ?
			std::vector<Mesh*> m_meshes;
		private:
			std::string m_directory;




		};
	
}