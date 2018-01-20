#pragma once

class DepthStencil
{
public:
	static DepthStencil* Get();
	static void Delete();

	void SetOnState();
	void SetOffState();
	void SetmirrorPreState();
	void SetmirrorState();

	void SetDefaultRenderView();

	ID3D11DepthStencilView* GetDepthView()
	{
		return depthView;
	}

	ID3D11RenderTargetView* GetRenderView()
	{
		return renderView;
	}

private:
	void CreateTexture();
	void CreateOnState();
	void CreateOffState();
	void CreateMirrorStencilTest();


	void CreateRenderTarget();
	void CreateView();

	static DepthStencil* instance;

	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* depthView;
	ID3D11RenderTargetView* renderView;

	ID3D11DepthStencilState* onState;
	ID3D11DepthStencilState* offState;
	ID3D11DepthStencilState* mirrorState;
	ID3D11DepthStencilState* mirrorState2;

	DepthStencil();
	~DepthStencil();
};