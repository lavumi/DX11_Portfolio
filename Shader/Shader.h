#pragma once

#include "CameraBuffer.h"
#include "WorldBuffer.h"

class Shader
{
public:
	static void CreateBuffer();
	static void DeleteBuffer();
	static void bufferupdate();

protected:
	Shader(wstring vertexshaderFile);
	~Shader();

	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);

	virtual void Update();
	virtual void Render(UINT indexCount);
	void MirrorUpdate();

	ID3D10Blob* vertexBlob;
	ID3D10Blob* pixelBlob;

	WorldBuffer* worldBuffer;
	ID3D11InputLayout* layout;


	static CameraBuffer* cameraBuffer;





	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(HRESULT hr, ID3D10Blob* error);




	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;


};