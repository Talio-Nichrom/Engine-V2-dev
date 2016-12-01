#include "SMGraphics.h"

using namespace sm;

//*************
//Конструктор и деструктор
//*************
GraphicsResourceManager::GraphicsResourceManager( LPGraphicsCore pGraphicsCore ) :
	mpGraphicsCore( pGraphicsCore ),
	m2DObjects()
{
	m2DObjects = std::map< ULONG, Node< G2DObject > >();
}

GraphicsResourceManager::~GraphicsResourceManager()
{
	mTextures.clear();
	mFonts.clear();
	m3DObjects.clear();
	m2DObjects.clear();
	mpGraphicsCore = 0;
}


//*************
//Создание ресурсов
//*************
ULONG GraphicsResourceManager::create3DPlaneModel( float width, float height, ULONG textureId, DWORD color, float partWidth, float partHeight )
{
	LPGraphicsObject pLocalGraphicsObject = new GraphicsObject( mpGraphicsCore );

	LPMesh pMesh = 0;
	LPSkin pSkin = 0;
	LPSkeletInfo pSkeletInfo = 0;
	LPFrame pRootFrame = 0;
	LPAnimation pAnimation = 0;

	Vertex vertices[ 4 ] =
	{
		{ ( -0.5f * width ), 0.0f, ( 0.5f * height ), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ ( 0.5f * width ), 0.0f, ( 0.5f * height ), 0.0f, 1.0f, 0.0f, ( width / partWidth ), 0.0f },
		{ ( 0.5f * width ), 0.0f, ( -0.5f * height ), 0.0f, 1.0f, 0.0f, ( width / partWidth ), ( height / partHeight ) },
		{ ( -0.5f * width ), 0.0f, ( -0.5f * height ), 0.0f, 1.0f, 0.0f, 0.0f, ( height / partHeight ) },
	};

	USHORT indices[ 6 ] =
	{
		0, 1, 3,
		1, 2, 3,
	};

	pMesh = new Mesh( mpGraphicsCore );
	pMesh->setVertices( 4, vertices );
	pMesh->setIndices( 6, indices );
	pMesh->setFaceGroupCount( 1 );
	pMesh->setFaceGroup( 0, 0, 2, 0 );

	LPMaterial pMaterial = new Material( mpGraphicsCore, _T( "PlaneMaterial" ) );
	pMaterial->setMaterialAmbientColor( color );
	pMaterial->setMaterialDiffuseColor( color );
	pMaterial->setMaterialSpecularColor( color );

	pMaterial->setTexture( getTextureResource( textureId ) );

	pSkin = new Skin( mpGraphicsCore, _T( "PlaneSkin" ), 1 );
	pSkin->setMaterial( 0, pMaterial );

	s_deleteP( pMaterial );

	pLocalGraphicsObject->setParameters( _T( "Plane" ), pMesh, pSkin, pSkeletInfo, pRootFrame, pAnimation );

	return add3DObjectResource( pLocalGraphicsObject );
}

ULONG GraphicsResourceManager::create3DCubeModel( float width, float height, float deep, ULONG textureId, DWORD color, bool outside )
{
	LPGraphicsObject pLocalGraphicsObject = new GraphicsObject( mpGraphicsCore );

	LPMesh pMesh = 0;
	LPSkin pSkin = 0;
	LPSkeletInfo pSkeletInfo = 0;
	LPFrame pRootFrame = 0;
	LPAnimation pAnimation = 0;

	float w = width / 2.0f;
	float h = height / 2.0f;
	float d = deep / 2.0f;

	float out = outside ? 1.0f : -1.0f;

	Vertex vertices[ 24 ] = {
		//Front
		{ -w, h, -d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.25f },
		{ w, h, -d  * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.25f },
		{ w, -h, -d  * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f },
		{ -w, -h, -d  * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f },

		//Back
		{ w, h, d * out, 0.0f, 0.0f, 0.0f, 0.75f, 0.25f },
		{ -w, h, d * out, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f },
		{ -w, -h, d * out, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f },
		{ w, -h, d * out, 0.0f, 0.0f, 0.0f, 0.75f, 0.5f },

		//Left
		{ -w, h, d * out, 0.0f, 0.0f, 0.0f, 0.0f, 0.25f },
		{ -w, h, -d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.25f },
		{ -w, -h, -d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f },
		{ -w, -h, d * out, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f },

		//Right
		{ w, h, -d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.25f },
		{ w, h, d * out, 0.0f, 0.0f, 0.0f, 0.75f, 0.25f },
		{ w, -h, d * out, 0.0f, 0.0f, 0.0f, 0.75f, 0.5f },
		{ w, -h, -d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f },

		//Top
		{ -w, h, d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.0f },
		{ w, h, d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f },
		{ w, h, -d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.25f },
		{ -w, h, -d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.25f },

		//Bottom
		{ -w, -h, -d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f },
		{ w, -h, -d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f },
		{ w, -h, d * out, 0.0f, 0.0f, 0.0f, 0.5f, 0.75f },
		{ -w, -h, d * out, 0.0f, 0.0f, 0.0f, 0.25f, 0.75f }
	};

	USHORT indices[ 36 ] =
	{
		0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23
	};

	pMesh = new Mesh( mpGraphicsCore );
	pMesh->setVertices( 24, vertices );
	pMesh->setIndices( 36, indices );
	pMesh->setFaceGroupCount( 1 );
	pMesh->setFaceGroup( 0, 0, 12, 0 );

	LPMaterial pMaterial = new Material( mpGraphicsCore, _T( "CubeMaterial" ) );
	pMaterial->setMaterialAmbientColor( color );
	pMaterial->setMaterialDiffuseColor( color );
	pMaterial->setMaterialSpecularColor( color );

	pMaterial->setTexture( getTextureResource( textureId ) );

	pSkin = new Skin( mpGraphicsCore, _T( "CubeSkin" ), 1 );
	pSkin->setMaterial( 0, pMaterial );

	s_deleteP( pMaterial );

	pLocalGraphicsObject->setParameters( _T( "Cube" ), pMesh, pSkin, pSkeletInfo, pRootFrame, pAnimation );

	return add3DObjectResource( pLocalGraphicsObject );
}

ULONG GraphicsResourceManager::create3DSphereModel( float radius, ULONG segments, ULONG rings, ULONG textureId, DWORD color, bool outside )
{
	LPGraphicsObject pLocalGraphicsObject = new GraphicsObject( mpGraphicsCore );

	LPMesh pMesh = 0;
	LPSkin pSkin = 0;
	LPSkeletInfo pSkeletInfo = 0;
	LPFrame pRootFrame = 0;
	LPAnimation pAnimation = 0;

	if( rings < 1 )
		rings = 1;

	if( segments < 4 )
		segments = 4;

	ULONG vertexCount = ( segments + 1 ) * ( rings + 2 );
	Vertex* vertices = new Vertex[ vertexCount ];

	float horAngle = D3DX_PI * 2.0f / segments;
	float verAngle = D3DX_PI / ( rings + 1 );
	float texturePartWidth = 1.0f / segments;
	float texturePartHeight = 1.0f / ( rings + 1 );
	if( outside )
		horAngle = -horAngle;

	for( ULONG i = 0; i <= segments; i++ )
	{
		for( ULONG j = 0; j < rings + 2; j++ )
		{
			float x = radius * sin( verAngle * j ) * sin( horAngle * i );
			float y = radius * cos( verAngle * j );
			float z = radius * sin( verAngle * j ) * cos( horAngle * i );

			vertices[ i * ( rings + 2 ) + j ] = { x, y, z, 0.0f, 0.0f, 0.0f, i * texturePartWidth, j * texturePartHeight };
		}
	}

	ULONG faceCount = segments * ( rings + 1 ) * 2;
	ULONG indexCount = faceCount * 3;
	USHORT* indices = new USHORT[ indexCount ];

	for( ULONG i = 0, currentIndex = 0; i < segments; i++ )
	{
		for( ULONG j = 0; j <= rings; j++ )
		{
			USHORT lt = USHORT( j + i * ( rings + 2 ) );
			USHORT rt = USHORT( j + ( i + 1 ) * ( rings + 2 ) );
			USHORT rb = USHORT( j + ( i + 1 ) * ( rings + 2 ) + 1 );
			USHORT lb = USHORT( j + i * ( rings + 2 ) + 1 );

			indices[ currentIndex ] = lt;
			currentIndex++;
			indices[ currentIndex ] = rt;
			currentIndex++;
			indices[ currentIndex ] = lb;
			currentIndex++;
			indices[ currentIndex ] = rt;
			currentIndex++;
			indices[ currentIndex ] = rb;
			currentIndex++;
			indices[ currentIndex ] = lb;
			currentIndex++;
		}
	}

	pMesh = new Mesh( mpGraphicsCore );
	pMesh->setVertices( vertexCount, vertices );
	pMesh->setIndices( indexCount, indices );
	pMesh->setFaceGroupCount( 1 );
	pMesh->setFaceGroup( 0, 0, faceCount, 0 );

	//ПРОВЕРИТЬ
	//s_deleteA( vertices );
	//s_deleteA( indices );

	LPMaterial pMaterial = new Material( mpGraphicsCore, _T( "SphereMaterial" ) );
	pMaterial->setMaterialAmbientColor( color );
	pMaterial->setMaterialDiffuseColor( color );
	pMaterial->setMaterialSpecularColor( color );

	pMaterial->setTexture( getTextureResource( textureId ) );

	pSkin = new Skin( mpGraphicsCore, _T( "SphereSkin" ), 1 );
	pSkin->setMaterial( 0, pMaterial );

	s_deleteP( pMaterial );

	pLocalGraphicsObject->setParameters( _T( "Sphere" ), pMesh, pSkin, pSkeletInfo, pRootFrame, pAnimation );

	return add3DObjectResource( pLocalGraphicsObject );
}

ULONG GraphicsResourceManager::create3DSkyBoxModel( float radius, ULONG textureId, DWORD color )
{
	return create3DCubeModel( radius * 2.0f, radius * 2.0f, radius * 2.0f, textureId, color,  false );
}

ULONG GraphicsResourceManager::create2DStaticModel()
{
	return 0;
}


//*************
//Загрузка ресурсов из файла
//*************
ULONG GraphicsResourceManager::loadTextureFromFile( LPCTSTR pFileName, ULONG id )
{
	if( !mpGraphicsCore || getTextureResource( id )|| !pFileName )
		return 0;

	tstring fileName = _T( "Textures//" );
	fileName += tstring( pFileName );

	for( auto elem = mTextures.begin(); elem != mTextures.end(); elem++ )
	{
		auto node = elem->second;
		if( node.fileName == fileName )
		{
			return elem->first;
		}
	}

	LPTexture pLocalTexture = new Texture( mpGraphicsCore, pFileName, fileName.c_str() );
	if( pLocalTexture->create() )
	{
		s_deleteP( pLocalTexture );
		return 0;
	}

	return addTextureResource( pLocalTexture, id, pFileName );
}

ULONG GraphicsResourceManager::load3DModelFromFile( LPCTSTR pFileName, ULONG id )
{
	if( !mpGraphicsCore || get3dObjectResource( id ) || !pFileName )
		return 0;

	tstring fileName = _T( "Models//" );
	fileName += tstring( pFileName );

	for( auto elem = m3DObjects.begin(); elem != m3DObjects.end(); elem++ )
	{
		auto node = elem->second;
		if( node.fileName == fileName )
		{
			return elem->first;
		}
	}

	//Загрузка из файл mops
	//Открытие файла
	FILE* pFile = 0;
	_tfopen_s( &pFile, fileName.c_str(), _T( "rb" ) );
	if( !pFile )
		return 0;

	//Считывание размера файла
	fseek( pFile, 0, SEEK_END );
	DWORD dataSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	//Считывание всего файла в буфер и закрытие файла
	byte* buffer = new byte[ dataSize ];
	fread_s( buffer, dataSize, sizeof( byte ), dataSize, pFile );

	fclose( pFile );

	//Обработка считанных данных
	std::map< std::string, byte* > dataPointers;
	std::string conv;
	byte* pDataPointer = buffer;

	//Проверка типа файла по главному чанку
	std::string main_chunk_id = std::string( (char*) pDataPointer );

	if( main_chunk_id != std::string( "Mops_in_bytes" ) )
		return 0;

	pDataPointer += 20 * sizeof( char ) + 2 * sizeof( ULONG );
	ULONG readBytes = 20 * sizeof( char ) + 2 * sizeof( ULONG );

	std::string chunkId;
	ULONG element_size = 0;
	ULONG element_count = 0;

	//Считывание данных
	//Разбитие файла на блоки данных
	while( readBytes < dataSize )
	{
		chunkId = std::string( (char*) pDataPointer );
		readBytes += 20 * sizeof( char );
		pDataPointer += 20 * sizeof( char );

		dataPointers[ chunkId ] = pDataPointer;
		element_size = *( (ULONG*) pDataPointer );
		pDataPointer += sizeof( ULONG );
		readBytes += sizeof( ULONG );
		element_count = *( (ULONG*) pDataPointer );
		pDataPointer += sizeof( ULONG );
		readBytes += sizeof( ULONG );
		pDataPointer += element_count * element_size;
		readBytes += element_count * element_size;

	}

	//Создание пустого объекта
	LPGraphicsObject pLocalGraphicsObject = new GraphicsObject( mpGraphicsCore );

	//Элементы инициализации объекта
	LPMesh pMesh = 0;
	LPSkin pSkin = 0;
	LPSkeletInfo pSkeletInfo = 0;
	LPFrame pRootFrame = 0;
	LPAnimation pAnimation = 0;

	//Создание скина
	byte* pMaterialsData = dataPointers.at( "MATT" );
	pMaterialsData += sizeof( ULONG );
	ULONG materialCount = *( (ULONG*) pMaterialsData );
	pMaterialsData += sizeof( ULONG );

	if( materialCount == 0 )
	{
		pSkin = new Skin( mpGraphicsCore, _T( "Empty Skin" ), 1 );
	}
	else
	{
		pSkin = new Skin( mpGraphicsCore, _T( "Standart Skin" ), materialCount );

		for( ULONG i = 0; i < materialCount; i++ )
		{
			conv = std::string( (char*) pMaterialsData );
			tstring materialName = stringToTstring( conv );
			pMaterialsData += 64 * sizeof( char );
			conv = std::string( (char*) pMaterialsData );
			tstring textureName = stringToTstring( conv );
			pMaterialsData += 64 * sizeof( char );
			conv = std::string( (char*) pMaterialsData );
			tstring textureFileName = stringToTstring( conv );
			pMaterialsData += 64 * sizeof( char );

			LPMaterial pMaterial = new Material( mpGraphicsCore, materialName.c_str() );

			ULONG textureId = loadTextureFromFile( textureFileName.c_str() );
			pMaterial->setTexture( getTextureResource( textureId ) );

			pMaterial->setMaterialAmbientColor( *( (ULONG*) pMaterialsData ) );
			pMaterialsData += sizeof( ULONG );
			pMaterial->setMaterialDiffuseColor( *( (ULONG*) pMaterialsData ) );
			pMaterialsData += sizeof( ULONG );
			pMaterial->setMaterialSpecularColor( *( (ULONG*) pMaterialsData ) );
			pMaterialsData += sizeof( ULONG );
			pMaterial->setMaterialEmmissiveColor( *( (ULONG*) pMaterialsData ) );
			pMaterialsData += sizeof( ULONG );
			pMaterial->setMaterialPower( *( (float*) pMaterialsData ) );
			pMaterialsData += sizeof( float );

			pSkin->setMaterialPointer( i, pMaterial );
		}
	}

	//Создание сетки
	byte* pPointsData = dataPointers.at( "PNTS" );
	byte* pVerticesData = dataPointers.at( "VERT" );
	byte* pFacesData = dataPointers.at( "FACE" );

	pPointsData += 2 * sizeof( ULONG );
	pVerticesData += sizeof( ULONG );
	pFacesData += sizeof( ULONG );

	//Количество вершин
	ULONG vertexCount = *( (ULONG*) pVerticesData );
	pVerticesData += sizeof( ULONG );

	//Количество граней
	DWORD faceCount = *( (DWORD*) pFacesData );
	pFacesData += sizeof( DWORD );

	LPVertexBuffer pVertexBuffer = new VertexBuffer( mpGraphicsCore, vertexCount, sizeof( Vertex ), SM_FVF );
	LPIndexBuffer pIndexBuffer = new IndexBuffer( mpGraphicsCore, faceCount * 3 );
	pMesh = new Mesh( mpGraphicsCore );

	//Установка групп граней
	if( materialCount == 0 )
	{
		pMesh->setFaceGroupCount( 1 );
	}
	else
	{
		pMesh->setFaceGroupCount( materialCount );
	}

	//Запись вершин
	Vertex* pVertices = (Vertex*) pVertexBuffer->lockBuffer();
	for( ULONG i = 0; i < vertexCount; i++ )
	{
		USHORT pointIndex = *( (USHORT*) pVerticesData );
		pVerticesData += sizeof( USHORT );
		pVertices->u = *( (float*) pVerticesData );
		pVerticesData += sizeof( float );
		pVertices->v = *( (float*) pVerticesData );
		pVerticesData += sizeof( float );

		float* point = (float*) pPointsData + 3 * pointIndex;
		pVertices->x = *point;
		point++;
		pVertices->y = *point;
		point++;
		pVertices->z = *point;

		pVertices++;
	}
	pVertexBuffer->unlockBuffer();

	//Запись индексов граней
	USHORT* pIndices = pIndexBuffer->lockBuffer();

	USHORT groupIndex = *( (USHORT*) pFacesData + 3 );

	for( ULONG i = 0, count = 0, offset = 0; i < faceCount; i++ )
	{
		*pIndices = *( (USHORT*) pFacesData );
		pIndices++;
		pFacesData += sizeof( USHORT );

		*pIndices = *( (USHORT*) pFacesData );
		pIndices++;
		pFacesData += sizeof( USHORT );

		*pIndices = *( (USHORT*) pFacesData );
		pIndices++;
		pFacesData += sizeof( USHORT );

		USHORT matIndex = *( (USHORT*) pFacesData );
		pFacesData += sizeof( USHORT );

		if( matIndex != groupIndex )
		{
			pMesh->setFaceGroup( groupIndex, offset, count, 0 );
			offset = i;
			count = 0;
			groupIndex = matIndex;
		}

		count++;

		if( i == faceCount - 1 )
		{
			pMesh->setFaceGroup( groupIndex, offset, count, 0 );
		}
	}
	pIndexBuffer->unlockBuffer();

	pMesh->setVertices( pVertexBuffer );
	pMesh->setIndices( pIndexBuffer );
	pMesh->initNormals();


	//Проверка наличия скелетной информации
	byte* pBonesData = dataPointers.at( "BONE" );
	byte* pInfluenceData = dataPointers.at( "INFLUENCE" );

	pBonesData += sizeof( ULONG );
	pInfluenceData += 2 * sizeof( ULONG );

	ULONG boneCount = *( (ULONG*) pBonesData );
	pBonesData += sizeof( ULONG );

	if( boneCount > 0 )
	{
		//Считывание скелетной структуры
		pSkeletInfo = new SkeletInfo( _T( "Standart Skelet" ), boneCount );
		pRootFrame = new Frame( _T( "Root" ) );

		for( ULONG i = 0; i < boneCount; i++ )
		{
			conv = std::string( (char*) pBonesData );
			tstring boneName = stringToTstring( conv );
			pBonesData += 64 * sizeof( char );
			conv = std::string( (char*) pBonesData );
			tstring boneParentName = stringToTstring( conv );
			pBonesData += 64 * sizeof( char );

			float positionX = *( (float*) pBonesData );
			pBonesData += sizeof( float );
			float positionY = *( (float*) pBonesData );
			pBonesData += sizeof( float );
			float positionZ = *( (float*) pBonesData );
			pBonesData += sizeof( float );

			float quatX = *( (float*) pBonesData );
			pBonesData += sizeof( float );
			float quatY = *( (float*) pBonesData );
			pBonesData += sizeof( float );
			float quatZ = *( (float*) pBonesData );
			pBonesData += sizeof( float );
			float quatW = *( (float*) pBonesData );
			pBonesData += sizeof( float );

			ULONG weightCount = *( (ULONG*) pBonesData );
			pBonesData += sizeof( ULONG );

			LPBone pBone = new Bone( boneName.c_str(), weightCount );
			USHORT* pBoneVertices = pBone->getVertices();
			float* pBoneWeights = pBone->getWeights();

			for( ULONG k = 0; k < weightCount; k++ )
			{
				ULONG boneIndex = *( (ULONG*) ( pInfluenceData ) );
				pInfluenceData += sizeof( ULONG );

				pBoneVertices[ k ] = USHORT( *( (ULONG*) ( pInfluenceData ) ) );
				pInfluenceData += sizeof( ULONG );
				pBoneWeights[ k ] = *( (float*) ( pInfluenceData ) );
				pInfluenceData += sizeof( float );
			}

			pSkeletInfo->setBone( pBone, i );

			D3DXMATRIX matrixBase;
			D3DXQUATERNION quat( quatX, quatY, quatZ, quatW );
			D3DXQuaternionNormalize( &quat, &quat );
			D3DXMatrixRotationQuaternion( &matrixBase, &quat );
			matrixBase._41 = positionX;
			matrixBase._42 = positionY;
			matrixBase._43 = positionZ;

			if( i == 0 )
			{
				pRootFrame->setName( boneName.c_str() );
				pRootFrame->setBasicMatrix( &matrixBase );
			}
			else
			{
				LPFrame pParentFrame = pRootFrame->find( boneParentName.c_str() );
				LPFrame pLocalFrame = new Frame( boneName.c_str(), &matrixBase, pParentFrame );
				if( !pParentFrame )
					pRootFrame->addSiblingFrame( pLocalFrame );
			}
		}
	}

	//Считывание анимаций
	byte* pAnimationsData = dataPointers[ "ANIMAT" ];
	byte* pAnimBonesData = dataPointers[ "ANIMBONE" ];
	byte* pAnimKeysData = dataPointers[ "ANIMKEY" ];

	pAnimationsData += sizeof( ULONG );
	ULONG animationCount = *( (ULONG*) pAnimationsData );
	pAnimationsData += sizeof( ULONG );

	pAnimBonesData += 2 * sizeof( ULONG );
	pAnimKeysData += 2 * sizeof( ULONG );

	for( ULONG i = 0; i < animationCount; i++ )
	{
		conv = std::string( (char*) ( pAnimationsData ) );
		tstring animationName = stringToTstring( conv );
		pAnimationsData += 64 * sizeof( char );

		LPAnimation pLocalAnimation = new Animation( animationName.c_str() );

		ULONG animBoneCount = *( (ULONG*) pAnimationsData );
		pAnimationsData += 3 * sizeof( ULONG );

		for( ULONG j = 0; j < animBoneCount; j++ )
		{
			conv = std::string( (char*) ( pAnimBonesData ) );
			tstring animBoneName = stringToTstring( conv );
			pAnimBonesData += 64 * sizeof( char );
			ULONG animKeyCount = *( (ULONG*) pAnimBonesData );
			pAnimBonesData += sizeof( ULONG );

			LPAnimationBone pLocalAnimationBone = new AnimationBone( animBoneName.c_str() );

			LPAnimationKey pAnimKey = 0;
			for( ULONG k = 0; k < animKeyCount; k++ )
			{
				float positionX = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );
				float positionY = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );
				float positionZ = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );

				float quatX = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );
				float quatY = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );
				float quatZ = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );
				float quatW = *( (float*) pAnimKeysData );
				pAnimKeysData += sizeof( float );

				ULONG timeKey = *( (ULONG*) pAnimKeysData );
				pAnimKeysData += sizeof( ULONG );

				D3DXMATRIX matrixTransform;
				D3DXQUATERNION quat( quatX, quatY, quatZ, quatW );
				D3DXQuaternionNormalize( &quat, &quat );
				D3DXMatrixRotationQuaternion( &matrixTransform, &quat );
				matrixTransform._41 = positionX;
				matrixTransform._42 = positionY;
				matrixTransform._43 = positionZ;

				LPAnimationKey pLocalKey = new AnimationKey( timeKey, &matrixTransform );

				if( pAnimKey )
				{
					pAnimKey->addAnimationKey( pLocalKey );
				}
				else
				{
					pAnimKey = pLocalKey;
				}

			}
			pLocalAnimationBone->setAnimationKeys( pAnimKey );
			pLocalAnimation->addAnimationBone( pLocalAnimationBone );
		}

		if( pAnimation )
		{
			pAnimation->addAnimation( pLocalAnimation );
		}
		else
		{
			pAnimation = pLocalAnimation;
		}
	}

	//Установка параметров объекта
	byte* pInfoData = dataPointers.at( "INFO" );
	pInfoData += 2 * sizeof( ULONG );
	
	conv = std::string( (char*) pInfoData );
	tstring objectName = stringToTstring( conv );
	pLocalGraphicsObject->setParameters( objectName.c_str(), pMesh, pSkin, pSkeletInfo, pRootFrame, pAnimation );

	s_deleteA( buffer );

	return add3DObjectResource( pLocalGraphicsObject, id, fileName.c_str() );
}

ULONG GraphicsResourceManager::load2DModelFromFile( LPCTSTR pFileName, ULONG id )
{
	//TODO
	//Загрузка из файла manx
	return 0;
}


//*************
//Добавление ресурсов в память
//*************
ULONG GraphicsResourceManager::addTextureResource( LPTexture pTexture, ULONG id, LPCTSTR pFileName )
{
	if( !pTexture )
		return 0;

	std::shared_ptr< Texture > spObject( pTexture );

	tstring fileName;
	if( pFileName )
		fileName = pFileName;
	else
		fileName = _T( "Texture Resource" );

	Node<Texture> node{ fileName, spObject };

	if( id == 0 )
	{
		id = mTextures.size() + AUTO_RESOURCE_ID;
		while( getTextureResource( id ) )
			id++;
	}

	mTextures.emplace( id, node );

	return id;
}

ULONG GraphicsResourceManager::addFontResource( LPFont pFont, ULONG id, LPCTSTR pFileName )
{
	if( !pFont )
		return 0;

	std::shared_ptr< Font > spObject( pFont );

	tstring fileName;
	if( pFileName )
		fileName = pFileName;
	else
		fileName = _T( "Font Resource" );

	Node<Font> node{ fileName, spObject };

	if( id == 0 )
	{
		id = mFonts.size() + AUTO_RESOURCE_ID;
		while( getFontResource( id ) )
			id++;
	}

	mFonts.emplace( id, node );

	return id;
}

ULONG GraphicsResourceManager::add3DObjectResource( LPGraphicsObject p3DObject, ULONG id, LPCTSTR pFileName )
{
	if( !p3DObject )
		return 0;

	std::shared_ptr< GraphicsObject > spObject( p3DObject );

	tstring fileName;
	if( pFileName )
		fileName = pFileName;
	else
		fileName = _T( "3DObject Resource" );

	Node<GraphicsObject> node{ fileName, spObject };

	if( id == 0 )
	{
		id = m3DObjects.size() + AUTO_RESOURCE_ID;
		while( get3dObjectResource( id ) )
			id++;
	}

	m3DObjects.emplace( id, node );

	return id;
}

ULONG GraphicsResourceManager::add2DObjectResource( LPG2DObject p2DObject, ULONG id, LPCTSTR pFileName )
{
	if( !p2DObject )
		return 0;

	std::shared_ptr< G2DObject > spObject( p2DObject );

	tstring fileName;
	if( pFileName )
		fileName = pFileName;
	else
		fileName = _T( "2DObject Resource" );

	Node<G2DObject> node{ fileName, spObject };

	if( id == 0 )
	{
		id = m2DObjects.size() + AUTO_RESOURCE_ID;
		while( get2DObjectResource( id ) )
			id++;
	}

	m2DObjects.emplace( id, node );

	return id;
}


//*************
//Getter-ы
//*************
LPGraphicsCore GraphicsResourceManager::getGraphicsCore()
{
	return mpGraphicsCore;
}

std::shared_ptr< Texture > GraphicsResourceManager::getTextureResource( ULONG id )
{
	try
	{
		return mTextures.at( id ).resource;
	}
	catch( const std::out_of_range& ex )
	{
		return 0;
	}
}

std::shared_ptr< Font > GraphicsResourceManager::getFontResource( ULONG id )
{
	try
	{
		return mFonts.at( id ).resource;
	}
	catch( const std::out_of_range& ex )
	{
		return 0;
	}
}

std::shared_ptr< GraphicsObject > GraphicsResourceManager::get3dObjectResource( ULONG id )
{
	try
	{
		return m3DObjects.at( id ).resource;
	}
	catch( const std::out_of_range& ex )
	{
		return 0;
	}
}

std::shared_ptr< G2DObject > GraphicsResourceManager::get2DObjectResource( ULONG id )
{
	try
	{
		return m2DObjects.at( id ).resource;
	}
	catch( const std::out_of_range& ex )
	{
		return 0;
	}
}


//*************
//Удаление ресурсов
//*************
void GraphicsResourceManager::destroyAll()
{
	mTextures.clear();
	mFonts.clear();
	m3DObjects.clear();
	m2DObjects.clear();
}

void GraphicsResourceManager::destroyTextureResource( ULONG id )
{
	mTextures.erase( id );
}

void GraphicsResourceManager::destroyFontResource( ULONG id )
{
	mFonts.erase( id );
}

void GraphicsResourceManager::destroy3DObjectResource( ULONG id )
{
	m3DObjects.erase( id );
}

void GraphicsResourceManager::destroy2DObjectResource( ULONG id )
{
	m2DObjects.erase( id );
}


//*************
//Обновление ресурсов
//*************
void GraphicsResourceManager::onLostDevice()
{
	for( auto elem = mFonts.begin(); elem != mFonts.end(); elem++ )
	{
		if( elem->second.resource )
		{
			elem->second.resource->disable();
		}
	}
}

void GraphicsResourceManager::onResetDevice()
{
	for( auto elem = mFonts.begin(); elem != mFonts.end(); elem++ )
	{
		if( elem->second.resource )
		{
			elem->second.resource->restore();
		}
	}
}