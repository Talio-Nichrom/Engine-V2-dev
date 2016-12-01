#include "SMGraphics.h"

using namespace sm;


//*************
//������������ � ����������
//*************
Skin::Skin( LPGraphicsCore pGraphicsCore, LPCTSTR pName, DWORD materialCount ) :
	mpGraphicsCore( pGraphicsCore ),
	mMaterialCount( materialCount ),
	mapMaterials( 0 )
{
	setName( pName );

	if( mMaterialCount )
	{
		mapMaterials = new LPMaterial[ mMaterialCount ];
		ZeroMemory( mapMaterials, mMaterialCount * sizeof( LPMaterial ) );
		for( DWORD i = 0; i < mMaterialCount; i++ )
		{
			mapMaterials[ i ] = new Material( mpGraphicsCore );
		}
	}
}

Skin::Skin( const Skin& root )
{
}

Skin::~Skin()
{
	for( DWORD i = 0; i < mMaterialCount; i++ )
	{
		s_deleteP( mapMaterials[ i ] );
	}
	s_deleteA( mapMaterials );

	mMaterialCount = 0;
}


//*************
//���������
//*************
Skin& Skin::operator=( Skin& root )
{
	return *this;
}


//*************
//������������ ������� � ������ ������������� ���������� ��������
//*************
Skin* Skin::clone()
{
	LPSkin pLocalSkin = new Skin( mpGraphicsCore, getName(), getMaterialCount() );

	for( DWORD i = 0; i < pLocalSkin->getMaterialCount(); i++ )
	{
		pLocalSkin->setMaterial( i, mapMaterials[ i ] );
	}

	return pLocalSkin;
}


//*************
//Setter-�
//*************
void Skin::setName( LPCTSTR pName )
{
	if( pName )
		mName = pName;
	else
		mName = _T( "BasicSkin" );
}

void Skin::setMaterial( DWORD id, LPMaterial pMaterial )
{
	if( id < mMaterialCount )
	{
		if( pMaterial )
		{
			s_deleteP( mapMaterials[ id ] );
			mapMaterials[ id ] = pMaterial->clone();
		}
		else
		{
			mapMaterials[ id ]->setBasicMaterial();
		}
	}
}

void Skin::setMaterialPointer( DWORD id, LPMaterial pMaterial )
{
	if( id < mMaterialCount )
	{
		if( pMaterial )
		{
			s_deleteP( mapMaterials[ id ] );
			mapMaterials[ id ] = pMaterial;
		}
		else
		{
			mapMaterials[ id ]->setBasicMaterial();
		}
	}
}

//*************
//Getter-�
//*************
LPCTSTR Skin::getName()
{
	return mName.c_str();
}

DWORD Skin::getMaterialCount()
{
	return mMaterialCount;
}

LPMaterial Skin::getMaterial( DWORD id )
{
	if( id < mMaterialCount )
	{
		return mapMaterials[ id ];
	}

	return 0;
}


//*************
//��������� ����������
//*************
void Skin::activate( DWORD id )
{
	if( id < mMaterialCount )
	{
		mapMaterials[ id ]->activate();
	}
}