#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_


#include <core/Object.h>

namespace engine {
	class InputManager : public Object
	{
	public:
		InputManager();
		~InputManager();


		void setMouseCoords(float x, float y);

		float getMouseX();
		float getMouseY();

		void setArrowKeyValues(float x, float y);

		float getArrowKeyX();
		float getArrowKeyY();

	private:

		float m_mouseX;
		float m_mouseY;

		float m_arrowkeyX;
		float m_arrowkeyY;
	};

	
}

#endif // !_INPUT_MANAGER_H_
