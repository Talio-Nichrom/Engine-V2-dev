#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Light::Light( LIGHT type, float x, float y, float z, float red, float green, float blue, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ )
{
	Type = (D3DLIGHTTYPE) type;
	Position = D3DXVECTOR3( x, y, z );

	Specular.a = Diffuse.a = Ambient.a = 0.0f;
	Specular.r = Diffuse.r = Ambient.r = red;
	Specular.g = Diffuse.g = Ambient.g = green;
	Specular.b = Diffuse.b = Ambient.b = blue;

	Range = range;

	Falloff = falloff;
	Phi = phi;
	Theta = theta;

	Attenuation0 = 0.0f;
	Attenuation1 = 0.0f;
	Attenuation2 = 0.0f;

	D3DXVECTOR3 localDirectionVector( directionX, directionY, directionZ );
	D3DXVec3Normalize( (D3DXVECTOR3*) &Direction, &localDirectionVector );
}

Light::Light( const Light& aLight )
{
	Type = aLight.Type;
	Position = aLight.Position;

	Diffuse.a = aLight.Diffuse.a;
	Diffuse.r = aLight.Diffuse.r;
	Diffuse.g = aLight.Diffuse.g;
	Diffuse.b = aLight.Diffuse.b;

	Ambient.a = aLight.Ambient.a;
	Ambient.r = aLight.Ambient.r;
	Ambient.g = aLight.Ambient.g;
	Ambient.b = aLight.Ambient.b;

	Range = aLight.Range;

	Falloff = aLight.Falloff;
	Phi = aLight.Phi;
	Theta = aLight.Theta;
}

Light::~Light()
{

}


//*************
//Создание источников света
//*************
Light* Light::createLightPoint( float x, float y, float z, float red, float green, float blue, float range )
{
	return new Light( LIGHT_POINT, x, y, z, red, green, blue, range, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

Light* Light::createLightSpot( float x, float y, float z, float red, float green, float blue, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ )
{
	return new Light( LIGHT_SPOT, x, y, z, red, green, blue, range, falloff, phi, theta, directionX, directionY, directionZ );
}

Light* Light::createLightDirectional( float red, float green, float blue, float directionX, float directionY, float directionZ )
{
	return new Light( LIGHT_DIRECTIONAL, 0.0f, 0.0f, 0.0f, red, green, blue, 0.0f, 0.0f, 0.0f, 0.0f, directionX, directionY, directionZ );
}

Light* Light::createLightPointWhite( float x, float y, float z, float range )
{
	return new Light( LIGHT_POINT, x, y, z, 1.0f, 1.0f, 1.0f, range, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

Light* Light::createLightSpotWhite( float x, float y, float z, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ )
{
	return new Light( LIGHT_SPOT, x, y, z, 1.0f, 1.0f, 1.0f, range, falloff, phi, theta, directionX, directionY, directionZ );
}

Light* Light::createLightDirectionalWhite( float directionX, float directionY, float directionZ )
{
	return new Light( LIGHT_DIRECTIONAL, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, directionX, directionY, directionZ );
}


//*************
//Setter-ы
//*************
void Light::setLightType( LIGHT type )
{
	Type = (D3DLIGHTTYPE) type;
}

void Light::setPosition( float x, float y, float z )
{
	Position = D3DXVECTOR3( x, y, z );
}

void Light::setAmbientColor( float red, float green, float blue )
{
	Ambient.r = red;
	Ambient.g = green;
	Ambient.b = blue;
}

void Light::setDiffuseColor( float red, float green, float blue )
{
	Diffuse.r = red;
	Diffuse.g = green;
	Diffuse.b = blue;
}

void Light::setRange( float range )
{
	Range = range;
}

void Light::setCone( float falloff, float phi, float theta )
{
	Falloff = falloff;
	Phi = phi;
	Theta = theta;
}

void Light::setDirection( float directionX, float directionY, float directionZ )
{
	Direction = D3DXVECTOR3( directionX, directionY, directionZ );
}