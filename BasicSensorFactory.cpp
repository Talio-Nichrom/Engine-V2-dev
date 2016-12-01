#include "SMGUI.h"

using namespace sm;

BasicSensorsFactory::BasicSensorsFactory( LPGraphicsCore pGraphicsCore, LPGraphicsResourceManager pGraphicsResourceManager ) :
	mpGraphicsCore( pGraphicsCore ),
	mpGraphicsResourceManager( pGraphicsResourceManager )
{

}

BasicSensorsFactory::~BasicSensorsFactory()
{

}


LPSensor BasicSensorsFactory::createStatic( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text )
{
	LPStatic pNewStatic = new Static( id, x, y, width, height, text );

	std::shared_ptr< G2DObject > gElement = mpGraphicsResourceManager->get2DObjectResource( 1001 );
	LPG2DObject pObject = 0;

	if( !gElement )
	{
		pObject = new G2DObject( mpGraphicsCore, 0, 0, 100, 100 );
		
		LPSprite pSprite = new Sprite( mpGraphicsCore );
		mpGraphicsResourceManager->loadTextureFromFile( _T( "BasicStaticTexture.png" ), 1001 );
		auto spTexture = mpGraphicsResourceManager->getTextureResource( 1001 );
		pSprite->setTexture( spTexture );
		pSprite->setTextureRect( { 0, 0, (LONG)spTexture->getWidth(), (LONG)spTexture->getHeight() } );
		pSprite->setFont( getDefaultFont() );
		pSprite->setFontColor( 0xffffffff );
		std::shared_ptr< Sprite > spSprite( pSprite );
		pObject->setStateSprite( 0, spSprite );

		pObject->setState( 0 );
		mpGraphicsResourceManager->add2DObjectResource( pObject, 1001 );
		gElement = mpGraphicsResourceManager->get2DObjectResource( 1001 );
	}

	pObject = gElement->clone();
	gElement = std::shared_ptr< G2DObject >( pObject );

	gElement->setSize( width, height );
	gElement->setFontOffsetRect( { 3, 3, width - 3, height - 3 } );
	pNewStatic->addGElement( gElement );

	return pNewStatic;
}

LPSensor BasicSensorsFactory::createButton( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text )
{
	LPButton pNewButton = new Button( id, x, y, width, height, text );

	std::shared_ptr< G2DObject > gElement = mpGraphicsResourceManager->get2DObjectResource( 1002 );
	LPG2DObject pObject = 0;

	if( !gElement )
	{
		pObject = new G2DObject( mpGraphicsCore, 0, 0, 100, 100 );

		mpGraphicsResourceManager->loadTextureFromFile( _T( "BasicButtonTexture.png" ), 1002 );
		auto spTexture = mpGraphicsResourceManager->getTextureResource( 1002 );

		for( int i = 0; i < 4; i++ )
		{
			LPSprite pSprite = new Sprite( mpGraphicsCore );
			pSprite->setTexture( spTexture );
			pSprite->setTextureRect( { 0, i * ( (LONG)spTexture->getHeight() / 4 ), (LONG)spTexture->getWidth(), ( i + 1 ) * ( (LONG)spTexture->getHeight() / 4 ) } );
			pSprite->setFont( getDefaultFont() );
			pSprite->setFontColor( 0xffff0000 );
			pSprite->setFontFormat( DT_CENTER | DT_VCENTER );
			pObject->setStateSprite( i, std::shared_ptr< Sprite >( pSprite ) );
		}

		pObject->setState( 0 );
		mpGraphicsResourceManager->add2DObjectResource( pObject, 1002 );
		gElement = mpGraphicsResourceManager->get2DObjectResource( 1002 );
	}

	pObject = gElement->clone();
	gElement = std::shared_ptr< G2DObject >( pObject );

	gElement->setSize( width, height );
	gElement->setFontOffsetRect( { 3, 3, width - 3, height - 3 } );
	pNewButton->addGElement( gElement );

	return pNewButton;
}

std::shared_ptr< Sensor > BasicSensorsFactory::createStaticSPtr( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text )
{
	std::shared_ptr< Sensor > spSensor( createStatic( id, x, y, width, height, text ) );
	return spSensor;
}

std::shared_ptr< Sensor > BasicSensorsFactory::createButtonSPtr( UINT id, LONG x, LONG y, LONG width, LONG height, LPCTSTR text )
{
	std::shared_ptr< Sensor > spSensor( createButton( id, x, y, width, height, text ) );
	return spSensor;
}

std::shared_ptr< Font > BasicSensorsFactory::getDefaultFont()
{
	auto spFont = mpGraphicsResourceManager->getFontResource( 1 );
	if( !spFont )
	{
		LPFont pFont = new Font( mpGraphicsCore, _T( "Default UI Font" ), _T( "Arial" ), 14, 0xff000000, false, true );
		pFont->create();
		mpGraphicsResourceManager->addFontResource( pFont, 1 );
		spFont = mpGraphicsResourceManager->getFontResource( 1 );
	}
	return spFont;
}