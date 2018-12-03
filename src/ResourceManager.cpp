#include "ResourceManager.h"
#include <string>

std::array<Shader, 1> ResourceManager::_shaders = {std::string("4.1.texture")};

Shader& ResourceManager::getShader(SHADER_INDEX i) {
  Shader& s = _shaders[static_cast<std::size_t>(i)];
  if (s.ID == 0) s.load();
  return s;
}

// Texture2D& ResourceManager::getTextureAtlas() {
//   if (_atlas == nullptr) {
//     _atlas = new Texture2D();
//     int width, height, nrChannels; // nrChannels is ignored
//     stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//     unsigned char *image = stbi_load("textures/grass.png", &width, &height, &nrChannels, 0);
//     if (image) {
//       _atlas->Generate(width, height, image);
//     } else {
//       std::cout << "Failed to load texture" << std::endl;
//       exit(1);
//     }
//     stbi_image_free(image);

//     return *_atlas;
//   } else {
//     return *_atlas;
//   }
// }