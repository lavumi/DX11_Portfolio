#pragma once
#include "ModelEnums.h"

class Model;
class ModelMaterial;
class ModelBuffer;
class ModelSkeleton;
class ModelAnimation;
class ModelAnimationController;
class ModelBoneWeights;
class Shader;

class ModelScene
{
public:
	ModelScene();
	~ModelScene();

	static wstring TexturePath;

	//wstring GetFileName() { return fileName; }
	vector<ModelMaterial*>* GetMaterials() { return &materials; }
	vector<Model*>* GetModels() { return &models; }
	ModelSkeleton* GetSkeleton() { return skeleton; }
	ModelAnimation* GetAnimation(wstring animName);

	void Update(D3DXMATRIX* worldPos);
	void Render();


	void SetAni(int);

	void LoadFbx(wstring filePath);
	void LoadAniFbx(wstring filePath, wstring aniName);
	void SaveBinary(wstring filePath, wstring fileName);
	void LoadBinary(wstring filePath, wstring fileName);

private:
	void ProcessScene();
	void ProcessMaterial();
	void ProcessNode(FbxNode* node, FbxNodeAttribute::EType attribute);
	void ProcessSkeleton(FbxNode* node);
	void ProcessMesh(FbxNode* node);
	void ProcessAnimations(wstring aniName = L"");
	void ProcessAnimation(FbxNode* node, wstring takeName, float frameRate, float start, float stop);

	void ProcessBoneWeights(FbxMesh* mesh, vector<ModelBoneWeights>& meshBoneWeights);
	void ProcessBoneWeights(FbxSkin* skin, vector<ModelBoneWeights>& meshBoneWeights);
	
	ModelMaterial* LinkMaterialWithPolygon
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	int GetMappingIndex
	(
		FbxLayerElement::EMappingMode mode
		, int polygonIndex
		, int polygonVertexIndex
		, int vertexIndex
	);

	D3DXVECTOR2 GetUV
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	ModelMaterial* GetMaterial(FbxSurfaceMaterial* fbxMaterial);
	D3DXMATRIX GetGeometricOffset(FbxNode* node);
	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode* node, FbxTime time);
	D3DXMATRIX GetLocalTransformFromCurrentTake(FbxNode* node, FbxTime time);
	void AddAnimation(ModelAnimation* animation);

	void ExportMaterials(wstring fileName);
	void ExportSkeleton(wstring fileName);
	void ExportModels(wstring fileName);
	void ExportAnimations(wstring fileName);

	void ImportMaterials(wstring fileName);
	void ImportSkeleton(wstring fileName);
	void ImportModels(wstring fileName);
	void ImportAnimations(wstring fileName);

	wstring filePath;

	FbxManager* manager;
	FbxScene* scene;
	FbxGeometryConverter* converter;
	FbxImporter* importer;

	vector<ModelMaterial*> materials;
	vector<FbxSurfaceMaterial*> fbxMaterials;
	ModelSkeleton* skeleton;
	vector<Model*> models;
	map<wstring, ModelAnimation*> animations;

	ModelBuffer* modelBuffer;

	ModelAnimationController* animationController;



	D3DXMATRIX scaleBias;
};