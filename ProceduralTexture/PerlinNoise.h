#pragma once

class RenderTexture;
class OrthoWindowPlane;
class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	void MakePerlinNoise(float r = 1, float g = 1, float b = 1);
	void Render();

	ID3D11ShaderResourceView** GetPerlinNoise();

	RenderTexture* rndNoise;
private:

	D3DXCOLOR color;
	ID3D11Buffer*		colorBuffer;

	void CreateShader(wstring file);
	void CreateBuffer();


	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*	pixelShader;
	ID3D11InputLayout*	layout;
	ID3D11VertexShader* vertexShaderfinal;
	ID3D11PixelShader*	pixelShaderfinal;
	ID3D11InputLayout*	layoutfinal;
	ID3D10Blob*			vertexBlob;
	ID3D10Blob*			pixelBlob;
	ID3D11Buffer*		vertexBuffer;
	ID3D11Buffer*		indexBuffer;

	wstring basePath;

	ID3D11ShaderResourceView* finalResult;
};