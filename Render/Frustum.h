#pragma once
class LightManager;
class FrustumBuffer;
class RenderTexture;
class Frustum
{
public:
	Frustum();
	~Frustum();

	void Initialize(RenderTexture* mainRender, LightManager* light);

	
	void Update();

	bool CheckCapsule(D3DXVECTOR3 start, D3DXVECTOR3 end, float radius);
	bool CheckPoint(float x, float y, float z);
	bool CheckPoint(D3DXVECTOR3 point);

	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(D3DXVECTOR3 center,  float radius);

	void SplitFrustum(UINT count);

	void Render();

	D3DXMATRIX GetCropMatrix(UINT index);
	D3DXPLANE* GetPlanes() {
		return &m_planes[0];
	}


	bool fixFrustum;
private:
	D3DXPLANE m_planes[6];
	D3DXVECTOR3* vtx;

	
	void SetFrustum();


	//mainRenderingTexture 지정해두자
	RenderTexture* mainRenderTarget;

	vector<D3DXVECTOR3> splitedVtx;
	UINT splitCount;
	D3DXMATRIX cropMatrix[3];
	
	void MakeCropMatrix();
	


	LightManager* lightM;
	

	WorldBuffer* wBuffer;
	D3DXMATRIX ViewProjectionInverse;
	D3DXMATRIX frustumWorld;
	void CreateBuffer();
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

};