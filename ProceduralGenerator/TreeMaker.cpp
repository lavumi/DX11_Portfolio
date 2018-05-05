#include "../stdafx.h"
#include "TreeMaker.h"
#include "../Shader/NormalShaderBuffer.h"

TreeMaker::TreeMaker()
{
	root = 0;
	vertexBuffer = 0;
	indexBuffer = 0;

	leafVertexBuffer = 0;
	instanceBuffer = 0;


	diffuseMap = 0;
	normalMap = 0;

	wBuffer = 0;

	leafCount = 0;

}

TreeMaker::~TreeMaker()
{
	DeleteTree(root);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_RELEASE(leafVertexBuffer);
	SAFE_RELEASE(instanceBuffer);

	SAFE_RELEASE(diffuseMap);
	SAFE_RELEASE(normalMap);

	SAFE_DELETE(wBuffer);
	SAFE_DELETE(buffer);

}

void TreeMaker::Initialize(D3DXVECTOR3 position)
{
	wBuffer = new WorldBuffer();
	buffer = new NormalShaderBuffer();

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Environment/treebark_diffuse.jpg", nullptr, nullptr, &diffuseMap, nullptr);
	assert(SUCCEEDED(hr));
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Environment/treebark_normal.jpg", nullptr, nullptr, &normalMap, nullptr);
	assert(SUCCEEDED(hr));
	
	CreateTree(position.x, position.y, position.z);

	
}

void TreeMaker::CreateTree(float x, float y, float z)
{
	
	D3DXMATRIX scale;
	D3DXMatrixTranslation(&world, x, y, z);
	D3DXMatrixScaling(&scale, 0.3f, 0.3f, 0.3f);
	D3DXMatrixMultiply(&world, &scale, &world);

	wBuffer->SetWorld(world);
	buffer->SetWorld(world);
	buffer->SetMaterial(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),1,1,1);

	treePosition = D3DXVECTOR3(x, y, z);


	root = new TreeNode(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0),1);
	nodeCount = 0;
	leafInstanceData.clear();
	Branching(root);



	CreateMesh(root,0,1);
	

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(leafVertexBuffer);
	SAFE_RELEASE(instanceBuffer);

	CreateBuffer();

	DeleteTree(root);
	vertexData.clear();
	indexData.clear();
	//leafInstanceData.clear();

}



void TreeMaker::Render()
{
	UINT stride = sizeof(VertexTextureNormalTangent);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	wBuffer->SetBuffer();
	buffer->SetBuffers();

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &normalMap);
	//D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &heightMap);

	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	
}

void TreeMaker::LeafRender()
{
	UINT stride[2];
	stride[0] = sizeof(VertexTexture);
	stride[1] = sizeof(D3DXMATRIX);


	UINT offset[2];
	offset[0] = 0;
	offset[1] = 0;

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = leafVertexBuffer;
	bufferPointers[1] = instanceBuffer;


	D3D::GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	wBuffer->SetBuffer();
	D3D::GetDeviceContext()->DrawInstanced(6, leafCount, 0, 0);
	//D3D::GetDeviceContext()->Draw(vertexCount, 0);
}

void TreeMaker::Branching(TreeNode * node)
{

	node->height = 1;

	float widthVar = 0.9f;// (float)(rand() % (widthVariation[grade][1] - widthVariation[grade][0])) / 10 + (float)widthVariation[grade][0] / 10;


	float nextWidth = node->width *  widthVar;
	nodeCount++;
	

	//굵기에 따라 단계 적용
	int grade;
	if (node->width > 0.7f)
		grade = 0;
	else if (node->width > 0.4f)
		grade = 1;
	else if (node->width > 0.1f)
		grade = 2;
	else if (node->width > 0.02f)
		grade = 3;
	else 
		grade = 4;



	//4단계일 경우 잎사귀 위치 세팅
	if (grade == 4) {
		//node->leaf = true;
		LeafPosotionSetting(node);
		return;
	}
	if (nextWidth < 0.01f)		return;


	if (node->direction.y < 0)		node->direction.y *= -1;


	D3DXVECTOR3 nextNodePoint = node->point + node->direction;

	//가지를 꺾을 방향 설정
	//현재 가지에 대해 수직인 렌덤한 벡터 하나 생성
	D3DXVECTOR3 nextBranchVector;

	nextBranchVector.z = (float)(rand() % 100) / 100.0f - 0.5f;
	nextBranchVector.x = (float)(rand() % 100) / 100.0f - 0.5f;
	if (node->direction.y != 0)
		nextBranchVector.y = (0 - nextBranchVector.z * node->direction.z - nextBranchVector.x * node->direction.x) / node->direction.y;
	else
		nextBranchVector.y = 0;
	D3DXVec3Normalize(&nextBranchVector, &nextBranchVector);




	D3DXVECTOR3 branchDirection;
	TreeNode * subnode;
	float rnd = (float)(rand() % 100)/100;

	float branchAngle = (float)(rand() % (branchingMMAngle[grade][1] - branchingMMAngle[grade][0])) / 10 + (float)branchingMMAngle[grade][0] / 10;
	float curveAngle = (float)(rand() % (curveMMAngle[grade][1] - curveMMAngle[grade][0])) / 10 + (float)curveMMAngle[grade][0] / 10;


	if (rnd < branchingChanceByGrade[grade][0]) {

		branchDirection = node->direction + nextBranchVector * curveAngle;
		subnode = new TreeNode(nextNodePoint, branchDirection, nextWidth);
		node->child[0] = subnode;
		Branching(subnode);
	}
	else if (rnd < branchingChanceByGrade[grade][0] + branchingChanceByGrade[grade][1]) {
		branchDirection = node->direction + nextBranchVector * branchAngle;
		D3DXVec3Normalize(&branchDirection, &branchDirection);
		subnode = new TreeNode(nextNodePoint, branchDirection, nextWidth* 0.7f);
		node->child[0] = subnode;
		Branching(subnode);


		branchDirection = node->direction - nextBranchVector * branchAngle;
		D3DXVec3Normalize(&branchDirection, &branchDirection);
		subnode = new TreeNode(nextNodePoint, branchDirection, nextWidth* 0.7f);
		node->child[1] = subnode;
		Branching(subnode);

	}
	else
	{
		branchDirection = node->direction + nextBranchVector * branchAngle;
		D3DXVec3Normalize(&branchDirection, &branchDirection);
		subnode = new TreeNode(nextNodePoint, branchDirection, nextWidth * 0.8f);
		node->child[0] = subnode;
		Branching(subnode);


		branchDirection = node->direction + nextBranchVector * curveAngle;;
		D3DXVec3Normalize(&branchDirection, &branchDirection);
		subnode = new TreeNode(nextNodePoint, branchDirection, nextWidth);
		node->child[1] = subnode;
		Branching(subnode);
	}








	//nextBranchVector *= RandFloat(0.1f, 0.3f);





}

void TreeMaker::LeafPosotionSetting(TreeNode * node)
{
	D3DXVECTOR3 leafPos; 
	D3DXVECTOR3 leafSpace_x, leafSpace_y, leafSpace_z;// = D3DXVECTOR3(0, 1, 0);

	D3DXVec3Normalize(&leafSpace_z, &node->direction);


	leafSpace_x.z = 0;
	leafSpace_x.y = 1;
	if (node->direction.x != 0)
		leafSpace_x.x = (0 - leafSpace_x.z * node->direction.z - leafSpace_x.y * node->direction.y) / node->direction.x;
	else
		leafSpace_x.x = 0;

	D3DXVec3Normalize(&leafSpace_x, &leafSpace_x);

	
	D3DXVec3Cross(&leafSpace_y, &leafSpace_x, &leafSpace_z);
	D3DXVec3Normalize(&leafSpace_y, &leafSpace_y);



	D3DXMATRIX matrix, translate;
	//잎사귀를 적당한 공간을 만들어서 변화시키자
	//나뭇가지 벡터 + z 단위백터 + 외적 결과물 로 만든 공간으로 이전시킨다.

	//나무가지 끝에 달린 이파리의 space matrix
	matrix = D3DXMATRIX( 
		leafSpace_x.x,	leafSpace_x.y,	leafSpace_x.z,		0,
		leafSpace_y.x,	leafSpace_y.y,	leafSpace_y.z,		0,
		leafSpace_z.x,	leafSpace_z.y,	leafSpace_z.z,		0,
		0,				0,				0,					1
	);
	//나뭇가지 끝으로 이동
	
	leafPos = node->point + node->direction;
	D3DXMatrixTranslation(&translate, leafPos.x, leafPos.y, leafPos.z);
	D3DXMatrixMultiply(&matrix,&matrix, &translate);
	D3DXMatrixMultiply(&matrix, &matrix, &world);
	leafInstanceData.push_back(matrix);


	//여기부터 나뭇가지 중간에 달린 이파리
	//z+ 방향 이파리
	D3DXMATRIX matrixzp = D3DXMATRIX(
		leafSpace_z.x,	leafSpace_z.y,	leafSpace_z.z,		0,
		leafSpace_y.x,	leafSpace_y.y,	leafSpace_y.z,		0,
		leafSpace_x.x,	leafSpace_x.y,	leafSpace_x.z,		0,
		0,				0,				0,					1
	);

	D3DXMATRIX matrixzm = D3DXMATRIX(
		leafSpace_z.x,		leafSpace_z.y,		leafSpace_z.z,			0,
		leafSpace_y.x,		leafSpace_y.y,		leafSpace_y.z,			0,
		leafSpace_x.x*-1,	leafSpace_x.y*-1,	leafSpace_x.z*-1,		0,
		0,					0,					0,						1
	);


	float sideleafCount = 5;
	
	D3DXMATRIX treeTrans;
	D3DXMatrixTranslation(&treeTrans, treePosition.x, treePosition.y, treePosition.z);
	for (int i = 0; i < (int)sideleafCount; i++) {

		leafPos = node->point + node->direction * (float)i / sideleafCount;
		D3DXMatrixTranslation(&translate, leafPos.x, leafPos.y, leafPos.z);
		
		D3DXMatrixMultiply(&matrix, &matrixzp, &translate);
		D3DXMatrixMultiply(&matrix, &matrix, &world);
		leafInstanceData.push_back(matrix);

		D3DXMatrixMultiply(&matrix, &matrixzm, &translate);
		D3DXMatrixMultiply(&matrix, &matrix, &world);
		leafInstanceData.push_back(matrix);
	}






}

void TreeMaker::DeleteTree(TreeNode * node)
{
	if (node == 0)
		return;

	if (node->child[0] != nullptr)
		DeleteTree(node->child[0]);
	else
		SAFE_DELETE(node->child[0]);


	if (node->child[1] != nullptr)
		DeleteTree(node->child[1]);
	else
		SAFE_DELETE(node->child[1]);
}

void TreeMaker::CreateMesh(TreeNode * node, UINT lowerIndex, float uv)
{


	//float uv = node->grade;
	VertexTextureNormalTangent vertex;
	//vertex.color = D3DXCOLOR(0, 0, 0, 1);

	UINT upperIndex = (UINT)vertexData.size();

	if (upperIndex == 0) {
		vertex.position = D3DXVECTOR3(1, 0, 0);
		vertex.uv = D3DXVECTOR2(1, 0);
		vertex.normal = D3DXVECTOR3(1, 0, 0);
		D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
		vertexData.push_back(vertex);
		vertex.position = D3DXVECTOR3(-1, 0, 0);
		vertex.normal = D3DXVECTOR3(-1, 0, 0);
		D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
		vertexData.push_back(vertex);

		vertex.position = D3DXVECTOR3(0, 0, 1);
		vertex.uv = D3DXVECTOR2(0, 0);
		vertex.normal = D3DXVECTOR3(0, 0, 1);
		D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
		vertexData.push_back(vertex);
		vertex.position = D3DXVECTOR3(0, 0, -1);
		vertex.normal = D3DXVECTOR3(0, 0, -1);
		D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
		vertexData.push_back(vertex);
	
		upperIndex = (UINT)vertexData.size();
	}
		


	D3DXVECTOR3 trunkVector, trunkVectorCross;
	trunkVector.z = 1;
	trunkVector.x = 0;
	if (node->direction.y != 0)
		trunkVector.y = (0 - node->direction.z) / node->direction.y;
	else
		trunkVector.y = 0;
	
	D3DXVec3Cross(&trunkVectorCross, &trunkVector, &node->direction);
	//밑부분을 4개 버텍스로 형성
	
	//D3DXVec3Normalize(&trunkVector, &trunkVector);
	//D3DXVec3Normalize(&trunkVectorCross, &trunkVectorCross);
	//
	//trunkVector *= node->startWidth;
	//trunkVectorCross *= node->startWidth;
	//
	//vertex.position = node->point + trunkVector;
	//vertexData.push_back(vertex);
	//
	//vertex.position = node->point - trunkVector;
	//vertexData.push_back(vertex);
	//
	//vertex.position = node->point + trunkVectorCross;
	//vertexData.push_back(vertex);
	//
	//vertex.position = node->point - trunkVectorCross;
	//vertexData.push_back(vertex);

	//윗부분을 4개 버택스로
	D3DXVec3Normalize(&trunkVector, &trunkVector);
	D3DXVec3Normalize(&trunkVectorCross, &trunkVectorCross);

	trunkVector *= node->width;
	trunkVectorCross *= node->width;

	vertex.position = node->point + node->direction + trunkVector;
	vertex.uv = D3DXVECTOR2(0, uv);
	vertex.normal = trunkVector;
	D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
	vertexData.push_back(vertex);

	vertex.position = node->point + node->direction - trunkVector;
	vertex.normal = -trunkVector;
	D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
	vertexData.push_back(vertex);

	vertex.position = node->point + node->direction + trunkVectorCross;
	vertex.uv = D3DXVECTOR2(1, uv);
	vertex.normal = trunkVectorCross;
	D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
	vertexData.push_back(vertex);

	vertex.position = node->point + node->direction - trunkVectorCross;
	vertex.normal = -trunkVectorCross;
	D3DXVec3Cross(&vertex.tangent, &vertex.normal, &node->direction);
	vertexData.push_back(vertex);




	indexData.push_back(lowerIndex);
	indexData.push_back(upperIndex);
	indexData.push_back(lowerIndex+2);

	indexData.push_back(lowerIndex+2);
	indexData.push_back(upperIndex);
	indexData.push_back(upperIndex+2);


	indexData.push_back(lowerIndex+2);
	indexData.push_back(upperIndex+2);
	indexData.push_back(lowerIndex+1);

	indexData.push_back(lowerIndex+1);
	indexData.push_back(upperIndex+2);
	indexData.push_back(upperIndex+1);


	indexData.push_back(lowerIndex+1);
	indexData.push_back(upperIndex+1);
	indexData.push_back(lowerIndex+3);

	indexData.push_back(lowerIndex+3);
	indexData.push_back(upperIndex+1);
	indexData.push_back(upperIndex+3);


	indexData.push_back(lowerIndex+3);
	indexData.push_back(upperIndex+3);
	indexData.push_back(lowerIndex);

	indexData.push_back(lowerIndex);
	indexData.push_back(upperIndex+3);
	indexData.push_back(upperIndex);





	if (node->child[0] != nullptr)
		CreateMesh(node->child[0], upperIndex,!uv);
	if (node->child[1] != nullptr)
		CreateMesh(node->child[1], upperIndex,!uv);
}



void TreeMaker::CreateBuffer()
{
	HRESULT hr;
	


	vertexCount = vertexData.size();
	indexCount = indexData.size();

	
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	
	
	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalTangent) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &vertexData[0];
	
	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));
	
	
	
	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &indexData[0];
	
	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
	




	//Leaf Data
	VertexTexture* leafData = new VertexTexture[6];

	int i = 0;
	leafData[i].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			leafData[i++].uv = D3DXVECTOR2(0, 1);// D3DXVECTOR2(0, 1);
	leafData[i].position = D3DXVECTOR3(-0.2f, 0.0f, 0.2f);			leafData[i++].uv = D3DXVECTOR2(0, 0);// D3DXVECTOR2(0, 0);
	leafData[i].position = D3DXVECTOR3(0.2f, 0.0f, 0.2f);			leafData[i++].uv = D3DXVECTOR2(1, 1);// D3DXVECTOR2(1, 1);
																										 
	leafData[i].position = D3DXVECTOR3(0.0f, 0.0f, 0.4f);			leafData[i++].uv = D3DXVECTOR2(1, 0);// D3DXVECTOR2(1, 0);
	leafData[i].position = D3DXVECTOR3(0.2f, 0.0f, 0.2f);			leafData[i++].uv = D3DXVECTOR2(1, 1);// D3DXVECTOR2(1, 1);
	leafData[i].position = D3DXVECTOR3(-0.2f, 0.0f, 0.2f);			leafData[i++].uv = D3DXVECTOR2(0, 0);// D3DXVECTOR2(0, 0);
		

	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = leafData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &leafVertexBuffer);
	assert(SUCCEEDED(hr));


	SAFE_DELETE_ARRAY(leafData);


	leafCount = leafInstanceData.size();
	if (leafCount == 0)
		return;


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(D3DXMATRIX) * leafCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &leafInstanceData[0];
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;



	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
	assert(SUCCEEDED(hr));

	
	//SAFE_DELETE_ARRAY(vertexData);
//	SAFE_DELETE_ARRAY(indexData);
}

//void TreeGenerator::CreateBufferTest()
//{
//	HRESULT hr;
//
//
//	vertexCount = 24;
//	indexCount = 36;
//
//	VertexColor* testData = new VertexColor[24];
//	int i = 0;
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, 1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, 1.0, -1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, -1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(1.0, -1.0, 1.0);			 testData[i++].color = D3DXCOLOR(0,0,0,1);
//	testData[i].position = D3DXVECTOR3(-1.0, -1.0, 1.0);		 testData[i++].color = D3DXCOLOR(0,0,0,1);
//
//	UINT* testIndexData = new UINT[36]{
//	0, 1, 3, 2, 3, 1,
//	5, 4, 6, 4, 7, 6,
//	8, 11, 9, 11, 10, 9,
//	12, 13, 15, 13, 14, 15,
//	17, 16, 19, 17, 19, 18,
//	20, 21, 23,22, 23, 21 };
//
//	D3D11_BUFFER_DESC desc;
//	D3D11_SUBRESOURCE_DATA data;
//
//
//	//1. Vertex Buffer
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(VertexColor) * vertexCount;
//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//	data.pSysMem =testData;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
//	assert(SUCCEEDED(hr));
//
//
//
//	//2. Index Buffer
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(UINT) * indexCount;
//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//	data.pSysMem = testIndexData;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
//	assert(SUCCEEDED(hr));
//}
