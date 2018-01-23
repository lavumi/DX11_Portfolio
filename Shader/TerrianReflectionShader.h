#pragma once

#include "Shader.h"

class TerrianReflectionShader : public Shader {
public:
	TerrianReflectionShader();
	~TerrianReflectionShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
		ID3D11ShaderResourceView** diffuseMap,
		ID3D11ShaderResourceView* normalMap,
		ID3D11ShaderResourceView* lightMap
		);






	void CreateBuffers();


	ID3D11Buffer* LightBuffer;
	ID3D11Buffer* MaterialBuffer;
	ID3D11Buffer* ExtraBuffer;

	struct MaterialData {
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR globalAmbient;
	} material;
	struct ExtraData {
		D3DXMATRIX invTransWorld;
	} extraData;


};