#include "Sprite.h"
#include <core/ResourceManager.h>


Sprite::Sprite(float x, float y, std::string texturePath, int bytesPerPixel, void* manager)
{
	position.x = x;
	position.y = y;
	
	int w, h;

	engine::ResourceManager::createTexture(texturePath, w, h, bytesPerPixel, manager);
	texture = engine::ResourceManager::getTexture(texturePath);
	
	texture->width = w;
	texture->height = h;
	LOGI("width: %d, height: %d\n", (int)texture->width, (int)texture->height);
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
		position.x + 0.0f, position.y + texture->height, depth,			// vertex 0
		position.x + 0.0f, position.y + 0.0f, depth,					// vertex 1
		position.x + texture->width, position.y + 0.0f, depth,			// vertex 2

		position.x + texture->width, position.y + texture->height, depth,	// vertex 3
		position.x + texture->width, position.y + 0.0f, depth,				// vertex 4
		position.x + 0.0f, position.y + texture->height, depth,				// vertex 5

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
	glBindTexture(GL_TEXTURE_2D, texture->id);
	

	//draw vertex array as triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
