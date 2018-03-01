#pragma once
#include "../Utility/BinaryInputOutputHandler.h"

class ModelScene;
class ModelPart;
class ModelMaterial;
class ModelBuffer;
class ModelBoneWeights;
class ModelAnimationKeyFrames;
class ModelAnimationController;
class FBXModelShader;

class Model : public BinaryInputOutputHandler
{
public:
	Model(ModelScene* modelScene);
	Model(ModelScene* modelScene, wstring name);
	Model(const Model& otherModel);
	~Model();

	void Update(bool isAnimation, D3DXMATRIX* worldPos);
	void UpdateAnimation(ModelAnimationController* animationController);
	void Render(FBXModelShader* shader);


	ModelScene* GetModelScene() { return modelScene; }
	
	wstring GetName() { return name; }

	//void SetModelBuffer(ModelBuffer* modelBuffer);

	void AddAnimationKeyFrames(ModelAnimationKeyFrames* animationKeyFrames);
	ModelAnimationKeyFrames* GetAnimationKeyFrames(wstring animationName);

	void SetGeometricOffset(const D3DXMATRIX& matGeometricOffset) { this->matGeometricOffset = matGeometricOffset; }
	D3DXMATRIX GetGeometricOffset() { return matGeometricOffset; }

	D3DXMATRIX GetAbsoluteTransform() { return absoluteTransform; }
	void SetAbsoluteTransform(const D3DXMATRIX& absoluteTransform) { this->absoluteTransform = absoluteTransform; }

	D3DXMATRIX GetAnimationTransform() { return matAnimationTransform; };
	void SetAnimationTransform(const D3DXMATRIX& matAnimationTransform) { this->matAnimationTransform = matAnimationTransform; }

	void AddVertex(ModelMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const ModelBoneWeights& boneWeights);
	void CreateData();
	void CreateBuffer();

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	ModelScene* modelScene;

	wstring name;

	D3DXMATRIX absoluteTransform;
	D3DXMATRIX matGeometricOffset;
	D3DXMATRIX matAnimationTransform;

	vector<ModelPart *> parts;
	
	map<wstring, ModelAnimationKeyFrames*> animationKeyFrames;


	D3DXMATRIX scale;

};