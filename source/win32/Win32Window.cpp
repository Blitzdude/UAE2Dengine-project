/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <Win32Window.h>
#include <graphics/GraphicsApplication.h>
#include <EGL/eglplatform.h>
#include <iostream>

namespace engine
{
	LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;                       // handle to device context 
		RECT windowRect;               // window area rectangle 
		POINT ptWindowUL;              // window upper left corner
		POINT ptWindowLR;			   // window lower right corner 
		static POINTS ptsBegin;        // beginning point 
		static POINTS ptsEnd;          // new endpoint 
		static POINTS ptsPrevEnd;      // previous endpoint 
		static BOOL fPrevLine = FALSE; // previous line flag 

		LRESULT  lRet = 1;
		switch (uMsg)
		{
		case WM_CREATE:
			break;

		case WM_PAINT:
		{
			Win32Window *window = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);			
			// Call application render and pass pointer to Graphics-object.
 			window->getApplication()->render(window, window->getGraphics());
			ValidateRect(window->getNativeWindow(), NULL);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_CHAR:
		{
			POINT      point;
	//		Win32Window *esContext = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
			GetCursorPos(&point);
		}
		break;

		// Input
		case WM_LBUTTONDOWN:
		{
		
		
		
		}
		break;

		case WM_MOUSEMOVE:
		{

			// get window context 
			Win32Window *window = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

			// Capture mouse input. 

			static POINT cursor;
			GetCursorPos(&cursor);
			ScreenToClient(hWnd, &cursor);

			// set the mouse coordinates for the input manager. 
			window->getInputManager()->setMouseCoords((float)cursor.x, (float)cursor.y);

			
		}
		break;

		case WM_LBUTTONUP: 
		{
			// The user has finished drawing the line. Reset the 
			// previous line flag, release the mouse cursor, and 
			// release the mouse capture. 
		}
		break;

		case WM_KEYDOWN:
		{
			// get window context 
			Win32Window *window = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

			switch (wParam)
			{
			case VK_UP:
				//window->getInputManager()->setArrowKeyValues(0.0f, 1.0f);
				window->getInputManager()->setKeyPressedValues(UP, true);
				break;
			case VK_RIGHT:
				//window->getInputManager()->setArrowKeyValues(1.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(RIGHT, true);
				break;
			case VK_DOWN:
				//window->getInputManager()->setArrowKeyValues(0.0f, -1.0f);
				window->getInputManager()->setKeyPressedValues(DOWN, true);
				break;
			case VK_LEFT:
				//window->getInputManager()->setArrowKeyValues(-1.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(LEFT, true);
				break;
			case 87:
				window->getInputManager()->setKeyPressedValues(W, true);
				break;
			case 68:
				window->getInputManager()->setKeyPressedValues(D, true);
				break;
			case 83:
				window->getInputManager()->setKeyPressedValues(S, true);
				break;
			case 65:
				window->getInputManager()->setKeyPressedValues(A, true);
				break;
			default:
				printf("Not ArrowKey Pressed: %d \n", wParam );
				break;
			}
			break;
		}
		break;

		case WM_KEYUP:
		{
			// get window context 
			Win32Window *window = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

			switch (wParam)
			{
			case VK_UP:
				//window->getInputManager()->setArrowKeyValues(0.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(0, false);
				break;
			case VK_RIGHT:
				//window->getInputManager()->setArrowKeyValues(0.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(1, false);
				break;
			case VK_DOWN:
				//window->getInputManager()->setArrowKeyValues(0.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(2, false);
				break;
			case VK_LEFT:
				//window->getInputManager()->setArrowKeyValues(0.0f, 0.0f);
				window->getInputManager()->setKeyPressedValues(3, false);
				break;
			case 87:
				window->getInputManager()->setKeyPressedValues(W, false);
				break;
			case 68:
				window->getInputManager()->setKeyPressedValues(D, false);
				break;
			case 83:
				window->getInputManager()->setKeyPressedValues(S, false);
				break;
			case 65:
				window->getInputManager()->setKeyPressedValues(A, false);
				break;
			}
			break;
		}
		break;

		default:
			lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}

		return lRet;
	}



	Win32Window::Win32Window(int width, int height, const std::wstring& title)
		: Window()
		, m_hwnd(NULL)
		, m_active(false) // Assume not yet active
	{
		setSize(width, height); // Set size to base class

		WNDCLASS wndclass = { 0 };
		DWORD    wStyle = 0;
		RECT     windowRect;
		HINSTANCE hInstance = GetModuleHandle(NULL);
		
		wndclass.style = CS_OWNDC;
		wndclass.lpfnWndProc = (WNDPROC)WindowProc;
		wndclass.hInstance = hInstance;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndclass.lpszClassName = L"opengles2.0";

		if (!RegisterClass(&wndclass))
			return; // fail!
		
		wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

		// Adjust the window rectangle so that the client area has
		// the correct number of pixels
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = width - windowRect.left;
		windowRect.bottom = height - windowRect.top;

		AdjustWindowRect(&windowRect, wStyle, FALSE);
		
		m_hwnd = CreateWindow(
			L"opengles2.0",
			title.c_str(),
			wStyle,
			0,
			0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			hInstance,
			NULL);

		// Set the ESContext* to the GWL_USERDATA so that it is available to the 
		// ESWindowProc
		SetWindowLongPtr(m_hwnd, GWL_USERDATA, (LONG)(LONG_PTR)this);
		
		if (m_hwnd == NULL)
		{
			// TODO: Unregister window class if fail
			return; // fail!
		}

		ShowWindow(m_hwnd, TRUE);

		// success!
		m_active = true;
	}


	Win32Window::~Win32Window()
	{
	}


	// Returns native window handle
	EGLNativeWindowType Win32Window::getNativeWindow() const
	{
		return m_hwnd;
	}


	bool Win32Window::updateMessages()
	{
		MSG msg = { 0 };
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		if (gotMsg)
		{
			if (msg.message == WM_QUIT)
			{
				m_active = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			SendMessage(getNativeWindow(), WM_PAINT, 0, 0);
		}
		return m_active;
	}
}