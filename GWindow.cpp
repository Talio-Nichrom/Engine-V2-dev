#include "SMGUI.h"

using namespace sm;

GWindow::GWindow( HINSTANCE hInstance ) :
	Window( hInstance ),
	mpGraphicsCore( 0 ),
	mpInputController( 0 )
{

}

GWindow::~GWindow()
{
	mGUIElements.clear();
	InputController::destroy();
	GraphicsCore::destroy();
}

int GWindow::initInputController()
{
	if( !( mpInputController = InputController::create( mHInstance, mHWnd ) ) )
		return 1;
	return 0;
}

int GWindow::initIntupController( LPInputController pInputController )
{
	if( !( mpInputController = pInputController ) )
		return 1;
	return 0;
}

int GWindow::initGraphicsCore( bool bWindowed, UINT wWidth, UINT wHeight )
{
	if( !( mpGraphicsCore = GraphicsCore::create( mHWnd, bWindowed, wWidth, wHeight ) ) )
		return 1;
	return 0;
}

int GWindow::initGraphicsCore( LPGraphicsCore pGraphicsCore )
{
	if( !( mpGraphicsCore = pGraphicsCore ) )
		return 1;
	return 0;
}

int GWindow::initDefault( LPCTSTR winName )
{
	if( initFullScreenWindow( winName ) )
		return 1;

	if( !( mpGraphicsCore = GraphicsCore::createDefault( mHWnd ) ) )
		return 2;

	if( initInputController() )
		return 3;

	return 0;
}


void GWindow::addGUIElement( LPSensor pSensor )
{
	std::shared_ptr< Sensor > spSensor( pSensor );
	mGUIElements.push_back( spSensor );
}

void GWindow::setCursor( std::shared_ptr< Texture > spCursorTexture )
{
	mCursor.setStaticCursor( mpGraphicsCore, spCursorTexture );
}


std::shared_ptr< Sensor > GWindow::getSensor( UINT id )
{
	for( auto elem = mGUIElements.begin(); elem != mGUIElements.end(); elem++ )
	{
		if( *elem )
		{
			if( ( *elem )->getSensorID() == id )
				return *elem;
		}
	}

	return 0;
}

int GWindow::inputProcess( DWORD deltaTime )
{
	if( !mpInputController )
		return 1;

	if( !mpInputController->update() )
	{
		//Проверка движения мыши
		POINT mouseMove;
		if( mpInputController->getMouseMove( &mouseMove ) )
		{
			if( !mMouseOverSensor.expired() )
			{
				mMouseOverSensor.lock()->onMouseEvent( WM_MOUSEMOVE, (WPARAM) mouseMove.x, (LPARAM) mouseMove.y, 0 );
			}
			else
			{
				onMouseEvent( WM_MOUSEMOVE, (WPARAM) mouseMove.x, (LPARAM) mouseMove.y, 0 );
			}
		}

		//Проверка текущей позиции курсора
		POINT mousePosition = { -1, -1 };
		//mCursor->update();
		mCursor.getCursorPosition( &mousePosition );

		//Проверка вращения колесика мыши
		LONG mouseWheel = mpInputController->getMouseWheel();
		if( mouseWheel )
		{
			if( !mMouseOverSensor.expired() )
			{
				mMouseOverSensor.lock()->onMouseEvent( WM_MOUSEMOVE, (WPARAM) mouseMove.x, (LPARAM) mouseMove.y, 0 );
			}
			else
			{
				onMouseEvent( WM_MOUSEMOVE, (WPARAM) mouseMove.x, (LPARAM) mouseMove.y, 0 );
			}
		}

		//Изменение текущего объекта по курсору
		setMouseOverSensor( findMouseOverSensor( mousePosition ) );

		//Изменение объка фокуса ввода клавиатуры
		if( ( mpInputController->getMouseButtonEvent( 0 ) & KDOWN ) || ( mpInputController->getMouseButtonEvent( 1 ) & KDOWN ) )
		{
			setFocusSensor( mMouseOverSensor.lock() );
		}

		//Все события нажитий клавиш мыши и клавиатуры
		UINT eventType;
		UINT buttonId;
		while( mpInputController->getNextMBEvent( &eventType, &buttonId ) )
		{
			if( !mMouseOverSensor.expired() )
			{
				mMouseOverSensor.lock()->onMouseEvent( eventType, 0, (LPARAM) buttonId, 0 );
			}
			else
			{
				onMouseEvent( eventType, 0, (LPARAM) buttonId, 0 );
			}
		}

		while( mpInputController->getNextKBEvent( &eventType, &buttonId ) )
		{
			if( !mFocusSensor.expired() )
			{
				mFocusSensor.lock()->onKeyboardEvent( eventType, 0, (LPARAM) buttonId, 0 );
			}
			else
			{
				onKeyboardEvent( eventType, 0, (LPARAM) buttonId, 0 );
			}
		}

	}

	return 0;
}

void GWindow::onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{

}

void GWindow::onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{

}

void GWindow::onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pSrc )
{

}

void GWindow::onLostDevice()
{
	
}

void GWindow::onResetDevice()
{

}

int GWindow::testGraphicsCore()
{
	if( mpGraphicsCore->isValidDevice() )
	{
		return 0;
	}
	if( mpGraphicsCore->isNotResetDevice() )
	{
		onLostDevice();

		mpGraphicsCore->reset();

		onResetDevice();

		return 2;
	}

	return 1;
}

int GWindow::renderGUI( DWORD deltaTime )
{
	for( auto elem = mGUIElements.begin(); elem != mGUIElements.end(); elem++ )
	{
		auto gElem = ( *elem );

		if( gElem )
		{
			gElem->draw( deltaTime );
		}
	}

	return 0;
}

std::shared_ptr< Sensor > GWindow::findMouseOverSensor( POINT pt )
{
	for( auto elem = mGUIElements.rbegin(); elem != mGUIElements.rend(); elem++ )
	{
		if( *elem && (*elem)->isVisible() )
		{
			if( ( *elem )->containsPoint( pt ) )
				return *elem;
		}
	}

	return 0;
}

void GWindow::setMouseOverSensor( std::shared_ptr< Sensor> spSensor )
{
	if( mMouseOverSensor.lock() != spSensor )
	{
		if( mMouseOverSensor.lock() )
		{
			mMouseOverSensor.lock()->onMouseLeave();
		}

		if( spSensor )
		{
			spSensor->onMouseEnter();
		}
		
		mMouseOverSensor = spSensor;
	}
}

void GWindow::setFocusSensor( std::shared_ptr< Sensor > spSensor )
{
	if( mFocusSensor.lock() != spSensor )
	{
		if( mFocusSensor.lock() )
		{
			mFocusSensor.lock()->onFocusOut();
			mFocusSensor.reset();
		}

		if( spSensor && spSensor->canHaveFocus() )
		{
			spSensor->onFocusIn();
			mFocusSensor = spSensor;
		}		
	}
}