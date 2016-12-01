#include "SMGUI.h"

using namespace sm;

Button::Button( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR pText ) :
	Static( id, x, y, width, height, pText)
{

}

Button::Button( UINT id, LPCTSTR pText ) :
	Static( id, pText )
{

}

Button::~Button()
{

}

void Button::onMouseLeave()
{
	Sensor::onMouseLeave();
	mbPressed = false;
}

void Button::onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData )
{
	switch( message )
	{
		case WM_KEYDOWN:
		{
			if( lParam == 0 )
				mbPressed = true;
		}
		break;
		case WM_KEYUP:
		{
			if( mbPressed && lParam == 0 )
			{
				mbPressed = false;
				onEvent( EVENT_BUTTON, BUTTON_CLICK, 0, 0, 0 );
			}
		}
		break;
	}

	Sensor::onMouseEvent( message, wParam, lParam, pUserData );
}

void Button::draw( DWORD deltaTime )
{
	for( auto elem = mGElements.begin(); elem != mGElements.end(); elem++ )
	{
		auto el = ( *elem );
		if( el )
		{
			if( !mbEnabled )
			{
				el->setOffset( 0, 0 );
				el->setState( 3 );
			}
			else if( mbPressed )
			{
				el->setOffset( 1, 1 );
				el->setState( 2 );
			}
			else if( mbMouseOver )
			{
				el->setOffset( -1, -1 );
				el->setState( 1 );
			}
			else
			{
				el->setOffset( 0, 0 );
				el->setState( 0 );
			}
		}
	}

	Static::draw( deltaTime );
}