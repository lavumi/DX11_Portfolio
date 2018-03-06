#pragma once

#include "Shader.h"

class TerrainBuffer;
class TerrainShader : public Shader {
public:
	TerrainShader();
	~TerrainShader();


	virtual void Update();

	virtual void Render(UINT indexCount, D3DXMATRIX world, 
		ID3D11ShaderResourceView ** diffuseMap, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* lightMap);

	//virtual void Render(UINT indexCount,
	//	D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	//	ID3D11ShaderResourceView** diffuseMap,
	//	ID3D11ShaderResourceView* normalMap,
	//	ID3D11ShaderResourceView* lightMap, D3DXPLANE clipPlane
	//	);





	TerrainBuffer* buffer;



	//void CreateBuffers();
	//
	//
	//ID3D11Buffer* LightBuffer;
	//ID3D11Buffer* MaterialBuffer;
	//ID3D11Buffer* ExtraBuffer;
	//ID3D11Buffer* clipPlaneBuffer;
	//ID3D11Buffer* cameraSpaceBuffer;
	//
	//struct MaterialData {
	//	D3DXCOLOR ambient;
	//	D3DXCOLOR diffuse;
	//	D3DXCOLOR globalAmbient;
	//} material;
	//
	//struct ExtraData {
	//	D3DXMATRIX invTransWorld;
	//} extraData;


};