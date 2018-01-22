#pragma once
#include "Shader.h"

class MirrorShader : public Shader {
public:
	MirrorShader();
	~MirrorShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, 
		ID3D11ShaderResourceView** diffuseMap,
		ID3D11ShaderResourceView* normalMap,
		ID3D11ShaderResourceView* heightMap, 
		ID3D11ShaderResourceView* lightMap);






	void CreateBuffers();
	
	ID3D11Buffer* LightBuffer;
	ID3D11Buffer* mirrorViewBuffer;
	ID3D11Buffer* ExtraBuffer;
	ID3D11Buffer* MaterialBuffer;
	ID3D11Buffer* parallaxBuffer;
	ID3D11Buffer* lightMatrixBuffer;

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
		D3DXVECTOR4 options;

	} extraData;


	struct ParallexData {
		float scale, layer;
		float drawTexture, pad2;
	} parallexData;


	struct MirrorCameraData
	{
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXVECTOR3 position;
		float padding;
	} mirrorCameraData;

};