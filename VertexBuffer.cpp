#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
VertexBuffer::VertexBuffer( LPGraphicsCore pGraphicsCore, DWORD vertexCount, DWORD vertexSize, DWORD vertexFormat, void* pSrc ) :
	mpGraphicsCore( pGraphicsCore ),
	mpBuffer( 0 ),
	mVertexCount( vertexCount ),
	mVertexSize( vertexSize ),
	mVertexFormat( vertexFormat )
{
	if( pGraphicsCore && vertexCount && vertexSize && vertexFormat )
	{
		DWORD bufferSize = vertexCount * vertexSize;
		mpBuffer = mpGraphicsCore->createVertexBuffer( bufferSize, vertexFormat );

		if( mpBuffer )
		{
			void* pLocalPointer = 0;
			if( SUCCEEDED( mpBuffer->Lock( 0, bufferSize, &pLocalPointer, 0 ) ) )
			{
				if( pSrc )
				{
					memcpy_s( pLocalPointer, bufferSize, pSrc, bufferSize );
				}
				else
				{
					ZeroMemory( pLocalPointer, bufferSize );
				}
				mpBuffer->Unlock();
			}
		}
	}
}

VertexBuffer::VertexBuffer( const VertexBuffer& root )
{
}

VertexBuffer::~VertexBuffer()
{
	s_release( mpBuffer );
	mVertexCount = 0;
	mVertexSize = 0;
	mVertexFormat = 0;
	mpGraphicsCore = 0;
}


//*************
//Операторы
//*************
VertexBuffer& VertexBuffer::operator=( const VertexBuffer& root )
{
	return *this;
}


//*************
//Получение доступа к буферу для изменений
//*************
void* VertexBuffer::lockBuffer()
{
	if( mpBuffer )
	{
		void* pLocalPointer = 0;
		if( FAILED( mpBuffer->Lock( 0, mVertexCount * mVertexSize, &pLocalPointer, 0 ) ) )
			return 0;

		return pLocalPointer;
	}

	return 0;
}

void VertexBuffer::unlockBuffer()
{
	if( mpBuffer )
		mpBuffer->Unlock();
}


//*************
//Активация буфера как источника вершин
//*************
int VertexBuffer::activate()
{
	int result = 0;

	if( mpGraphicsCore && mpBuffer )
	{
		result += mpGraphicsCore->setFVF( mVertexFormat );
		result += mpGraphicsCore->setVertexSource( mpBuffer, 0, mVertexSize );
	}

	return result;
}


//*************
//Getter-ы
//*************
DWORD VertexBuffer::getVertexCount()
{
	return mVertexCount;
}

DWORD VertexBuffer::getVertexSize()
{
	return mVertexSize;
}

DWORD VertexBuffer::getVertexFormat()
{
	return mVertexFormat;
}
