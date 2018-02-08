#pragma once



struct D3DInfo
{
	wstring appName;
	HINSTANCE instance;
	HWND mainHandle;
	UINT screenWidth;
	UINT screenHeight;
	bool isVsync;
	bool isFullScreen;
	D3DXCOLOR clearColor;
};

class D3D
{
public:
	static D3D* Get();
	static void Delete();

	static ID3D11Device* GetDevice()
	{
		return device;
	}

	static ID3D11DeviceContext* GetDeviceContext()
	{
		return deviceContext;
	}

	static void GetInfo(D3DInfo* info)
	{
		*info = d3dInfo;
	}

	static void SetInfo(D3DInfo info)
	{
		d3dInfo = info;
	}

	void GetProjection(D3DXMATRIX* projection)
	{
		memcpy(projection, this->currentProjection, sizeof(D3DXMATRIX));
	}


	void GetOrthoProjection(D3DXMATRIX* projection)
	{
		memcpy(projection, this->defaultOrthoProjection, sizeof(D3DXMATRIX));
	}

	
	void SetProjection(D3DXMATRIX* projection)
	{
		memcpy(this->currentProjection, projection,  sizeof(D3DXMATRIX));
	}


	IDXGISwapChain* GetSwapChain() { return swapChain; }

	void BeginScene();
	void EndScene();


	void SetDefaultRenderView();





	//DepthStencil State

	enum DS_state {
		onState=0,
		offState,
		mirrorPlaneRenderState,
		mirrorObjectRenderState,
		mirrorSkyplaneState,
	};

	void SetDepthStencilState(DS_state);




	//Blender

	enum BL_state {
		Linear,
		Off,
		Max,
		None,
		BlendFacter,
		Add,
		AlphaCoverage,
	};

	void SetBlender(BL_state, float factor = 0);
	//void SetBlender_Linear();
	//void SetBlender_MaxBlend();
	//void SetBlender_Off();
	//void SetBlender_None();
	//void SetBlender_AddBlend();
	//void SetBlender_BlendFacter(float factor);
	//void SetBlender_alphaCoverage();

	void ClearDepthStencil(UINT clearFlag, float depth, UINT8 stencil);



	ID3D11ShaderResourceView** GetBackBufferSubRenderTexture(int);

	ID3D11Texture2D* GetBackBufferSubRenderTexture2D(int);

	void TestMultiTexture(int);

private:
	D3D();
	~D3D();

	void CreateAdapter();
	void CreateSwapChain();

	static D3D* instance;
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static D3DInfo d3dInfo;


	D3D_FEATURE_LEVEL version;

	bool isVsync;
	UINT gpuMemorySize;
	wstring gpuDescription;

	UINT numerator;
	UINT denominator;

	IDXGISwapChain* swapChain;


	//projection 관리

	D3D11_VIEWPORT viewport;
	
	float screenAspect; //화면비율
	D3DXMATRIX currentProjection;
	D3DXMATRIX defaultProjection;
	D3DXMATRIX defaultOrthoProjection;


	//DepthStencil

	void CreateDefaultDepthStencilTexture();


	void CreateDefaultRenderTarget();
	void CreateDefaultDepthView();

	

	void CreateDepthStencil();






	//Blender

	void CreateBlenders();
	ID3D11BlendState** d3dBlendState;



	ID3D11Texture2D* defaultDepthTexture;
	ID3D11DepthStencilView* defaultDepthView;
	ID3D11RenderTargetView* defaultRenderView[2];
	ID3D11Texture2D* subRenderTexture[1];

	ID3D11ShaderResourceView* subRenderShaderResource[1];

	ID3D11DepthStencilState** depthstencilstate;

};