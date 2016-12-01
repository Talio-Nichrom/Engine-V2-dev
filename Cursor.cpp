#include "SMGUI.h"

using namespace sm;

Cursor::Cursor() :
	mpGraphicsCore( 0 ),
	mspTexture( 0 ),
	mbVisible( true ),
	mLockedPosition( { 0 } )
{

}


void Cursor::setStaticCursor( LPGraphicsCore pGraphicsCore, std::shared_ptr< Texture > spTexture )
{
	mpGraphicsCore = pGraphicsCore;
	mspTexture = spTexture;
	init();
}


void Cursor::getCursorPosition( POINT* pPt )
{
	if( pPt )
	{
		if( mbVisible )
			GetCursorPos( pPt );
		else
			*pPt = mLockedPosition;
	}
}


void Cursor::setVisible( bool bVisible )
{
	
	if( mbVisible && !bVisible )
	{
		GetCursorPos( &mLockedPosition );
	}
	else if( !mbVisible && bVisible )
	{
		SetCursorPos( (int)mLockedPosition.x, (int)mLockedPosition.y );
	}

	mbVisible = bVisible;

	ShowCursor( bVisible );
	if( mpGraphicsCore )
		mpGraphicsCore->showCursor( bVisible );
}


void Cursor::init()
{
	if( mpGraphicsCore && mspTexture )
	{
		SetCursor( NULL );
		LPDIRECT3DSURFACE9 pTextureSurface = mspTexture->getTextureSurface();
		mpGraphicsCore->setCursor( pTextureSurface );
	}
	else
	{
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		mpGraphicsCore = 0;
		mspTexture = 0;
	}
}