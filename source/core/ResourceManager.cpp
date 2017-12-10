#include "core/ResourceManager.h"

namespace engine {

TextureCache ResourceManager::m_textureCache;

Texture2D ResourceManager::getTexture(std::string texturePath, int &width, int &height, int &bytesPerPixel, void* manager)
{
	return m_textureCache.getTexture(texturePath, width, height, bytesPerPixel, manager);
}


}
