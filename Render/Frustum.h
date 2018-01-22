#pragma once
class Frustum
{
public:
	Frustum();
	~Frustum();

	void SetFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);


	bool CheckCapsule(D3DXVECTOR3 start, D3DXVECTOR3 end, float radius);
	bool CheckPoint(float x, float y, float z);
	bool CheckPoint(D3DXVECTOR3 point);

	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(D3DXVECTOR3 center,  float radius);

	void Render();
private:
	D3DXPLANE m_planes[6];
	D3DXPLANE m_plane[6];
	
};