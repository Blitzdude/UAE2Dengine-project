#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <core/Log.h>
#include <glm/common.hpp>
#include <OGL/Shader.h>
#include <string>
#include <graphics/Texture2D.h>
#include <core/stb_image.h>


class Sprite
{
public:
	Sprite(float x, float y, std::string texturePath, int bytesPerPixel, void* manager);
	~Sprite();

	void draw(engine::Shader* program);
	
	glm::vec2 position;
	
	int bytesPerPixel;
	engine::Texture2D* texture;

};
#endif // !_SPRITE_H_

