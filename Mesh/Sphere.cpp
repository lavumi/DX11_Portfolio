#include "../stdafx.h"
#include "Sphere.h"

Sphere::Sphere()
	: file(L"./Contents/Meshes/Sphere.txt")
{
	wifstream stream(file);
	assert(stream.good() == true);

	wstring line;
	vector<wstring> lines;
	while (getline(stream, line))
		lines.push_back(line);


	vector<wstring> datas;

	vertexCount = (UINT)stoi(lines[0]);
	vertexData = new VertexTextureNormal[vertexCount];
	for (UINT i = 1; i <= vertexCount; i++)
	{
		datas.clear();
		String::SplitString(&datas, lines[i], L" ");

		vertexData[i - 1].position.x = stof(datas[0]);
		vertexData[i - 1].position.y = stof(datas[1]);
		vertexData[i - 1].position.z = stof(datas[2]);

		vertexData[i - 1].uv.x = stof(datas[3]);
		vertexData[i - 1].uv.y = stof(datas[4]);

		vertexData[i - 1].normal.x = stof(datas[5]);
		vertexData[i - 1].normal.y = stof(datas[6]);
		vertexData[i - 1].normal.z = stof(datas[7]);
	}
}

Sphere::~Sphere()
{
	SAFE_DELETE_ARRAY(vertexData);
}

void Sphere::Copy(Vertex ** vertex, UINT * count)
{
	*count = vertexCount;
	*vertex = new Vertex[vertexCount];

	for (UINT i = 0; i < vertexCount; i++)
		(*vertex)[i] = vertexData[i].position;
}

void Sphere::Copy(VertexTextureNormal ** vertex, UINT * count)
{
	*count = vertexCount;
	*vertex = new VertexTextureNormal[vertexCount];
	memcpy(*vertex, vertexData, sizeof(VertexTextureNormal) * vertexCount);
}

