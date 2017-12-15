#include "Sprite.h"
#include <core/ResourceManager.h>


Sprite::Sprite(float x, float y, std::string texturePath, void* manager, bool enableCol)
{
	position.x = x;
	position.y = y;
	
	hasCollider = enableCol;

	m_texture = engine::ResourceManager::getTexture(texturePath, m_texture.width, m_texture.height, m_bytesPerPixel, manager);
	
	dimensions.x = m_texture.width;
	dimensions.y = m_texture.height;

	if (hasCollider) {
		LOGI("Collider placed! \n");
		collider.position.x = x;
		collider.position.y = y;
		collider.dimensions.x = m_texture.width;
		collider.dimensions.y = m_texture.height;
	}
	//LOGI("width: %d, height: %d, bpp: %d \n", (int)m_texture.width, (int)m_texture.height, m_bytesPerPixel);
}


Sprite::~Sprite()
{
}

void Sprite::draw(engine::Shader* program)
{
	// set up vertices
	float depth = 0.0f;

	GLuint texCoordLoc = program->getAttributeLocation("a_texCoord");
	GLuint posCoordLoc = program->getAttributeLocation("a_vertexPosition");

	GLfloat vertices[] = {
		position.x + 0.0f,				position.y + m_texture.height,	depth,		// vertex 0
		position.x + 0.0f,				position.y + 0.0f,				depth,		// vertex 1
		position.x + m_texture.width,	position.y + 0.0f,				depth,		// vertex 2

		position.x + m_texture.width,	position.y + m_texture.height,	depth,		// vertex 3
		position.x + m_texture.width,	position.y + 0.0f,				depth,		// vertex 4
		position.x + 0.0f,				position.y + m_texture.height,	depth,		// vertex 5

	};

	GLfloat textureCoords[] = {
		0,0,
		0,1,
		1,1,
		1,0,
		1,1,
		0,0
	};

	// positions
	glVertexAttribPointer(posCoordLoc, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(posCoordLoc);

	// Texture coordinates
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
	glEnableVertexAttribArray(texCoordLoc);

	// bind texture location
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture.id);
	

	//draw vertex array as triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Sprite::Move(float xPos, float yPos)
{
	position.x += xPos;
	position.y += yPos;
	collider.position.x += xPos;
	collider.position.y += yPos;
}

