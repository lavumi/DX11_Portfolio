#pragma once

#include "WorldBuffer.h"

class Shader
{
public:

	void SetMatrix(D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);

	
protected:
	Shader(wstring vertexshaderFile);
	~Shader();

	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);
	void CreateMATRIXBuffer();

	virtual void Update();
	


	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;
	ID3D10Blob* geoBlob;

	ID3D11InputLayout* layout;



	struct WorldMatrix {
		D3DXMATRIX world;
	} wData;


	struct ViewProjectionMatrix {
		D3DXMATRIX view;
		D3DXMATRIX projection;
	} vpData;

	ID3D11Buffer* wBuffer;
	ID3D11Buffer* vpBuffer;

	void CreateVertexShader();
	void CreatePixelShader();
	void CreateGeometryShader();
	void CheckShaderError(HRESULT hr, ID3D10Blob* error);




	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geoShader;


};