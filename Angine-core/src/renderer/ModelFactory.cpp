#include "ModelFactory.h"
#include <math.h>
#include "../math/Vector3f.h"
#include "Texture.h"
#include "TextureManager.h"
namespace Angine {

//#define M_PI 3.14159265359
//#define M_PI_2 M_PI / 2
// Model* ModelFactory::createSphere(float radius, unsigned int rings,
//                                  unsigned int sectors) {
//  std::vector<VertexData> _data;
//  std::vector<GLfloat> vertices;
//  std::vector<GLfloat> normals;
//  std::vector<GLfloat> texcoords;
//  std::vector<unsigned int> indices;
//  float const R = 1. / (float)(rings - 1);
//  float const S = 1. / (float)(sectors - 1);
//  int r, s;
//
//  vertices.resize(rings * sectors * 3);
//  normals.resize(rings * sectors * 3);
//  texcoords.resize(rings * sectors * 2);
//  std::vector<GLfloat>::iterator v = vertices.begin();
//  std::vector<GLfloat>::iterator n = normals.begin();
//  std::vector<GLfloat>::iterator t = texcoords.begin();
//  for (r = 0; r < rings; r++)
//    for (s = 0; s < sectors; s++) {
//      float const y = sin(-M_PI_2 + M_PI * r * R);
//      float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
//      float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
//      VertexData vertex;
//      vertex.uv.x = s * S;
//      vertex.uv.y = r * R;
//      /**t++ = s*S;
//       *t++ = r*R;
//       */
//      vertex.position.x = x * radius;
//      vertex.position.y = y * radius;
//      vertex.position.z = z * radius;
//      /*	*v++ = x * radius;
//       *v++ = y * radius;
//       *v++ = z * radius;*/
//
//      /**n++ = x;
//       *n++ = y;
//       *n++ = z;*/
//
//      vertex.normal.x = x;
//      vertex.normal.y = y;
//      vertex.normal.z = z;
//      _data.push_back(vertex);
//    }
//
//  indices.resize(rings * sectors * 4);
//  std::vector<unsigned int>::iterator i = indices.begin();
//  for (r = 0; r < rings - 1; r++)
//    for (s = 0; s < sectors - 1; s++) {
//      *i++ = r * sectors + s;
//      *i++ = r * sectors + (s + 1);
//      *i++ = (r + 1) * sectors + (s + 1);
//      *i++ = (r + 1) * sectors + s;
//    }
//
//  Model* m = new Model();
//  GLuint sphereVAO = 0;
//  GLuint vbo, ebo;
//  glGenBuffers(1, &vbo);
//  glGenBuffers(1, &ebo);
//
//  glGenVertexArrays(1, &sphereVAO);
//  glBindVertexArray(sphereVAO);
//  glBindBuffer(GL_ARRAY_BUFFER, vbo);
//  glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(float), _data.data(),
//               GL_STATIC_DRAW);
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
//               &indices[0], GL_STATIC_DRAW);
//
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
//                        (const void*)offsetof(VertexData, position));
//  glEnableVertexAttribArray(1);
//  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
//                        (const void*)offsetof(VertexData, normal));
//  glEnableVertexAttribArray(2);
//  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
//                        (const void*)offsetof(VertexData, uv));
//
//  Mesh* mesh = new Mesh(sphereVAO, indices.size() / 4, ebo, GL_QUADS);
//  m->m_meshes.push_back(mesh);
//  return m;
//}
//
// Model* ModelFactory::createPlane() {
//  float planeVertices[] = {
//      // positions            // normals         // texcoords
//      10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
//      -10.0f, -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
//      -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,
//
//      10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
//      -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,
//      10.0f,  -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};
//  // plane VAO
//  unsigned int planeVAO, planeVBO;
//  glGenVertexArrays(1, &planeVAO);
//  glGenBuffers(1, &planeVBO);
//  glBindVertexArray(planeVAO);
//  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
//               GL_STATIC_DRAW);
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
//  (void*)0); glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3,
//  GL_FLOAT, GL_FALSE, 8 * sizeof(float),
//                        (void*)(3 * sizeof(float)));
//  glEnableVertexAttribArray(2);
//  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
//                        (void*)(6 * sizeof(float)));
//  glBindVertexArray(0);
//
//  Model* m = new Model();
//  Mesh* mesh = new Mesh(planeVAO, 6, -1, GL_TRIANGLES);
//  m->m_meshes.push_back(mesh);
//  return m;
//}

static bool loadFromScene(const aiScene* pScene, const std::string& Filename,
                          RenderDevice& renderDevice, Model& model,
                          std::string& textureloc);

static bool initMaterials(const aiScene* pScene, const std::string& Filename,
                          std::vector<Texture*>* textures,
                          RenderDevice& renderDevice, std::string& textureloc);

static void initMesh(unsigned int Index, const aiMesh* paiMesh,
                     std::vector<Mesh>* meshes, std::string& textureloc);

bool ModelFactory::loadModel(RenderDevice& renderDevice,
                             const std::string& filename, Model& model,
                             std::string& textureloc) {
  Assimp::Importer Importer;

  const aiScene* pScene = Importer.ReadFile(
      filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                            aiProcess_FlipUVs |
                            aiProcess_JoinIdenticalVertices);

  if (pScene) {
    return loadFromScene(pScene, filename, renderDevice, model, textureloc);
  } else {
    printf("Error parsing '%s': '%s'\n", filename.c_str(),
           Importer.GetErrorString());

    return false;
  }
}

static bool loadFromScene(const aiScene* pScene, const std::string& Filename,
                          RenderDevice& renderDevice, Model& model,
                          std::string& textureloc) {
  model.meshes.resize(pScene->mNumMeshes);

  model.textures.resize(pScene->mNumMaterials);

  // Initialize the meshes
  for (unsigned int i = 0; i < model.meshes.size(); i++) {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    initMesh(i, paiMesh, &model.meshes, textureloc);
  }

  return initMaterials(pScene, Filename, &model.textures, renderDevice,
                       textureloc);
}
static void initMesh(unsigned int Index, const aiMesh* paiMesh,
                     std::vector<Mesh>* meshes, std::string& textureloc) {
  (*meshes)[Index].materialIndex = paiMesh->mMaterialIndex;

  std::vector<VertexData> Vertices;
  std::vector<uint32> Indices;

  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
    const aiVector3D* pPos = &(paiMesh->mVertices[i]);
    const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
    const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0)
                                      ? &(paiMesh->mTextureCoords[0][i])
                                      : &Zero3D;

    VertexData v(Vector3f(pPos->x, pPos->y, pPos->z),
                 Vector2f(pTexCoord->x, pTexCoord->y),
                 Vector3f(pNormal->x, pNormal->y, pNormal->z));

    Vertices.push_back(v);
  }

  for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
    const aiFace& Face = paiMesh->mFaces[i];
    assert(Face.mNumIndices == 3);
    Indices.push_back(Face.mIndices[0]);
    Indices.push_back(Face.mIndices[1]);
    Indices.push_back(Face.mIndices[2]);
  }

  (*meshes)[Index].init(Vertices, Indices);
}
static bool initMaterials(const aiScene* pScene, const std::string& Filename,
                          std::vector<Texture*>* textures,
                          RenderDevice& renderDevice, std::string& textureloc) {
  // Extract the directory part from the file name
  std::string::size_type SlashIndex = Filename.find_last_of("/");
  std::string Dir;

  if (SlashIndex == std::string::npos) {
    Dir = ".";
  } else if (SlashIndex == 0) {
    Dir = "/";
  } else {
    Dir = Filename.substr(0, SlashIndex);
  }

  bool Ret = true;

  // Initialize the materials
  for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

	(*textures)[i] = nullptr;

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString Path;

      if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL,
                                NULL, NULL, NULL) == AI_SUCCESS) {
        std::string FullPath = Dir + textureloc + "/" + Path.data;
        (*textures)[i] = TextureManager::loadTexture(
            renderDevice, FullPath.c_str());
      }
    }

    // Load a white texture in case the model does not include its own texture
    if ((*textures)[i]==nullptr) {
      (*textures)[i] = TextureManager::loadTexture(
          renderDevice, "./res/textures/white.png");
      Ret = true;
    }
  }

  return Ret;
}

}  // namespace Angine
