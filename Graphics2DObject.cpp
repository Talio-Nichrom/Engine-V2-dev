#include "SMGraphics.h"

using namespace sm;

Graphics2DObject::Graphics2DObject( LPGraphicsCore pGraphicsCore, LONG x, LONG y, LONG width, LONG height ) :
	mpGraphicsCore( pGraphicsCore ),
	mXPosition( x ),
	mYPosition( y ),
	mWidth( width ),
	mHeight( height ),
	mCurrentSprite( 0 )
{
	mOffset = { 0 };
	mFontRect = { 0, 0, width, height };
}

Graphics2DObject::Graphics2DObject( const Graphics2DObject& root )
{
	mpGraphicsCore = root.mpGraphicsCore;
	mXPosition = root.mXPosition;
	mYPosition = root.mXPosition;
	mWidth = root.mWidth;
	mHeight = root.mHeight;
	mFontRect = root.mFontRect;
	mOffset = root.mOffset;
	mStateSprites = std::map< UINT, std::shared_ptr< Sprite > >( root.mStateSprites );
	mCurrentSprite = root.mCurrentSprite;
}

Graphics2DObject::~Graphics2DObject()
{
	mStateSprites.clear();
}


Graphics2DObject* Graphics2DObject::clone()
{
	Graphics2DObject* pResult = 0;

	pResult = new Graphics2DObject( mpGraphicsCore, mXPosition, mYPosition, mWidth, mHeight );
	pResult->mFontRect = mFontRect;
	pResult->mOffset = mOffset;
	pResult->mCurrentSprite = mCurrentSprite;
	pResult->mStateSprites = mStateSprites;
	pResult->setState( pResult->mCurrentSprite );

	return pResult;
}


void Graphics2DObject::setPosition( LONG x, LONG y )
{
	mXPosition = x;
	mYPosition = y;
}

void Graphics2DObject::setPosition( POINT pt )
{
	mXPosition = pt.x;
	mYPosition = pt.y;
}

void Graphics2DObject::setSize( LONG width, LONG height )
{
	mWidth = width;
	mHeight = height;
}

void Graphics2DObject::setFontOffsetRect( RECT rect )
{
	mFontRect = rect;
}

void Graphics2DObject::setOffset( LONG x, LONG y )
{
	mOffset.x = x;
	mOffset.y = y;
}

void Graphics2DObject::setOffset( POINT pt )
{
	mOffset = pt;
}

void Graphics2DObject::addOffset( LONG x, LONG y )
{
	mOffset.x += x;
	mOffset.y += y;
}

void Graphics2DObject::addOffset( POINT pt )
{
	mOffset.x += pt.x;
	mOffset.y += pt.y;
}

void Graphics2DObject::setStateSprite( UINT stateId, std::shared_ptr< Sprite > spSprite )
{
	mStateSprites.emplace( stateId, spSprite );
}

void Graphics2DObject::setState( UINT stateId )
{
	std::shared_ptr< Sprite > spSprite = getStateSprite( stateId );
	if( spSprite )
	{
		mCurrentSprite = stateId;
	}
	else
	{
		mCurrentSprite = -1;
	}
}


void Graphics2DObject::move( LONG x, LONG y )
{
	setPosition( mXPosition + x, mYPosition + y );
}


void Graphics2DObject::getPosition( LONG* x, LONG* y )
{
	if( x )
		*x = mXPosition;
	if( y )
		*y = mYPosition;
}

void Graphics2DObject::getPosition( POINT* pPt )
{
	if( pPt )
	{
		pPt->x = mXPosition;
		pPt->y = mYPosition;
	}
}

void Graphics2DObject::getSize( LONG* width, LONG* height )
{
	if( width )
		*width = mWidth;
	if( height )
		*height = mHeight;
}

void Graphics2DObject::getSize( POINT* pPt )
{
	if( pPt )
	{
		pPt->x = mWidth;
		pPt->y = mHeight;
	}
}

RECT Graphics2DObject::getVisibleRect()
{
	RECT rect;
	rect.left = mXPosition + mOffset.x;
	rect.right = mXPosition + mOffset.x + mWidth;
	rect.top = mYPosition + mOffset.y;
	rect.bottom = mYPosition + mOffset.y + mHeight;

	return rect;
}

RECT Graphics2DObject::getFontRect()
{
	RECT rect = mFontRect;
	rect.left += mXPosition + mOffset.x;
	rect.right += mXPosition + mOffset.x;
	rect.top += mYPosition + mOffset.y;
	rect.bottom += mYPosition + mOffset.y;
	return rect;
}

void Graphics2DObject::getOffset( LONG* x, LONG* y )
{
	if( x )
		*x = mOffset.x;
	if( y )
		*y = mOffset.y;
}

void Graphics2DObject::getOffset( POINT* pPt )
{
	if( pPt )
	{
		*pPt = mOffset;
	}
}

std::shared_ptr< Sprite > Graphics2DObject::getStateSprite( UINT stateId )
{
	return mStateSprites.at( stateId );
}

LPSprite Graphics2DObject::getCurrentSprite()
{
	return getStateSprite( mCurrentSprite ).get();
}


void Graphics2DObject::print( LPCTSTR pCharString )
{
	auto sprite = getStateSprite( mCurrentSprite );
	if( sprite )
	{
		sprite->print( pCharString, getFontRect() );
	}
}

void Graphics2DObject::print( int num )
{
	auto sprite = getStateSprite( mCurrentSprite );
	if( sprite )
	{
		sprite->print( num, getFontRect() );
	}
}

void Graphics2DObject::print( tstring& string )
{
	auto sprite = getStateSprite( mCurrentSprite );
	if( sprite )
	{
		sprite->print( string, getFontRect() );
	}
}

void Graphics2DObject::print( float num )
{
	auto sprite = getStateSprite( mCurrentSprite );
	if( sprite )
	{
		sprite->print( num, getFontRect() );
	}
}

void Graphics2DObject::draw( DWORD deltaTime )
{
	auto sprite = getStateSprite( mCurrentSprite );
	if( sprite )
	{
		sprite->draw( getVisibleRect() );
	}
}