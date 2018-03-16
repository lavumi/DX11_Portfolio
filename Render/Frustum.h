#pragma once
class Frustum
{
public:
	Frustum();
	~Frustum();

	void SetFrustum(D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);


	bool CheckCapsule(D3DXVECTOR3 start, D3DXVECTOR3 end, float radius);
	bool CheckPoint(float x, float y, float z);
	bool CheckPoint(D3DXVECTOR3 point);

	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(D3DXVECTOR3 center,  float radius);

	void SplitFrustum(UINT count);

	void Render();


	D3DXMATRIX GetCropMatrix(UINT index);

private:
	D3DXPLANE m_planes[6];
	D3DXVECTOR3 vtx[8];

	vector<D3DXVECTOR3> splitedVtx;
	UINT splitCount;
};