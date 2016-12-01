#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Camera::Camera( LPGraphicsCore pGraphicsCore, float minDistance, float maxDistance ) :
	mpGraphicsCore( pGraphicsCore ),
	mPosition( { 0.0f, 0.0f, 0.0f } ),
	mMinDistance( minDistance ),
	mMaxDistance( maxDistance ),
	mFocusDistance( ( maxDistance - minDistance ) / 2 ),
	mwpWorldPosition(),
	mFollowing( false ),
	mLockedViewPoint( false )
{
	setOrientation( 0.0f, 0.0f );
}

Camera::Camera( const Camera& root )
{

}

Camera::~Camera()
{

}


//*************
//Управление синглтоном
//*************
LPCamera Camera::mpInstance = 0;

Camera* Camera::getInstance()
{
	return mpInstance;
}

void Camera::createCamera( LPGraphicsCore pGraphicsCore, float minDistance, float maxDistance )
{
	if( !mpInstance && pGraphicsCore )
	{
		mpInstance = new Camera( pGraphicsCore, minDistance, maxDistance );
	}
}

void Camera::destroy()
{
	s_deleteP( mpInstance );
}


//*************
//Активация камеры
//*************
void Camera::activate()
{
	initViewMatrix();
	mpGraphicsCore->setViewTransformation( &mViewMatrix );
}


//*************
//Setter-ы
//*************
void Camera::setPosition( D3DXVECTOR3 point )
{
	mPosition = point;
}

void Camera::setFocusDistance( float value )
{
	if( value > mMaxDistance )
		value = mMaxDistance;
	if( value < mMinDistance )
		value = mMinDistance;

	mFocusDistance = value;
}

void Camera::setOrientation( float horAngle, float vertAngle )
{
	mHorAngle = horAngle;
	mVertAngle = vertAngle;
	float rate = D3DX_PI / 180.0f;
	D3DXQuaternionRotationYawPitchRoll( &mOrientation, horAngle * rate, vertAngle * rate, 0.0f );
}

void Camera::enableLockingViewPoint( bool enable )
{
	mLockedViewPoint = enable;
}

void Camera::enableFollowing( bool enable )
{
	mFollowing = enable;
}


//*************
//Привязка камеры к позиции
//*************
void Camera::setWorldPositionPointer( std::weak_ptr< WorldPosition > spWorldPosition, bool lockViewPoint, bool following )
{
	mwpWorldPosition = spWorldPosition;
	mLockedViewPoint = lockViewPoint;
	mFollowing = following;
}

void Camera::release()
{
	mwpWorldPosition.reset();
	mLockedViewPoint = false;
	mFollowing = false;
}


//*************
//Управление камерой
//*************
void Camera::distance( float value )
{
	setFocusDistance( mFocusDistance + value );
}

void Camera::translate( D3DXVECTOR3 point )
{
	setPosition( mPosition + point );
}

void Camera::move( float ahead, float right, float up )
{
	D3DXVECTOR3 vec( right, up, ahead );
	D3DXQUATERNION quat;
	D3DXQuaternionInverse( &quat, &mOrientation );
	GraphicsCore::RotateVectorQuaternion( &vec, &quat );
	mPosition += vec;
}

void Camera::rotate( float horAngle, float vertAngle )
{
	setOrientation( mHorAngle + horAngle, mVertAngle + vertAngle );
}

//*************
//Getter-ы
//*************
D3DXVECTOR3 Camera::getPosition()
{
	return mPosition;
}


//*************
//Инициализация матрицы просмотра
//*************
void Camera::initViewMatrix()
{
	D3DXVECTOR3 vec = mPosition;
	D3DXQUATERNION quat = mOrientation;
	D3DXQuaternionInverse( &quat, &quat );

	if( !mwpWorldPosition.expired() )
	{
		if( mFollowing )
		{
			vec = mwpWorldPosition.lock()->getPosition();
			vec.y += 5.0f;
		}
		if( mLockedViewPoint )
		{
			if( mFollowing )
			{
				D3DXVECTOR3 vecDist( 0.0f, 0.0f, -mFocusDistance );
				GraphicsCore::RotateVectorQuaternion( &vecDist, &quat );
				vec += vecDist;
			}
			else
			{

			}
		}
	}

	mPosition = vec;
	D3DXMatrixIdentity( &mViewMatrix );
	D3DXMATRIX matrixTranslate;
	D3DXMatrixTranslation( &matrixTranslate, -mPosition.x, -mPosition.y, -mPosition.z );

	D3DXMATRIX matrixRot;

	D3DXMatrixRotationQuaternion( &matrixRot, &quat );

	D3DXMatrixMultiply( &mViewMatrix, &mViewMatrix, &matrixTranslate );
	D3DXMatrixMultiply( &mViewMatrix, &mViewMatrix, &matrixRot );
	int x = 0;
}