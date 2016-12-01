#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
SkeletInfo::SkeletInfo( LPCTSTR pName, DWORD boneCount ) :
	mName( pName ),
	mBoneCount( boneCount ),
	maspBones( 0 ),
	mapBoneMatrices( 0 )
{
	if( boneCount )
	{
		maspBones = new std::shared_ptr< Bone >[ mBoneCount ];
		mapBoneMatrices = new D3DXMATRIX*[ mBoneCount ];
		ZeroMemory( mapBoneMatrices, sizeof( D3DXMATRIX* ) * mBoneCount );
	}
}

SkeletInfo::SkeletInfo( const SkeletInfo& root )
{
}

SkeletInfo::~SkeletInfo()
{
	if( mBoneCount > 0 )
	{
		s_deleteA( maspBones );
		s_deleteA( mapBoneMatrices );
	}
	mBoneCount = 0;
}


//*************
//Операторы
//*************
SkeletInfo& SkeletInfo::operator=( const SkeletInfo& root )
{
	return *this;
}


//*************
//Клонирование объекта с учетом использования совместных ресурсов
//*************
SkeletInfo* SkeletInfo::clone()
{
	LPSkeletInfo pLocalSkeletInfo = new SkeletInfo( mName.c_str(), mBoneCount );

	for( DWORD i = 0; i < mBoneCount; i++ )
	{
		pLocalSkeletInfo->maspBones[ i ] = maspBones[ i ];
	}

	return pLocalSkeletInfo;
}


//*************
//Setter-ы
//*************
void SkeletInfo::setName( LPCTSTR pName )
{
	if( pName )
		mName = pName;
	else
		mName = _T( "BasikSkelet" );
}

void SkeletInfo::setBone( LPBone pBone, DWORD boneIndex )
{
	if( boneIndex >= mBoneCount )
		return;

	maspBones[ boneIndex ] = std::shared_ptr< Bone >( pBone );
}

void SkeletInfo::setBoneMatrixTransformation( LPCTSTR pBoneName, D3DXMATRIX* pMatrixTransformation )
{
	if( !pBoneName )
		return;

	for( DWORD i = 0; i < mBoneCount; i++ )
	{
		LPBone pLocalBone = maspBones[ i ].get();
		if( pLocalBone )
		{
			if( tstring( pLocalBone->getName() ) == tstring( pBoneName ) )
			{
				mapBoneMatrices[ i ] = pMatrixTransformation;
				return;
			}
		}
	}
}


//*************
//Getter-ы
//*************
LPCTSTR SkeletInfo::getName()
{
	return mName.c_str();
}

DWORD SkeletInfo::getBoneCount()
{
	return mBoneCount;
}

const std::shared_ptr< Bone >* SkeletInfo::getBones()
{
	return maspBones;
}

D3DXMATRIX* SkeletInfo::getBoneMatrixTransformation( DWORD boneIndex )
{
	if( boneIndex >= mBoneCount )
		return 0;

	return mapBoneMatrices[ boneIndex ];
}