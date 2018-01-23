#pragma once
#include "Shader.h"

class WaterShader : public Shader {
public:
	WaterShader();
	
	~WaterShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
		ID3D11ShaderResourceView * normalMap, 
		ID3D11ShaderResourceView* perlin, 
		ID3D11ShaderResourceView * reflectionTexture, ID3D11ShaderResourceView * refractionTexture);






	void CreateBuffers();
	
	ID3D11Buffer* LightBuffer;
	ID3D11Buffer* cameraPosBuffer;
	ID3D11Buffer* ExtraBuffer;
	ID3D11Buffer* MaterialBuffer;
	ID3D11Buffer* waterBuffer;


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

	struct WaterData {
		float translation;
		float scale;
		float angle;
		float padding2;
	} waterData;

	float speed;
	float angle;
};