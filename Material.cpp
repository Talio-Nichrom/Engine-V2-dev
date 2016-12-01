#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Material::Material( LPGraphicsCore pGraphicsCore, LPCTSTR pName, D3DMATERIAL9* pMaterial, LPTexture pTexture ) :
	mpGraphicsCore( pGraphicsCore )
{
	setName( pName );
	mpMaterial = new D3DMATERIAL9;
	setMaterial( pMaterial );
	setTexture( pTexture );
}

Material::Material( const Material& root )
{
}

Material::~Material()
{
	s_deleteP( mpMaterial );
}


//*************
//Операторы
//*************
Material& Material::operator=( Material& root )
{
	return *this;
}


//*************
//Клонирование
//*************
Material* Material::clone()
{
	LPMaterial pLocalMaterial = new Material( mpGraphicsCore, mName.c_str(), mpMaterial );
	pLocalMaterial->mspTexture = mspTexture;
	return pLocalMaterial;
}


//*************
//Setter-ы
//*************
void Material::setName( LPCTSTR pName )
{
	if( pName )
		mName = pName;
	else
		mName = _T( "BasicMaterial" );
}

void Material::setMaterial( D3DMATERIAL9* pMaterial )
{
	if( pMaterial )
	{
		memcpy( mpMaterial, pMaterial, sizeof( D3DMATERIAL9 ) );
	}
	else
	{
		mpMaterial->Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		mpMaterial->Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		mpMaterial->Specular = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		mpMaterial->Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
		mpMaterial->Power = 1.0f;
	}
}

void Material::setTexture( std::shared_ptr< Texture > spTexture )
{
	mspTexture = spTexture;
}

void Material::setTexture( LPTexture pTexture )
{
	mspTexture = std::shared_ptr< Texture >( pTexture );
}

void Material::setBasicMaterial()
{
	setName( 0 );
	setMaterial();
	setTexture( 0 );
}

void Material::setMaterialDiffuseColor( DWORD color )
{
	mpMaterial->Diffuse = D3DXCOLOR( color );
}

void Material::setMaterialAmbientColor( DWORD color )
{
	mpMaterial->Ambient = D3DXCOLOR( color );
}

void Material::setMaterialSpecularColor( DWORD color )
{
	mpMaterial->Specular = D3DXCOLOR( color );
}

void Material::setMaterialEmmissiveColor( DWORD color )
{
	mpMaterial->Emissive = D3DXCOLOR( color );
}

void Material::setMaterialPower( float power )
{
	mpMaterial->Power = power;
}


//*************
//Getter-ы
//*************
LPCTSTR Material::getName()
{
	return mName.c_str();
}


//*************
//Активация материала для рисования
//*************
void Material::activate()
{
	if( mpGraphicsCore )
	{
		mpGraphicsCore->setMaterial( mpMaterial );
		if( mspTexture )
			mpGraphicsCore->setTexture( mspTexture->getTexture() );
		else
			mpGraphicsCore->setTexture( 0 );
	}
}