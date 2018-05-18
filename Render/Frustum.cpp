#include "../stdafx.h"
#include "Frustum.h"
#include "../System/BoundingBox.h"
#include "LightManager.h"
#include "../Shader/FrustumBuffer.h"
#include "../Render/RenderTexture.h"

Frustum::Frustum()
{
	wBuffer = new WorldBuffer();
	CreateBuffer();
	fixFrustum = false;
}

Frustum::~Frustum()
{
}

void Frustum::Initialize(RenderTexture* mainRender, LightManager* light)
{
	lightM = light;
	mainRenderTarget = mainRender;
}

void Frustum::SetFrustum()
{

	D3DXMATRIX view, projection, projection2;
	Camera::Get()->GetView(&view);

	mainRenderTarget->GetProjection(projection);
	//D3D::Get()->GetProjection(&projection);
	vtx = new D3DXVECTOR3[8];
	D3DXVECTOR4 temp[8];

	//뷰, 프로젝션 연산이 끝나면 모든 점은 (-1, -1, 0) ~ (1, 1, 1)  사이의 값으로 변환된다
	//그 꼭지점 값들을 지정해둔다...
	vtx[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vtx[1] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vtx[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vtx[3] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);

	vtx[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vtx[5] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vtx[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vtx[7] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);



	//뷰, 프로젝션 연산의 역행렬을 구해서
	D3DXMATRIX viewProj = view * projection;
	D3DXMatrixInverse(&ViewProjectionInverse, NULL, &viewProj);

	//각 점에 곱해주면 점들의 기존 위치를 알수 있게 된다.
	//이 점들을 이용해 평면을 만든다.
	for (int i = 0; i < 8; i++) {
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &ViewProjectionInverse);
	}
		
	
	 vtx[4] = vtx[0] * 0.5f + vtx[4]*0.5f;
	 vtx[5] = vtx[1] * 0.5f + vtx[5]*0.5f;
	 vtx[6] = vtx[2] * 0.5f + vtx[6]*0.5f;
	 vtx[7] = vtx[3] * 0.5f + vtx[7]*0.5f;

	
	 D3DXVECTOR3 closeVtx[3];

	 closeVtx[0] = vtx[0] * 0.5f + vtx[4] * 0.5f;
	 closeVtx[1] = vtx[1] * 0.5f + vtx[5] * 0.5f;
	 closeVtx[2] = vtx[2] * 0.5f + vtx[6] * 0.5f;

	D3DXPlaneFromPoints(&m_planes[0], &vtx[5], &vtx[6], &vtx[7]);		// 원 평면(far)
	D3DXPlaneFromPoints(&m_planes[1], &vtx[0], &vtx[4], &vtx[7]);		// 좌 평면(left)
	D3DXPlaneFromPoints(&m_planes[2], &vtx[1], &vtx[2], &vtx[6]);		// 우 평면(right)
	//	D3DXPlaneFromPoints(&m_plane[3], &vtx[4], &vtx[7], &vtx[6]);	// 상 평면(top)
	//	D3DXPlaneFromPoints(&m_plane[4], &vtx  ,  &vtx[1], &vtx[2]);	// 하 평면(bottom)
	//	D3DXPlaneFromPoints(&m_plane[5], &vtx  ,  &vtx[4], &vtx[5]);	// 근 평면(near)



	D3DXPlaneFromPoints(&m_planes[4], &closeVtx[0], &closeVtx[1], &closeVtx[2]);

	SplitFrustum(3);

	SAFE_DELETE_ARRAY(vtx);
}

void Frustum::SplitFrustum(UINT count)
{
	splitCount = count;
	splitedVtx.clear();

	splitedVtx.push_back(vtx[0]);
	splitedVtx.push_back(vtx[1]);
	splitedVtx.push_back(vtx[2]);
	splitedVtx.push_back(vtx[3]);
	for (UINT i = 0; i < splitCount - 1; i++) {
		float split = ((float)i + 1) / splitCount;
		split *= split;
		splitedVtx.push_back(vtx[0] * (1 - split) + vtx[4] * split);
		splitedVtx.push_back(vtx[1] * (1 - split) + vtx[5] * split);
		splitedVtx.push_back(vtx[2] * (1 - split) + vtx[6] * split);
		splitedVtx.push_back(vtx[3] * (1 - split) + vtx[7] * split);
	}
	splitedVtx.push_back(vtx[4]);
	splitedVtx.push_back(vtx[5]);
	splitedVtx.push_back(vtx[6]);
	splitedVtx.push_back(vtx[7]);







	assert(splitedVtx.size() == (splitCount + 1) * 4);
}



void Frustum::MakeCropMatrix()
{
	D3DXVECTOR3 vtx_input[8];
	D3DXMATRIX viewXproj;
	lightM->GetViewXProjection(&viewXproj);

	//lightM->GetView(&view);
	//lightM->GetProjection(&projection);

	float scaleX, scaleY, scaleZ;
	float offsetX, offsetY, offsetZ;


	for (UINT index = 0; index < 3; index++) {
		for (UINT i = 0; i < 8; i++) {
			vtx_input[i] = splitedVtx[i + index * 4];
		}


		BoundingBox box = BoundingBox::CreateAABB(vtx_input, viewXproj);


		box.min.z = 0;


		scaleX = 2.0f / (box.max.x - box.min.x);
		scaleY = 2.0f / (box.max.y - box.min.y);
		offsetX = -0.5f * (box.max.x + box.min.x) * scaleX;
		offsetY = -0.5f * (box.max.y + box.min.y) * scaleY;
		scaleZ = 1.0f;
		offsetZ = -box.min.z * scaleZ;

		cropMatrix[index] = D3DXMATRIX(
			scaleX, 0.0f, 0.0f, 0.0f,
			0.0f, scaleY, 0.0f, 0.0f,
			0.0f, 0.0f, scaleZ, 0.0f,
			offsetX, offsetY, offsetZ, 1.0f);
	}

}

void Frustum::Update()
{
	SetFrustum();
	MakeCropMatrix();



	return;
	if (!fixFrustum) {
		frustumWorld = ViewProjectionInverse;
		wBuffer->SetWorld(frustumWorld);
	}

}

bool Frustum::CheckCapsule(D3DXVECTOR3 start, D3DXVECTOR3 end, float radius)
{

	for (int i = 0; i<3; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &start)  > radius)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &start)  > radius)
		{
			continue;
		}
		return false;
	}
	return true;
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i<3; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) > 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckPoint(D3DXVECTOR3 point)
{
	for (int i = 0; i<3; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &point) > 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	for (int i = 0; i<4; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) > radius)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckSphere(D3DXVECTOR3 center, float radius)
{
	for (int i = 0; i<3; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &center) > radius)
		{
			return false;
		}
	}
	return true;
}



void Frustum::Render()
{
	
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	wBuffer->SetBuffer();

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3D::GetDeviceContext()->DrawIndexed(36, 0, 0);
}



D3DXMATRIX Frustum::GetCropMatrix(UINT index)
{
	return cropMatrix[index];
}

void Frustum::CreateBuffer()
{
	HRESULT hr;

	VertexColor* vertexData = new VertexColor[24];
	int i = 0;

	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	1.0);		vertexData[i++].color = D3DXCOLOR(1,0,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,0,0.3f);	
																											
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(0,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(0,1,0,0.3f);	
																										
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	0.0);		vertexData[i++].color = D3DXCOLOR(0,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,0,1,0.3f);	
																										
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	0.0);		vertexData[i++].color = D3DXCOLOR(1,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	1.0);		vertexData[i++].color = D3DXCOLOR(1,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,1,0,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(1,1,0,0.3f);	
																									
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, 1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(1,0,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, 1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(1,0,1,0.3f);	
																										
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	0.0);		vertexData[i++].color = D3DXCOLOR(0,1,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		0.0);		vertexData[i++].color = D3DXCOLOR(0,1,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(1.0, -1.0,		1.0);		vertexData[i++].color = D3DXCOLOR(0,1,1,0.3f);	
	vertexData[i].position = D3DXVECTOR3(-1.0, -1.0,	1.0);		vertexData[i++].color = D3DXCOLOR(0,1,1,0.3f);	









	UINT* indexData = new UINT[36]{
		0, 1, 3, 2, 3, 1,
		5, 4, 6, 4, 7, 6,
		8, 11, 9, 11, 10, 9,
		12, 13, 15, 13, 14, 15,
		17, 16, 19, 17, 19, 18,
		20, 21, 23,22, 23, 21 };





	

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;


	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexColor) * 24;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 36;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}
