#include "SMGraphics.h"

using namespace sm;


//*************
//Конструкторы и деструктор
//*************
Frame::Frame( LPCTSTR pName, D3DXMATRIX* pBaseMatrix, Frame* pParent ) :
	mpParent( pParent ),
	mpSibling( 0 ),
	mpChild( 0 )
{
	setName( pName );

	if( pBaseMatrix )
	{
		mMatrixBase = *pBaseMatrix;
	}
	else
	{
		D3DXMatrixIdentity( &mMatrixBase );
	}
	D3DXMatrixIdentity( &mMatrixTransformation );
	D3DXMatrixIdentity( &mMatrixCombined );

	if( pParent )
	{
		pParent->addChildFrame( this );
	}
}

Frame::~Frame()
{
	s_deleteP( mpChild );
	s_deleteP( mpSibling );
	mpParent = 0;
}


//*************
//Операторы
//*************
Frame& Frame::operator=( const Frame& root )
{
	return *this;
}


//*************
//Клонирование иерархии
//*************
Frame* Frame::clone()
{
	LPFrame pResult = new Frame( getName(), getMatrixBase() );

	LPFrame pChild = 0;
	LPFrame pSibling = 0;

	for( LPFrame pDstFrame = pResult, pSrcFrame = this; pDstFrame != 0; pDstFrame = pDstFrame->next(), pSrcFrame = pSrcFrame->next() )
	{
		if( pSrcFrame->mpSibling )
		{
			pSibling = new Frame( pSrcFrame->mpSibling->getName(), pSrcFrame->mpSibling->getMatrixBase() );
			pDstFrame->addSiblingFrame( pSibling );
			pSibling = 0;
		}

		if( pSrcFrame->mpChild )
		{
			pChild = new Frame( pSrcFrame->mpChild->getName(), pSrcFrame->mpChild->getMatrixBase() );
			pDstFrame->addChildFrame( pChild );
			pChild = 0;
		}
	}

	return pResult;
}


//*************
//Итерирование в дереве
//*************
LPFrame Frame::find( LPCTSTR pName )
{
	if( !pName )
		return 0;

	for( LPFrame pLocalFramePointer = this; pLocalFramePointer != 0; pLocalFramePointer = pLocalFramePointer->next() )
	{
		if( tstring( pName ) == tstring( pLocalFramePointer->getName() ) )
		{
			return pLocalFramePointer;
		}
	}

	return 0;
}

LPFrame Frame::next()
{
	if( mpChild )
	{
		return mpChild;
	}
	else if( mpSibling )
	{
		return mpSibling;
	}

	LPFrame pLocalFrame = mpParent;

	while( pLocalFrame )
	{
		if( pLocalFrame->mpSibling )
			return pLocalFrame->mpSibling;

		pLocalFrame = pLocalFrame->mpParent;
	}

	return 0;
}


//*************
//Setter-ы
//*************
void Frame::addChildFrame( Frame* pFrame )
{
	if( !mpChild )
	{
		mpChild = pFrame;
		pFrame->mpParent = this;
	}
	else
	{
		mpChild->addSiblingFrame( pFrame );
	}
}

void Frame::addSiblingFrame( Frame* pFrame )
{
	LPFrame pLocalFramePointer = this;

	while( pLocalFramePointer->mpSibling )
	{
		pLocalFramePointer = pLocalFramePointer->mpSibling;
	}

	pLocalFramePointer->mpSibling = pFrame;

	if( pFrame )
		pFrame->mpParent = pLocalFramePointer->mpParent;
}

void Frame::setBasicMatrix( D3DXMATRIX* pMatrix )
{
	if( pMatrix )
	{
		mMatrixBase = *pMatrix;
	}
	else
	{
		D3DXMatrixIdentity( &mMatrixBase );
	}
}

void Frame::setTransformationMatrix( D3DXMATRIX* pMatrix )
{
	if( pMatrix )
	{
		mMatrixTransformation = *pMatrix;
	}
	else
	{
		D3DXMatrixIdentity( &mMatrixTransformation );
	}
}

void Frame::addTransformationMatrix( D3DXMATRIX* pMatrix )
{
	if( pMatrix )
	{
		D3DXMatrixMultiply( &mMatrixTransformation, &mMatrixTransformation, pMatrix );
	}
}

void Frame::setName( LPCTSTR pName )
{
	if( pName )
		mName = pName;
	else
		mName = _T( "Root" );
}


//*************
//Getter-ы
//*************
LPCTSTR Frame::getName()
{
	return mName.c_str();
}

D3DXMATRIX* Frame::getMatrixTransformation()
{
	return &mMatrixTransformation;
}

D3DXMATRIX* Frame::getMatrixBase()
{
	return &mMatrixBase;
}

D3DXMATRIX* Frame::getMatrixCombined()
{
	return &mMatrixCombined;
}


//*************
//Обновление преобразований
//*************
void Frame::updateHierarchy()
{
	for( LPFrame pLocalFrame = this; pLocalFrame != 0; pLocalFrame = pLocalFrame->next() )
	{
		D3DXMATRIX parentTransformation;
		D3DXMATRIX backBaseMatrix;

		D3DXMatrixIdentity( &parentTransformation );
		D3DXMatrixIdentity( &backBaseMatrix );

		D3DXMatrixInverse( &backBaseMatrix, 0, &pLocalFrame->mMatrixBase );

		if( pLocalFrame->mpParent && pLocalFrame->mpParent != this )
		{
			parentTransformation = *pLocalFrame->mpParent->getMatrixCombined();
		}

		pLocalFrame->mMatrixCombined = backBaseMatrix * pLocalFrame->mMatrixTransformation * pLocalFrame->mMatrixBase * parentTransformation;
		int x = 0;
	}
}

void Frame::toResetPose()
{
	for( LPFrame pLocalFrame = this; pLocalFrame != 0; pLocalFrame = pLocalFrame->next() )
	{
		pLocalFrame->setTransformationMatrix( 0 );
	}
}