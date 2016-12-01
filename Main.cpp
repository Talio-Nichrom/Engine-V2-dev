#include "SMEngine.h"

using namespace sm;

//Основная функция приложения
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
{
	Game::create( hInstance );

	LPGame pGame = Game::getInstance();

	if( !pGame )
		return 1;

	if( pGame->load() )
	{
		Game::destroy();
		return 2;
	}

	pGame->mainLoop();

	pGame->unload();

	Game::destroy();
	
	return 0;
}