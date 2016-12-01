#include "SMEngine.h"
#include <cstdlib>	//Для генерации случайных
#include <ctime>	//координат объектов

using namespace sm;

Application* Application::mpInstance = 0;

Application::Application( HINSTANCE hInstance ) :
	GWindow( hInstance )
{
	initDefault( _T( "SMEngine" ) );
	mpGraphicsResourceManager = new GraphicsResourceManager( mpGraphicsCore );
	mpBasicSensorsFactory = new BasicSensorsFactory( mpGraphicsCore, mpGraphicsResourceManager );
	mpScene = new Scene( mpGraphicsCore );
}

Application::Application( const Application& root ) :
	GWindow( root )
{

}

Application& Application::operator=( const Application& )
{
	return *this;
}

Application::~Application()
{
	s_deleteP( mpBasicSensorsFactory );
	s_deleteP( mpGraphicsResourceManager );
	s_deleteP( mpScene );
}


Application* Application::create( HINSTANCE hInstance )
{
	mpInstance = new Game( hInstance );
	return mpInstance;
}

void Application::destroy()
{
	s_deleteP( mpInstance );
}


int Application::load()
{
	std::srand( time( NULL ) );

	ULONG cursorTexture = mpGraphicsResourceManager->loadTextureFromFile( _T( "Cursor.png" ), 1 );
	setCursor( mpGraphicsResourceManager->getTextureResource( cursorTexture ) );

	addGUIElement( mpBasicSensorsFactory->createStatic( 100, 0, 0, 200, 100, _T( "Тестовая строкa. Test string." ) ) );

	LPSensor button = mpBasicSensorsFactory->createButton( 101, 0, 110, 200, 50, _T( "Выход" ) );
	EventDelegate evDel;
	evDel.connect( this, &Game::onQuit );
	button->setHandler( EVENT_BUTTON, evDel );
	addGUIElement( button );

	button = mpBasicSensorsFactory->createButton( 102, 0, 200, 200, 50, _T( "Добавить объект" ) );
	evDel.connect( this, &Game::onCreateNewObject );
	button->setHandler( EVENT_BUTTON, evDel );
	addGUIElement( button );

	ULONG skyBoxTexture = mpGraphicsResourceManager->loadTextureFromFile( _T( "SkyBox.png" ) );
	ULONG skyBoxModel = mpGraphicsResourceManager->create3DSkyBoxModel( 10000.0f, skyBoxTexture, 0xffffffff );
	LPGraphicsObject skyBox = mpGraphicsResourceManager->get3dObjectResource( skyBoxModel )->clone();
	mpScene->setSkyBox( std::shared_ptr< GraphicsObject >( skyBox ) );

	ULONG planeTexture = mpGraphicsResourceManager->loadTextureFromFile( _T( "RoadBrick.jpg" ) );
	ULONG planeModel = mpGraphicsResourceManager->create3DPlaneModel( 10000.0f, 10000.0f, planeTexture, 0xffffffff, 50.0f, 50.0f ); 
	LPGraphicsObject plane = mpGraphicsResourceManager->get3dObjectResource( planeModel )->clone();
	mpScene->addObject( std::shared_ptr< GraphicsObject >( plane ), { 0.0f, 0.0f, 0.0f } );

	ULONG sphereTexture = mpGraphicsResourceManager->loadTextureFromFile( _T( "Venus.jpg" ) );
	ULONG sphereModel = mpGraphicsResourceManager->create3DSphereModel( 40.0f, 128, 32, sphereTexture, 0xffffffff, true );
	LPGraphicsObject sphere = mpGraphicsResourceManager->get3dObjectResource( sphereModel )->clone();
	mpScene->addObject( std::shared_ptr< GraphicsObject >( sphere ), { 250.0f, 200.0f, 0.0f } );

	ULONG testObjectModel = mpGraphicsResourceManager->load3DModelFromFile( _T( "script_test.mops" ), 105 );
	LPGraphicsObject obj = mpGraphicsResourceManager->get3dObjectResource( testObjectModel )->clone();
	mpScene->setManagedObject( std::shared_ptr< GraphicsObject >( obj ), { 0.0f, 0.0f, 0.0f } );
	mpScene->getCamera()->rotate( 180.0f, 0.0f );

	D3DXVECTOR3 vec = { 0.0f, 0.0f, -200.0f };
	onCreateNewObject( 0, 0, testObjectModel, &vec );

	vec = { 0.0f, 0.0f, 100.0f };
	onCreateNewObject( 0, 0, testObjectModel, &vec );

	vec = { 50.0f, 0.0f, 0.0f };
	onCreateNewObject( 0, 0, testObjectModel, &vec );

	mpGraphicsCore->enableLighting( true );
	mpGraphicsCore->setAmbientLight( 0xffffffff );

	return 0;
}

int Application::unload()
{
	return 0;
}


int Application::process( DWORD deltaTime )
{
	mpScene->processScene( deltaTime );

	auto elem = std::static_pointer_cast< Static, Sensor >( mGUIElements[ 0 ] );
	tstring info = _T( "**Demo Engine V2**\nW, A, S, D - передвижение\nSPACE - прыжок\nПКМ - вращение камеры\nESC - выход\nFPS: " );
	elem->setText( ( info + toTstring( mFPS ) ).c_str() );

	return 0;
}

int Application::render( DWORD deltaTime )
{
	if( !testGraphicsCore() )
	{
		mpGraphicsCore->showCursor();

		mpGraphicsCore->clear( 0xface8d );

		mpGraphicsCore->beginScene();

		mpScene->drawScene( deltaTime );

		renderGUI( deltaTime );

		mpGraphicsCore->endScene();

		mpGraphicsCore->present();
	}

	return 0;
}

void Application::onLostDevice()
{
	mpGraphicsResourceManager->onLostDevice();
}

void Application::onResetDevice()
{
	mpGraphicsResourceManager->onResetDevice();
}


Application* Application::getInstance()
{
	return mpInstance;
}


void Application::onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{
	static bool bCamMove = false;

	switch( message )
	{
		case WM_KEYDOWN:
			{
				if( lParam == 1 )
				{
					bCamMove = true;
					mCursor.setVisible( false );
				}
			}
			break;
		case WM_KEYUP:
			{
				if( lParam == 1 )
				{
					bCamMove = false;
					mCursor.setVisible( true );
				}
			}
			break;

		case WM_MOUSEMOVE:
			{
				if( bCamMove )
					mpScene->getCamera()->rotate( (LONG) wParam / 10.0f, (LONG) lParam / 10.0f );
			}
			break;
	}
}

void Application::onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{
	switch( message )
	{
		case WM_KEYDOWN:
		case WM_KEYPRESS:
		{
			switch( lParam )
			{
				case DIK_ESCAPE:
					quitMainLoop();
					break;
				case DIK_W:
					mpScene->setMoving( 0.2f );
					break;
				case DIK_A:
					mpScene->getManagedObject()->rotateRel( 0.1f, 0.0f, 0.0f );
					break;
				case DIK_S:
					mpScene->setMoving( -0.1f );
					break;
				case DIK_D:
					mpScene->getManagedObject()->rotateRel( -0.1f, 0.0f, 0.0f );
					break;
				case DIK_SPACE:
					mpScene->getManagedObject()->enableAnimation( _T( "Jump" ), true, false );
					break;
			}
		}
		break;
	}
}

void Application::onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{

}


void Application::onQuit( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{
	quitMainLoop();
}

void Application::onCreateNewObject( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{
	LPGraphicsObject obj = mpGraphicsResourceManager->get3dObjectResource( 105 )->clone();

	float x = 0.0f;
	float z = 0.0f;

	if( pSrc )
	{
		LPD3DXVECTOR3 vec = (LPD3DXVECTOR3)pSrc;
		x = vec->x;
		z = vec->z;
	}
	else
	{
		auto camPos = mpScene->getCamera()->getPosition();
		x = camPos.x + ( ( rand() % 40000 - 20000 ) / 100.0f );
		z = camPos.z + ( ( rand() % 40000 - 20000) / 100.0f );
		float horAngle = ( ( rand() % 36000 ) / 100.0f );
		obj->rotate( horAngle, 0.0f, 0.0f );
	}

	mpScene->addObject( std::shared_ptr< GraphicsObject >( obj ), { x, 0.0f, z } );

}