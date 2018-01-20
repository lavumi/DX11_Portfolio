#pragma once

class RenderTexture;
class OrthoWindowPlane;
class RNDNoise
{
public:
	RNDNoise();
	~RNDNoise();

	void MakeRNDNoise();

	ID3D11ShaderResourceView** GetRNDNoise();

	RenderTexture* rndNoise;
private:



	void CreateShader(wstring file);
	void CreateBuffer();


	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;

	ID3D11InputLayout* layout;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	wstring basePath;

	ID3D11ShaderResourceView* finalResult;
};