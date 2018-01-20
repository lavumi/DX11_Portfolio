#pragma once

class Mesh
{
public:
	Mesh(wstring type);
	~Mesh();

	//void Copy(VertexTextureNormal** vertex, UINT* vertexCount, UINT** index, UINT*indexCount);
	//void Copy(Vertex** vertex, UINT* count);

	VertexTextureNormal* vertexData;
	UINT vertexCount;

	UINT* indexData;
	UINT indexCount;

private:
	wstring file;
};