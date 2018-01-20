#pragma once


class RenderTexture
{
public:
	RenderTexture(UINT width = 0, UINT height = 0);
	~RenderTexture();

	void Clear(float r=1, float g = 1, float b = 1, float a = 1);
	void SetTarget(bool ortho  = false);

	void SaveTexture(wstring fileName);
	ID3D11ShaderResourceView** GetShadowResourceView() {
		return &resourceView;
	}

	void ClearDepthStencil(UINT clearFlag, float depth, UINT8 stencil);
private:
	UINT width, height;

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* renderView;
	ID3D11ShaderResourceView* resourceView;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;


	D3D11_VIEWPORT viewport;



	D3DXMATRIX projectionMatrix, orthoMatrix;
};