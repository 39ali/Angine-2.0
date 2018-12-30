#pragma once
#include <unordered_map>
#include "../common.h"
#include "Texture.h"
#include "RenderDevice.h"
#include "../Utils/imageLoader.h"
namespace Angine {

class TextureManager {
 public:
  static Texture* TextureManager::loadTexture(RenderDevice& renderDevice,
                                              const char* filename);
   Texture* LoadCubeMap(const std::vector<std::string>& faces);
  void clean(RenderDevice& renderDevice);


 private:
  TextureManager(){};
  ~TextureManager(){};
  static std::unordered_map<std::string, Texture> textures;
};

}  // namespace Angine
