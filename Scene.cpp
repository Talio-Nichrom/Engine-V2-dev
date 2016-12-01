#include "SMEngine.h"

using namespace sm;

Scene::Scene( LPGraphicsCore pGraphicsCore ) :
	mpGraphicsCore( pGraphicsCore ),
	mMoving( 0 )
{
	Camera::createCamera( mpGraphicsCore, 0, 1000 );
	mpCamera = Camera::getInstance();
	mpCamera->setPosition( { 0.0f, 5.0f, 200.0f } );
	mpCamera->setFocusDistance( 30.0f );
}

Scene::~Scene()
{
	Camera::destroy();
}


void Scene::processScene( DWORD deltaTime )
{
	mpCamera->activate();
	mspSkybox->translate( mspManagedObject->getWorldPosition().lock()->getPosition() );

	if( mMoving )
	{
		mspManagedObject->move( mMoving, 0.0f, 0.0f );
		mMoving = 0.0f;
	}
	else
	{
		mspManagedObject->enableAnimation( _T( "Move" ), false, true );
	}

	for( auto elem = mObjects.begin(); elem != mObjects.end(); elem++ )
	{
		( *elem )->update( deltaTime );
	}
}

void Scene::drawScene( DWORD deltaTime )
{
	for( auto elem = mObjects.begin(); elem != mObjects.end(); elem++ )
	{
		( *elem )->draw();
	}
}


void Scene::setSkyBox( std::shared_ptr< GraphicsObject > spObject )
{
	addObject( spObject, { 0.0f, 0.0f, 0.0f } );
	mspSkybox = spObject;
}

void Scene::setManagedObject( std::shared_ptr< GraphicsObject > spObject, D3DXVECTOR3 position )
{
	addObject( spObject, position );
	mspManagedObject = spObject;
	mpCamera->setWorldPositionPointer( spObject->getWorldPosition().lock(), true, true );
}

void Scene::addObject( std::shared_ptr< GraphicsObject > spObject, D3DXVECTOR3 position )
{
	spObject->translate( position );
	mObjects.push_back( spObject );
	mObjects.shrink_to_fit();
}

void Scene::removeObject( UINT id )
{

}


void Scene::setMoving( float moving )
{
	if( moving && !mMoving )
	{
		mspManagedObject->enableAnimation( _T( "Move" ), true, true );
	}

	mMoving = moving;
}


LPCamera Scene::getCamera()
{
	return mpCamera;
}

std::shared_ptr< GraphicsObject > Scene::getManagedObject()
{
	return mspManagedObject;
}