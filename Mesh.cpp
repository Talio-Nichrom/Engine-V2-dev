#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Mesh::Mesh( LPGraphicsCore pGraphicsCore ) :
	mpGraphicsCore( pGraphicsCore ),
	mpVertexBuffer( 0 ),
	maOriginalVertexBuffer( 0 ),
	mpIndexBuffer( 0 ),
	mFaceGroupCount( 0 ),
	maFaceGroups( 0 )
{
}

Mesh::Mesh( const Mesh& root )
{
}

Mesh::~Mesh()
{
	s_deleteA( maFaceGroups );
	mFaceGroupCount = 0;
	s_deleteP( mpIndexBuffer );
	s_deleteA( maOriginalVertexBuffer );
	s_deleteP( mpVertexBuffer );
	mpGraphicsCore = 0;
}


//*************
//Setter-ы
//*************
void Mesh::setVertices( LPVertexBuffer pVertexBuffer )
{
	s_deleteP( mpVertexBuffer );
	mpVertexBuffer = pVertexBuffer;
}

void Mesh::setVertices( DWORD vertexCount, void* pVertices )
{
	if( mpGraphicsCore )
	{
		s_deleteP( mpVertexBuffer );
		mpVertexBuffer = new VertexBuffer( mpGraphicsCore, vertexCount, sizeof( Vertex ), SM_FVF, pVertices );
	}
}

void Mesh::setIndices( LPIndexBuffer pIndexBuffer )
{
	s_deleteP( mpIndexBuffer );
	mpIndexBuffer = pIndexBuffer;
}

void Mesh::setIndices( DWORD indexCount, WORD* pIndices )
{
	s_deleteP( mpIndexBuffer );
	mpIndexBuffer = new IndexBuffer( mpGraphicsCore, indexCount, pIndices );
}

void Mesh::setFaceGroupCount( DWORD groupCount )
{
	mFaceGroupCount = groupCount;
	if( maFaceGroups )
	{
		s_deleteA( maFaceGroups );
	}

	if( groupCount )
	{
		maFaceGroups = new Mesh::FaceGroup[ groupCount ];
		ZeroMemory( maFaceGroups, sizeof( FaceGroup ) * groupCount );
	}
}

void Mesh::setFaceGroup( DWORD id, DWORD offset, DWORD faceCount, DWORD indexBase )
{
	if( id < mFaceGroupCount && maFaceGroups )
	{
		maFaceGroups[ id ].offset = offset;
		maFaceGroups[ id ].count = faceCount;
		maFaceGroups[ id ].indexBase = indexBase;
	}
}


//*************
//Getter-ы
//*************
DWORD Mesh::getGroupCount()
{
	return mFaceGroupCount;
}


//*************
//Обновление вершин
//*************
void Mesh::initNormals()
{
	if( mpVertexBuffer && mpIndexBuffer )
	{
		LPVertex pVertices = 0;
		WORD* pIndices = 0;
		pVertices = (LPVertex) mpVertexBuffer->lockBuffer();
		pIndices = (WORD*) mpIndexBuffer->lockBuffer();

		if( pVertices && pIndices )
		{
			for( DWORD i = 0; i < mpIndexBuffer->getIndexCount(); i += 3 )
			{
				WORD i1 = pIndices[ i ];
				WORD i2 = pIndices[ i + 1 ];
				WORD i3 = pIndices[ i + 2 ];

				D3DXVECTOR3 vec1 = D3DXVECTOR3( pVertices[ i2 ].x - pVertices[ i1 ].x, pVertices[ i2 ].y - pVertices[ i1 ].y, pVertices[ i2 ].z - pVertices[ i1 ].z );
				D3DXVECTOR3 vec2 = D3DXVECTOR3( pVertices[ i3 ].x - pVertices[ i1 ].x, pVertices[ i3 ].y - pVertices[ i1 ].y, pVertices[ i3 ].z - pVertices[ i1 ].z );
				D3DXVECTOR3 normal;

				D3DXVec3Cross( &normal, &vec1, &vec2 );
				D3DXVec3Normalize( &normal, &normal );

				pVertices[ i1 ].nx += normal.x;
				pVertices[ i2 ].nx += normal.x;
				pVertices[ i3 ].nx += normal.x;
				pVertices[ i1 ].ny += normal.y;
				pVertices[ i2 ].ny += normal.y;
				pVertices[ i3 ].ny += normal.y;
				pVertices[ i1 ].nz += normal.z;
				pVertices[ i2 ].nz += normal.z;
				pVertices[ i3 ].nz += normal.z;
			}
		}

		for( DWORD i = 0; i<mpVertexBuffer->getVertexCount(); i++ )
		{
			D3DXVECTOR3 normal = D3DXVECTOR3( pVertices[ i ].nx, pVertices[ i ].ny, pVertices[ i ].nz );
			D3DXVec3Normalize( &normal, &normal );
			pVertices[ i ].nx = normal.x;
			pVertices[ i ].ny = normal.y;
			pVertices[ i ].nz = normal.z;
		}

		mpVertexBuffer->unlockBuffer();
		mpIndexBuffer->unlockBuffer();
	}
}

void Mesh::updateMesh( LPSkeletInfo pSkeletInfo )
{
	if( !mpVertexBuffer )
		return;

	LPVertex pVertices = (Vertex*) mpVertexBuffer->lockBuffer();

	if( pSkeletInfo )
	{
		if( !maOriginalVertexBuffer )
		{
			DWORD vertexCount = mpVertexBuffer->getVertexCount();
			maOriginalVertexBuffer = new Vertex[ vertexCount ];
			memcpy_s( maOriginalVertexBuffer, vertexCount * sizeof( Vertex ), pVertices, vertexCount * sizeof( Vertex ) );
		}
		else
		{
			DWORD vertexCount = mpVertexBuffer->getVertexCount();
			memcpy_s( pVertices, vertexCount * sizeof( Vertex ), maOriginalVertexBuffer, vertexCount * sizeof( Vertex ) );
		}

		const std::shared_ptr< Bone >* ppBones = pSkeletInfo->getBones();
		if( ppBones )
		{
			bool hasTransform = false;

			for( DWORD i = 0; i < pSkeletInfo->getBoneCount(); i++ )
			{
				LPBone pBone = ppBones[ i ].get();

				if( pBone )
				{
					D3DXMATRIX* matrixTransform = pSkeletInfo->getBoneMatrixTransformation( i );
					const WORD* pVertexIndices = pBone->getVertices();
					const float* pWeights = pBone->getWeights();

					if( pVertexIndices && pWeights && matrixTransform )
					{
						for( DWORD j = 0; j < pBone->getVertexCount(); j++ )
						{
							DWORD index = pVertexIndices[ j ];
							float weight = pWeights[ j ];

							if( weight != 0.0f )
								hasTransform = true;
							else
								continue;

							D3DXMATRIX vertexTransform = *matrixTransform;
							vertexTransform._11 -= 1.0f;
							vertexTransform._22 -= 1.0f;
							vertexTransform._33 -= 1.0f;
							vertexTransform._44 -= 1.0f;
							vertexTransform *= weight;
							vertexTransform._11 += 1.0f;
							vertexTransform._22 += 1.0f;
							vertexTransform._33 += 1.0f;
							vertexTransform._44 += 1.0f;

							D3DXVECTOR3 vec( pVertices[ index ].x, pVertices[ index ].y, pVertices[ index ].z );
							D3DXVec3TransformCoord( &vec, &vec, &vertexTransform );

							pVertices[ index ].x = vec.x;
							pVertices[ index ].y = vec.y;
							pVertices[ index ].z = vec.z;
						}
					}
				}
			}

			if( hasTransform )
			{
				initNormals();
			}
		}
	}
	else
	{
		if( maOriginalVertexBuffer )
		{
			DWORD vertexCount = mpVertexBuffer->getVertexCount();
			memcpy_s( pVertices, vertexCount * sizeof( Vertex ), maOriginalVertexBuffer, vertexCount * sizeof( Vertex ) );
		}
	}

	mpVertexBuffer->unlockBuffer();
}


//*************
//Рисование сетки
//*************
void Mesh::draw()
{
	if( mpGraphicsCore && mpVertexBuffer && mpIndexBuffer )
	{
		mpVertexBuffer->activate();
		mpIndexBuffer->activate();
		mpGraphicsCore->drawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, mpVertexBuffer->getVertexCount(), 0, mpIndexBuffer->getIndexCount() / 3 );
	}
}

void Mesh::drawSubset( DWORD id )
{
	if( mpGraphicsCore && mpVertexBuffer && mpIndexBuffer && mFaceGroupCount && maFaceGroups && id < mFaceGroupCount )
	{
		mpVertexBuffer->activate();
		mpIndexBuffer->activate();
		mpGraphicsCore->drawIndexedPrimitive( D3DPT_TRIANGLELIST, maFaceGroups[ id ].indexBase, 0, mpVertexBuffer->getVertexCount(), maFaceGroups[ id ].offset * 3, maFaceGroups[ id ].count );
	}
}
