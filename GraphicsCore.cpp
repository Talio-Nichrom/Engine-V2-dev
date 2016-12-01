#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
GraphicsCore::GraphicsCore() :
	mHWnd( 0 ),
	mpDirect( 0 ),
	mpDevice( 0 ),
	mpSprite( 0 )
{
	ZeroMemory( &mPresentParameters, sizeof( PresentParameters ) );
}

GraphicsCore::GraphicsCore( const GraphicsCore &root )
{

}

GraphicsCore::~GraphicsCore()
{
	ZeroMemory( &mPresentParameters, sizeof( PresentParameters ) );
	s_release( mpSprite );
	s_release( mpDevice );
	s_release( mpDirect );
	mHWnd = 0;
}


//*************
//Операторы
//*************
GraphicsCore& GraphicsCore::operator=( const GraphicsCore& root )
{
	return *this;
}


//*************
//Создание и удаление ядра
//*************
GraphicsCore* GraphicsCore::mpInstance = 0;

GraphicsCore* GraphicsCore::create( HWND hWnd, bool windowed, UINT wWidth, UINT wHeight )
{
	if( mpInstance )
		return mpInstance;

	if( !( mpInstance = new GraphicsCore() ) )
		return 0;

	if( !( mpInstance->mpDirect = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		destroy();
		return 0;
	}

	D3DDISPLAYMODE displayMode;
	ZeroMemory( &displayMode, sizeof( D3DDISPLAYMODE ) );
	if( FAILED( mpInstance->mpDirect->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode ) ) )
	{
		destroy();
		return 0;
	}

	if( wWidth == 0 )
		wWidth = displayMode.Width;

	if( wHeight == 0 )
		wHeight = displayMode.Height;

	mpInstance->mHWnd = hWnd;

	mpInstance->mPresentParameters.BackBufferWidth = wWidth;
	mpInstance->mPresentParameters.BackBufferHeight = wHeight;

	mpInstance->mPresentParameters.Windowed = windowed;
	mpInstance->mPresentParameters.BackBufferFormat = displayMode.Format;
	mpInstance->mPresentParameters.BackBufferCount = 2;
	mpInstance->mPresentParameters.hDeviceWindow = hWnd;
	mpInstance->mPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mpInstance->mPresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	mpInstance->mPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	mpInstance->mPresentParameters.EnableAutoDepthStencil = true;
	mpInstance->mPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;

	mpInstance->mPresentParameters.enableZBuffer = true;

	mpInstance->mPresentParameters.perspectiveFOV = D3DX_PI / 4;
	mpInstance->mPresentParameters.perspectiveAspect = float( wWidth ) / float( wHeight );
	mpInstance->mPresentParameters.perspectiveNear = 1.0f;
	mpInstance->mPresentParameters.perspectiveFar = 100000.0f;

	mpInstance->mPresentParameters.alphaBlendingSrc = D3DBLEND_SRCALPHA;
	mpInstance->mPresentParameters.alphaBlendingDesc = D3DBLEND_INVSRCALPHA;

	mpInstance->mPresentParameters.ambientColor = 0xffffffff;

	if( FAILED( mpInstance->mpDirect->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&mpInstance->mPresentParameters,
		&mpInstance->mpDevice
		) ) )
	{
		destroy();
		return 0;
	}

	if( FAILED( D3DXCreateSprite( mpInstance->mpDevice, &mpInstance->mpSprite ) ) )
	{
		destroy();
		return 0;
	}

	mpInstance->enableStates();

	return mpInstance;
}

GraphicsCore* GraphicsCore::createDefault( HWND hWnd )
{
	return create( hWnd, false, 0, 0 );
}

int GraphicsCore::destroy()
{
	if( !mpInstance )
		return 1;

	s_deleteP( mpInstance );
	return 0;
}

GraphicsCore* GraphicsCore::getInstance()
{
	return mpInstance;
}


//*************
//Установка параметров сцены
//*************
int GraphicsCore::enableLighting( bool enable )
{
	if( FAILED( mpDevice->SetRenderState( D3DRS_LIGHTING, enable ) ) )
		return 1;

	if( FAILED( mpDevice->SetRenderState( D3DRS_NORMALIZENORMALS, enable ) ) )
		return 2;

	if( FAILED( mpDevice->SetRenderState( D3DRS_SPECULARENABLE, enable ) ) )
		return 3;

	mPresentParameters.enableLigthing = enable;

	return 0;
}

int GraphicsCore::setAmbientLight( DWORD color )
{
	if( FAILED( mpDevice->SetRenderState( D3DRS_AMBIENT, color ) ) )
		return 1;

	mPresentParameters.ambientColor = color;

	return 0;
}

int GraphicsCore::setAmbientLight( BYTE red, BYTE green, BYTE blue, BYTE alpha )
{
	return setAmbientLight( D3DCOLOR_RGBA( red, green, blue, alpha ) );
}

int GraphicsCore::enableZBuffer( bool enable )
{
	if( FAILED( mpDevice->SetRenderState( D3DRS_ZENABLE, enable ? D3DZB_TRUE : D3DZB_FALSE ) ) )
		return 1;

	mPresentParameters.enableZBuffer = enable;

	return 0;
}

int GraphicsCore::enableAlphaBlending( bool enable, DWORD src, DWORD desc )
{
	if( FAILED( mpDevice->SetRenderState( D3DRS_SRCBLEND, src ) ) )
		return 1;

	mPresentParameters.alphaBlendingSrc = src;

	if( FAILED( mpDevice->SetRenderState( D3DRS_DESTBLEND, desc ) ) )
		return 2;

	mPresentParameters.alphaBlendingDesc = desc;

	if( FAILED( mpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, enable ) ) )
		return 3;

	mPresentParameters.enableAlphaBlending = enable;

	return 0;
}

int GraphicsCore::enableAlphaTesting( bool enable )
{
	if( FAILED( mpDevice->SetRenderState( D3DRS_ALPHATESTENABLE, enable ) ) )
		return 1;

	if( FAILED( mpDevice->SetRenderState( D3DRS_ALPHAREF, 0x08 ) ) )
		return 2;

	if( FAILED( mpDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL ) ) )
		return 3;

	mPresentParameters.enableAlphaTesting = enable;

	return 0;
}

int GraphicsCore::setPerspective( float FOV, float Aspect, float Near, float Far )
{
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, FOV, Aspect, Near, Far );

	if( FAILED( mpDevice->SetTransform( D3DTS_PROJECTION, &matProj ) ) )
		return 1;

	mPresentParameters.perspectiveFOV = FOV;
	mPresentParameters.perspectiveAspect = Aspect;
	mPresentParameters.perspectiveNear = Near;
	mPresentParameters.perspectiveFar = Far;

	return 0;
}

int GraphicsCore::setWindowSize( UINT wWidth, UINT wHeight, bool savePerspective )
{
	mPresentParameters.BackBufferWidth = wWidth;
	mPresentParameters.BackBufferHeight = wHeight;

	if( !savePerspective )
		mPresentParameters.perspectiveAspect = float( wWidth ) / float( wHeight );

	return reset();
}

int GraphicsCore::enableWindowed( bool enable )
{
	mPresentParameters.Windowed = enable;

	return reset();
}


//*************
//Установка текущих параметров рендеринга
//*************
int GraphicsCore::setViewTransformation( D3DMATRIX *pMatrix )
{
	if( pMatrix )
	{
		if( FAILED( mpDevice->SetTransform( D3DTS_VIEW, pMatrix ) ) )
			return 1;
	}
	else
	{
		D3DXVECTOR3 vecVP( 0, 0, 0 ), vecTP( 0, 0, -10 ), vecUp( 0, 1, 0 );
		D3DXMATRIX localMatrix;
		D3DXMatrixLookAtLH( &localMatrix, &vecTP, &vecVP, &vecUp );
		if( FAILED( mpDevice->SetTransform( D3DTS_VIEW, &localMatrix ) ) )
			return 2;
	}

	return 0;
}

int GraphicsCore::setWorldTransformation( D3DMATRIX *pMatrix )
{
	if( pMatrix )
	{
		if( FAILED( mpDevice->SetTransform( D3DTS_WORLD, pMatrix ) ) )
			return 1;
	}
	else
	{
		D3DXMATRIX localMatrix;
		D3DXMatrixIdentity( &localMatrix );
		if( FAILED( mpDevice->SetTransform( D3DTS_WORLD, &localMatrix ) ) )
			return 2;
	}

	return 0;
}

int GraphicsCore::setLight( D3DLIGHT9* pLight, DWORD num )
{
	if( FAILED( mpDevice->SetLight( num, pLight ) ) )
		return 1;

	return 0;
}

int GraphicsCore::enableLight( DWORD num, bool enable )
{
	if( FAILED( mpDevice->LightEnable( num, enable ) ) )
		return 1;

	return 0;
}

int GraphicsCore::setMaterial( D3DMATERIAL9* pMaterial )
{
	if( pMaterial )
	{
		if( FAILED( mpDevice->SetMaterial( pMaterial ) ) )
			return 1;
	}
	else
	{
		D3DMATERIAL9 localMaterial;
		ZeroMemory( &localMaterial, sizeof( D3DMATERIAL9 ) );
		localMaterial.Power = 1.0f;
		if( FAILED( mpDevice->SetMaterial( (D3DMATERIAL9*) &localMaterial ) ) )
			return 2;
	}

	return 0;
}

int GraphicsCore::setTexture( LPDIRECT3DTEXTURE9 pTexture, DWORD num )
{
	if( FAILED( mpDevice->SetTexture( num, pTexture ) ) )
		return 1;

	return 0;
}

int GraphicsCore::setFVF( DWORD fvf )
{
	if( FAILED( mpDevice->SetFVF( fvf ) ) )
		return 1;

	return 0;
}

int GraphicsCore::setVertexSource( LPDIRECT3DVERTEXBUFFER9 pVertexBuffer, DWORD offsetInBytes, DWORD vertexSize )
{
	if( !pVertexBuffer )
		return 1;

	if( FAILED( mpDevice->SetStreamSource( 0, pVertexBuffer, offsetInBytes, vertexSize ) ) )
		return 2;

	return 0;
}

int GraphicsCore::setIndexSource( LPDIRECT3DINDEXBUFFER9 pIndexBuffer )
{
	if( !pIndexBuffer )
		return 1;

	if( FAILED( mpDevice->SetIndices( pIndexBuffer ) ) )
		return 2;

	return 0;
}

int GraphicsCore::setCursor( LPDIRECT3DSURFACE9 pCursor, bool bVisible, UINT xOffset, UINT yOffset )
{
	if( !mpDevice || !pCursor )
		return 1;
	
	if( mpDevice->SetCursorProperties( xOffset, yOffset, pCursor ) != D3D_OK )
		return 2;

	showCursor( true );

	return 0;
}

int GraphicsCore::setCursorPosition( int x, int y )
{
	mpDevice->SetCursorPosition( x, y, 0 );

	return 0;
}


//*************
//Очистка буферов
//*************
int GraphicsCore::clear( DWORD color, float zBuffer )
{
	if( FAILED( mpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, zBuffer, 0 ) ) )
		return 1;
	return 0;
}

int GraphicsCore::clearDisplay( DWORD color )
{
	if( FAILED( mpDevice->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0 ) ) )
		return 1;
	return 0;
}

int GraphicsCore::clerZBuffer( float zBuffer )
{
	if( FAILED( mpDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, zBuffer, 0 ) ) )
		return 1;
	return 0;
}


//*************
//Тест устройства с попыткой восстановления
//*************
bool GraphicsCore::isValidDevice()
{
	if( mpDevice->TestCooperativeLevel() == D3D_OK )
		return true;

	return false;
}

bool GraphicsCore::isNotResetDevice()
{
	if( mpDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
		return true;

	return false;
}

int GraphicsCore::reset()
{
	mpSprite->OnLostDevice();

	if( FAILED( mpDevice->Reset( &mPresentParameters ) ) )
		return 1;

	mpSprite->OnResetDevice();

	enableStates();

	return 0;
}


//*************
//Рисование сцены
//*************
int GraphicsCore::beginScene()
{
	if( FAILED( mpDevice->BeginScene() ) )
		return 1;
	return 0;
}

int GraphicsCore::endScene()
{
	if( FAILED( mpDevice->EndScene() ) )
		return 1;
	return 0;
}

int GraphicsCore::present()
{
	if( FAILED( mpDevice->Present( NULL, NULL, NULL, NULL ) ) )
		return 1;
	return 0;
}

int GraphicsCore::beginSprite()
{
	if( FAILED( mpSprite->Begin( D3DXSPRITE_ALPHABLEND ) ) )
		return 1;
	return 0;
}

int GraphicsCore::endSprite()
{
	if( FAILED( mpSprite->End() ) )
		return 1;
	return 0;
}

int GraphicsCore::drawPrimitive( D3DPRIMITIVETYPE primitiveType, UINT startVertex, UINT primCount )
{
	if( FAILED( mpDevice->DrawPrimitive( primitiveType, startVertex, primCount ) ) )
		return 1;

	return 0;
}

int GraphicsCore::drawIndexedPrimitive( D3DPRIMITIVETYPE primitiveType, int baseVertexIndex, UINT minVertexIndex, UINT vertexCount, UINT startIndex, UINT primCount )
{
	if( FAILED( mpDevice->DrawIndexedPrimitive( primitiveType, baseVertexIndex, minVertexIndex, vertexCount, startIndex, primCount ) ) )
		return 1;

	return 0;
}

int GraphicsCore::drawSprite( LPDIRECT3DTEXTURE9 pTexture, const RECT* pSrcRect, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition, D3DXVECTOR3* pScaling, DWORD Color )
{
	if( !mpSprite )
		return 1;

	if( pScaling && pPosition )
	{
		D3DXMATRIX localMatrix;
		D3DXMatrixScaling( &localMatrix, pScaling->x, pScaling->y, pScaling->z );
		D3DXVECTOR3 vec( pPosition->x / pScaling->x, pPosition->y / pScaling->y, pPosition->z / pScaling->z );
		//D3DXVECTOR3 vec( pPosition->x * pScaling->x, pPosition->y * pScaling->y, pPosition->z * pScaling->z );
		pPosition = &vec;
		mpSprite->SetTransform( &localMatrix );
	}

	if( FAILED( mpSprite->Draw( pTexture, pSrcRect, pCenter, pPosition, Color ) ) )
		return 2;
	
	return 0;
}

bool GraphicsCore::showCursor( bool bEnable )
{
	return mpDevice->ShowCursor( bEnable );
}


//*************
//Создание элементов DirectX
//*************
LPDIRECT3DVERTEXBUFFER9 GraphicsCore::createVertexBuffer( UINT bufferSize, DWORD vertexFormat )
{
	LPDIRECT3DVERTEXBUFFER9 pLocalVertexBuffer = 0;
	if( mpDevice && SUCCEEDED( mpDevice->CreateVertexBuffer( bufferSize, 0, vertexFormat, D3DPOOL_MANAGED, &pLocalVertexBuffer, 0 ) ) )
	{
		return pLocalVertexBuffer;
	}
	return 0;
}

LPDIRECT3DINDEXBUFFER9 GraphicsCore::createIndexBuffer( UINT bufferSize )
{
	LPDIRECT3DINDEXBUFFER9 pLocalIndexBuffer = 0;
	if( mpDevice && SUCCEEDED( mpDevice->CreateIndexBuffer( bufferSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pLocalIndexBuffer, 0 ) ) )
	{
		return pLocalIndexBuffer;
	}
	return 0;
}

LPDIRECT3DTEXTURE9 GraphicsCore::createTextureFromFile( LPCTSTR pFileName )
{
	LPDIRECT3DTEXTURE9 pLocalTexture = 0;

	if( mpDevice && pFileName && SUCCEEDED( D3DXCreateTextureFromFile( mpDevice, pFileName, &pLocalTexture ) ) )
	{
		return pLocalTexture;
	}
	return 0;
}

ID3DXFont* GraphicsCore::createFont( D3DXFONT_DESC* pFontDesc )
{
	ID3DXFont* pLocalFont = 0;
	if( mpDevice && pFontDesc )
	{
		if( SUCCEEDED( D3DXCreateFontIndirect( mpDevice, pFontDesc, &pLocalFont ) ) )
			return pLocalFont;
	}
	return 0;
}

LPDIRECT3DSURFACE9 GraphicsCore::createSurface( UINT width, UINT height, D3DFORMAT format )
{
	LPDIRECT3DSURFACE9 pResult = 0;

	if( mpDevice )
	{
		if( ( mpDevice->CreateOffscreenPlainSurface( width, height, format, D3DPOOL_DEFAULT, &pResult, 0 ) ) != D3D_OK )
		{
			return 0;
		}
	}

	return pResult;
}


//*************
//Дополнительные полезные методы
//*************
void GraphicsCore::RotateVectorQuaternion( D3DXVECTOR3* pVector, D3DXQUATERNION* pQuaternion )
{
	if( !pVector || !pQuaternion )
		return;

	D3DXQUATERNION localQuaternion( pVector->x, pVector->y, pVector->z, 0.0f ), inverseQuaternion;
	D3DXQuaternionInverse( &inverseQuaternion, pQuaternion );
	D3DXQuaternionMultiply( &localQuaternion, pQuaternion, &localQuaternion );
	D3DXQuaternionMultiply( &localQuaternion, &localQuaternion, &inverseQuaternion );

	pVector->x = localQuaternion.x;
	pVector->y = localQuaternion.y;
	pVector->z = localQuaternion.z;
}

int GraphicsCore::loadSurfaceFromSurface( LPDIRECT3DSURFACE9 pSrc, RECT* pSrcRect, LPDIRECT3DSURFACE9 pDst, RECT* pDstRect )
{
	if( D3DXLoadSurfaceFromSurface( pDst, NULL, pDstRect, pSrc, NULL, pSrcRect, D3DX_DEFAULT, 0xff000000 ) != D3D_OK )
	{
		return 1;
	}

	return 0;
}


//*************
//Перезапуск всех параметров ядра
//*************
int GraphicsCore::enableStates()
{
	int result = 0;

	result += enableLighting( mPresentParameters.enableLigthing );
	result += setAmbientLight( mPresentParameters.ambientColor );
	result += enableZBuffer( mPresentParameters.enableZBuffer );
	result += enableAlphaBlending( mPresentParameters.enableAlphaBlending, mPresentParameters.alphaBlendingSrc, mPresentParameters.alphaBlendingDesc );
	result += enableAlphaTesting( mPresentParameters.enableAlphaTesting );
	result += setPerspective( mPresentParameters.perspectiveFOV, mPresentParameters.perspectiveAspect, mPresentParameters.perspectiveNear, mPresentParameters.perspectiveFar );

	return result;
}