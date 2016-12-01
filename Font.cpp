#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Font::Font( LPGraphicsCore pGraphicsCore, LPCTSTR pName, LPCTSTR fontType, int size, DWORD color, bool italic, bool bold ) :
	mpGraphicsCore( pGraphicsCore ),
	mpFont( 0 ),
	mName( pName ),
	mColor( color ),
	mOutFormat( DT_LEFT | DT_TOP )
{
	ZeroMemory( &mFontDesc, sizeof( LOGFONT ) );
	ZeroMemory( &mWorkRect, sizeof( RECT ) );

	ua_tcscpy_s( mFontDesc.FaceName, fontType );

	mFontDesc.Height = -size;

	mFontDesc.Italic = italic;

	if( bold )
		mFontDesc.Weight = 700;

	mFontDesc.PitchAndFamily = DEFAULT_PITCH | FF_MODERN;
}

Font::Font( const Font& f )
{
}

Font::~Font()
{
	destroy();
}


//*************
//Операторы
//*************
Font& Font::operator=( Font& root )
{
	return *this;
}


//*************
//Методы ресурса
//*************
int Font::create()
{
	mpFont = mpGraphicsCore->createFont( &mFontDesc );

	if( !mpFont )
	{
		return 1;
	}

	return 0;
}

int Font::destroy()
{
	mpGraphicsCore = 0;
	s_release( mpFont );
	ZeroMemory( &mFontDesc, sizeof( D3DXFONT_DESC ) );
	ZeroMemory( &mWorkRect, sizeof( RECT ) );
	mColor = 0;

	return 0;
}

int Font::disable()
{
	return mpFont->OnLostDevice();
}

int Font::restore()
{
	return mpFont->OnResetDevice();
}


//*************
//Установка параметров вывода текста
//*************
void Font::setWorkRect( long x, long y, long width, long height )
{
	mWorkRect.left = x;
	mWorkRect.top = y;
	mWorkRect.right = mWorkRect.left + width;
	mWorkRect.bottom = mWorkRect.right + height;
}

void Font::setWorkRect( RECT rect )
{
	mWorkRect = rect;
}

void Font::setColor( DWORD color )
{
	mColor = color;
}

void Font::setOutFormat( DWORD format )
{
	mOutFormat = format;
}


//*************
//Вывод на экран
//*************
void Font::print( LPCTSTR pCharString )
{
	mpFont->DrawText( 0, pCharString, -1, &mWorkRect, mOutFormat, mColor );
}

void Font::print( int num )
{
	mpFont->DrawText( 0, toTstring( num ).c_str(), -1, &mWorkRect, mOutFormat, mColor );
}

void Font::print( tstring& string )
{
	mpFont->DrawText( 0, string.c_str(), -1, &mWorkRect, mOutFormat, mColor );
}

void Font::print( float num )
{
	mpFont->DrawText( 0, toTstring( num ).c_str(), -1, &mWorkRect, mOutFormat, mColor );
}

void Font::print( LPCTSTR pCharString, RECT rect, DWORD color, DWORD format )
{
	setWorkRect( rect );
	setColor( color );
	setOutFormat( format );
	print( pCharString );
}

void Font::print( int num, RECT rect, DWORD color, DWORD format )
{
	setWorkRect( rect );
	setColor( color );
	setOutFormat( format );
	print( num );
}

void Font::print( tstring& string, RECT rect, DWORD color, DWORD format )
{
	setWorkRect( rect );
	setColor( color );
	setOutFormat( format );
	print( string );
}

void Font::print( float num, RECT rect, DWORD color, DWORD format )
{
	setWorkRect( rect );
	setColor( color );
	setOutFormat( format );
	print( num );
}

void Font::print( LPCTSTR pCharString, long x, long y, long width, long height, DWORD color, DWORD format )
{
	setWorkRect( x, y, width, height );
	setColor( color );
	setOutFormat( format );
	print( pCharString );
}

void Font::print( int num, long x, long y, long width, long height, DWORD color, DWORD format )
{
	setWorkRect( x, y, width, height );
	setColor( color );
	setOutFormat( format );
	print( num );
}

void Font::print( tstring& string, long x, long y, long width, long height, DWORD color, DWORD format )
{
	setWorkRect( x, y, width, height );
	setColor( color );
	setOutFormat( format );
	print( string );
}

void Font::print( float num, long x, long y, long width, long height, DWORD color, DWORD format )
{
	setWorkRect( x, y, width, height );
	setColor( color );
	setOutFormat( format );
	print( num );
}


//*************
//Getter-ы
//*************
LPCTSTR Font::getName()
{
	return mName.c_str();
}