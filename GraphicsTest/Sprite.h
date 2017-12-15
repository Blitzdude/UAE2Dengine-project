#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <core/Log.h>
#include <glm/common.hpp>
#include <OGL/Shader.h>
#include <string>
#include <graphics/Texture2D.h>
#include <core/stb_image.h>

struct Collider {
	glm::vec2 position;
	glm::vec2 dimensions;
};

class Sprite
{
public:
	Sprite(float x, float y, std::string texturePath, void* manager, bool enableCol);
	~Sprite();

	void draw(engine::Shader* program);

	void SetColliderOffsetValues(float xOff, float yOff, float xDim, float yDim)
	{
		collider.position.x += xOff;
		collider.position.y += yOff;
		collider.dimensions.x += xDim;
		collider.dimensions.y += yDim;
	};

	void Move(float xPos, float yPos);

	// members
	bool hasCollider;

	Collider collider;
	
	glm::vec2 position;
	glm::vec2 dimensions;
private:
	int m_bytesPerPixel;
	engine::Texture2D m_texture;

};
#endif // !_SPRITE_H_

