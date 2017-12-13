/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <graphics/GraphicsSystem.h>
#include <graphics/Window.h>
#include <math.h>
#include <core/IOManager.h>
#include <glm/glm.hpp>
#include <core/stb_image.h>
#include "Sprite.h"


#include "TestApplication.h"

namespace engine
{

	const float SPEED = 20.0f;

	//prototypes
	void drawBackground();

	//Collision Detect Function
	void TestApplication::DetectCollidingObjects()
	{
		/*
		printf("Car PositionX %f \n", m_sprites[0].position.x); 
		printf("Car PositionY %f \n", m_sprites[0].position.y);
		printf("Mr T PositionX %f \n", m_sprites[1].position.x);
		printf("Mr T PositionY %f \n", m_sprites[1].position.y);
		printf("Car DimensionsX %f \n", m_sprites[0].dimensions.x);
		printf("Car DimensionsY %f \n", m_sprites[0].dimensions.y);
		printf("Mr T DimensionsX %f \n", m_sprites[1].dimensions.x);
		printf("Mr T DimensionsY %f \n", m_sprites[1].dimensions.y);
		*/
		for (int a = 0; a < m_sprites.size(); a++)
		{
			for (int b = a+1; b < m_sprites.size(); b++)
			{
				if (m_sprites[a].position.x < m_sprites[b].position.x + m_sprites[b].dimensions.x && 
					m_sprites[a].position.x + m_sprites[a].dimensions.x > m_sprites[b].position.x && 
					m_sprites[a].position.y < m_sprites[b].position.y + m_sprites[b].dimensions.y && 
					m_sprites[a].position.y + m_sprites[a].dimensions.y > m_sprites[b].position.y)
				{
					printf("Collision Detected! \n");
					m_sprites.erase(m_sprites.begin() + b);
				}
				else
				{
					//printf("Collision Not Detected! \n");
				}
			}
		}
	}


	TestApplication::TestApplication( Window * window, GraphicsSystem * graphics, InputManager* inputMgr, void * manager /* = nullptr */)
            : GraphicsApplication(window, graphics)
			, m_inputManager(inputMgr)
			, m_assetManager(manager)
            , m_totalTime(0.0f)
	{
		LOGI("Starting UP...\n");

		init();
	}

	TestApplication::~TestApplication()
	{
	}

	void TestApplication::init()
	{
		initShaders();
		// create camera and initilize it
		m_camera = new Camera2D();
		m_camera->init(getWindow()->getWidth(), getWindow()->getHeight());

		// init sprites
		Sprite foo = Sprite(-300 , 50.0f, "NissanSkyline.png", m_assetManager);
		m_sprites.push_back(foo);
		Sprite foo2 = Sprite(300.0f, 0.0f, "mr_t.png", m_assetManager);
		m_sprites.push_back(foo2);
	}

	void TestApplication::initShaders()
	{
		getGraphicsSystem()->createNewShader("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag", m_assetManager); // indexed at 0
		auto shaderPtr = getGraphicsSystem()->getShader(0);
		shaderPtr->linkShaders();

	}

	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		processInput(getWindow());

		//Check Collision
		DetectCollidingObjects();

		m_camera->update();
		
		return true;
	}


	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		// set OpenGL drawing window display to entire window.
		glViewport(0, 0, window->getWidth(), window->getHeight());

		//float val = fabsf(sinf(2.0f*m_totalTime));
		// set the base depth to 1.0f		
		glClearDepthf(1.0f);
		// Clear screen with pulsating yellow: 
		graphics->clearScreen(0.5f, 0.5f, 0.5f, true); 
		
		//coords = m_camera->convertScreenToWorld(coords);
		
		(void)window;	
		
		auto shaderProg = graphics->getShader(0);

		shaderProg->use();

		// draw code goes here
		//drawBackground();
		//setCamera matrix
		GLint pLocation = shaderProg->getUniformLocation("u_mvpMatrix");
		glUniform1i(pLocation, 0);

		glm::mat4 cameraMatrix = m_camera->getCameraMatrix();
		m_camera->setScale(0.40f);



		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		// draw each sprite in sprites vector
		for (auto itr : m_sprites)
		{
			itr.draw(shaderProg);
		};

		shaderProg->unUse();
		
		// switch secondary buffer to be displayed on screen. 
		graphics->swapBuffers();
	}

	void TestApplication::processInput(Window* window)
	{
		glm::vec2 cameraCoords = m_camera->getPosition();
		//InputManager Ver 2.0
		if (m_inputManager->getKeyPressedValue(UP)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x, cameraCoords.y + 1.0f * SPEED));
			m_sprites[0].position.y += 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(DOWN)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x, cameraCoords.y - 1.0f * SPEED));
			m_sprites[0].position.y -= 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(RIGHT)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x + 1.0f * SPEED, cameraCoords.y));
			m_sprites[0].position.x += 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(LEFT)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x - 1.0f * SPEED, cameraCoords.y));
			m_sprites[0].position.x -= 1.0f * SPEED;
		}


		// Camera controls
		if (m_inputManager->getKeyPressedValue(W)){
			m_camera->setPosition(glm::vec2(cameraCoords.x, cameraCoords.y + 1.0f * SPEED));
			//m_sprites[0].position.y += 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(S)) {
			m_camera->setPosition(glm::vec2(cameraCoords.x, cameraCoords.y - 1.0f * SPEED));
			//m_sprites[0].position.y -= 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(D)) {
			m_camera->setPosition(glm::vec2(cameraCoords.x + 1.0f * SPEED, cameraCoords.y));
			//m_sprites[0].position.x += 1.0f * SPEED;
		}
		if (m_inputManager->getKeyPressedValue(A)) {
			m_camera->setPosition(glm::vec2(cameraCoords.x - 1.0f * SPEED, cameraCoords.y));
			//m_sprites[0].position.x -= 1.0f * SPEED;
		}
	}


	void drawBackground()
	{
	}

}