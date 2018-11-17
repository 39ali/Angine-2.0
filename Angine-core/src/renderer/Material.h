#pragma once
#include <unordered_map>
#include "../math/Matrix4f.h"
namespace Angine {

struct MaterialSpec {
  std::unordered_map<std::string, std::string> textureNames;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, Vector3f> vectors;
  std::unordered_map<std::string, mat4f> matrices;
};

}  // namespace Angine
