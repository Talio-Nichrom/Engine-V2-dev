#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
IndexBuffer::IndexBuffer( LPGraphicsCore pGraphicsCore, DWORD indexCount, WORD* pSrc ) :
	mpGraphicsCore( pGraphicsCore ),
	mpBuffer( 0 ),
	mIndexCount( indexCount )
{
	if( pGraphicsCore && indexCount )
	{
		DWORD bufferSize = indexCount * sizeof( WORD );
		mpBuffer = mpGraphicsCore->createIndexBuffer( bufferSize );
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

IndexBuffer::IndexBuffer( const IndexBuffer& root )
{
}

IndexBuffer::~IndexBuffer()
{
	s_release( mpBuffer );
	mIndexCount = 0;
	mpGraphicsCore = 0;
}


//*************
//Получение доступа к буферу для изменений
//*************
IndexBuffer& IndexBuffer::operator=( const IndexBuffer& root )
{
	return *this;
}


//*************
//Получение доступа к буферу для изменений
//*************
WORD* IndexBuffer::lockBuffer()
{
	if( mpBuffer )
	{
		void* pLocalPointer = 0;
		if( FAILED( mpBuffer->Lock( 0, mIndexCount * sizeof( WORD ), &pLocalPointer, 0 ) ) )
			return 0;

		return (WORD*) pLocalPointer;
	}

	return 0;
}

void IndexBuffer::unlockBuffer()
{
	if( mpBuffer )
		mpBuffer->Unlock();
}


//*************
//Активация буфера как источника вершин
//*************
int IndexBuffer::activate()
{
	int result = 0;

	if( mpGraphicsCore && mpBuffer )
	{
		result += mpGraphicsCore->setIndexSource( mpBuffer );
	}

	return result;
}


//*************
//Getter-ы
//*************
DWORD IndexBuffer::getIndexCount()
{
	return mIndexCount;
}
