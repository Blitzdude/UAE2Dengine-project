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

#include <stdlib.h>
#include <time.h>


#include "TestApplication.h"

namespace engine
{
	// global variables
	const float SPEED = 3.0f;
	const int MAX_FRUITS = 6;

	TestApplication::TestApplication( Window * window, GraphicsSystem * graphics, InputManager* inputMgr, void * manager /* = nullptr */)
            : GraphicsApplication(window, graphics)
			, m_inputManager(inputMgr)
			, m_assetManager(manager)
            , m_totalTime(0.0f)
			, m_score(0)
	{
		LOGI("Starting UP...\n");
		m_screenWidth = window->getWidth();
		m_screenHeight = window->getHeight();
		init();
	}

	TestApplication::~TestApplication()
	{
		m_sprites.clear();
	}

	void TestApplication::init()
	{
		initShaders();

		// create camera and initilize it
		m_camera = new Camera2D();
		m_camera->init(m_screenWidth, m_screenHeight);
		m_camera->setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

		// init random seed
		srand(time(NULL));

		initGame();
		m_gameState = START;
	}

	void TestApplication::initShaders()
	{
		getGraphicsSystem()->createNewShader("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag", m_assetManager); // indexed at 0
		auto shaderPtr = getGraphicsSystem()->getShader(0);
		shaderPtr->linkShaders();

	}

	void TestApplication::initGame()
	{
		//Zero the game score
		m_score = 0;
		m_sprites.clear();
		// init sprites			Sprites with collider first

		//Player Sprite First
		Sprite player = Sprite(m_screenWidth / 2, m_screenHeight / 2, "Textures/Hippobiili1.png", m_assetManager, true);
		m_sprites.push_back(player);

		
	}

	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		
		switch (m_gameState)
		{
			case START: {

				//Wait 5 seconds then start the game
				//Check totaltime
				if (m_totalTime >= 5.0f)
				{
					//Reset totaltime
					m_totalTime = 0.0f;
					m_audioEngine.Play(L"Sounds/tada.wav");
					LOGI("GO! \n");

					m_gameState = PLAYING;
				}
				//break;
			}

			case PLAYING: {
				// add fruit
				while (m_sprites.size() < MAX_FRUITS) 
				{
					if(rand() % 2 == 1) 
					{
						Sprite fruit(rand() % (m_screenWidth - 50), rand() % (m_screenHeight - 50), "Textures/Banaaloni1.png", m_assetManager, true);
						m_sprites.push_back(fruit);	
					}
					else 
					{
						Sprite fruit(rand() % (m_screenWidth - 50), rand() % (m_screenHeight - 50), "Textures/Kirsitruuna1.png", m_assetManager, true);
						m_sprites.push_back(fruit);
					}
				}

				processInput(getWindow());

				//Check Collision
				DetectCollidingObjects();

				//Check totaltime
				if (m_totalTime >= 20.0f)
				{
					m_gameState = END;

					//Reset totaltime
					m_totalTime = 0.0f;
					m_audioEngine.Play(L"Sounds/chimes.wav");
					LOGI("Game Ended! \n");
					LOGI("Game Score: %d \n", m_score);

				}

				m_camera->update();

				break;
			}

			case END: {
				if (m_totalTime >= 5.0f) {
					//Reset totaltime
					m_totalTime = 0.0f;
					//ReInitialize the level
					initGame();
					//Change game state 
					m_gameState = START;
				}
				break;
			}
			default: {
				break;
			}
		}
		
		return true;
	}


	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		// background sprite
		static Sprite bg(0.0f, 0.0f, "Textures/asfalt_bg.png", m_assetManager, false);
		bg.dimensions.x = m_screenWidth;
		bg.dimensions.y = m_screenHeight;
		// text sprites;
		static Sprite readyText(m_screenWidth / 2 - 50.0f, m_screenHeight * 0.75f, "Textures/ReadyText.png", m_assetManager, false);
		static Sprite goText(m_screenWidth / 2 - 50.0f, m_screenHeight * 0.75f, "Textures/GoText.png", m_assetManager, false);
		static Sprite winText(m_screenWidth / 2, m_screenHeight * 0.75f, "Textures/WinText.png", m_assetManager, false);
		static Sprite loseText(m_screenWidth / 2, m_screenHeight * 0.75f, "Textures/LoseText.png", m_assetManager, false);

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
		//setCamera matrix
		GLint pLocation = shaderProg->getUniformLocation("u_mvpMatrix");
		glUniform1i(pLocation, 0);

		glm::mat4 cameraMatrix = m_camera->getCameraMatrix();

		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		//draw the background;
		bg.draw(shaderProg);
		
		// draw each sprite in sprites vector
		for (auto itr : m_sprites)
		{
			itr.draw(shaderProg);
		};

		if (m_gameState == START) {
			readyText.draw(shaderProg);
		}
		else if (m_gameState == PLAYING && m_totalTime < 3.0f) {
			goText.draw(shaderProg);
		}
		else if (m_gameState == END) {
			if (m_score >= 30) {
				winText.draw(shaderProg);
			}
			else {
				loseText.draw(shaderProg);
			}
		}

		shaderProg->unUse();
		
		// switch secondary buffer to be displayed on screen. 
		graphics->swapBuffers();
	}

	void TestApplication::processInput(Window* window)
	{
		glm::vec2 clamp(window->getWidth() - m_sprites[0].dimensions.x,
			window->getHeight() - m_sprites[0].dimensions.y);

		if (m_gameState == PLAYING)
			{

			glm::vec2 cameraCoords = m_camera->getPosition();
			//InputManager Ver 2.0
			if (m_inputManager->getKeyPressedValue(UP)) {
				if (m_sprites[0].position.y < clamp.y)
				{
					//Move Player Sprite Up
					m_sprites[0].Move(0.0f, 1.0f * SPEED);
				}
			
			}
			if (m_inputManager->getKeyPressedValue(DOWN)) {
				if (m_sprites[0].position.y > 0.0f) 
				{
					//Move Player Sprite Down
					m_sprites[0].Move(0.0f, -1.0f * SPEED);
				}
			}
			if (m_inputManager->getKeyPressedValue(RIGHT)) {
				if (m_sprites[0].position.x < clamp.x)
				{
					//Move Player Sprite Right
					m_sprites[0].Move(1.0f*SPEED, 0.0f);
				}
			}
			if (m_inputManager->getKeyPressedValue(LEFT)) {
				if (m_sprites[0].position.x > 0.0f) {
					//Move Player Sprite Left
					m_sprites[0].Move(-1.0f*SPEED,0.0f);
				}
			}
			LOGI("Window: %d , %d, Player position: %f , %f \r", 
				window->getWidth(), window->getHeight(),	
				m_sprites[0].position.x, m_sprites[0].position.y);
		}

		// Camera controls NOT NEEDED
	
		glm::vec2 cameraCoords = m_camera->getPosition();

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
						//Play sound
						m_audioEngine.Play(L"Sounds/eat.wav");
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

}