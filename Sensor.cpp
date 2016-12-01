#include "SMGUI.h"

using namespace sm;

Sensor::Sensor( UINT id, LONG x, LONG y, LONG width, LONG height) :
	mID( id ),
	mSensorXPosition( x ),
	mSensorYPosition( y ),
	mSensorWidth( width ),
	mSensorHeight( height ),
	mbEnabled( true ),
	mbVisible( true ),
	mbHasFocus( false ),
	mbMouseOver( false )
{
	mSensorSensitiveRect = { 0, 0, width, height };
}

Sensor::Sensor( UINT id ) :
	Sensor( id, 0, 0, 10, 10 )
{

}

Sensor::~Sensor()
{
	mGElements.clear();
	mHandlers.clear();
}


void Sensor::onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pUserData )
{
	mHandlers[ evType ]( message, wParam, lParam, pUserData );
}

void Sensor::onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData )
{
	mHandlers[ EVENT_MOUSE ]( message, wParam, lParam, pUserData );
}

void Sensor::onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData )
{
	mHandlers[ EVENT_KEYBOARD ]( message, wParam, lParam, pUserData );
}


bool Sensor::canHaveFocus()
{
	return false;
}

void Sensor::onFocusIn()
{
	mbHasFocus = true;
}

void Sensor::onFocusOut()
{
	mbHasFocus = false;
}

void Sensor::onMouseEnter()
{
	mbMouseOver = true;
}

void Sensor::onMouseLeave()
{
	mbMouseOver = false;
}


void Sensor::setSensorPosition( LONG x, LONG y )
{
	for( auto elem = mGElements.begin(); elem != mGElements.end(); elem++ )
	{
		POINT pt;
		( *elem )->getPosition( &pt );
		pt.x += x - mSensorXPosition;
		pt.y += y - mSensorYPosition;
		( *elem )->setPosition( pt );
	}

	mSensorXPosition = x;
	mSensorYPosition = y;
}

void Sensor::setSensorPosition( POINT pt )
{
	setSensorPosition( pt.x, pt.y );
}

void Sensor::setSensorSize( LONG width, LONG height )
{
	mSensorWidth = width;
	mSensorHeight = height;
}

void Sensor::setSensorSensitiveRect( RECT rect )
{
	mSensorSensitiveRect = rect;
}

void Sensor::addGElement( std::shared_ptr< G2DObject > spGElement )
{
	if( spGElement )
	{
		spGElement->move( mSensorXPosition, mSensorYPosition );
	}
	mGElements.push_back( spGElement );
}

void Sensor::setEnable( bool bEnabled )
{
	mbEnabled = bEnabled;
}

void Sensor::setVisible( bool bVisible )
{
	mbVisible = bVisible;
}

void Sensor::setHandler( UINT evType, EventDelegate handler )
{
	mHandlers.emplace( evType, handler );
}

void Sensor::setMouseHandler( EventDelegate handler )
{
	mHandlers.emplace( EVENT_MOUSE, handler );
}

void Sensor::setKeyboardHandler( EventDelegate handler )
{
	mHandlers.emplace( EVENT_KEYBOARD, handler );
}


UINT Sensor::getSensorID()
{
	return mID;
}

void Sensor::getSensorPosition( LONG* x, LONG* y )
{
	if( x )
		*x = mSensorXPosition;
	if( y )
		*y = mSensorYPosition;
}

void Sensor::getSensorPosition( POINT* pPt )
{
	if( pPt )
	{
		pPt->x = mSensorXPosition;
		pPt->y = mSensorYPosition;
	}
}

void Sensor::getSensorSize( LONG* width, LONG* height )
{
	if( width )
		*width = mSensorWidth;
	if( height )
		*height = mSensorHeight;
}

void Sensor::getSensorSize( POINT* pPt )
{
	if( pPt )
	{
		pPt->x = mSensorWidth;
		pPt->y = mSensorHeight;
	}
}

std::shared_ptr< G2DObject > Sensor::getGElement( UINT id )
{
	return mGElements.at( id );
}

bool Sensor::isEnable()
{
	return mbEnabled;
}

bool Sensor::isVisible()
{
	return mbVisible;
}

bool Sensor::containsPoint( POINT pt )
{
	RECT rect = mSensorSensitiveRect;
	rect.left += mSensorXPosition;
	rect.right += mSensorXPosition;
	rect.top += mSensorYPosition;
	rect.bottom += mSensorYPosition;
	return (bool)PtInRect( &rect, pt );
}


void Sensor::draw( DWORD deltaTime )
{
	if( mbVisible )
	{
		for( auto elem = mGElements.begin(); elem != mGElements.end(); elem++ )
		{
			( *elem )->draw( deltaTime );
		}
	}
}