#include "../stdafx.h"
#include "Frustum.h"
#include "../System/BoundingBox.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::SetFrustum(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{

	//D3DXVECTOR3* vtx = new D3DXVECTOR3[8];
	

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
	D3DXMATRIX viewProj = viewMatrix * projectionMatrix;
	D3DXMATRIXA16	matInv;
	D3DXMatrixInverse(&matInv, NULL, &viewProj);


	//각 점에 곱해주면 점들의 기존 위치를 알수 있게 된다.
	//이 점들을 이용해 평면을 만든다.
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &matInv);


	D3DXPlaneFromPoints(&m_planes[0], &vtx[5], &vtx[6], &vtx[7]);		// 원 평면(far)
	D3DXPlaneFromPoints(&m_planes[1], &vtx[0], &vtx[4], &vtx[7]);		// 좌 평면(left)
	D3DXPlaneFromPoints(&m_planes[2], &vtx[1], &vtx[2], &vtx[6]);		// 우 평면(right)
	//	D3DXPlaneFromPoints(&m_plane[3], &vtx[4], &vtx[7], &vtx[6]);	// 상 평면(top)
	//	D3DXPlaneFromPoints(&m_plane[4], &vtx  ,  &vtx[1], &vtx[2]);	// 하 평면(bottom)
	//	D3DXPlaneFromPoints(&m_plane[5], &vtx  ,  &vtx[4], &vtx[5]);	// 근 평면(near)

	SplitFrustum(3);
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
	for (int i = 0; i<3; i++)
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

void Frustum::SplitFrustum(UINT count)
{
	splitCount = count;
	//splitVtx = new D3DXVECTOR3[(splitCount -1)*4];
	splitedVtx.clear();

	splitedVtx.push_back(vtx[0]);
	splitedVtx.push_back(vtx[1]);
	splitedVtx.push_back(vtx[2]);
	splitedVtx.push_back(vtx[3]);
	for (UINT i = 0; i < splitCount - 1; i++) {
		float split = ((float)i+1)/ splitCount;
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

void Frustum::Render()
{

}

D3DXMATRIX Frustum::GetCropMatrix(UINT index)
{
	D3DXMATRIX view, projection;
	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);

	D3DXVECTOR3 vtx_input[8];
	if (index >= splitCount)
		return D3DXMATRIX();
	else {
		for (UINT i = 0; i < 8; i++) {
			vtx_input[i] = splitedVtx[i + index*4];
		}
	}
	

	BoundingBox box = BoundingBox::CreateAABB(vtx_input, vtx, view*projection);


	box.min.z = 0;
	float scaleX, scaleY, scaleZ;
	float offsetX, offsetY, offsetZ;

	scaleX = 2.0f / (box.max.x - box.min.x);
	scaleY = 2.0f / (box.max.y - box.min.y);
	offsetX = -0.5f * (box.max.x + box.min.x) * scaleX;
	offsetY = -0.5f * (box.max.y + box.min.y) * scaleY;
	scaleZ = 1.0f / (box.max.z - box.min.z);
	offsetZ = -box.min.z * scaleZ;

	return D3DXMATRIX(
		scaleX,		0.0f,		0.0f,		0.0f,
		0.0f,		scaleY,		0.0f,		0.0f,
		0.0f,		0.0f,		scaleZ,		0.0f,
		offsetX,	offsetY,	offsetZ,	1.0f);
}
