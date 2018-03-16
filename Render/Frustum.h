#pragma once
class Frustum
{
public:
	Frustum();
	~Frustum();

	void SetFrustum(D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);
	void Update();

	bool CheckCapsule(D3DXVECTOR3 start, D3DXVECTOR3 end, float radius);
	bool CheckPoint(float x, float y, float z);
	bool CheckPoint(D3DXVECTOR3 point);

	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(D3DXVECTOR3 center,  float radius);

	void SplitFrustum(UINT count);

	void Render();


	D3DXMATRIX GetCropMatrix(UINT index);
	bool fixFrustum;
private:
	D3DXPLANE m_planes[6];
	D3DXVECTOR3 vtx[8];

	vector<D3DXVECTOR3> splitedVtx;
	UINT splitCount;

	

	WorldBuffer* wBuffer;
	D3DXMATRIX ViewProjectionInverse;
	D3DXMATRIX frustumWorld;
	void CreateBuffer();
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};