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
#include <core/AudioEngine.h>


#include "TestApplication.h"

namespace engine
{

	const float SPEED = 20.0f;
	
	//prototypes
	void drawBackground();

	void TestApplication::DetectCollidingObjects()
	{
		for (int p = 0; p < 1; p++)
		{
			for (int other = p + 1; other < m_sprites.size(); other++)
			{
				if (m_sprites[other].hasCollider == true)
				{
					if (m_sprites[p].collider.position.x  < m_sprites[other].collider.position.x + m_sprites[other].collider.dimensions.x &&
						m_sprites[p].collider.position.x + m_sprites[p].collider.dimensions.x > m_sprites[other].collider.position.x &&
						m_sprites[p].collider.position.y < m_sprites[other].collider.position.y + m_sprites[other].collider.dimensions.y &&
						m_sprites[p].collider.position.y + m_sprites[p].collider.dimensions.y > m_sprites[other].collider.position.y)
					{
						LOGI("Collision Detected! \n");
						//Delete Collided sprite
						m_sprites.erase(m_sprites.begin() + other);
						//Add point to score
						m_score++;
					}
					else
					{
						//LOGI("Collision Not Detected! \n");
					}
				}
			}
		}
	}


	TestApplication::TestApplication( Window * window, GraphicsSystem * graphics, InputManager* inputMgr, void * manager /* = nullptr */)
            : GraphicsApplication(window, graphics)
			, m_inputManager(inputMgr)
			, m_assetManager(manager)
            , m_totalTime(0.0f)
			, m_score(0)
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

		// init sprites			Sprites with collider first

		//Player Sprite First
		Sprite foo = Sprite(0.0f, 0.0f, "NissanSkyline.png", m_assetManager, true);
		m_sprites.push_back(foo);

		//Collider Objects
		Sprite foo2 = Sprite(600.0f, 600.0f, "mr_t.png", m_assetManager, true);
		foo2.SetColliderOffsetValues(0.0f, 0.0f, 300.0f, 300.0f);
		m_sprites.push_back(foo2);
		
		//Test: multiple colliding sprite objects
		Sprite foo3 = Sprite(-800.0f, 0.0f, "mr_t.png", m_assetManager, true);
		m_sprites.push_back(foo3);
		Sprite foo4 = Sprite(-800.0f,-200.0f, "mr_t.png", m_assetManager, true);
		m_sprites.push_back(foo4);
		Sprite foo5 = Sprite(300.0f, -800.0f, "mr_t.png", m_assetManager, true);
		m_sprites.push_back(foo5);
		Sprite foo6 = Sprite(800.0f, 100.0f, "mr_t.png", m_assetManager, true);
		m_sprites.push_back(foo6);
		
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

		//Check totaltime
		if (m_totalTime >= 20.0f) 
		{ 
			//Reset totaltime
			m_totalTime = 0.0f;
			AudioEngine audio;
			audio.Play(L"chimes.wav");
			LOGI("Game Ended! \n"); 
			LOGI("Game Score: %d \n", m_score);
		}

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
			//m_sprites[0].position.y += 1.0f * SPEED;

			//Move Player Sprite Up
			m_sprites[0].Move(0.0f, 1.0f*SPEED);
			
		}
		if (m_inputManager->getKeyPressedValue(DOWN)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x, cameraCoords.y - 1.0f * SPEED));
			//m_sprites[0].position.y -= 1.0f * SPEED;

			//Move Player Sprite Down
			m_sprites[0].Move(0.0f, -1.0f*SPEED);
		}
		if (m_inputManager->getKeyPressedValue(RIGHT)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x + 1.0f * SPEED, cameraCoords.y));
			//m_sprites[0].position.x += 1.0f * SPEED;

			//Move Player Sprite Right
			m_sprites[0].Move(1.0f*SPEED,0.0f);
		}
		if (m_inputManager->getKeyPressedValue(LEFT)) {
			//m_camera->setPosition(glm::vec2(cameraCoords.x - 1.0f * SPEED, cameraCoords.y));
			//m_sprites[0].position.x -= 1.0f * SPEED;

			//Move Player Sprite Left
			m_sprites[0].Move(-1.0f*SPEED,0.0f);
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