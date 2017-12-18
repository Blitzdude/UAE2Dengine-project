#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include<core/Object.h>
/*
Include
windows.h
mmsystem.h
to get "PlaySound" function to work
also you need "Winmm.lib" being added to project
*/
#if defined(_WIN32)
#include <windows.h>
#include <mmsystem.h>
#endif // _WIN32

namespace engine
{
	class AudioEngine : public Object
	{
	public:
		AudioEngine();
		~AudioEngine();
		void Play(wchar_t* soundFileName);
	private:
		
	};
}
#endif // _AUDIO_ENGINE_H_