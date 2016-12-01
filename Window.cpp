#include "SMWindow.h"

using namespace sm;

//*************
//Конструкторы и деструктор
//*************
Window::Window( HINSTANCE hInstance ) :
	mHWnd( 0 ),
	mHInstance( hInstance ),
	mHWndParent( 0 ),
	mHMenu( 0 ),
	mWinName( _T( "" ) ),
	mWinBasicRect( { 0, 0, 0, 0 } ),
	mbQuitMainLoop( false ),
	mFPS( 0 )
{
	
}

Window::~Window()
{

};


//*************
//Инициализаторы
//*************
int Window::registerClass( WNDCLASS* wc )
{
	if( !wc )
		return 1;

	if( !( RegisterClass( wc ) ) )
		return 2;

	return 0;
}

int Window::initWindow( LPCTSTR winName, LPCTSTR className, LONG x, LONG y, LONG width, LONG height, DWORD dwStyle, DWORD dwStyleEx, HWND hWndParent, HMENU hMenu )
{
	if( !( mHWnd = CreateWindowEx(
		dwStyleEx,
		className,
		winName,
		dwStyle,
		x,
		y,
		width,
		height,
		hWndParent,
		hMenu,
		mHInstance,
		NULL
		) ) )
		return 1;

	mHWndParent = hWndParent;
	mHMenu = hMenu;
	mWinName = winName;
	mWinBasicRect = { x, y, x + width, y + height };

	return 0;
}

int Window::initWindowIndirect( LPWindowInfo pWinInfo )
{
	if( int result = registerClass( pWinInfo ) )
		return result;

	return initWindow(
		pWinInfo->winName,
		pWinInfo->lpszClassName,
		pWinInfo->xPosition,
		pWinInfo->yPosition,
		pWinInfo->width,
		pWinInfo->height,
		pWinInfo->dwStyle,
		pWinInfo->dwStyleEx,
		pWinInfo->hWndParent,
		pWinInfo->hMenu
		);
}

int Window::initFullScreenWindow( LPCTSTR winName )
{
	WindowInfo wi;
	wi.cbClsExtra = 0;
	wi.cbWndExtra = 0;
	wi.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wi.hCursor = LoadCursor( NULL, IDC_ARROW );
	wi.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wi.hInstance = mHInstance;
	wi.lpfnWndProc = stdWndProc;
	wi.lpszClassName = _T( "SMFullScreenWindowClass" );
	wi.lpszMenuName = NULL;
	wi.style = CS_HREDRAW | CS_VREDRAW;
	wi.xPosition = 0;
	wi.yPosition = 0;
	wi.width = GetSystemMetrics( SM_CXSCREEN );
	wi.height = GetSystemMetrics( SM_CYSCREEN );
	wi.dwStyle = WS_POPUP | WS_VISIBLE;
	wi.dwStyleEx = WS_EX_TOPMOST;
	wi.hWndParent = 0;
	wi.hMenu = 0;
	wi.winName = winName;

	return initWindowIndirect( &wi );
}

int Window::initStandartWindow( LPCTSTR winName )
{
	WindowInfo wi;
	wi.cbClsExtra = 0;
	wi.cbWndExtra = 0;
	wi.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wi.hCursor = LoadCursor( NULL, IDC_ARROW );
	wi.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wi.hInstance = mHInstance;
	wi.lpfnWndProc = stdWndProc;
	wi.lpszClassName = _T( "SMStandartWindowClass" );
	wi.lpszMenuName = NULL;
	wi.style = CS_HREDRAW | CS_VREDRAW;
	DWORD screenWidth = GetSystemMetrics( SM_CXSCREEN );
	DWORD screenHeight = GetSystemMetrics( SM_CYSCREEN );
	wi.xPosition = screenWidth / 5;
	wi.yPosition = screenHeight / 5;
	wi.width = 3 * screenWidth / 5;
	wi.height = 3 * screenHeight / 5;
	wi.dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
	wi.dwStyleEx = 0;
	wi.hWndParent = 0;
	wi.hMenu = 0;
	wi.winName = winName;

	return initWindowIndirect( &wi );
}


//*************
//Getter-ы
//*************
HWND Window::getHwnd() const
{
	return mHWnd;
}

HINSTANCE Window::gethInstance() const
{
	return mHInstance;
}

HWND Window::getHwndParent() const
{
	return mHWndParent;
}

HMENU Window::getHmenu() const
{
	return mHMenu;
}

LPCTSTR Window::getWindowName() const
{
	return mWinName.c_str();
}

RECT Window::getWindowWorkRect() const
{
	RECT workRect = { 0, 0, 0, 0 };
	GetClientRect( mHWnd, &workRect );
	return workRect;
}


//*************
//Setter-ы
//*************
void Window::setWindowHandler( WNDPROC wndProc )
{
	SetWindowLong( mHWnd, GWL_WNDPROC, (LONG)wndProc );
}

void Window::quitMainLoop()
{
	mbQuitMainLoop = true;
}


//*************
//Основной цикл работы приложения
//*************
void Window::mainLoop()
{
	if( !mHWnd )
		return;

	DWORD lastTime = timeGetTime();

	DWORD epsTime = 0;
	UINT frame = 0;

	while( !mbQuitMainLoop )
	{
		DWORD currentTime = timeGetTime();
		DWORD deltaTime = currentTime - lastTime;

		if( inputProcess( deltaTime ) )
			return;

		if( process( deltaTime ) )
			return;
		
		if( render( deltaTime ) )
			return;

		epsTime += deltaTime;
		if( epsTime >= 1000 )
		{
			epsTime %= 1000;
			mFPS = frame;
			frame = 0;
		}
		frame++;

		lastTime = currentTime;
	}
}

int Window::inputProcess( DWORD deltaTime )
{
	PeekMessage( &mWinMessage, NULL, 0, 0, PM_REMOVE );
	if( mWinMessage.message == WM_QUIT )
	{
		quitMainLoop();
		return 0;
	}
	else
	{
		TranslateMessage( &mWinMessage );
		DispatchMessage( &mWinMessage );
	}

	return 0;
}


//*************
//Стандартный метод обработки сообщений
//*************
LRESULT APIENTRY Window::stdWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}