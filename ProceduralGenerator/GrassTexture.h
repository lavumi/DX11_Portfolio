#pragma once

class RenderTexture;
class GrassTexture 
{
public:
	GrassTexture();
	~GrassTexture();

	void DrawTexture();

	
	void Render();


	

	ID3D11ShaderResourceView* diffuse;
	ID3D11ShaderResourceView* finalResult;
	ID3D11ShaderResourceView* getGrassTexture();

private:
	wstring basePath;
	vector<wstring> ShaderList;


	void DrawGrassSingle();
	void DirectionalWarp();
	void RND_SRT();
	void RND_SRT_Instancing();

	void DrawGrassTexture();

	void CreateShader(int);
	void CreateBuffer();

	//void DrawTexture(ID3D11ShaderResourceView* texture = nullptr);

	RenderTexture* grassSingle;
	RenderTexture* gradient;
	RenderTexture* directionalWarp;
	RenderTexture* rndDraw;

	RenderTexture* grassTexture;

	int width;

	float banding;
	float banding2;

	int count;
	int rotation;
	float minSize; 
	float maxSize;
	D3DXCOLOR color1; 
	D3DXCOLOR color2;








	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;

	ID3D11InputLayout* layout;


	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;



	//ID3D11ShaderResourceView* finalResult;
	void SetFinalResult(RenderTexture*);


	ID3D11Buffer* vertexBufferFinal;
	

};