
#include "core/InputManager.h"

namespace engine {

	InputManager::InputManager() : m_mouseX(0.0f), m_mouseY(0.0f), m_arrowkeyX(0.0f), m_arrowkeyY(0.0f)
	{
		
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::setMouseCoords(float x, float y) {
		m_mouseX = x;
		m_mouseY = y;
	}

	float InputManager::getMouseX() {
		return m_mouseX;
	}

	float InputManager::getMouseY() {
		return m_mouseY;
	}

	void InputManager::setArrowKeyValues(float x, float y)
	{
		m_arrowkeyX = x;
		m_arrowkeyY = y;
	}

	float InputManager::getArrowKeyX(){
		return m_arrowkeyX;
	}

	float InputManager::getArrowKeyY() {
		return m_arrowkeyY;
	}
}
