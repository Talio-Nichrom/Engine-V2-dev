#pragma once

#ifndef SM_GRAPHICS
#define SM_GRAPHICS

#include "SMBasics.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

namespace sm
{
	//*************
	//Параметры графического ядра
	//*************
	typedef struct PresentParameters : public D3DPRESENT_PARAMETERS
	{
		bool enableLigthing;
		DWORD ambientColor;

		bool enableZBuffer;
		bool enableAlphaTesting;
		
		bool enableAlphaBlending;
		DWORD alphaBlendingSrc;
		DWORD alphaBlendingDesc;

		float perspectiveFOV;
		float perspectiveAspect;
		float perspectiveNear;
		float perspectiveFar;

	} *LPPresentParameters;


	//*************
	//Графическое ядро
	//*************
	typedef class GraphicsCore
	{
		private:
			//Конструкторы закрыты
			//Наследование запрещено
			GraphicsCore();
			GraphicsCore( const GraphicsCore& root );
			~GraphicsCore();

			//Операторы
			GraphicsCore& operator=( const GraphicsCore& root );
			
		public:
			//Создание и уничтожение ядра
			static GraphicsCore* create( HWND hWnd, bool windowed, UINT wWidth, UINT wHeight );
			static GraphicsCore* createDefault( HWND hWnd );
			static int destroy();
			static GraphicsCore* getInstance();

			//Работа с ядром

			//Установка параметров сцены
			int enableLighting( bool enable = true );
			int setAmbientLight( DWORD color );
			int setAmbientLight( BYTE red, BYTE green, BYTE blue, BYTE alpha );
			int enableZBuffer( bool enable = true );
			int enableAlphaBlending( bool enable = true, DWORD src = D3DBLEND_SRCALPHA, DWORD desc = D3DBLEND_INVSRCALPHA );
			int enableAlphaTesting( bool enable = true );
			int setPerspective( float FOV, float Aspect, float Near = 1.0f, float Far = 10000.0f );
			int setWindowSize( UINT wWidth, UINT wHeight, bool savePerspective = false );
			int enableWindowed( bool enable = true );

			//Установка текущих параметров рендеринга
			int setViewTransformation( D3DMATRIX *pMatrix );
			int setWorldTransformation( D3DMATRIX *pMatrix );
			int setLight( D3DLIGHT9* pLight, DWORD num = 0 );
			int enableLight( DWORD num, bool enable = true );
			int setMaterial( D3DMATERIAL9* pMaterial );
			int setTexture( LPDIRECT3DTEXTURE9 pTexture, DWORD num = 0 );
			int setFVF( DWORD fvf );
			int setVertexSource( LPDIRECT3DVERTEXBUFFER9 pVertexBuffer, DWORD offsetInBytes, DWORD vertexSize );
			int setIndexSource( LPDIRECT3DINDEXBUFFER9 pIndexBuffer );
			int setCursor( LPDIRECT3DSURFACE9 pCursor, bool bVisible = true, UINT xOffset = 0, UINT yOffset = 0 );
			int setCursorPosition( int x, int y );

			//Очистка буферов
			int clear( DWORD color = 0, float zBuffer = 1.0f );
			int clearDisplay( DWORD color = 0 );
			int clerZBuffer( float zBuffer = 1.0f );

			//Тест устройства и восстановление
			bool isValidDevice();
			bool isNotResetDevice();
			int reset();

			//Рисование сцены
			int beginScene();
			int endScene();
			int present();
			int beginSprite();
			int endSprite();
			int drawPrimitive( D3DPRIMITIVETYPE primitiveType, UINT startVertex, UINT primCount );
			int drawIndexedPrimitive( D3DPRIMITIVETYPE primitiveType, int baseVertexIndex, UINT minVertexIndex, UINT vertexCount, UINT startIndex, UINT primCount );
			int drawSprite( LPDIRECT3DTEXTURE9 pTexture, const RECT* pSrcRect, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition, D3DXVECTOR3* pScaling, DWORD Color );
			bool showCursor( bool bEnable = true );

			//Создание элементов DirectX
			LPDIRECT3DVERTEXBUFFER9 createVertexBuffer( UINT bufferSize, DWORD vertexFormat );
			LPDIRECT3DINDEXBUFFER9 createIndexBuffer( UINT bufferSize );
			LPDIRECT3DTEXTURE9 createTextureFromFile( LPCTSTR pFileName );
			ID3DXFont* createFont( D3DXFONT_DESC* pFontDesc );
			LPDIRECT3DSURFACE9 createSurface( UINT width, UINT height, D3DFORMAT format );

			//Дополнительные полезные методы
			static void RotateVectorQuaternion( D3DXVECTOR3* pVector, D3DXQUATERNION* pQuaternion );
			static int loadSurfaceFromSurface( LPDIRECT3DSURFACE9 pSrc, RECT* pSrcRect, LPDIRECT3DSURFACE9 pDst, RECT* pDstRect );

		private:
			//Указатель синглтона
			static GraphicsCore *mpInstance;

			//Внутренние данные графического ядра
			HWND mHWnd;
			LPDIRECT3D9 mpDirect;
			LPDIRECT3DDEVICE9 mpDevice;
			LPD3DXSPRITE mpSprite;
			PresentParameters mPresentParameters;

			//Перезапуск всех параметров ядра
			int enableStates();

	} *LPGraphicsCore;


	//*************
	//Шрифты
	//*************
	typedef class Font
	{
		public:
			//Конструкторы и деструктор
			Font( LPGraphicsCore pGraphicsCore, LPCTSTR pName, LPCTSTR pFontType, int size, DWORD color = 0xffffffff, bool italic = false, bool bold = false );
			Font( const Font& f );
			~Font();

			//Операторы
			Font& operator=( Font& root );

			//Методы ресурса
			int create();
			int destroy();
			int disable();
			int restore();

			//Установка параметров вывода текста
			void setWorkRect( long x, long y, long width, long height );
			void setWorkRect( RECT rect );
			void setColor( DWORD color );
			void setOutFormat( DWORD format );

			//Вывод на экран
			void print( LPCTSTR pCharString );
			void print( int num );
			void print( tstring& string );
			void print( float num );
			void print( LPCTSTR pCharString, RECT rect, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( int num, RECT rect, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( tstring& string, RECT rect, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( float num, RECT rect, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( LPCTSTR pCharString, long x, long y, long width, long height, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( int num, long x, long y, long width, long heigh, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( tstring& string, long x, long y, long width, long height, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			void print( float num, long x, long y, long width, long height, DWORD color = 0xffffffff, DWORD format = DT_LEFT | DT_TOP );
			
			//Getter-ы
			LPCTSTR getName();

		private:

			//Внутренние данные шрифта
			LPGraphicsCore mpGraphicsCore;
			ID3DXFont* mpFont;
			tstring mName;
			D3DXFONT_DESC mFontDesc;
			RECT mWorkRect;
			DWORD mColor;
			DWORD mOutFormat;

	} *LPFont;


	//*************
	//Текстуры
	//*************
	typedef class Texture
	{
		public:
			//Конструкторы и деструктор
			Texture( LPGraphicsCore pGraphicsCore, LPCTSTR pName, LPCTSTR pFileName );
			Texture( const Texture& root );
			~Texture();

			//Операторы
			Texture& operator=( Texture& root );

			//Методы ресурса
			virtual int create();
			virtual int destroy();

			//Рисование текстуры
			void draw( float xPosition, float yPosition, RECT workRect, DWORD color, float xScale = 1.0f, float yScale = 1.0f );

			//Getter-ы
			LPDIRECT3DTEXTURE9 getTexture();
			LPCTSTR getName();
			LPCTSTR getFileName();
			DWORD getWidth();
			DWORD getHeight();
			LPDIRECT3DSURFACE9 getTextureSurface();

		private:
			
			//Внутренние данные текстуры
			LPGraphicsCore mpGraphicsCore;
			LPDIRECT3DTEXTURE9 mpTexture;
			tstring mName;
			tstring mFileName;
			DWORD mWidth;
			DWORD mHeight;

	} *LPTexture;


	//*************
	//Материалы
	//*************
	typedef class Material
	{
		public:
			//Конструкторы и деструктор
			Material( LPGraphicsCore pGraphicsCore, LPCTSTR pName = 0, D3DMATERIAL9* pMaterial = 0, LPTexture pTexture = 0 );
			~Material();

		private:
			Material( const Material& root );

			//Операторы
			Material& operator=( Material& root );

		public:
			//Клонирование
			Material* clone();

			//Setter-ы
			void setName( LPCTSTR pName );
			void setMaterial( D3DMATERIAL9* pMaterial = 0 );
			void setTexture( std::shared_ptr< Texture > spTexture );
			void setTexture( LPTexture pTexture );
			void setBasicMaterial();
			void setMaterialDiffuseColor( DWORD color );
			void setMaterialAmbientColor( DWORD color );
			void setMaterialSpecularColor( DWORD color );
			void setMaterialEmmissiveColor( DWORD color );
			void setMaterialPower( float power );

			//Getter-ы
			LPCTSTR getName();

			//Активация материала для рисования
			void activate();

		private:
			//Внутренние данные материала
			LPGraphicsCore mpGraphicsCore;
			tstring mName;
			D3DMATERIAL9* mpMaterial;
			std::shared_ptr< Texture > mspTexture;

	} *LPMaterial;


	//*************
	//Буфер вершин
	//*************
	typedef class VertexBuffer
	{
			//Конструкторы и деструктор
		public:
			VertexBuffer( LPGraphicsCore pGraphicsCore, DWORD vertexCount, DWORD vertexSize, DWORD vertexFormat, void* pSrc = 0 );
			~VertexBuffer();

		private:
			VertexBuffer( const VertexBuffer& root );

			//Операторы
			VertexBuffer& operator=( const VertexBuffer& root );

		public:
			//Получение доступа к буферу для изменений
			void* lockBuffer();
			void unlockBuffer();

			//Активация буфера как источника вершин
			int activate();

			//Getter-ы
			DWORD getVertexCount();
			DWORD getVertexFormat();
			DWORD getVertexSize();

		private:
			//Внутренние данные буфера вершин
			LPGraphicsCore mpGraphicsCore;
			LPDIRECT3DVERTEXBUFFER9 mpBuffer;
			DWORD mVertexCount;
			DWORD mVertexSize;
			DWORD mVertexFormat;

	} *LPVertexBuffer;
	

	//*************
	//Буфер индексов
	//*************
	typedef class IndexBuffer
	{
			//Конструкторы и деструктор
		public:
			IndexBuffer( LPGraphicsCore pGraphicsCore, DWORD indexCount, WORD* pSrc = 0 );
			~IndexBuffer();

		private:
			IndexBuffer( const IndexBuffer& root );

			//Операторы
			IndexBuffer& operator=( const IndexBuffer& root );

		public:
			//Получение доступа к буферу для изменений
			WORD* lockBuffer();
			void unlockBuffer();

			//Активация буфера как источника индексов
			int activate();

			//Getter-ы
			DWORD getIndexCount();

		private:
			//Внутренние данные буфера вершин
			LPGraphicsCore mpGraphicsCore;
			LPDIRECT3DINDEXBUFFER9 mpBuffer;
			DWORD mIndexCount;
			
	} *LPIndexBuffer;


	//*************
	//Кость
	//*************
	typedef class Bone
	{
		public:
			//Конструкторы и деструктор
			Bone( LPCTSTR pName, DWORD vertexCount );
			~Bone();

		private:
			Bone( const Bone& root );

			//Операторы
			Bone& operator=( const Bone& root );

		public:

			//Setter-ы
			void setName( LPCTSTR pName );
			void setVertexCount( DWORD vertexCount );
			void setVertices( WORD* pVertices, DWORD vertexCount );
			void setVerticesPointer( WORD* pVertices );
			void setWeights( float* pWeights, DWORD vertexCount );
			void setWeightsPointer( float* pWeights );

			//Getter-ы
			LPCTSTR getName();
			DWORD getVertexCount();
			WORD* getVertices();
			float* getWeights();

		private:
			//Внутренние данные кости
			tstring mName;
			DWORD mVertexCount;
			WORD* maVertices;
			float* maWeights;

	} *LPBone;


	//*************
	//Скелетная информация
	//*************
	typedef class SkeletInfo
	{
		public:
			//Конструкторы и деструктор
			SkeletInfo( LPCTSTR pName, DWORD boneCount );
			~SkeletInfo();

		private:
			SkeletInfo( const SkeletInfo& root );

			//Операторы
			SkeletInfo& operator=( const SkeletInfo& root );

		public:
			//Клонирование объекта с учетом использования совместных ресурсов
			SkeletInfo* clone();

			//Setter-ы
			void setName( LPCTSTR pName );
			void setBone( LPBone pBone, DWORD boneIndex );
			void setBoneMatrixTransformation( LPCTSTR pBoneName, D3DXMATRIX* pMatrixTransformation );

			//Getter-ы
			LPCTSTR getName();
			DWORD getBoneCount();
			const std::shared_ptr< Bone >* getBones();
			D3DXMATRIX* getBoneMatrixTransformation( DWORD boneIndex );

		private:
			tstring mName;
			DWORD mBoneCount;
			std::shared_ptr< Bone >* maspBones;
			D3DXMATRIX** mapBoneMatrices;

	} *LPSkeletInfo;


	//*************
	//Стандартная сетка ( Используется индексированный набор треугольников )
	//*************
	#define SM_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

	typedef struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	} *LPVertex;

	typedef struct TFace
	{
		WORD i1, i2, i3;
	} *LPTFace;

	typedef class Mesh
	{
			//Конструкторы и деструктор
		public:
			Mesh( LPGraphicsCore pGraphicsCore );
			~Mesh();

		private:
			Mesh( const Mesh& root );

		public:
			//Setter-ы
			void setVertices( LPVertexBuffer pVertexBuffer );
			void setVertices( DWORD vertexCount, void* pVertices );
			void setIndices( LPIndexBuffer pIndexBuffer );
			void setIndices( DWORD indexCount, WORD* pIndices );
			void setFaceGroupCount( DWORD groupCount );
			void setFaceGroup( DWORD id, DWORD offset, DWORD faceCount, DWORD indexBase );

			//Getter-ы
			DWORD getGroupCount();

			//Обновление вершин
			void initNormals();
			void updateMesh( LPSkeletInfo pSkeletInfo = 0 );

			//Рисование сетки
			void draw();
			void drawSubset( DWORD id );

		private:
			//Внутренние данные сетки
			typedef struct FaceGroup
			{
				public:
				DWORD offset;
				DWORD count;
				DWORD indexBase;
			} *LPFaceGroup;

			LPGraphicsCore mpGraphicsCore;
			LPVertexBuffer mpVertexBuffer;
			LPVertex maOriginalVertexBuffer;
			LPIndexBuffer mpIndexBuffer;
			DWORD mFaceGroupCount;
			LPFaceGroup maFaceGroups;

	} *LPMesh;


	//*************
	//Кожа сетки ( Набор материалов и текстур, учитывающих внутреннюю группировку сетки )
	//*************
	typedef class Skin
	{
			//Конструкторы и деструктор
		public:
			Skin( LPGraphicsCore pGraphicsCore, LPCTSTR pName, DWORD materialCount );
			~Skin();

		private:
			Skin( const Skin& root );

			//Операторы
			Skin& operator=( Skin& root );

		public:
			//Клонирование объекта с учетом использования совместных ресурсов
			Skin* clone();

			//Setter-ы
			void setName( LPCTSTR pName );
			void setMaterial( DWORD id, LPMaterial pMaterial );
			void setMaterialPointer( DWORD id, LPMaterial pMaterial );

			//Getter-ы
			LPCTSTR getName();
			DWORD getMaterialCount();
			LPMaterial getMaterial( DWORD id );

			//Активация материалов
			void activate( DWORD id );

		private:
			//Внутренние данные кожи
			LPGraphicsCore mpGraphicsCore;
			tstring mName;
			DWORD mMaterialCount;
			LPMaterial* mapMaterials;

	} *LPSkin;


	//*************
	//Скелетная иерархия
	//*************
	typedef class Frame
	{
			//Конструкторы и деструктор
		public:
			Frame( LPCTSTR pName, D3DXMATRIX* pBaseMatrix = 0, Frame* pParent = 0 );
			~Frame();

		private:
			Frame( const Frame &root );

			//Операторы
			Frame& operator=( const Frame& root );

		public:
			//Клонирование иерархии
			Frame* clone();

			//Итерирование в дереве
			Frame* find( LPCTSTR pName );
			Frame* next();

			//Setter-ы
			void addChildFrame( Frame* pFrame );
			void addSiblingFrame( Frame* pFrame );
			void setBasicMatrix( D3DXMATRIX* pMatrix );
			void setTransformationMatrix( D3DXMATRIX* pMatrix );
			void addTransformationMatrix( D3DXMATRIX* pMatrix );
			void setName( LPCTSTR pName );

			//Getter-ы
			LPCTSTR getName();
			D3DXMATRIX* getMatrixTransformation();
			D3DXMATRIX* getMatrixBase();
			D3DXMATRIX* getMatrixCombined();

			//Обновление преобразований
			void updateHierarchy();
			void toResetPose();

		private:
			//Внутренние данные фрейма
			tstring mName;
			Frame* mpParent;
			Frame* mpSibling;
			Frame* mpChild;
			D3DXMATRIX mMatrixBase;
			D3DXMATRIX mMatrixTransformation;
			D3DXMATRIX mMatrixCombined;

	} *LPFrame;


	//*************
	//Анимация
	//*************
	typedef class AnimationKey
	{
		public:
			//Конструкторы и деструктор
			AnimationKey( DWORD timeKey, D3DXMATRIX *pMatrix );
			~AnimationKey();
		
		private:
			AnimationKey( const AnimationKey& root );
			
			//Операторы
			AnimationKey& operator=( const AnimationKey& root );

		public:

			//Добавление ключевого кадра
			void addAnimationKey( AnimationKey* pAnimationKey );

			//Getter-ы
			DWORD getTimeKey();
			D3DXMATRIX getMatrixTransformation();
			AnimationKey* getNext();

		private:
			//Внутренние данные ключевого кадра
			DWORD mTimeKey;
			D3DXMATRIX mMatrixTransformation;
			AnimationKey* mpNextAnimationKey;

	} *LPAnimationKey;


	typedef class AnimationBone
	{
		public:
			//Конструкторы и деструктор
			AnimationBone( LPCTSTR pName, LPAnimationKey pKeys = 0 );
			~AnimationBone();

		private:
			AnimationBone( const AnimationBone& root );
			//Операторы
			AnimationBone& operator=( const AnimationBone& root );

		public:
			//Клонирование с учетом использования совместных ресурсов
			AnimationBone* clone();

			//Setter-ы
			void setAnimationKeys( LPAnimationKey pAnimationKeys );
			void addAnimationBone( AnimationBone* pAnimationBone );
			void setFramePointer( LPFrame pFrame );

			//Getter-ы
			LPCTSTR getName();
			AnimationBone* getNext();
			DWORD getMaxTime();

			//Обновление связанной кости
			bool update( DWORD timeKey );
			void toBase();

		private:
			//Внутренние данные анимации кости
			tstring mName;
			LPFrame mpFrame;
			std::shared_ptr< AnimationKey > mspKeys;
			AnimationBone* mpNextAnimationBone;

	} *LPAnimationBone;


	typedef class Animation
	{
		public:
			//Конструкторы и деструктор
			Animation( LPCTSTR pName );
			~Animation();

		private:
			Animation( const Animation& root );

			//Операторы
			Animation& operator=( const Animation& root );

		public:
			//Клонирование с учетом использования совместных ресурсов
			Animation* clone();
			
			//Добавление элементов
			void addAnimationBone( LPAnimationBone pAnimationBone );
			void addAnimation( Animation* pAnimation );

			//Getter-ы
			LPCTSTR getName();
			Animation* getNext();
			Animation* getAnimation( LPCTSTR pName );
			bool isEnabled();
			bool isPaused();

			//Привязка костей к иерархии
			void bindFrames( LPFrame pRootFrame );

			//Управление ходом анимации
			void enableAnimation( bool bEnable, bool bRepeat );
			void pause( bool bPause );
			bool update( DWORD deltaTime );

		private:
			//Внутренние данные анимации
			tstring mName;
			DWORD mLastTime;
			DWORD mMaxTime;
			bool mEnable;
			bool mRepeat;
			bool mPause;
			LPAnimationBone mpAnimationBones;
			Animation* mpNextAnimation;
			LPFrame mpRootFrame;

	} *LPAnimation;


	//*************
	//Мировые преобразования
	//*************
	typedef class WorldPosition
	{
		public:
			//Конструкторы и деструктор
			WorldPosition();
			WorldPosition( const WorldPosition& root );
			~WorldPosition();

			//Getter-ы
			D3DXMATRIX* getWorldTransformMatrix();
			D3DXVECTOR3 getPosition();

			//Setter-ы
			void setCombineMatrixPointer( D3DXMATRIX* pCombineMatrix = 0 );

			//Управление позицией
			void translate( float xPos, float yPos, float zPos );
			void translateRel( float xAdd, float yAdd, float zAdd );
			void move( float ahead, float right, float up );
			void rotate( float yaw, float pitch, float roll );
			void rotateRel( float yawAdd, float pitchAdd, float rollAdd );
			void scale( float xScale, float yScale, float zScale );
			void scaleRel( float xAdd, float yAdd, float zAdd );

		private:
			//Обновление матриц
			void updatePositionMatrix();

			//Внутренни данные мировой позиции
			D3DXVECTOR3 mPosition;
			D3DXQUATERNION mRotation;
			D3DXVECTOR3 mScale;
			D3DXMATRIX* mpCombineMatrix;
			D3DXMATRIX mPositionMatrix;

	} *LPWorldPosition;


	//*************
	//Графический объект
	//*************
	typedef class GraphicsObject
	{
			//Конструкторы и деструктор
		public:
			GraphicsObject( LPGraphicsCore pGraphicsCore, LPCTSTR pModelName = 0 );
			~GraphicsObject();

		protected:
			GraphicsObject( const GraphicsObject& root );

		public:
			//Клонирование объекта как модели - клон использует совместные ресурсы
			GraphicsObject* clone();

			//Setter-ы
			void setModelName( LPCTSTR pModelName );
			void setMeshPointer( LPMesh pMesh );
			void setSkinPointer( LPSkin pSkin );
			void setSkeletInfoPointer( LPSkeletInfo pSkeletInfo );
			void setFramePointer( LPFrame pRootFrame );
			void setAnimationPointer( LPAnimation pAnimation );
			void setParameters( LPCTSTR pModelName, LPMesh pMesh, LPSkin pSkin, LPSkeletInfo pSkeletInfo, LPFrame pRootFrame, LPAnimation pAnimation );

			//Getter-ы
			std::weak_ptr< WorldPosition > getWorldPosition();
			LPCTSTR getModelName();

			//Привязка внутренних объектов
			void bindFrameObjects();

			//Отрисовка объекта
			void draw();

			//Обновление иерархии
			void update( DWORD deltaTime );

			//Управление анимацией
			void enableAnimation( LPCTSTR pName, bool enable, bool repeat );
			void pauseAnimation( LPCTSTR pName, bool pause );

			//Управление положением и размером объекта
			void translate( D3DXVECTOR3 point );		
			void move( float ahead, float right, float up );
			void rotate( float yaw, float pitch, float roll );
			void scale( D3DXVECTOR3 point );
			void translateRel( D3DXVECTOR3 point );
			void rotateRel( float yaw, float pitch, float roll );
			void scaleRel( D3DXVECTOR3 point );

			LPMesh getMesh()
			{
				return mspMesh.get();
			}

		private:
			//Внутренние данные графического объекта
			LPGraphicsCore mpGraphicsCore;
			tstring mModelName;
			std::shared_ptr< Mesh > mspMesh;
			LPSkin mpSkin;
			LPSkeletInfo mpSkeletInfo;
			LPFrame mpRootFrame;
			LPAnimation mpAnimation;
			std::shared_ptr< WorldPosition > mspWorldPosition;

	} *LPGraphicsObject;


	//*************
	//Камера
	//*************
	typedef class Camera
	{
			//Конструкторы и деструктор
		private:
			Camera( LPGraphicsCore pGraphicsCore, float minDistance, float maxDistance );
			Camera( const Camera& root );
			~Camera();

		public:
			//Управление синглтоном
			static Camera* getInstance();
			static void createCamera( LPGraphicsCore pGraphicsCore, float minDistance, float maxDistance );
			static void destroy();

			//Активация камеры
			void activate();

			//Setter-ы
			void setPosition( D3DXVECTOR3 point );
			void setFocusDistance( float value );
			void setOrientation( float horAngle, float vertAngle );
			void enableLockingViewPoint( bool enable );
			void enableFollowing( bool enable );

			//Привязка камеры к позиции
			void setWorldPositionPointer( std::weak_ptr< WorldPosition > spWorldPosition, bool lockViewPoint, bool following );
			void release();

			//Управление камерой
			void distance( float value );
			void translate( D3DXVECTOR3 point );
			void move( float ahead, float right, float up );
			void rotate( float horAngle, float vertAngle );

			//Getter-ы
			D3DXVECTOR3 getPosition();

		private:
			//Внутренние данные камеры
			static Camera* mpInstance;

			LPGraphicsCore mpGraphicsCore;
			D3DXVECTOR3 mPosition;
			float mHorAngle;
			float mVertAngle;
			D3DXQUATERNION mOrientation;
			float mFocusDistance;

			//Ограничения
			float mMinDistance;
			float mMaxDistance;
			std::weak_ptr< WorldPosition > mwpWorldPosition;
			bool mFollowing;
			bool mLockedViewPoint;
			D3DXMATRIX mViewMatrix;

			//Инициализация матрицы просмотра
			void initViewMatrix();

	} *LPCamera;


	//*************
	//Источники света
	//*************
	enum LIGHT
	{
		LIGHT_POINT = D3DLIGHT_POINT,
		LIGHT_SPOT = D3DLIGHT_SPOT,
		LIGHT_DIRECTIONAL = D3DLIGHT_DIRECTIONAL
	};

	typedef class Light : D3DLIGHT9
	{
		public:
			//Конструкторы и деструктор
			Light( LIGHT type, float x, float y, float z, float red, float green, float blue, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ );
			Light( const Light& aLight );
			~Light();

			//Создание источников света
			static Light* createLightPoint( float x, float y, float z, float red, float green, float blue, float range );
			static Light* createLightSpot( float x, float y, float z, float red, float green, float blue, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ );
			static Light* createLightDirectional( float red, float green, float blue, float directionX, float directionY, float directionZ );
			static Light* createLightPointWhite( float x, float y, float z, float range );
			static Light* createLightSpotWhite( float x, float y, float z, float range, float falloff, float phi, float theta, float directionX, float directionY, float directionZ );
			static Light* createLightDirectionalWhite( float directionX, float directionY, float directionZ );
			
			//Setter-ы
			void setLightType( LIGHT type );
			void setPosition( float x, float y, float z );
			void setAmbientColor( float red, float green, float blue );
			void setDiffuseColor( float red, float green, float blue );
			void setRange( float range );
			void setCone( float falloff, float phi, float theta );
			void setDirection( float directionX, float directionY, float directionZ );

	} *LPLight;


	typedef class Sprite
	{
		public:
			Sprite( LPGraphicsCore pGraphicsCore,
					std::shared_ptr< Texture > pTexture = 0,
					RECT textureRect = { 0, 0, 0, 0 },
					DWORD textureColor = 0xffffffff,
					std::shared_ptr< Font > pFont = 0,
					DWORD fontColor = 0xff000000,
					DWORD fontFormat = DT_LEFT | DT_TOP );
			~Sprite();

			void setTexture( std::shared_ptr< Texture > spTexture );
			void setTextureRect( RECT rect );
			void setTextureColor( DWORD color );
			void setFont( std::shared_ptr< Font > spFont );
			void setFontColor( DWORD color );
			void setFontFormat( DWORD format );

			std::shared_ptr< Texture > getTexture();
			RECT getTextureRect();
			DWORD getTextureColor();
			std::shared_ptr< Font > getFont();
			DWORD getFontColor();
			DWORD getFontFormat();

			void draw( RECT rect );
			void print( LPCTSTR pText, RECT rect );
			void print( tstring text, RECT rect );
			void print( int num, RECT rect );
			void print( float num, RECT rect );

		private:
			LPGraphicsCore mpGraphicsCore;
			std::shared_ptr< Texture > mspTexture;
			RECT mTextureRect;
			DWORD mTextureColor;
			std::shared_ptr< Font > mspFont;
			DWORD mFontColor;
			DWORD mFontFormat;
	} *LPSprite;


	typedef class Graphics2DObject
	{
		public:
			Graphics2DObject( LPGraphicsCore pGraphicsCore, LONG x, LONG y, LONG width, LONG height );
			Graphics2DObject( const Graphics2DObject& root );
			~Graphics2DObject();

			Graphics2DObject* clone();

			void setPosition( LONG x, LONG y );
			void setPosition( POINT pt );
			void setSize( LONG width, LONG height );
			void setFontOffsetRect( RECT rect );
			void setOffset( LONG x, LONG y );
			void setOffset( POINT pt );
			void addOffset( LONG x, LONG y );
			void addOffset( POINT pt );
			void setStateSprite( UINT stateId, std::shared_ptr< Sprite > spSprite );
			void setState( UINT stateId );

			void move( LONG x, LONG y );

			void getPosition( LONG* x, LONG* y );
			void getPosition( POINT* pPt );
			void getSize( LONG* width, LONG* height );
			void getSize( POINT* pPt );
			RECT getVisibleRect();
			RECT getFontRect();
			void getOffset( LONG* x, LONG* y );
			void getOffset( POINT* pPt );
			std::shared_ptr< Sprite > getStateSprite( UINT stateId );
			LPSprite getCurrentSprite();

			void print( LPCTSTR pCharString );
			void print( int num );
			void print( tstring& string );
			void print( float num );
			void draw( DWORD deltaTime );

		private:
			LPGraphicsCore mpGraphicsCore;
			LONG mXPosition;
			LONG mYPosition;
			LONG mWidth;
			LONG mHeight;
			RECT mFontRect;
			POINT mOffset;
			LONG mCurrentSprite;
			std::map< UINT, std::shared_ptr< Sprite > > mStateSprites;

	} *LPGraphics2DObject, G2DObject, *LPG2DObject;
	

	//*************
	//Управление ресурсами
	//*************
	#define AUTO_RESOURCE_ID	10000

	typedef class GraphicsResourceManager
	{
		public:
			//Конструктор и деструктор
			GraphicsResourceManager( LPGraphicsCore pGraphicsCore );
			~GraphicsResourceManager();

			//Создание ресурсов
			ULONG create3DPlaneModel( float width, float height, ULONG textureId, DWORD color, float partWidth, float partHeight );
			ULONG create3DCubeModel( float width, float height, float deep, ULONG textureId, DWORD color, bool outside );
			ULONG create3DSphereModel( float radius, ULONG segments, ULONG rings, ULONG textureId, DWORD color, bool outside );
			ULONG create3DSkyBoxModel( float radius, ULONG textureId, DWORD color );
			ULONG create2DStaticModel();

			//Загрузка ресурсов из файла
			ULONG loadTextureFromFile( LPCTSTR pFileName, ULONG id = 0 );
			ULONG load3DModelFromFile( LPCTSTR pFileName, ULONG id = 0 );
			ULONG load2DModelFromFile( LPCTSTR pFileName, ULONG id = 0 );

			//Добавление ресурсов в память
			ULONG addTextureResource( LPTexture pTexture, ULONG id = 0, LPCTSTR pFileName = 0 );
			ULONG addFontResource( LPFont pFont, ULONG id = 0, LPCTSTR pFileName = 0 );
			ULONG add3DObjectResource( LPGraphicsObject p3DObject, ULONG id = 0, LPCTSTR pFileName = 0 );
			ULONG add2DObjectResource( LPG2DObject p2DObject, ULONG id = 0, LPCTSTR pFileName = 0 );

			//Getter-ы
			LPGraphicsCore getGraphicsCore();
			std::shared_ptr< Texture > getTextureResource( ULONG id );
			std::shared_ptr< Font > getFontResource( ULONG id );
			std::shared_ptr< GraphicsObject > get3dObjectResource( ULONG id );
			std::shared_ptr< G2DObject > get2DObjectResource( ULONG id );

			//Удаление ресурсов
			void destroyAll();
			void destroyTextureResource( ULONG id );
			void destroyFontResource( ULONG id );
			void destroy3DObjectResource( ULONG id );
			void destroy2DObjectResource( ULONG id );

			//Обновление ресурсов
			void onLostDevice();
			void onResetDevice();

		private:

			template< typename T >
			struct Node
			{
				tstring fileName;
				std::shared_ptr< T > resource;
			};

			LPGraphicsCore mpGraphicsCore;
			std::map< ULONG, Node< Texture > > mTextures;
			std::map< ULONG, Node< Font > > mFonts;
			std::map< ULONG, Node< GraphicsObject > > m3DObjects;
			std::map< ULONG, Node< G2DObject > > m2DObjects;

	} *LPGraphicsResourceManager;
}

#endif