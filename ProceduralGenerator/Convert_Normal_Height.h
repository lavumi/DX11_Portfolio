#pragma once
class RenderTexture;
class Convert_Normal_Height {
public:
	Convert_Normal_Height();
	~Convert_Normal_Height();

	ID3D11ShaderResourceView* Convert(wstring file, bool HeightMap = 0);

private:

	void CreateShader(wstring file);
	void CreateBuffer();

	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader*	pixelShader;
	ID3D11InputLayout*	layout;
	ID3D10Blob*			vertexBlob;
	ID3D10Blob*			pixelBlob;
	ID3D11Buffer*		vertexBuffer;
	ID3D11Buffer*		indexBuffer;

	RenderTexture* target;
};