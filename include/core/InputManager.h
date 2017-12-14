#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_


#include <core/Object.h>

namespace engine {
	enum buttons {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		W,
		D,
		S,
		A,
		LAST
	};

	class InputManager : public Object
	{
	public:
		InputManager();
		~InputManager();


		void setMouseCoords(float x, float y);

		float getMouseX();
		float getMouseY();
		/*
		void setArrowKeyValues(float x, float y);

		float getArrowKeyX();
		float getArrowKeyY();
		*/

		//Inputmanager Ver. 2.0
		void setKeyPressedValue(int index, bool value);
		bool getKeyPressedValue(int index);

	private:

		float m_mouseX;
		float m_mouseY;

		/*
		float m_arrowkeyX;
		float m_arrowkeyY;
		*/

		//InputManager Ver. 2.0
		/*
		Up Arrow Key in 0 index
		Right Arrow Key in 1 index
		Down Arrow Key in 2 index
		Left Arrow Key in 3 index
		*/
		bool keyPressed[LAST] = { false };
	};

	
}

#endif // !_INPUT_MANAGER_H_
