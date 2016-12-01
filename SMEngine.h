#include "SMBasics.h"
#include "SMGraphics.h"
#include "SMControl.h"
#include "SMWindow.h"
#include "SMSound.h"
#include "SMGUI.h"

namespace sm
{
	typedef class Scene
	{
		public:
			Scene( LPGraphicsCore pGraphicsCore );
			~Scene();

			void processScene( DWORD deltaTime );
			void drawScene( DWORD deltaTime );

			void setSkyBox( std::shared_ptr< GraphicsObject > spObject );
			void setManagedObject( std::shared_ptr< GraphicsObject > spObject, D3DXVECTOR3 position );
			void addObject( std::shared_ptr< GraphicsObject > spObject, D3DXVECTOR3 position );
			void removeObject( UINT id );

			void setMoving( float moving );

			LPCamera getCamera();
			std::shared_ptr< GraphicsObject > getManagedObject();

		private:
			LPGraphicsCore mpGraphicsCore;
			ULONG mCreationModel;
			std::vector< std::shared_ptr< GraphicsObject > > mObjects;
			std::shared_ptr< GraphicsObject > mspSkybox;
			std::shared_ptr< GraphicsObject > mspManagedObject;
			float mMoving;
			LPCamera mpCamera;

	} *LPScene;

	typedef class Application : public GWindow
	{
		private:
			Application( HINSTANCE hInstance );
			Application( const Application& root );
			Application& operator=( const Application& );

		public:
			virtual ~Application();

			static Application* create( HINSTANCE hInstance );
			static void destroy();

			int load();
			int unload();

			virtual int process( DWORD deltaTime );
			virtual int render( DWORD deltaTime );

			virtual void onLostDevice();
			virtual void onResetDevice();

			static Application* getInstance();

			virtual void onMouseEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			virtual void onKeyboardEvent( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			virtual void onEvent( UINT evType, UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );

			void onQuit( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );
			void onCreateNewObject( UINT message, WPARAM wParam, LPARAM lParam, void* pSrc );

		private:

			static Application* mpInstance;

			LPGraphicsResourceManager mpGraphicsResourceManager;
			LPBasicSensorsFactory mpBasicSensorsFactory;
			LPScene mpScene;

	} Game, *LPGame;
}