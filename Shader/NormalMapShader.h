#pragma once
#include "Shader.h"

class NormalMapShader : public Shader {
public:
	NormalMapShader();
	~NormalMapShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
		ID3D11ShaderResourceView* diffuseMap,
		ID3D11ShaderResourceView* normalMap,
		ID3D11ShaderResourceView* heightMap, 
		ID3D11ShaderResourceView* lightMap);






	void CreateBuffers();
	
	ID3D11Buffer* LightBuffer;
	ID3D11Buffer* cameraPosBuffer;
	ID3D11Buffer* ExtraBuffer;
	ID3D11Buffer* MaterialBuffer;
	ID3D11Buffer* parallaxBuffer;

	struct MaterialData {
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXCOLOR globalAmbient;
		float shininess;
		D3DXVECTOR3 padd;
	} material;

	struct ExtraData {
		D3DXMATRIX invTransWorld;
		float shadowBias;
		
		D3DXVECTOR3 extraData;

	} extraData;


	struct ParallexData {
		float scale, layer;
		float drawTexture, pad2;
	} parallexData;


};