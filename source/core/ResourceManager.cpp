#include "core/ResourceManager.h"

namespace engine {

TextureCache ResourceManager::m_textureCache;

Texture2D ResourceManager::getTexture(std::string texturePath)
{
	return m_textureCache.getTexture(texturePath);
}

Texture2D ResourceManager::createTexture(std::string texturePath, int width, int height, int bytesPerPixel, void * manager)
{
	return m_textureCache.createTexture(texturePath, width, height, bytesPerPixel, manager);
}

}
