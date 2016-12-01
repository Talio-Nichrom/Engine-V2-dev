#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Bone::Bone( LPCTSTR pName, DWORD vertexCount ) :
	mVertexCount( vertexCount ),
	maVertices( 0 ),
	maWeights( 0 )
{
	setName( pName );
	if( mVertexCount > 0 )
	{
		maVertices = new WORD[ mVertexCount ];
		ZeroMemory( maVertices, mVertexCount * sizeof( WORD ) );
		maWeights = new float[ mVertexCount ];
		ZeroMemory( maWeights, mVertexCount * sizeof( float ) );
	}
}

Bone::Bone( const Bone& root )
{
}

Bone::~Bone()
{
	mVertexCount = 0;
	s_deleteA( maVertices );
	s_deleteA( maWeights );
}


//*************
//Конструкторы и деструктор
//*************
Bone& Bone::operator=( const Bone& root )
{
	return *this;
}


//*************
//Setter-ы
//*************
void Bone::setName( LPCTSTR pName )
{
	if( pName )
		mName = pName;
	else
		mName = _T( "Root" );
}

void Bone::setVertexCount( DWORD vertexCount )
{
	if( mVertexCount != vertexCount )
	{
		WORD* pNewVertices = 0;
		float* pNewWeights = 0;

		if( vertexCount > 0 )
		{
			pNewVertices = new WORD[ vertexCount ];
			pNewWeights = new float[ vertexCount ];
			ZeroMemory( pNewVertices, vertexCount * sizeof( WORD ) );
			ZeroMemory( pNewWeights, vertexCount * sizeof( float ) );
			if( maVertices && maWeights )
			{
				memcpy_s( pNewVertices, sizeof( WORD ) * vertexCount, maVertices, sizeof( WORD ) * mVertexCount );
				memcpy_s( pNewWeights, sizeof( float ) * vertexCount, maWeights, sizeof( float ) * mVertexCount );
			}
		}

		s_deleteA( maVertices );
		s_deleteA( maWeights );
		maVertices = pNewVertices;
		maWeights = pNewWeights;

		mVertexCount = vertexCount;
	}
}

void Bone::setVertices( WORD* pVertices, DWORD vertexCount )
{
	setVertexCount( vertexCount );

	if( mVertexCount > 0 )
	{
		memcpy_s( maVertices, sizeof( WORD ) * mVertexCount, pVertices, sizeof( WORD ) * mVertexCount );
	}
}

void Bone::setVerticesPointer( WORD* pVertices )
{
	s_deleteA( maVertices );
	maVertices = pVertices;
}

void Bone::setWeights( float* pWeights, DWORD vertexCount )
{
	setVertexCount( vertexCount );

	if( mVertexCount > 0 )
	{
		memcpy_s( maWeights, sizeof( float ) * mVertexCount, pWeights, sizeof( float ) * mVertexCount );
	}
}

void Bone::setWeightsPointer( float* pWeights )
{
	s_deleteA( maWeights );
	maWeights = pWeights;
}


//*************
//Getter-ы
//*************
LPCTSTR Bone::getName()
{
	return mName.c_str();
}

DWORD Bone::getVertexCount()
{
	return mVertexCount;
}

WORD* Bone::getVertices()
{
	return maVertices;
}

float* Bone::getWeights()
{
	return maWeights;
}