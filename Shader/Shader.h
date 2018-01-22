#pragma once

#include "CameraBuffer.h"
#include "WorldBuffer.h"

class Shader
{
public:

	void SetMatrix(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);

	
protected:
	Shader(wstring vertexshaderFile);
	~Shader();

	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);
	void CreateMATRIXBuffer();

	virtual void Update();
	


	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;


	ID3D11InputLayout* layout;



	struct WVP {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	} wvpData;


	ID3D11Buffer* wvpBuffer;


	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(HRESULT hr, ID3D10Blob* error);




	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;


};