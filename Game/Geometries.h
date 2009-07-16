/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__GEOMETRIES_H__
#define	__GEOMETRIES_H__



#include <cmath>

#include <Ogre.h>


inline void createLine(const std::string& name, const float size = 1)
{
	MeshPtr line = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh* faceVertex[] =
	{
		line->createSubMesh("line"),
	};

	line->sharedVertexData = new VertexData();
	VertexData* vertexData = line->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);

	// allocate the vertex buffer
	vertexData->vertexCount = 24;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	Real* pVertex = static_cast<Real*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	unsigned short* pFaceIndices = NULL;
	HardwareIndexBufferSharedPtr iFaceBuf;
	faceVertex[0]->operationType = Ogre::RenderOperation::OT_LINE_LIST;
	faceVertex[0]->indexData->indexCount = 2;
	faceVertex[0]->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, faceVertex[0]->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	iFaceBuf = faceVertex[0]->indexData->indexBuffer;
	pFaceIndices = static_cast<unsigned short*>(iFaceBuf->lock(HardwareBuffer::HBL_DISCARD));

	static const float vertexBuffer[][3] =
	{	// x, y, z
		{0, 0, 0},
		{size, 0, 0,},
	};

	static const unsigned short faceIndexBuffer[] =
	{
		0,1,
	};

	memcpy(pVertex, vertexBuffer, sizeof(vertexBuffer));
	memcpy(pFaceIndices, faceIndexBuffer, sizeof(faceIndexBuffer));

	// Unlock
	vBuf->unlock();
	iFaceBuf->unlock();
	// Generate face list
	faceVertex[0]->useSharedVertices = true;

	// the original code was missing this line:
	line->_setBounds(AxisAlignedBox(Vector3(0, 0, 0), Vector3(size, 0, 0)), false);
	line->_setBoundingSphereRadius(size);

	// this line makes clear the mesh is loaded (avoids memory leakes)
	line->load();
}


inline void createCube(const std::string& name, const float size = 1)
{
	MeshPtr cube = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh* faceVertex[6] =
	{
		cube->createSubMesh("face0"),
		cube->createSubMesh("face1"),
		cube->createSubMesh("face2"),
		cube->createSubMesh("face3"),
		cube->createSubMesh("face4"),
		cube->createSubMesh("face5"),
	};

	cube->sharedVertexData = new VertexData();
	VertexData* vertexData = cube->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = 24;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	Real* pVertex = static_cast<Real*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	unsigned short* pFaceIndices[6];
	HardwareIndexBufferSharedPtr iFaceBuf[6];
	for(size_t i = 0; i < 6; ++ i)
	{
		faceVertex[i]->indexData->indexCount = 6;
		faceVertex[i]->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, faceVertex[i]->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		iFaceBuf[i] = faceVertex[i]->indexData->indexBuffer;
		pFaceIndices[i] = static_cast<unsigned short*>(iFaceBuf[i]->lock(HardwareBuffer::HBL_DISCARD));
	}

	const float halfsize = size / 2;
	static const float vertexBuffer[][8] =
	{	// x, y, z,							normal,		texcoord
		// right side
		{+halfsize, -halfsize, +halfsize,	1,0,0,		0,1,},
		{+halfsize, -halfsize, -halfsize,	1,0,0,		1,1,},
		{+halfsize, +halfsize, -halfsize,	1,0,0,		1,0,},
		{+halfsize, +halfsize, +halfsize,	1,0,0,		0,0,},

		// left side
		{-halfsize, -halfsize, -halfsize,	-1,0,0,		0,1,},
		{-halfsize, -halfsize, +halfsize,	-1,0,0,		1,1,},
		{-halfsize, +halfsize, +halfsize,	-1,0,0,		1,0,},
		{-halfsize, +halfsize, -halfsize,	-1,0,0,		0,0,}, 

		// up side
		{-halfsize, +halfsize, +halfsize,	0,1,0,		0,1,},
		{+halfsize, +halfsize, +halfsize,	0,1,0,		1,1,},
		{+halfsize, +halfsize, -halfsize,	0,1,0,		1,0,},
		{-halfsize, +halfsize, -halfsize,	0,1,0,		0,0,},

		// down side
		{-halfsize, -halfsize, -halfsize,	0,-1,0,		0,1,},
		{+halfsize, -halfsize, -halfsize,	0,-1,0,		1,1,},
		{+halfsize, -halfsize, +halfsize,	0,-1,0,		1,0,},
		{-halfsize, -halfsize, +halfsize,	0,-1,0,		0,0,},

		// front side
		{-halfsize, -halfsize, +halfsize,	0,0,1,		0,1,},
		{+halfsize, -halfsize, +halfsize,	0,0,1,		1,1,},
		{+halfsize, +halfsize, +halfsize,	0,0,1,		1,0,},
		{-halfsize, +halfsize, +halfsize,	0,0,1,		0,0,},

		// back side
		{+halfsize, -halfsize, -halfsize,	0,0,-1,		0,1,},
		{-halfsize, -halfsize, -halfsize,	0,0,-1,		1,1,},
		{-halfsize, +halfsize, -halfsize,	0,0,-1,		1,0,},
		{+halfsize, +halfsize, -halfsize,	0,0,-1,		0,0,},
	};

	static const unsigned short faceIndexBuffer[] =
	{
		// front
		0,1,2,
		0,2,3,

		// back
		4,5,6,
		4,6,7,

		// left
		8,9,10,
		8,10,11,

		// right
		12,13,14,
		12,14,15,

		// up
		16,17,18,
		16,18,19,

		// down
		20,21,22,
		20,22,23
	};

	memcpy(pVertex, vertexBuffer, sizeof(vertexBuffer));
	for(size_t i = 0; i < 6; ++ i)
		memcpy(pFaceIndices[i], &(faceIndexBuffer[i * 6]), sizeof(faceIndexBuffer) / 6);

	// Unlock
	vBuf->unlock();
	for(size_t i = 0; i < 6; ++ i)
	{
		iFaceBuf[i]->unlock();

		// Generate face list
		faceVertex[i]->useSharedVertices = true;
	}

	// the original code was missing this line:
	cube->_setBounds( AxisAlignedBox( Vector3(-halfsize, -halfsize, -halfsize), Vector3(halfsize, halfsize, halfsize) ), false );
	cube->_setBoundingSphereRadius(halfsize);
	// this line makes clear the mesh is loaded (avoids memory leakes)
	cube->load();
}


inline void createSphere(const std::string& name, const unsigned short rings, const unsigned short segments, const float radius = 1)
{
	assert(rings >= 2);
	assert(segments >= 3);

	MeshPtr sphere = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh* faceVertex = sphere->createSubMesh("face");

	sphere->sharedVertexData = new VertexData();
	VertexData* vertexData = sphere->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = (rings - 1) * (segments + 1) + 2;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	Real* pVertex = static_cast<Real*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	faceVertex->indexData->indexCount = (rings - 1) * segments * 6;
	faceVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, faceVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	HardwareIndexBufferSharedPtr iFaceBuf = faceVertex->indexData->indexBuffer;
	unsigned short* pFaceIndices = static_cast<unsigned short*>(iFaceBuf->lock(HardwareBuffer::HBL_DISCARD));

	// fill in vertex buffer
	{
		struct FillInVertexData
		{
			static void fill(Real* pVertex, const Real pitch, const Real yaw, const float radius)
			{
				// position
				pVertex[0] = radius * std::cos(yaw) * std::cos(pitch);
				pVertex[1] = radius * std::sin(pitch);
				pVertex[2] = radius * std::sin(yaw) * std::cos(pitch);

				// normal
				pVertex[3] = std::cos(yaw) * std::cos(pitch);
				pVertex[4] = std::sin(pitch);
				pVertex[5] = std::sin(yaw) * std::cos(pitch);

				// texture cordinates
				pVertex[6] = yaw / Math::TWO_PI;
				pVertex[7] = (pitch + Math::HALF_PI) / Math::PI;
			};
		};

		// top point
		FillInVertexData::fill(pVertex, Math::HALF_PI, 0, radius);
		pVertex += 8;

		// points on middle rings
		for(size_t i = 0; i < rings - 1u; ++ i)
			for(size_t j = 0; j < segments + 1u; ++ j)
			{
				const Real pitch = Math::HALF_PI - Math::PI * (i + 1) / rings;
				const Real yaw = Math::TWO_PI * j / segments;
				FillInVertexData::fill(pVertex, pitch, yaw, radius);

				pVertex += 8;
			}

		// bottom point
		FillInVertexData::fill(pVertex, -Math::HALF_PI, Math::TWO_PI, radius);
	}

	// fill in index buffer
	for(unsigned short i = 0; i < rings - 1; ++ i)
		for(unsigned short j = 0; j < segments; ++ j)
		{
			pFaceIndices[0] = (i == 0) ? 0 : ((i - 1) * (segments + 1) + j + 1 + 1);
			pFaceIndices[1] = i * (segments + 1) + j + 1 + 1;
			pFaceIndices[2] = i * (segments + 1) + j + 1;

			pFaceIndices += 3;

			pFaceIndices[0] = i * (segments + 1) + j + 1;
			pFaceIndices[1] = i * (segments + 1) + j + 1 + 1;
			pFaceIndices[2] = (i >= rings - 2) ? ((rings - 1) * (segments + 1) + 1) : ((i + 1) * (segments + 1) + j + 1);

			pFaceIndices += 3;
		}

	// Unlock
	vBuf->unlock();
	iFaceBuf->unlock();

	// Generate face list
	faceVertex->useSharedVertices = true;

	// the original code was missing this line:
	sphere->_setBounds(AxisAlignedBox(Vector3(-radius, -radius, -radius), Vector3(radius, radius, radius)), false);
	sphere->_setBoundingSphereRadius(radius);
	// this line makes clear the mesh is loaded (avoids memory leakes)
	sphere->load();
}

void createTetrahedron(const std::string& name, const float size = 1)
{
	MeshPtr tetra = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh* faceVertex[] =
	{
		tetra->createSubMesh("face0"),
		tetra->createSubMesh("face1"),
		tetra->createSubMesh("face2"),
		tetra->createSubMesh("face3"),
		/*tetra->createSubMesh("face4"),
		tetra->createSubMesh("face5"),
		tetra->createSubMesh("face6"),
		tetra->createSubMesh("face7"),*/
	};

	tetra->sharedVertexData = new VertexData();
	VertexData* vertexData = tetra->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = 4;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	Real* pVertex = static_cast<Real*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	unsigned short* pFaceIndices[4];
	HardwareIndexBufferSharedPtr iFaceBuf[4];
	for(size_t i = 0; i < 4; ++ i)
	{
		faceVertex[i]->indexData->indexCount = 3;
		faceVertex[i]->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, faceVertex[i]->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		iFaceBuf[i] = faceVertex[i]->indexData->indexBuffer;
		pFaceIndices[i] = static_cast<unsigned short*>(iFaceBuf[i]->lock(HardwareBuffer::HBL_DISCARD));
	}

	static const float sqrt6_3 = sqrt(6.0f) / 3;
	static const float sqrt3_6 = sqrt(3.0f) / 6;
	static const float vertexBuffer[][8] =
	{	// x, y, z,															normal,					texcoord
		{0,				sqrt6_3 * 0.75f * size,		0,						0, 1, 0,				0, 0},
		{-0.5f * size,	-sqrt6_3 * 0.25f * size,	-sqrt3_6 * size,		-0.5f, -0.2f, -sqrt3_6,	0, 0},
		{0.5f * size,	-sqrt6_3 * 0.25f * size,	-sqrt3_6 * size,		0.5f, -0.2f, -sqrt3_6,	0, 0},
		{0,				-sqrt6_3 * 0.25f * size,	sqrt3_6 * 2 * size,		0, -0.2f, sqrt3_6 * 2,	0, 0},

		/*{0,				-sqrt6_3 * 0.75f * size,		0,					0, -1, 0,				0, 0},
		{-0.5f * size,	sqrt6_3 * 0.25f * size,	sqrt3_6 * size,				-0.5f, 0.2f, sqrt3_6,	0, 0},
		{0.5f * size,	sqrt6_3 * 0.25f * size,	sqrt3_6 * size,				0.5f, 0.2f, sqrt3_6,	0, 0},
		{0,				sqrt6_3 * 0.25f * size,	-sqrt3_6 * 2 * size,		0, 0.2f, -sqrt3_6 * 2,	0, 0},*/
	};

	static const unsigned short faceIndexBuffer[] =
	{
		0, 2, 1,
		0, 3, 2,
		0, 1, 3,
		1, 2, 3,

		/*4, 6, 5,
		4, 7, 6,
		4, 5, 7,
		5, 6, 7,*/
	};

	memcpy(pVertex, vertexBuffer, sizeof(vertexBuffer));
	for(size_t i = 0; i < 4; ++ i)
		memcpy(pFaceIndices[i], &(faceIndexBuffer[i * 3]), sizeof(faceIndexBuffer) / 4);

	// Unlock
	vBuf->unlock();
	for(size_t i = 0; i < 4; ++ i)
	{
		iFaceBuf[i]->unlock();

		// Generate face list
		faceVertex[i]->useSharedVertices = true;
	}

	// the original code was missing this line:
	tetra->_setBounds(AxisAlignedBox(Vector3(-sqrt6_3 * size, -sqrt6_3 * size, -sqrt6_3 * size), Vector3(sqrt6_3 * size, sqrt6_3 * size, sqrt6_3 * size)), false);
	tetra->_setBoundingSphereRadius(sqrt6_3 * size);
	// this line makes clear the mesh is loaded (avoids memory leakes)
	tetra->load();
}



#endif	// !defined(__GEOMETRIES_H__)
