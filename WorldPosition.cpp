#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
WorldPosition::WorldPosition() :
	mPosition( { 0.0f, 0.0f, 0.0f } ),
	mRotation( 0.0f, 0.0f, 0.0f, 1.0f ),
	mScale( { 1.0f, 1.0f, 1.0f } ),
	mpCombineMatrix( 0 )
{
	D3DXMatrixIdentity( &mPositionMatrix );
}

WorldPosition::WorldPosition( const WorldPosition& root )
{

}

WorldPosition::~WorldPosition()
{

}


//*************
//Getter-ы
//*************
D3DXMATRIX* WorldPosition::getWorldTransformMatrix()
{
	updatePositionMatrix();
	if( mpCombineMatrix )
		mPositionMatrix = mPositionMatrix * ( *mpCombineMatrix );
	return &mPositionMatrix;
}

D3DXVECTOR3 WorldPosition::getPosition()
{
	return mPosition;
}


//*************
//Setter-ы
//*************
void WorldPosition::setCombineMatrixPointer( D3DXMATRIX* pCombineMatrix )
{
	mpCombineMatrix = pCombineMatrix;
}


//*************
//Управление позицией
//*************
void WorldPosition::translate( float xPos, float yPos, float zPos )
{
	mPosition.x = xPos;
	mPosition.y = yPos;
	mPosition.z = zPos;
}

void WorldPosition::translateRel( float xAdd, float yAdd, float zAdd )
{
	translate( mPosition.x + xAdd, mPosition.y + yAdd, mPosition.z + zAdd );
}

void WorldPosition::move( float ahead, float right, float up )
{
	D3DXVECTOR3 vec( right, up, -ahead );
	GraphicsCore::RotateVectorQuaternion( &vec, &mRotation );
	mPosition += vec;
}

void WorldPosition::rotate( float yaw, float pitch, float roll )
{
	D3DXQuaternionRotationYawPitchRoll( &mRotation, yaw, pitch, roll );
}

void WorldPosition::rotateRel( float yawAdd, float pitchAdd, float rollAdd )
{
	D3DXQUATERNION localQuaternion;
	D3DXQuaternionRotationYawPitchRoll( &localQuaternion, yawAdd, pitchAdd, rollAdd );
	mRotation *= localQuaternion;
}

void WorldPosition::scale( float xScale, float yScale, float zScale )
{
	mScale.x = xScale;
	mScale.y = yScale;
	mScale.z = zScale;
}

void WorldPosition::scaleRel( float xAdd, float yAdd, float zAdd )
{
	scale( mScale.x + xAdd, mScale.y + yAdd, mScale.z + zAdd );
}


//*************
//Обновление матриц
//*************
void WorldPosition::updatePositionMatrix()
{
	D3DXMatrixIdentity( &mPositionMatrix );
	//Ловкость рук и никакого мошеничества
	float x2 = mRotation.x + mRotation.x;
	float y2 = mRotation.y + mRotation.y;
	float z2 = mRotation.z + mRotation.z;
	float xx = mRotation.x * x2;
	float xy = mRotation.x * y2;
	float xz = mRotation.x * z2;
	float yy = mRotation.y * y2;
	float yz = mRotation.y * z2;
	float zz = mRotation.z * z2;
	float wx = mRotation.w * x2;
	float wy = mRotation.w * y2;
	float wz = mRotation.w * z2;

	mPositionMatrix._11 = ( 1.0f - ( yy + zz ) ) * mScale.x;
	mPositionMatrix._12 = xy - wz;
	mPositionMatrix._13 = xz + wy;

	mPositionMatrix._21 = xy + wz;
	mPositionMatrix._22 = ( 1.0f - ( xx + zz ) ) * mScale.y;
	mPositionMatrix._23 = yz - wx;

	mPositionMatrix._31 = xz - wy;
	mPositionMatrix._32 = yz + wx;
	mPositionMatrix._33 = ( 1.0f - ( xx + yy ) ) * mScale.z;

	mPositionMatrix._41 = mPosition.x;
	mPositionMatrix._42 = mPosition.y;
	mPositionMatrix._43 = mPosition.z;
}