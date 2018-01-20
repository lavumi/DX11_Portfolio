#include "../stdafx.h"
#include "Mesh.h"







Mesh::Mesh(wstring type)
	
{
	file = L"./Contents/Meshes/";
	file += type;
	file += L".data";

	ifstream ifs(file.c_str(), ios::binary);


	ifs.read((char*)&vertexCount, sizeof(UINT));
	vertexData = new VertexTextureNormal[vertexCount];
	ifs.read((char*)vertexData, sizeof(VertexTextureNormal)*vertexCount);
	

	ifs.read((char*)&indexCount, sizeof(UINT));
	indexData = new UINT[indexCount];
	ifs.read((char*)indexData, sizeof(UINT)*indexCount);

	ifs.close();
	

}

Mesh::~Mesh()
{
	SAFE_DELETE_ARRAY(vertexData);
}
//
//void Mesh::Copy(VertexTextureNormal** vertex, UINT* vtxCount, UINT** index, UINT* idxCount)
//{
//
//	*vtxCount = vertexCount;
//	*vertex = new VertexTextureNormal[vertexCount];
//	memcpy(*vertex, vertexData, sizeof(VertexTextureNormal) * vertexCount);
//
//	*idxCount = indexCount;
//	*index = new UINT[indexCount];
//	memcpy(*index, indexData, sizeof(UINT) * indexCount);
//
//
//
//}
//
//void Mesh::Copy(Vertex ** vertex, UINT* count)
//{
//	*count = vertexCount;
//	*vertex = new Vertex[vertexCount];
//
//	for (UINT i = 0; i < vertexCount; i++) {
//		(*vertex)[i] = vertexData[i].position;
//	}
//}
