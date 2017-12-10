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
	const float SPEED = 3.0f;

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
		Sprite foo = Sprite(50.0f, 50.0f, "mr_t.png", m_assetManager);
		m_sprites.push_back(foo);
		Sprite foo2 = Sprite(200.0f, 50.0f, "mr_t.png", m_assetManager);
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

		m_camera->update();
		
		return true;
	}


	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		float val = fabsf(sinf(2.0f*m_totalTime));
		// set the base depth to 1.0f		
		glClearDepthf(1.0f);
		// Clear screen with pulsating yellow: 
		graphics->clearScreen(val, val * 1.5f, val * 0.5f, true);
		
		//coords = m_camera->convertScreenToWorld(coords);
		
		(void)window;	
		
		auto shaderProg = graphics->getShader(0);

		shaderProg->use();

		// draw code goes here

		//setCamera matrix
		GLint pLocation = shaderProg->getUniformLocation("u_mvpMatrix");
		glUniform1i(pLocation, 0);

		glm::mat4 cameraMatrix = m_camera->getCameraMatrix();
		m_camera->setScale(0.20f);

		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		// draw each sprite in sprites vector
		for (auto itr : m_sprites)
		{
			itr.draw(shaderProg);
		};

		shaderProg->unUse();
		
		// set OpenGL drawing window display to entire window.
		glViewport(0, 0, window->getWidth(), window->getHeight());

		// switch secondary buffer to be displayed on screen. 
		graphics->swapBuffers();
	}

	void TestApplication::processInput(Window* window)
	{
		if (m_inputManager->getArrowKeyY() > 0.0f) { 
			for (unsigned int i = 0; i < m_sprites.size(); i++) {
				m_sprites[i].position.y += 1.0f * SPEED;
			}
		}
		if (m_inputManager->getArrowKeyY() < 0.0f) { 
			for (unsigned int i = 0; i < m_sprites.size(); i++) {
				m_sprites[i].position.y -= 1.0f * SPEED;
			}
		}
		if (m_inputManager->getArrowKeyX() > 0.0f) {
			for (unsigned int i = 0; i < m_sprites.size(); i++) {
				m_sprites[i].position.x += 1.0f * SPEED;
			}
		}
		if (m_inputManager->getArrowKeyX() < 0.0f) {
			for (unsigned int i = 0; i < m_sprites.size(); i++) {
				m_sprites[i].position.x -= 1.0f * SPEED;
			}
		}

		
	}

}