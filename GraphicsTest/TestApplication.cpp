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
#include <core/ResourceManager.h>
#include <glm/glm.hpp>
#include <core/stb_image.h>


#include "TestApplication.h"

namespace engine
{
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
		// create Spritebatch and initialize it
		m_spriteBatch = new SpriteBatch();
		m_spriteBatch->init();
		
		

	}

	void TestApplication::initShaders()
	{
		getGraphicsSystem()->createNewShader("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag", m_assetManager); // indexed at 0
		auto shaderPtr = getGraphicsSystem()->getShader(0);

		shaderPtr->addAttribute("vPosition");
		shaderPtr->addAttribute("vTexCoord");


		shaderPtr->linkShaders();

	}

	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		//processInput(getWindow());

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
		
		glActiveTexture(GL_TEXTURE0);

		glm::vec2 coords = glm::vec2(m_inputManager->getMouseX(), m_inputManager->getMouseY());

		coords = m_camera->convertScreenToWorld(coords);
		
		(void)window;	
		


		auto shaderProg = graphics->getShader(0);

		shaderProg->use();

		//setCamera matrix
		GLint pLocation = shaderProg->getUniformLocation("MVP");
		glUniform1i(pLocation, 0);

		glm::mat4 cameraMatrix = m_camera->getCameraMatrix();

		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		m_spriteBatch->begin();

		glm::vec4 position(coords.x, coords.y, 100.0f, 100.0f);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		
		//create textures
		int w = 5, h = 5;
		static Texture2D fooTexture = ResourceManager::createTexture("mr_t.png", w, h, 4, m_assetManager);
		ColorRGBA8 color(255, 255, 255, 255);
		m_spriteBatch->draw(position, uv, fooTexture.id, 0.0f, color);
		

		m_spriteBatch->end();

		m_spriteBatch->renderBatch();

		// unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderProg->unUse();
		
		// set OpenGL drawing window display to entire window.
		glViewport(0, 0, window->getWidth(), window->getHeight());

		// switch secondary buffer to be displayed on screen. 
		graphics->swapBuffers();
	}

	void TestApplication::processInput(Window* window)
	{
	}
}