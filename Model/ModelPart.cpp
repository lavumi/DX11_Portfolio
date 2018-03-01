#include "../stdafx.h"
#include "ModelPart.h"
#include "ModelScene.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelBoneWeights.h"
#include "ModelBuffer.h"

#include "../Shader/FBXModelShader.h"

ModelPart::ModelPart(Model * model)
	//: Shader(L"./Model/Model.fx")
	: model(model), material(NULL)
	, vertex(NULL), index(NULL)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	//CreateInputLayout(VertexTextureNormalTangentBlend::desc, VertexTextureNormalTangentBlend::count);
}

ModelPart::ModelPart(Model* model, ModelMaterial* material)
	//: Shader(L"./Model/Model.fx")
	: model(model), material(material)
	, vertex(NULL), index(NULL)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	materialIndex = material->GetIndex();
//	CreateInputLayout(VertexTextureNormalTangentBlend ::desc, VertexTextureNormalTangentBlend::count);
}

ModelPart::ModelPart(ModelPart & otherModelPart)
//	: Shader(L"./Model/Model.fx")
	: vertex(NULL), index(NULL)
	, vertexBuffer(NULL), indexBuffer(NULL)
{
	model = (model != NULL) ? otherModelPart.model : NULL;

	positions = otherModelPart.positions;
	normals = otherModelPart.normals;
	tangents = otherModelPart.tangents;
	uvs = otherModelPart.uvs;
	indices = otherModelPart.indices;
	boneWeights = otherModelPart.boneWeights;

	vertexCount = otherModelPart.vertexCount;
	vertex = new VertexTextureNormalTangentBlend[vertexCount];
	memcpy(vertex, otherModelPart.vertex, vertexCount * sizeof(VertexTextureNormalTangentBlend));

	indexCount = otherModelPart.indexCount;
	index = new UINT[indexCount];
	memcpy(index, otherModelPart.index, indexCount * sizeof(UINT));

	//modelBuffer = (modelBuffer != NULL) ? otherModelPart.modelBuffer : NULL;
	material = (material != NULL) ? otherModelPart.material : NULL;

	isSkinnedModel = otherModelPart.isSkinnedModel;

	//CreateInputLayout(VertexTextureNormalTangentBlend::desc, VertexTextureNormalTangentBlend::count);

	CreateBuffer();
}

ModelPart::~ModelPart()
{
	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void ModelPart::Update(bool isAnimation, D3DXMATRIX* worldPos)
{
	//D3DXMATRIX world;
	if (isAnimation == true)
	{
		if (isSkinnedModel == true)
			world = model->GetGeometricOffset();
		else
			world = model->GetGeometricOffset() * model->GetAnimationTransform();
	}
	else
		world = model->GetGeometricOffset();

	world *= model->GetAbsoluteTransform();



	world *= *worldPos;

//	worldBuffer->SetWorld(world);

	//Shader::Update();
}

void ModelPart::Render(FBXModelShader* shader)
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	UINT stride = sizeof(VertexTextureNormalTangentBlend);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	ID3D11ShaderResourceView* diffuseView = material->GetDiffuseView();
	dc->PSSetShaderResources(0, 1, &diffuseView);
	
	ID3D11ShaderResourceView* normalmapView = material->GetNormalMapView();
	dc->PSSetShaderResources(1, 1, &normalmapView);

	ID3D11ShaderResourceView* specularmapView = material->GetSpecularView();
	dc->PSSetShaderResources(2, 1, &specularmapView);


	D3DXMATRIX view, projection;
	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);

	//Shader::Render();
	shader->Render(indexCount, world, view, projection);
//	dc->DrawIndexed(indexCount, 0, 0);
}

void ModelPart::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv, const ModelBoneWeights& boneWeights)
{
	positions.push_back(position);
	normals.push_back(normal);
	tangents.push_back(D3DXVECTOR3(0, 0, 0));
	uvs.push_back(uv);
	indices.push_back((UINT)indices.size());
	
	this->boneWeights.push_back(boneWeights);
	if (boneWeights.GetBoneWeightCount() > 0)
		isSkinnedModel = true;
}

void ModelPart::CreateData()
{
	vertexCount = positions.size();
	indexCount = indices.size();

	vertex = new VertexTextureNormalTangentBlend[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertex[i].position = positions[i];
		vertex[i].normal = normals[i];
		vertex[i].uv = uvs[i];
	}

	for (UINT i = 0; i < boneWeights.size(); i++)
	{
		ModelBlendWeights weight = boneWeights[i].GetBlendWeights();
		vertex[i].blendIndices = weight.BlendIndices;
		vertex[i].blendWeights = weight.BlendWeights;
	}

	index = new UINT[indexCount];
	for (UINT i = 0; i < indexCount; i++)
		index[i] = indices[i];

	// Tangent 계산
	CalculateTangents();
	for (UINT i = 0; i < vertexCount; i++)
		vertex[i].tangent = tangents[i];
}

void ModelPart::CreateBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalTangentBlend) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	if (indexBuffer != NULL)
		SAFE_RELEASE(indexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void ModelPart::Export(BinaryWriter * bw)
{
	bw->Write(isSkinnedModel);

	bw->Write(materialIndex);

	bw->Write(vertexCount);
	bw->Write(vertex, sizeof(VertexTextureNormalTangentBlend), vertexCount);

	bw->Write(indexCount);
	bw->Write(index, sizeof(UINT), indexCount);
}

void ModelPart::Import(BinaryReader * br)
{
	isSkinnedModel = br->Bool();

	materialIndex = br->UInt();
	ModelScene* scene = model->GetModelScene();
	vector<ModelMaterial*>* materials = scene->GetMaterials();
	UINT size = materials->size();
	material = materials->at(materialIndex);

	vertexCount = br->UInt();
	SAFE_DELETE_ARRAY(vertex);
	vertex = new VertexTextureNormalTangentBlend[vertexCount];
	br->Read(vertex, sizeof(VertexTextureNormalTangentBlend), vertexCount);

	indexCount = br->UInt();
	SAFE_DELETE_ARRAY(index);
	index = new UINT[indexCount];
	br->Read(index, sizeof(UINT), indexCount);

	CreateBuffer();
}

void ModelPart::CalculateTangents()
{
	D3DXVECTOR3* tempTangents = new D3DXVECTOR3[vertexCount];
	ZeroMemory(tempTangents, vertexCount * sizeof(D3DXVECTOR3));

	D3DXVECTOR3* tempBinormals= new D3DXVECTOR3[vertexCount];
	ZeroMemory(tempBinormals, vertexCount * sizeof(D3DXVECTOR3));

	UINT triangleCount = indexCount / 3;

	for (UINT i = 0; i < triangleCount; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];
		
		const D3DXVECTOR3& pos0 = positions[index0];
		const D3DXVECTOR3& pos1 = positions[index1];
		const D3DXVECTOR3& pos2 = positions[index2];

		const D3DXVECTOR2& uv0 = uvs[index0];
		const D3DXVECTOR2& uv1 = uvs[index1];
		const D3DXVECTOR2& uv2 = uvs[index2];

		const D3DXVECTOR3& e0 = pos1 - pos0;
		const D3DXVECTOR3& e1 = pos2 - pos0;
		
		const float u0 = uv1.x - uv0.x;
		const float u1 = uv2.x - uv0.x;
		const float v0 = uv1.y - uv0.y;
		const float v1 = uv2.y - uv0.y;
		
		float r = 1.0f / (u0 * v1 - v0 * u1);

		float Tx = r * (v1 * e0.x - v0 * e1.x);
		float Ty = r * (v1 * e0.y - v0 * e1.y);
		float Tz = r * (v1 * e0.z - v0 * e1.z);
		D3DXVECTOR3 tangent(Tx, Ty, Tz);

		float Bx = r * (-u1 * e0.x + u0 * e1.x);
		float By = r * (-u1 * e0.y + u0 * e1.y);
		float Bz = r * (-u1 * e0.z + u0 * e1.z);
		D3DXVECTOR3 binormal(Bx, By, Bz);

		tempTangents[index0] += tangent;
		tempTangents[index1] += tangent;
		tempTangents[index2] += tangent;

		tempBinormals[index0] += binormal;
		tempBinormals[index1] += binormal;
		tempBinormals[index2] += binormal;
	}

	for (UINT i = 0; i < vertexCount; i++)
	{
		const D3DXVECTOR3& n = normals[i];
		const D3DXVECTOR3& t = tempTangents[i];

		// Gram-Schmidt 직교, 정규화
		tempTangents[i] = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&(tempTangents[i]), &(tempTangents[i]));

		// Draw방향에 따른 w값 할당
		/*D3DXVECTOR3 tempCross;
		D3DXVec3Cross(&tempCross, &n, &t);
		float w = (D3DXVec3Dot(&tempCross, &tempBinormals[i]) < 0.0f) ? -1.0f : 1.0F;
		tangents[i] = D3DXVECTOR4(tempTangents[i], w);*/

		tangents[i] = tempTangents[i];
	}

	SAFE_DELETE_ARRAY(tempTangents);
	SAFE_DELETE_ARRAY(tempBinormals);
}
