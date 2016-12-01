#pragma once

#ifndef SM_CONTROL
#define SM_CONTROL
#define INITGUID

#include "SMBasics.h"
#include <windowsx.h>
#include <dinput.h>

#pragma comment( lib, "dinput8.lib" )

#define KDOWN	0x80
#define KUP		0x01
#define KPRESS	0x81

#define WM_KEYPRESS WM_USER + 1

#define KSTATE( kL, kN )( ( ( kL ) >> 7 ) | ( kN ) ) 

#define MOUSE_LK 300
#define MOUSE_RK 301
#define MOUSE_MK 302

#define UPD_QUIT	-1
#define UPD_ERR		1

namespace sm
{
	//*************
	//Контроллер внешних устройств
	//*************
	typedef class InputController
	{
		//Конструкторы и деструктор
		private:
			InputController( HINSTANCE hInstance, HWND hWnd );
			InputController( const InputController& );
			~InputController();
			InputController& operator=( const InputController& root );

		public:
			static InputController* getInstance();
			static InputController* create( HINSTANCE hInstance, HWND hWnd );
			static void destroy();

			bool getMouseMove( POINT* pPt );
			bool getMouseMove( LONG* x, LONG* y );
			LONG getMouseWheel();
			bool getMouseButtonState( LONG mkId );
			BYTE getMouseButtonEvent( LONG mkId );
			bool getKeyboardButtonState( LONG kId );
			BYTE getKeyboardButtonEvent( LONG kId );
			bool getNextMBEvent( UINT* eventType, UINT* buttonId );
			bool getNextKBEvent( UINT* eventType, UINT* buttonId );

			UINT update();

		private:
			static InputController* mpInstance;
			
			HINSTANCE mHInstance;
			HWND mHWnd;
			LPDIRECTINPUT8 mpDirectInput;
			LPDIRECTINPUTDEVICE8 mpDirectMouse;
			LPDIRECTINPUTDEVICE8 mpDirectKeyboard;
			DIMOUSESTATE mLastMouseState;
			DIMOUSESTATE mNewMouseState;
			BYTE mLastKeyboardState[ 256 ];
			BYTE mNewKeyboardState[ 256 ];
			UINT mMouseEventPointer;
			UINT mKeyboardEventPointer;

			bool readDevice( LPDIRECTINPUTDEVICE8 pDevice, void* pBuffer, long bufferSize );

	} *LPInputController;

	
}
#endif