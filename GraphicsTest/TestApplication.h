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
#include <OGL/Texture2D.h>
#include <vector>

#if defined (ANDROID)
#include <android/asset_manager.h>
#endif

namespace engine
{
	class Window;
	class GraphicsSystem;
    class AAssetManager;

	class TestApplication :
		public GraphicsApplication
	{
	public:
	#if defined (ANDROID)
			TestApplication(Window* window, GraphicsSystem* graphics, AAssetManager* manager);
	#elif defined (_WIN32)
			TestApplication(Window* window, GraphicsSystem* graphics);
	#endif

		~TestApplication();

		void init();

		/// Updates application. Returns true, if application is running.
		virtual bool update(float deltaTime);
		
		/// Renders application.
		virtual void render(Window* window, GraphicsSystem* graphics);

	private:
		float						m_totalTime;
		std::vector<Shader*>		m_shaders;
		std::vector<Texture2D*>		m_textures;
	};

}

#endif

