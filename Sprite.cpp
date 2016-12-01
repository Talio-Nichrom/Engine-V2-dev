#include "SMGraphics.h"

using namespace sm;

Sprite::Sprite( LPGraphicsCore pGraphicsCore,
				std::shared_ptr< Texture > pTexture,
				RECT textureRect,
				DWORD textureColor,
				std::shared_ptr< Font > pFont,
				DWORD fontColor,
				DWORD fontFormat )
{
	mpGraphicsCore = pGraphicsCore;
	mspTexture = pTexture;
	mTextureRect = textureRect;
	mTextureColor = textureColor;
	mspFont = pFont;
	mFontColor = fontColor;
	mFontFormat = fontFormat;
}

Sprite::~Sprite()
{

}


void Sprite::setTexture( std::shared_ptr< Texture > spTexture )
{
	mspTexture = spTexture;
}

void Sprite::setTextureRect( RECT rect )
{
	mTextureRect = rect;
}

void Sprite::setTextureColor( DWORD color )
{
	mTextureColor = color;
}

void Sprite::setFont( std::shared_ptr< Font > spFont )
{
	mspFont = spFont;
}

void Sprite::setFontColor( DWORD color )
{
	mFontColor = color;
}

void Sprite::setFontFormat( DWORD format )
{
	mFontFormat = format;
}


std::shared_ptr< Texture > Sprite::getTexture()
{
	return mspTexture;
}

RECT Sprite::getTextureRect()
{
	return mTextureRect;
}

DWORD Sprite::getTextureColor()
{
	return mTextureColor;
}

std::shared_ptr< Font > Sprite::getFont()
{
	return mspFont;
}

DWORD Sprite::getFontColor()
{
	return mFontColor;
}

DWORD Sprite::getFontFormat()
{
	return mFontFormat;
}


void Sprite::draw( RECT rect )
{
	if( mspTexture )
	{
		float xScale = float( rect.right - rect.left ) / float( mTextureRect.right - mTextureRect.left );
		float yScale = float( rect.bottom - rect.top ) / float( mTextureRect.bottom - mTextureRect.top );
		mspTexture->draw( (float)rect.left, (float)rect.top, mTextureRect, mTextureColor, xScale, yScale );
	}
}

void Sprite::print( LPCTSTR pText, RECT rect )
{
	if( mspFont )
	{
		mspFont->print( pText, rect, mFontColor, mFontFormat );
	}
}

void Sprite::print( tstring text, RECT rect )
{
	if( mspFont )
	{
		mspFont->print( text, rect, mFontColor, mFontFormat );
	}
}

void Sprite::print( int num, RECT rect )
{
	if( mspFont )
	{
		mspFont->print( num, rect, mFontColor, mFontFormat );
	}
}

void Sprite::print( float num, RECT rect )
{
	if( mspFont )
	{
		mspFont->print( num, rect, mFontColor, mFontFormat );
	}
}