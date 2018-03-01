#pragma once
#include "../Utility/BinaryInputOutputHandler.h"

/********************************************************************************
@breif
Material 정보를 저장하는 Class
Ambient(주변), Diffuse(분산), Emissive(발광), Specular(반사), Shininess(빛남)의 정보를 저장한다
********************************************************************************/
class ModelMaterial : public BinaryInputOutputHandler
{
public:
	ModelMaterial();
	ModelMaterial(int number, FbxSurfaceMaterial* material);
	~ModelMaterial();

	UINT GetIndex() { return number; }

	ID3D11ShaderResourceView* GetAmbientView() { return ambientView; }
	ID3D11ShaderResourceView* GetEmissiveView() { return emissiveView; }
	ID3D11ShaderResourceView* GetDiffuseView() { return diffuseView; }
	ID3D11ShaderResourceView* GetSpecularView() { return specularView; }
	ID3D11ShaderResourceView* GetNormalMapView() { return normalMapView; }

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	D3DXCOLOR GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name, const char* factorName
		, wstring* textureName
	);

	void GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name
		, wstring* textureName
	);

	void CreateView(wstring path, ID3D11ShaderResourceView** view);

	UINT number; /// Material Index
	D3DXCOLOR ambient; /// 주변색: 빛이 없을때 마치 주변으로 부터의 반사된 빛의 나타내는 것 처럼 보여질 색
	D3DXCOLOR emissive; /// 발광색: 스스로 빛을 발산하는 색
	D3DXCOLOR diffuse; /// 분산색: 난반사로 인해 조명에 상관없이 관측자에게 동일하게 보여지는 색
	D3DXCOLOR specular; /// 반사색: 주광이 반사되어 관측자에게 향할 때 보여질 색
	float shininess; /// 빛남: 빛남의 정도

	/// 각 색의 Texture
	wstring ambientFile;
	wstring emissiveFile;
	wstring diffuseFile;
	wstring specularFile;
	wstring normalMapFile;

	/// 각 Texture를 통해 생성될 ResourceView
	ID3D11ShaderResourceView* ambientView;
	ID3D11ShaderResourceView* emissiveView;
	ID3D11ShaderResourceView* diffuseView;
	ID3D11ShaderResourceView* specularView;
	ID3D11ShaderResourceView* normalMapView;
};