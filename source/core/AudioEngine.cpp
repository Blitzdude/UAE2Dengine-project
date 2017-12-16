#include "core/AudioEngine.h"


namespace engine {
	AudioEngine::AudioEngine()
	{

	}
	AudioEngine::~AudioEngine()
	{

	}
	void AudioEngine::Play(wchar_t * soundFileName)
	{
		//Parse value soundFileName needs to be (L"sound.wav") format
		PlaySound(soundFileName, NULL, SND_FILENAME | SND_ASYNC);
	}
}