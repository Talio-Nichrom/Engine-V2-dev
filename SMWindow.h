#pragma once

#ifndef SM_WINDOW
#define SM_WINDOW

#include "SMBasics.h"

#pragma comment ( lib, "winmm.lib" )

namespace sm
{

	typedef struct WindowInfo : public WNDCLASS
	{
		LPCTSTR winName;
		DWORD dwStyle;
		DWORD dwStyleEx;
		LONG xPosition;
		LONG yPosition;
		LONG width;
		LONG height;
		HWND hWndParent;
		HMENU hMenu;

	} *LPWindowInfo;

	typedef class Window
	{
		public:
			//*************
			//Конструкторы и деструктор
			//*************
			Window( HINSTANCE hInstance );
			virtual ~Window();

			//*************
			//Инициализаторы
			//*************
			int registerClass( WNDCLASS* wc );
			int initWindow( LPCTSTR winName, LPCTSTR className, LONG x, LONG y, LONG width, LONG height, DWORD dwStyle, DWORD dwStyleEx, HWND hWndParent, HMENU hMenu );
			int initWindowIndirect( LPWindowInfo pWinInfo );
			int initFullScreenWindow( LPCTSTR winName );
			int initStandartWindow( LPCTSTR winName );

			//*************
			//Getter-ы
			//*************
			HWND getHwnd() const;
			HINSTANCE gethInstance() const;
			HWND getHwndParent() const;
			HMENU getHmenu() const;
			LPCTSTR getWindowName() const;
			RECT getWindowWorkRect() const;

			//*************
			//Setter-ы
			//*************
			virtual void setCursor()
			{
			}
			void setWindowHandler( WNDPROC wndProc );
			void quitMainLoop();

			//*************
			//Основной цикл работы приложения
			//*************
			void mainLoop();

		protected:
			//*************
			//Обработка входных данных / пользовательские вычисления / рисование
			//*************
			virtual int inputProcess( DWORD deltaTime );
			virtual int process( DWORD deltaTime ) = 0;
			virtual int render( DWORD deltaTime ) = 0;
		
			//*************
			//Внутренние данные
			//*************
			HWND mHWnd;
			HINSTANCE mHInstance;
			HWND mHWndParent;
			HMENU mHMenu;
			tstring mWinName;
			RECT mWinBasicRect;
			bool mbQuitMainLoop;
			MSG mWinMessage;
			UINT mFPS;

		private:
			//*************
			//Стандартная функия обработки сообщений.
			//Esc - закрытие программы
			//*************
			static LRESULT APIENTRY stdWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	} *LPWindow;
}

#endif