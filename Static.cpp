#include "SMGUI.h"

using namespace sm;

Static::Static( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR pText ) :
	Sensor( id, x, y, width, height )
{
	mWindowText = pText;
}

Static::Static( UINT id, LPCTSTR pText ):
	Sensor( id )
{
	mWindowText = pText;
}

Static::~Static()
{

}


void Static::setText( LPCTSTR pText )
{
	mWindowText = pText;
}


LPCTSTR Static::getText()
{
	return mWindowText.c_str();
}


void Static::draw( DWORD deltaTime )
{
	Sensor::draw( deltaTime );
	if( mbVisible && mGElements.size() > 0 )
	{
		if( auto elem = mGElements.at( 0 ) )
			elem->print( mWindowText );
	}
}