/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _ENGINE_TEST_APPLICATION_H_
#define _ENGINE_TEST_APPLICATION_H_

#include <graphics/GraphicsApplication.h>
#include <core/Log.h>
#include <GLES2/gl2.h>
#include <OGL/Shader.h>
#include <core/InputManager.h>
#include <core/Camera2D.h>
#include <core/AudioEngine.h>
#include <graphics/Texture2D.h>
#include "Sprite.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

	//game state enum
	enum gameState {
		INIT,
		START,
		PLAYING,
		END
	};

	class Window;

	class GraphicsSystem;


	class TestApplication :
		public GraphicsApplication
	{
	public:
		TestApplication(Window* window, GraphicsSystem* graphics, InputManager* inputMgr, void* manager = nullptr);

		~TestApplication();

		void init();
		void initShaders();
		void initGame();

		/// Updates application. Returns true, if application is running.
		virtual bool update(float deltaTime);
		
		/// Renders application.
		virtual void render(Window* window, GraphicsSystem* graphics);

		virtual void processInput(Window* window);

		//Collision
		void DetectCollidingObjects();

	private:
		int						m_screenWidth;
		int						m_screenHeight;
		int						m_gameState = INIT;
		float					m_totalTime;
		void* 					m_assetManager;
		AudioEngine				m_audioEngine;
		InputManager*			m_inputManager;
		Camera2D*				m_camera;
		std::vector<Sprite>		m_sprites;
		int						m_score;
	};

}

#endif

