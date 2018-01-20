#pragma once

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Copy(Vertex** vertex, UINT* count);
	void Copy(VertexTextureNormal** vertex, UINT* count);

private:
	const wstring file;

	VertexTextureNormal* vertexData;
	UINT vertexCount;
};