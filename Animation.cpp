#include "SMGraphics.h"

#ifdef SM_GRAPHICS

using namespace sm;


//*************
//AnimationKey
//*************
//Конструкторы и деструктор
//*************
AnimationKey::AnimationKey( DWORD timeKey, D3DXMATRIX *pMatrix ) :
	mTimeKey( timeKey ),
	mpNextAnimationKey( 0 )
{
	if( pMatrix )
		mMatrixTransformation = *pMatrix;
	else
		D3DXMatrixIdentity( &mMatrixTransformation );
}

AnimationKey::AnimationKey( const AnimationKey& root )
{
}

AnimationKey::~AnimationKey()
{
	s_deleteP( mpNextAnimationKey );
}


//*************
//Операторы
//*************
AnimationKey& AnimationKey::operator=( const AnimationKey& root )
{
	return *this;
}


//*************
//Добавление ключевого кадра
//*************
void AnimationKey::addAnimationKey( AnimationKey* pAnimationKey )
{
	LPAnimationKey localAnimationKey = this;

	while( localAnimationKey->mpNextAnimationKey )
	{
		localAnimationKey = localAnimationKey->mpNextAnimationKey;
	}

	localAnimationKey->mpNextAnimationKey = pAnimationKey;
}


//*************
//Getter-ы
//*************
DWORD AnimationKey::getTimeKey()
{
	return mTimeKey;
}

D3DXMATRIX AnimationKey::getMatrixTransformation()
{
	return mMatrixTransformation;
}

AnimationKey* AnimationKey::getNext()
{
	return mpNextAnimationKey;
}



//*************
//AnimationBone
//*************
//Конструкторы и деструктор
//*************
AnimationBone::AnimationBone( LPCTSTR pName, LPAnimationKey pKeys ) :
	mName( pName ),
	mpFrame( 0 ),
	mpNextAnimationBone( 0 )
{
	mspKeys = std::shared_ptr< AnimationKey >( pKeys );
}

AnimationBone::AnimationBone( const AnimationBone& root )
{
}

AnimationBone::~AnimationBone()
{
	mpFrame = 0;
	s_deleteP( mpNextAnimationBone );
}


//*************
//Операторы
//*************
AnimationBone& AnimationBone::operator=( const AnimationBone& root )
{
	return *this;
}


//*************
//Клонирование с учетом использования совместных ресурсов
//*************
AnimationBone* AnimationBone::clone()
{
	LPAnimationBone pLocalAnimationBone = new AnimationBone( getName() );
	pLocalAnimationBone->mspKeys = mspKeys;

	for( LPAnimationBone pNextBone = mpNextAnimationBone; pNextBone != 0; pNextBone = pNextBone->getNext() )
	{
		LPAnimationBone pBone = new AnimationBone( pNextBone->getName() );
		pBone->mspKeys = pNextBone->mspKeys;
		pLocalAnimationBone->addAnimationBone( pBone );
	}

	return pLocalAnimationBone;
}


//*************
//Setter-ы
//*************
void AnimationBone::setAnimationKeys( LPAnimationKey pAnimationKeys )
{
	mspKeys = std::shared_ptr< AnimationKey >( pAnimationKeys );
}

void AnimationBone::addAnimationBone( AnimationBone* pAnimationBone )
{
	LPAnimationBone localBoneAnimation = this;

	while( localBoneAnimation->mpNextAnimationBone )
	{
		localBoneAnimation = localBoneAnimation->mpNextAnimationBone;
	}

	localBoneAnimation->mpNextAnimationBone = pAnimationBone;
}

void AnimationBone::setFramePointer( LPFrame pFrame )
{
	mpFrame = pFrame;
}


//*************
//Getter-ы
//*************
LPCTSTR AnimationBone::getName()
{
	return mName.c_str();
}

LPAnimationBone AnimationBone::getNext()
{
	return mpNextAnimationBone;
}

DWORD AnimationBone::getMaxTime()
{
	LPAnimationKey pLocalAnimationKey = mspKeys.get();

	DWORD maxTime = 0;

	while( pLocalAnimationKey )
	{
		if( pLocalAnimationKey->getTimeKey() > maxTime )
			maxTime = pLocalAnimationKey->getTimeKey();

		pLocalAnimationKey = pLocalAnimationKey->getNext();
	}

	return maxTime;
}


//*************
//Обновление связанной кости
//*************
bool AnimationBone::update( DWORD timeKey )
{
	if( mpFrame )
	{
		LPAnimationKey pLocalAnimationKey = mspKeys.get();
		if( !pLocalAnimationKey )
			return false;

		LPAnimationKey key1 = 0, key2 = 0;

		while( pLocalAnimationKey )
		{
			if( timeKey >= pLocalAnimationKey->getTimeKey() )
			{
				key1 = pLocalAnimationKey;
				key2 = pLocalAnimationKey;
				pLocalAnimationKey = pLocalAnimationKey->getNext();
			}
			else
			{
				key2 = pLocalAnimationKey;
				pLocalAnimationKey = 0;
			}
		}

		D3DXMATRIX matrixTransformation1, matrixTransformation2;

		if( key1 )
		{
			matrixTransformation1 = key1->getMatrixTransformation();
		}
		else
		{
			D3DXMatrixIdentity( &matrixTransformation1 );
		}

		if( key2 )
		{
			matrixTransformation2 = key2->getMatrixTransformation();
		}
		else
		{
			D3DXMatrixIdentity( &matrixTransformation2 );
		}

		if( key1 == key2 )
		{
			mpFrame->addTransformationMatrix( &matrixTransformation1 );
		}
		else
		{
			DWORD keyTime1 = ( key1 ? key1->getTimeKey() : 0 );
			DWORD keyTime2 = ( key2 ? key2->getTimeKey() : 0 );
			float scalare = ( (float) ( timeKey - keyTime1 ) ) / ( keyTime2 - keyTime1 );
			D3DXMATRIX deltaMatrix = ( matrixTransformation2 - matrixTransformation1 ) * scalare;
			matrixTransformation1 += deltaMatrix;
			mpFrame->addTransformationMatrix( &matrixTransformation1 );
		}

		return true;
	}

	return false;
}

void AnimationBone::toBase()
{
	if( mpFrame )
	{
		mpFrame->setTransformationMatrix( 0 );
	}
}



//*************
//Animation
//*************
//Конструкторы и деструктор
//*************
Animation::Animation( LPCTSTR pName ) :
	mName( pName ),
	mLastTime( 0 ),
	mMaxTime( 0 ),
	mEnable( false ),
	mRepeat( false ),
	mPause( false ),
	mpAnimationBones( 0 ),
	mpNextAnimation( 0 ),
	mpRootFrame( 0 )
{

}

Animation::Animation( const Animation& root )
{
}

Animation::~Animation()
{
	mLastTime = 0;
	mEnable = false;
	mRepeat = false;
	mPause = false;
	s_deleteP( mpAnimationBones );
	s_deleteP( mpNextAnimation );
}


//*************
//Операторы
//*************
Animation& Animation::operator=( const Animation& root )
{
	return *this;
}


//*************
//Клонирование с учетом использования совместных ресурсов
//*************
Animation* Animation::clone()
{
	LPAnimation pLocalAnimation = 0;

	for( LPAnimation pNextAnimation = this; pNextAnimation != 0; pNextAnimation = pNextAnimation->getNext() )
	{
		LPAnimation pAnimation = new Animation( pNextAnimation->getName() );

		if( pNextAnimation->mpAnimationBones )
		{
			pAnimation->mpAnimationBones = pNextAnimation->mpAnimationBones->clone();
		}

		pAnimation->mMaxTime = pNextAnimation->mMaxTime;

		if( !pLocalAnimation )
			pLocalAnimation = pAnimation;
		else
			pLocalAnimation->addAnimation( pAnimation );
	}

	return pLocalAnimation;
}


//*************
//Добавление элементов
//*************
void Animation::addAnimationBone( LPAnimationBone pAnimationBone )
{
	if( mpAnimationBones )
	{
		mpAnimationBones->addAnimationBone( pAnimationBone );
	}
	else
	{
		mpAnimationBones = pAnimationBone;
	}

	LPAnimationBone localBoneAnimation = pAnimationBone;
	while( localBoneAnimation )
	{
		DWORD boneAnimationMaxTime = localBoneAnimation->getMaxTime();
		if( mMaxTime < boneAnimationMaxTime )
			mMaxTime = boneAnimationMaxTime;

		localBoneAnimation = localBoneAnimation->getNext();
	}
}

void Animation::addAnimation( Animation* pAnimation )
{
	LPAnimation localAnimation = this;

	while( localAnimation->mpNextAnimation )
	{
		localAnimation = localAnimation->mpNextAnimation;
	}

	localAnimation->mpNextAnimation = pAnimation;
}


//*************
//Getter-ы
//*************
LPCTSTR Animation::getName()
{
	return mName.c_str();
}

LPAnimation Animation::getNext()
{
	return mpNextAnimation;
}

LPAnimation Animation::getAnimation( LPCTSTR pName )
{
	LPAnimation pLocalAnimation = this;

	while( pLocalAnimation )
	{
		if( tstring( pName ) == tstring( pLocalAnimation->getName() ) )
		{
			return pLocalAnimation;
		}
		pLocalAnimation = pLocalAnimation->getNext();
	}

	return pLocalAnimation;
}

bool Animation::isEnabled()
{
	return mEnable;
}

bool Animation::isPaused()
{
	return mPause;
}


//*************
//Привязка костей к иерархии
//*************
void Animation::bindFrames( LPFrame pRootFrame )
{
	if( pRootFrame )
	{
		LPAnimation pLocalAnimation = this;
		while( pLocalAnimation )
		{
			pLocalAnimation->mpRootFrame = pRootFrame;
			LPAnimationBone pLocalBoneAnimation = pLocalAnimation->mpAnimationBones;
			while( pLocalBoneAnimation )
			{
				pLocalBoneAnimation->setFramePointer( pRootFrame->find( pLocalBoneAnimation->getName() ) );
				pLocalBoneAnimation = pLocalBoneAnimation->getNext();
			}
			pLocalAnimation = pLocalAnimation->mpNextAnimation;
		}
	}
}


//*************
//Управление ходом анимации
//*************
void Animation::enableAnimation( bool bEnable, bool bRepeat )
{
	if( bEnable == mEnable )
		return;

	mEnable = bEnable;
	mRepeat = bRepeat;
	mLastTime = 0;

	if( mEnable )
	{
		//update( 0 );
	}
	else
	{
		LPAnimationBone pLocalBoneAnimation = mpAnimationBones;
		while( pLocalBoneAnimation )
		{
			pLocalBoneAnimation->toBase();
			pLocalBoneAnimation = pLocalBoneAnimation->getNext();
		}
		mLastTime = 0;

		if( mpRootFrame )
		{
			mpRootFrame->updateHierarchy();
		}
	}
}

void Animation::pause( bool bPause )
{
	mPause = bPause;
}

bool Animation::update( DWORD deltaTime )
{
	bool result = false;

	if( mEnable && !mPause )
	{
		DWORD timeKey = mLastTime + deltaTime;

		//if( timeKey >= 600 )
		//{
		//	mPause = true;
		//}

		if( timeKey > mMaxTime )
		{
			if( !mRepeat )
			{
				enableAnimation( false, false );
				timeKey = 0;
			}
			else
			{
				if( mMaxTime )
					timeKey = timeKey % mMaxTime;
				else
					timeKey = 0;
			}
		}

		LPAnimationBone pLocalBoneAnimation = mpAnimationBones;
		while( pLocalBoneAnimation )
		{
			result = pLocalBoneAnimation->update( timeKey ) || result;
			pLocalBoneAnimation = pLocalBoneAnimation->getNext();
		}

		mLastTime = timeKey;
	}

	if( result && mpRootFrame )
	{
		mpRootFrame->updateHierarchy();
	}

	return result;
}

#endif