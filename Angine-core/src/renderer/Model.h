#pragma once
#include "../common.h"
#include "VertexData.h"
#include "Texture.h"
////
#define GLEW_STATIC
#include <glew\glew.h>
//TODO: delete this
namespace Angine {

class Mesh {
 public:
	 Mesh() {};
  Mesh(const std::vector<VertexData>& vertices,
       const std::vector<uint32>& indices) {
    init(vertices, indices);
  }

  void init(const std::vector<VertexData>& vertices,
            const std::vector<unsigned int>& indices) {
    numIndices = indices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vio);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices,
                 indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
  }
  ~Mesh() {}
  uint32 vio, vbo, vao;
  uint32 numIndices;
  uint32 materialIndex;
};

class Model {
 public:
  Model(){};
  ~Model(){};

  std::vector<Mesh> meshes;
  std::vector<Texture*> textures;
};

}  // namespace Angine
