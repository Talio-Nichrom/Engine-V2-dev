#include "SMControl.h"

using namespace sm;

InputController* InputController::mpInstance = 0;

InputController::InputController( HINSTANCE hInstance, HWND hWnd ) :
	mHInstance( hInstance ),
	mHWnd( hWnd ),
	mpDirectInput( 0 ),
	mpDirectMouse( 0 ),
	mpDirectKeyboard( 0 ),
	mMouseEventPointer( 0 ),
	mKeyboardEventPointer( 0 )
{
	mLastMouseState = { 0 };
	mNewMouseState = { 0 };
	ZeroMemory( &mLastKeyboardState, sizeof( BYTE ) * 256 );
	ZeroMemory( &mNewKeyboardState, sizeof( BYTE ) * 256 );
}

InputController::InputController( const InputController& )
{
	//Not allowed
}

InputController::~InputController()
{
	mpDirectKeyboard->Unacquire();
	mpDirectMouse->Unacquire();
	s_release( mpDirectMouse );
	s_release( mpDirectKeyboard );
	s_release( mpDirectInput );
}

 
InputController* InputController::getInstance()
{
	return mpInstance;
}

InputController* InputController::create( HINSTANCE hInstance, HWND hWnd )
{
	mpInstance = new InputController( hInstance, hWnd );
	if( FAILED( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &mpInstance->mpDirectInput, 0 ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectInput->CreateDevice( GUID_SysKeyboard, &mpInstance->mpDirectKeyboard, 0 ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectInput->CreateDevice( GUID_SysMouse, &mpInstance->mpDirectMouse, 0 ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectMouse->SetDataFormat( &c_dfDIMouse ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	if( FAILED( mpInstance->mpDirectMouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) )
	{
		s_deleteP( mpInstance );
		return 0;
	}

	return mpInstance;
}

void InputController::destroy()
{
	s_deleteP( mpInstance );
}


bool InputController::getMouseMove( POINT* pPt )
{
	if( pPt )
	{
		pPt->x = mNewMouseState.lX;
		pPt->y = mNewMouseState.lY;
	}

	if( mNewMouseState.lX || mNewMouseState.lY )
		return true;

	return false;
}

bool InputController::getMouseMove( LONG* x, LONG* y )
{
	if( x )
		*x = mNewMouseState.lX;

	if( y )
		*y = mNewMouseState.lY;

	if( mNewMouseState.lX || mNewMouseState.lY )
		return true;

	return false;
}

LONG InputController::getMouseWheel()
{
	return mNewMouseState.lZ;
}

bool InputController::getMouseButtonState( LONG mkId )
{
	return (bool)( 0x80 & mNewMouseState.rgbButtons[ mkId ] );
}

BYTE InputController::getMouseButtonEvent( LONG mkId )
{
	return KSTATE( mLastMouseState.rgbButtons[ mkId ] & 0x80, mNewMouseState.rgbButtons[ mkId ] & 0x80 );
}

bool InputController::getKeyboardButtonState( LONG kId )
{
	return (bool)( 0x80 & mNewKeyboardState[ kId ] );
}

BYTE InputController::getKeyboardButtonEvent( LONG kId )
{
	return KSTATE( mLastKeyboardState[ kId ] & 0x80 , mNewKeyboardState[ kId ] & 0x80 );
}

bool InputController::getNextMBEvent( UINT* eventType, UINT* buttonId )
{
	while( mMouseEventPointer < 4 )
	{
		BYTE mouseButtonEvent = getMouseButtonEvent( (UINT)mMouseEventPointer );

		mMouseEventPointer++;

		UINT evType = 0;
		
		switch( mouseButtonEvent )
		{
			case KUP:
			{
				if( eventType )
					*eventType = WM_KEYUP;

				if( buttonId )
					*buttonId = (UINT) ( mMouseEventPointer - 1 );

				return true;
			}
			case KDOWN:
			{
				if( eventType )
					*eventType = WM_KEYDOWN;

				if( buttonId )
					*buttonId = (UINT) ( mMouseEventPointer - 1 );

				return true;
			}
			case KPRESS:
			{
				if( eventType )
					*eventType = WM_KEYPRESS;

				if( buttonId )
					*buttonId = (UINT) ( mMouseEventPointer - 1 );

				return true;
			}
		}

	}

	return false;
}

bool InputController::getNextKBEvent( UINT* eventType, UINT* buttonId )
{
	while( mKeyboardEventPointer < 256 )
	{
		BYTE keyboardButtonEvent = getKeyboardButtonEvent( (LONG) mKeyboardEventPointer );

		mKeyboardEventPointer++;

		switch( keyboardButtonEvent )
		{
			case KUP:
			{
				if( eventType )
					*eventType = WM_KEYUP;

				if( buttonId )
					*buttonId = (UINT) ( mKeyboardEventPointer - 1 );

				return true;
			}
			case KDOWN:
			{
				if( eventType )
					*eventType = WM_KEYUP;

				if( buttonId )
					*buttonId = (UINT) ( mKeyboardEventPointer - 1 );

				return true;
			}
			case KPRESS:
			{
				if( eventType )
					*eventType = WM_KEYPRESS;

				if( buttonId )
					*buttonId = (UINT) ( mKeyboardEventPointer - 1 );

				return true;
			}
		}
	}

	return false;
}


UINT InputController::update()
{
	memcpy_s( mLastKeyboardState, sizeof( BYTE ) * 256, mNewKeyboardState, sizeof( BYTE ) * 256 );
	memcpy_s( &mLastMouseState, sizeof( mLastMouseState ), &mNewMouseState, sizeof( mNewMouseState ));
	mMouseEventPointer = 0;
	mKeyboardEventPointer = 0;

	if( !readDevice( mpDirectKeyboard, (void*) mNewKeyboardState, sizeof( BYTE ) * 256 ) )
		return UPD_ERR;

	if( !readDevice( mpDirectMouse, &mNewMouseState, sizeof( mNewMouseState ) ) )
		return UPD_ERR;

	return 0;
}


bool InputController::readDevice( LPDIRECTINPUTDEVICE8 pDevice, void* pBuffer, long bufferSize )
{
	HRESULT hr;

	while( true )
	{
		pDevice->Poll();

		if( SUCCEEDED( hr = pDevice->GetDeviceState( bufferSize, (LPVOID) pBuffer ) ) )
			break;

		if( hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED )
			return false;

		if( FAILED( pDevice->Acquire() ) )
			return false;
	}

	return true;
}