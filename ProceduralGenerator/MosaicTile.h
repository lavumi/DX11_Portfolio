#pragma once
#pragma once

class RenderTexture;
class MosaicTile
{
public:
	MosaicTile();
	~MosaicTile();

	void DrawTexture();


	void Render();

	ID3D11ShaderResourceView* finalResult;

	ID3D11ShaderResourceView* diffuse;
	ID3D11ShaderResourceView* normal;
	ID3D11ShaderResourceView* specular;
private:
	wstring basePath;
	vector<wstring> ShaderList;


	void CreateShader(int);
	void CreateBuffer();

	//void DrawTexture(ID3D11ShaderResourceView* texture = nullptr);

	RenderTexture* MosaicTileSingle;
	RenderTexture* TangentMap;
	RenderTexture* Patterns;
	RenderTexture* DiffuseMap;
	RenderTexture* rndNoise;





	void tileBase();
	void CreateTangentMap();
	void DrawPatterns();
	void DrawTile();

	void DrawNoise();





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





	struct PatternData{
		D3DXCOLOR color1;
		D3DXCOLOR color2;
		float LineWidth;
		float _LineCount;






		D3DXVECTOR2 pad;
	} patternData;


};