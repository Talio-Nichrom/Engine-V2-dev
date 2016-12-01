#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
GraphicsObject::GraphicsObject( LPGraphicsCore pGraphicsCore, LPCTSTR pModelName ) :
	mpGraphicsCore( pGraphicsCore ),
	mspMesh(),
	mpSkin( 0 ),
	mpSkeletInfo( 0 ),
	mpRootFrame( 0 ),
	mpAnimation( 0 )
{
	setModelName( pModelName );
	LPWorldPosition pWorldPosition = new WorldPosition();
	mspWorldPosition = std::shared_ptr< WorldPosition >( pWorldPosition );
}

GraphicsObject::GraphicsObject( const GraphicsObject& root )
{

}

GraphicsObject::~GraphicsObject()
{
	s_deleteP( mpAnimation );
	s_deleteP( mpRootFrame );
	s_deleteP( mpSkeletInfo );
	s_deleteP( mpSkin );
	mpGraphicsCore = 0;
}


//*************
//Клонирование объекта как модели - клон использует совместные ресурсы
//*************
GraphicsObject* GraphicsObject::clone()
{
	LPGraphicsObject pLocalGraphicsObject = new GraphicsObject( mpGraphicsCore );

	pLocalGraphicsObject->mModelName = mModelName;
	pLocalGraphicsObject->mspMesh = mspMesh;

	if( mpSkin )
		pLocalGraphicsObject->mpSkin = mpSkin->clone();

	if( mpSkeletInfo )
		pLocalGraphicsObject->mpSkeletInfo = mpSkeletInfo->clone();

	if( mpRootFrame )
		pLocalGraphicsObject->mpRootFrame = mpRootFrame->clone();

	if( mpAnimation )
		pLocalGraphicsObject->mpAnimation = mpAnimation->clone();

	pLocalGraphicsObject->bindFrameObjects();

	return pLocalGraphicsObject;
}


//*************
//Setter-ы
//*************
void GraphicsObject::setModelName( LPCTSTR pModelName )
{
	if( pModelName )
		mModelName = pModelName;
	else
		mModelName = _T( "BaseGraphicsObject" );
}

void GraphicsObject::setMeshPointer( LPMesh pMesh )
{
	mspMesh = std::shared_ptr< Mesh >( pMesh );
}

void GraphicsObject::setSkinPointer( LPSkin pSkin )
{
	s_deleteP( mpSkin );
	mpSkin = pSkin;
}

void GraphicsObject::setSkeletInfoPointer( LPSkeletInfo pSkeletInfo )
{
	s_deleteP( mpSkeletInfo );
	mpSkeletInfo = pSkeletInfo;
}

void GraphicsObject::setFramePointer( LPFrame pRootFrame )
{
	s_deleteP( mpRootFrame );
	mpRootFrame = pRootFrame;
}

void GraphicsObject::setAnimationPointer( LPAnimation pAnimation )
{
	s_deleteP( mpAnimation );
	mpAnimation = pAnimation;
}

void GraphicsObject::setParameters( LPCTSTR pModelName, LPMesh pMesh, LPSkin pSkin, LPSkeletInfo pSkeletInfo, LPFrame pRootFrame, LPAnimation pAnimation )
{
	setModelName( pModelName );
	setMeshPointer( pMesh );
	setSkinPointer( pSkin );
	setSkeletInfoPointer( pSkeletInfo );
	setFramePointer( pRootFrame );
	setAnimationPointer( pAnimation );
	bindFrameObjects();
}


//*************
//Getter-ы
//*************
std::weak_ptr< WorldPosition > GraphicsObject::getWorldPosition()
{
	return mspWorldPosition;
}

LPCTSTR GraphicsObject::getModelName()
{
	return mModelName.c_str();
}


//*************
//Привязка внутренних объектов
//*************
void GraphicsObject::bindFrameObjects()
{
	if( mpRootFrame )
	{
		mspWorldPosition->setCombineMatrixPointer( mpRootFrame->getMatrixCombined() );

		if( mpAnimation )
		{
			mpAnimation->bindFrames( mpRootFrame );
		}

		if( mpSkeletInfo )
		{
			for( LPFrame pLocalFrame = mpRootFrame->next(); pLocalFrame != 0; pLocalFrame = pLocalFrame->next() )
			{
				mpSkeletInfo->setBoneMatrixTransformation( pLocalFrame->getName(), pLocalFrame->getMatrixCombined() );
			}
		}
	}
}


//*************
//Отрисовка объекта
//*************
void GraphicsObject::draw()
{
	if( !mpGraphicsCore )
		return;

	mpGraphicsCore->setWorldTransformation( mspWorldPosition->getWorldTransformMatrix() );

	if( mspMesh )
	{
		mspMesh->updateMesh( mpSkeletInfo );

		for( DWORD i = 0; i < mspMesh->getGroupCount(); i++ )
		{
			if( mpSkin )
			{
				mpSkin->activate( i );
			}

			mspMesh->drawSubset( i );
		}

		mspMesh->updateMesh();
	}
}


//*************
//Обновление иерархии
//*************
void GraphicsObject::update( DWORD deltaTime )
{
	if( mpRootFrame )
	{
		mpRootFrame->toResetPose();
	}

	for( LPAnimation pLocalAnimation = mpAnimation; pLocalAnimation != 0; pLocalAnimation = pLocalAnimation->getNext() )
	{
		pLocalAnimation->update( deltaTime );
	}
}


//*************
//Управление анимацией
//*************
void GraphicsObject::enableAnimation( LPCTSTR pName, bool enable, bool repeat )
{
	LPAnimation pLocalAnimation = mpAnimation->getAnimation( pName );
	pLocalAnimation->enableAnimation( enable, repeat );
}

void GraphicsObject::pauseAnimation( LPCTSTR pName, bool pause )
{
	LPAnimation pLocalAnimation = mpAnimation->getAnimation( pName );
	if( pLocalAnimation->isEnabled() )
		pLocalAnimation->pause( pause );
}


//*************
//Управление положением и размером объекта
//*************
void GraphicsObject::translate( D3DXVECTOR3 point )
{
	mspWorldPosition.get()->translate( point.x, point.y, point.z );
}

void GraphicsObject::move( float ahead, float right, float up )
{
	mspWorldPosition.get()->move( ahead, right, up );
}

void GraphicsObject::rotate( float yaw, float pitch, float roll )
{
	mspWorldPosition.get()->rotate( yaw, pitch, roll );
}

void GraphicsObject::scale( D3DXVECTOR3 point )
{
	mspWorldPosition.get()->scale( point.x, point.y, point.z );
}

void GraphicsObject::translateRel( D3DXVECTOR3 point )
{
	mspWorldPosition.get()->translateRel( point.x, point.y, point.z );
}

void GraphicsObject::rotateRel( float yaw, float pitch, float roll )
{
	mspWorldPosition.get()->rotateRel( yaw, pitch, roll );
}

void GraphicsObject::scaleRel( D3DXVECTOR3 point )
{
	mspWorldPosition.get()->scaleRel( point.x, point.y, point.z );
}
