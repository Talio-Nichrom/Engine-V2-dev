#include "SMGraphics.h"

using namespace sm;

//*************
//Конструкторы и деструктор
//*************
Texture::Texture( LPGraphicsCore pGraphicsCore, LPCTSTR pName, LPCTSTR pFileName ) :
	mpGraphicsCore( pGraphicsCore ),
	mpTexture( 0 ),
	mName( pName ),
	mFileName( pFileName ),
	mWidth( 0 ),
	mHeight( 0 )
{
}

Texture::Texture( const Texture& root )
{
}

Texture::~Texture()
{
	destroy();
}


//*************
//Операторы
//*************
Texture& Texture::operator=( Texture& root )
{
	return *this;
}


//*************
//Методы ресурса
//*************
int Texture::create()
{
	if( !mpGraphicsCore )
		return 1;

	if( !( mpTexture = mpGraphicsCore->createTextureFromFile( mFileName.c_str() ) ) )
	{
		return 2;
	}

	D3DXIMAGE_INFO localTextureInfo;
	D3DXGetImageInfoFromFile( mFileName.c_str(), &localTextureInfo );
	mWidth = localTextureInfo.Width;
	mHeight = localTextureInfo.Height;

	return 0;
}

int Texture::destroy()
{
	mHeight = 0;
	mWidth = 0;
	s_release( mpTexture );
	mpGraphicsCore = 0;

	return 0;
}


//*************
//Рисование текстуры
//*************
void Texture::draw( float xPosition, float yPosition, RECT workRect, DWORD color, float xScale, float yScale )
{
	if( mpGraphicsCore )
	{
		mpGraphicsCore->beginSprite();
		D3DXVECTOR3 localVector = D3DXVECTOR3( xPosition, yPosition, 0.0f );
		D3DXVECTOR3 localScaleVector = D3DXVECTOR3( xScale, yScale, 1.0f );
		mpGraphicsCore->drawSprite( mpTexture, &workRect, 0, &localVector, &localScaleVector, color );
		mpGraphicsCore->endSprite();
	}
}


//*************
//Getter-ы
//*************
LPDIRECT3DTEXTURE9 Texture::getTexture()
{
	return mpTexture;
}

LPCTSTR Texture::getName()
{
	return mName.c_str();
}

LPCTSTR Texture::getFileName()
{
	return mFileName.c_str();
}

DWORD Texture::getWidth()
{
	return mWidth;
}

DWORD Texture::getHeight()
{
	return mHeight;
}

LPDIRECT3DSURFACE9 Texture::getTextureSurface()
{
	LPDIRECT3DSURFACE9 pSurface = 0;

	if( mpTexture )
	{
		if( mpTexture->GetSurfaceLevel( 0, &pSurface ) != D3D_OK )
			return 0;
	}

	return pSurface;
}