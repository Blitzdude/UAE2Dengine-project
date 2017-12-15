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
#include <windows.h>
#include <mmsystem.h>

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
#endif