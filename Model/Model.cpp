#include "../stdafx.h"
#include "Model.h"
#include "ModelScene.h"
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "ModelBuffer.h"
#include "ModelBoneWeights.h"
#include "ModelAnimation.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelAnimationController.h"
#include "../Shader/FBXModelShader.h"


Model::Model(ModelScene* modelScene)
	:modelScene(modelScene)
{
	//
}

Model::Model(ModelScene* modelScene, wstring name)
	: modelScene(modelScene), name(name)
{
	
	D3DXMatrixIdentity(&absoluteTransform);
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

Model::Model(const Model & otherModel)
{

	modelScene = otherModel.modelScene;

	name = otherModel.name;

	absoluteTransform = otherModel.absoluteTransform;
	matGeometricOffset = otherModel.matGeometricOffset;
	matAnimationTransform = otherModel.matAnimationTransform;

	size_t partsCount = otherModel.parts.size();
	parts.reserve(partsCount);
	for (size_t i = 0; i < partsCount; i++)
	{
		ModelPart* part = new ModelPart(*(otherModel.parts[i]));
		part->SetModel(this);
		parts.push_back(part);
	}

	//modelBuffer = (modelBuffer != NULL) ? otherModel.modelBuffer : NULL;

	size_t animationKeyFramesCount = otherModel.animationKeyFrames.size();

	map<wstring, ModelAnimationKeyFrames*>::const_iterator iter;
	for (iter = otherModel.animationKeyFrames.begin(); iter != otherModel.animationKeyFrames.end(); ++iter)
	{
		wstring tempName = iter->first;
		ModelAnimationKeyFrames tempKeyFrames = *(iter->second);

		animationKeyFrames.insert({ tempName, new ModelAnimationKeyFrames(tempKeyFrames) });
	}
}

Model::~Model()
{
	map<wstring, ModelAnimationKeyFrames*>::iterator iter;
	for (iter = animationKeyFrames.begin(); iter != animationKeyFrames.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	for (size_t i = 0; i < parts.size(); i++)
		SAFE_DELETE(parts[i]);
}

void Model::Update(bool isAnimation, D3DXMATRIX* worldPos)
{
	for each(ModelPart* part in parts)
		part->Update(isAnimation, worldPos);
}

void Model::Render(FBXModelShader* shader)
{
	for each(ModelPart* part in parts)
		part->Render(shader);
}

//void Model::SetModelBuffer(ModelBuffer * modelBuffer)
//{
//	this->modelBuffer = modelBuffer; 
//	
//	for (size_t i = 0; i < parts.size(); i++)
//		parts[i]->SetModelBuffer(modelBuffer);
//}

void Model::AddAnimationKeyFrames(ModelAnimationKeyFrames * animationKeyFrames)
{
	this->animationKeyFrames.insert({ animationKeyFrames->GetAnimationName(), animationKeyFrames });
}

ModelAnimationKeyFrames * Model::GetAnimationKeyFrames(wstring animationName)
{
	map<wstring, ModelAnimationKeyFrames*>::iterator iter;
	for (iter = animationKeyFrames.begin(); iter != animationKeyFrames.end(); ++iter)
	{
		if (iter->first == animationName)
			return iter->second;
	}

	assert(false); // map에 animationName에 하당하는 Key가 없는 경우

	return NULL;
}

void Model::AddVertex(ModelMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const ModelBoneWeights& boneWeights)
{
	bool isNew = true;

	ModelPart* part = NULL;
	for (size_t i = 0; i < parts.size(); i++)
	{
		part = parts[i];

		if (material == part->GetMaterial())
		{
			part->AddVertex(position, normal, uv, boneWeights);
			isNew = false;
		}
	}
	 
	if (isNew == true)
	{
		//part = new ModelPart(this, material, modelBuffer);
		part = new ModelPart(this, material);
		part->AddVertex(position, normal, uv, boneWeights);
		
		parts.push_back(part);
	}
}

void Model::CreateData()
{
	for each(ModelPart* part in parts)
		part->CreateData();
}

void Model::CreateBuffer()
{
	for each(ModelPart* part in parts)
		part->CreateBuffer();
}

void Model::Export(BinaryWriter * bw)
{
	bw->Write(name);

	bw->Write(absoluteTransform);
	bw->Write(matGeometricOffset);
	bw->Write(matAnimationTransform);

	bw->Write(parts);

	bw->Write(animationKeyFrames);
}

void Model::Import(BinaryReader * br)
{
	name = br->WString();

	absoluteTransform = br->Matrix();
	matGeometricOffset = br->Matrix();
	matAnimationTransform = br->Matrix();


	UINT partsCount = br->UInt();
	parts.clear();
	parts.reserve(partsCount);
	for (size_t i = 0; i < partsCount; i++)
	{
		ModelPart* modelPart = new ModelPart(this);
		modelPart->Import(br);
		parts.push_back(modelPart);
	}

	animationKeyFrames.clear();
	UINT animationKeyFramesCount = br->UInt();
	for (size_t i = 0; i < animationKeyFramesCount; i++)
	{
		wstring name = br->WString();
		ModelAnimationKeyFrames* modelAnimationKeyFrames = new ModelAnimationKeyFrames();
		modelAnimationKeyFrames->Import(br);
		animationKeyFrames.insert(make_pair(name, modelAnimationKeyFrames));
	}
}

void Model::UpdateAnimation(ModelAnimationController * animationController)
{
	if (animationController != NULL)
	{
		ModelAnimation* animation = animationController->GetCurrentAnimation();

		if (animation != NULL)
		{
			ModelAnimationKeyFrames* keyFrames = NULL;
			map<wstring, ModelAnimationKeyFrames*>::iterator iter;
			for (iter = animationKeyFrames.begin(); iter != animationKeyFrames.end(); ++iter)
			{
				if (iter->first == animation->GetName())
				{
					keyFrames = iter->second;

					break;
				}
			}
			
			if (keyFrames != NULL)
			{
				int keyFrame = animationController->GetCurrentKeyFrame();
				matAnimationTransform = keyFrames->GetKeyFrameTransform(keyFrame);
				
				return;
			}
		}
	}

	D3DXMatrixIdentity(&matAnimationTransform);
}
