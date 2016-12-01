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
			//������������ � ����������
			//*************
			Window( HINSTANCE hInstance );
			virtual ~Window();

			//*************
			//��������������
			//*************
			int registerClass( WNDCLASS* wc );
			int initWindow( LPCTSTR winName, LPCTSTR className, LONG x, LONG y, LONG width, LONG height, DWORD dwStyle, DWORD dwStyleEx, HWND hWndParent, HMENU hMenu );
			int initWindowIndirect( LPWindowInfo pWinInfo );
			int initFullScreenWindow( LPCTSTR winName );
			int initStandartWindow( LPCTSTR winName );

			//*************
			//Getter-�
			//*************
			HWND getHwnd() const;
			HINSTANCE gethInstance() const;
			HWND getHwndParent() const;
			HMENU getHmenu() const;
			LPCTSTR getWindowName() const;
			RECT getWindowWorkRect() const;

			//*************
			//Setter-�
			//*************
			virtual void setCursor()
			{
			}
			void setWindowHandler( WNDPROC wndProc );
			void quitMainLoop();

			//*************
			//�������� ���� ������ ����������
			//*************
			void mainLoop();

		protected:
			//*************
			//��������� ������� ������ / ���������������� ���������� / ���������
			//*************
			virtual int inputProcess( DWORD deltaTime );
			virtual int process( DWORD deltaTime ) = 0;
			virtual int render( DWORD deltaTime ) = 0;
		
			//*************
			//���������� ������
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
			//����������� ������ ��������� ���������.
			//Esc - �������� ���������
			//*************
			static LRESULT APIENTRY stdWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	} *LPWindow;
}

#endif