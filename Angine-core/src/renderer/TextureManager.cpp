#include "TextureManager.h"
namespace Angine {

std::unordered_map<std::string, Texture> TextureManager::textures;

Texture* TextureManager::loadTexture(RenderDevice& renderDevice,
                                     const char* filename) {
  auto it = textures.find(filename);
  if (it != textures.end()) {
    return &it->second;
  }
  Texture texture;
  unsigned int BPP;
  FIBITMAP* dib(0);
  BYTE* data = imageLoader(filename, texture.width, texture.height, BPP, dib);

  if (data == nullptr) {
    return nullptr;
  }

  if (BPP == 24) {
    texture.texId = renderDevice.createTexture2D(
        texture.width, texture.height, (const void*)data,
        OpenGLDevice::TexturePixelFormat::FORMAT_RGBA,
        OpenGLDevice::TexturePixelFormat::FORMAT_BGR, true);
  } else if (BPP == 32) {
    texture.texId = renderDevice.createTexture2D(
        texture.width, texture.height, (const void*)data,
        OpenGLDevice::TexturePixelFormat::FORMAT_RGBA, 
        OpenGLDevice::TexturePixelFormat::FORMAT_BGRA,true);
  }
  textures.insert({filename, texture});
  free(data);
  return &textures.find(filename)->second;
}  // namespace Angine

void TextureManager::clean(RenderDevice& renderDevice) {
  for (auto& texture : textures) {
    renderDevice.deleteTexture2D(texture.second.texId);
  }
  textures.clear();
};

/*Texture2D* TextureManager::LoadCubeMap(const std::vector<std::string>& faces)
{

        Texture2D* texture = new Texture2D();
        glGenTextures(1, &texture->m_texID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_texID);

        unsigned int  BPP;
        FIBITMAP * dib(0);
        for (unsigned int i = 0; i < faces.size(); i++)
        {
                BYTE* data = imageLoader(faces[i].c_str(), texture->m_width,
texture->m_height, BPP, dib); if (data)
                {
                        if (BPP == 24) {
                                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
0, GL_RGB, texture->m_width, texture->m_height, 0, GL_BGR, GL_UNSIGNED_BYTE,
data);
                        }
                        else if (BPP == 32)
                        {
                                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
0, GL_RGBA, texture->m_width, texture->m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
data);
                        }
                }
                else
                {
                        std::cout << "Cubemap texture failed to load at path: "
<< faces[i] << std::endl;
                        







                }
                free( data);
                







        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
GL_CLAMP_TO_EDGE); m_textures.insert({ faces[0], texture }); return texture;

}*/

}  // namespace Angine
