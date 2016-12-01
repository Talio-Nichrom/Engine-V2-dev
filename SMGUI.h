#pragma once

#ifndef SM_GUI
#define SM_GUI

#include "SMBasics.h"
#include "SMWindow.h"
#include "SMControl.h"
#include "SMGraphics.h"

namespace sm
{
	typedef Delegate< UINT, WPARAM, LPARAM, void* > EventDelegate;

	enum EVENTS
	{
		EVENT_MOUSE = 1,
		EVENT_KEYBOARD,
		EVENT_BUTTON,
	};

	enum MESSAGES
	{
		BUTTON_CLICK = 1001,
	};

	typedef class Sensor
	{
		public:
			Sensor( UINT id, LONG x, LONG y, LONG width, LONG height );
			Sensor( UINT id );
			virtual ~Sensor();

			virtual void onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			virtual void onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData );
			virtual void onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData );

			virtual bool canHaveFocus();
			virtual void onFocusIn();
			virtual void onFocusOut();
			virtual void onMouseEnter();
			virtual void onMouseLeave();

			void setSensorPosition( LONG x, LONG y );
			void setSensorPosition( POINT pt );
			void setSensorSize( LONG width, LONG height );
			void setSensorSensitiveRect( RECT rect );
			void addGElement( std::shared_ptr< G2DObject > spGElement );
			void setEnable( bool bEnabled );
			void setVisible( bool bVisible );
			void setHandler( UINT evType, EventDelegate handler );
			void setMouseHandler( EventDelegate handler );
			void setKeyboardHandler( EventDelegate handler );
			
			UINT getSensorID();
			void getSensorPosition( LONG* x, LONG* y );
			void getSensorPosition( POINT* pPt );
			void getSensorSize( LONG* width, LONG* height );
			void getSensorSize( POINT* pPt );
			std::shared_ptr< G2DObject > getGElement( UINT id );
			bool isEnable();
			bool isVisible();
			bool containsPoint( POINT pt );

			virtual void draw( DWORD deltaTime );

		protected:
			UINT mID;
			LONG mSensorXPosition;
			LONG mSensorYPosition;
			LONG mSensorWidth;
			LONG mSensorHeight;
			RECT mSensorSensitiveRect;
			std::vector< std::shared_ptr< G2DObject > > mGElements;
			bool mbEnabled;
			bool mbVisible;
			bool mbHasFocus;
			bool mbMouseOver;
			std::map< UINT, EventDelegate > mHandlers;

	} *LPSensor;

	typedef class Static : public Sensor
	{
		public:
			Static( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR pText );
			Static( UINT id, LPCTSTR pText );
			virtual ~Static();
			
			void setText( LPCTSTR pText );

			LPCTSTR getText();

			virtual void draw( DWORD deltaTime );

		protected:
			tstring mWindowText;

	} *LPStatic;

	typedef class Button : public Static
	{
		public:
			Button( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR pText );
			Button( UINT id, LPCTSTR pText );
			virtual ~Button();

			virtual void onMouseLeave();
			virtual void onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pUserData );

			virtual void draw( DWORD deltaTime );

		protected:
			bool mbPressed;
			
	} *LPButton;

	typedef class CheckBox
	{

	} *LPCheckBox;

	typedef class RadioButton
	{

	} *LPRadioButton;

	typedef class ComboBox
	{

	} *LPComboBox;

	typedef class ListBox
	{

	} *LPListBox;

	typedef class Slider
	{

	} *LPSlider;

	typedef class Selector
	{

	} *LPSelector;

	typedef class StatusBar
	{

	} *LPStatusBar;

	typedef class PageControl
	{

	} *LPPageControl;

	typedef class Edit
	{

	} *LPEdit;

	typedef class Dialog
	{

	} *LPDialog;

	typedef class Cursor
	{
		public:
			Cursor();

			void setStaticCursor( LPGraphicsCore pGraphicsCore, std::shared_ptr< Texture > spTexture );

			void getCursorPosition( POINT* pPt );

			void setVisible( bool bVisible );

		private:
			
			LPGraphicsCore mpGraphicsCore;
			std::shared_ptr< Texture > mspTexture;
			bool mbVisible;
			POINT mLockedPosition;

			void init();

	} *LPCursor;

	typedef class GWindow : public Window
	{
		public:
			GWindow( HINSTANCE hInstance );
			virtual ~GWindow();

			int initInputController();
			int initIntupController( LPInputController pInputController );
			int initGraphicsCore( bool bWindowed, UINT wWidth, UINT wHeight );
			int initGraphicsCore( LPGraphicsCore pGraphicsCore );
			int initDefault( LPCTSTR winName );

			void addGUIElement( LPSensor pSensor );
			void setCursor( std::shared_ptr< Texture > spCursorTexture );

			std::shared_ptr< Sensor > getSensor( UINT id );

		protected:
			virtual int inputProcess( DWORD deltaTime );
			virtual int process( DWORD deltaTime ) = 0;
			virtual int render( DWORD deltaTime ) = 0;

			virtual void onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			virtual void onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			virtual void onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );

			virtual void onLostDevice();
			virtual void onResetDevice();

			int testGraphicsCore();
			int renderGUI( DWORD deltaTime );
			
			std::vector< std::shared_ptr< Sensor > > mGUIElements;
			LPGraphicsCore mpGraphicsCore;
			LPInputController mpInputController;
			std::weak_ptr< Sensor > mMouseOverSensor;
			std::weak_ptr< Sensor > mFocusSensor;
			Cursor mCursor;

		private:
			std::shared_ptr< Sensor > findMouseOverSensor( POINT pt );
			void setMouseOverSensor( std::shared_ptr< Sensor> spSensor );
			void setFocusSensor( std::shared_ptr< Sensor > spSensor );

	} *LPGWindow;

	typedef class BasicSensorsFactory
	{
		public:
			BasicSensorsFactory( LPGraphicsCore pGraphicsCore, LPGraphicsResourceManager pGraphicsResourceManager );
			virtual~BasicSensorsFactory();

			virtual LPSensor createStatic( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text );
			virtual LPSensor createButton( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text );
			std::shared_ptr< Sensor > createStaticSPtr( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text );
			std::shared_ptr< Sensor > createButtonSPtr( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text );

		protected:
			
			std::shared_ptr< Font > getDefaultFont();

		private:
			LPGraphicsCore mpGraphicsCore;
			LPGraphicsResourceManager mpGraphicsResourceManager;

	} *LPBasicSensorsFactory;
}

#endif