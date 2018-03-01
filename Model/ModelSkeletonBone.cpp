#include "../stdafx.h"
#include "ModelSkeletonBone.h"
#include "ModelAnimationKeyFrames.h"

ModelSkeletonBone::ModelSkeletonBone()
{
}

ModelSkeletonBone::ModelSkeletonBone(wstring name, int parentBoneIndex)
	: name(name), parentBoneIndex(parentBoneIndex)
{
	D3DXMatrixIdentity(&matBindPoseTransform);
	D3DXMatrixIdentity(&matInvBindPoseTransform);
	D3DXMatrixIdentity(&matBoneReferenceTransform);
	D3DXMatrixIdentity(&matInvBoneReferenceTransform);
}

ModelSkeletonBone::ModelSkeletonBone(const ModelSkeletonBone & otherSkeletonBone)
{
	name = otherSkeletonBone.name;

	matBindPoseTransform = otherSkeletonBone.matBindPoseTransform;
	matInvBindPoseTransform = otherSkeletonBone.matInvBindPoseTransform;
	matBoneReferenceTransform = otherSkeletonBone.matBoneReferenceTransform;
	matInvBoneReferenceTransform = otherSkeletonBone.matInvBoneReferenceTransform;

	parentBoneIndex = otherSkeletonBone.parentBoneIndex;

	size_t animationKeyFramesCount = otherSkeletonBone.animationKeyFrames.size();

	map<wstring, ModelAnimationKeyFrames *>::const_iterator iter;
	for (iter = otherSkeletonBone.animationKeyFrames.begin(); iter != otherSkeletonBone.animationKeyFrames.end(); ++iter)
	{
		wstring tempName = iter->first;
		ModelAnimationKeyFrames tempKeyFrames = *(iter->second);

		animationKeyFrames.insert({ tempName, new ModelAnimationKeyFrames(tempKeyFrames) });
	}
}

ModelSkeletonBone::~ModelSkeletonBone()
{
	map<wstring, ModelAnimationKeyFrames *>::iterator iter;
	for (iter = animationKeyFrames.begin(); iter != animationKeyFrames.end(); ++iter)
		SAFE_DELETE(iter->second);
}

void ModelSkeletonBone::AddAnimationKeyFrames(ModelAnimationKeyFrames * keyFrames)
{
	animationKeyFrames.insert({ keyFrames->GetAnimationName(), keyFrames });
}

void ModelSkeletonBone::SetBindPoseTransform(const D3DXMATRIX & matBindPoseTransform)
{
	this->matBindPoseTransform = matBindPoseTransform;
	D3DXMatrixInverse(&this->matInvBindPoseTransform, NULL, &this->matBindPoseTransform);
}

void ModelSkeletonBone::SetBoneReferenceTransform(const D3DXMATRIX & matBoneReferenceTransform)
{
	this->matBoneReferenceTransform = matBoneReferenceTransform;
	D3DXMatrixInverse(&this->matInvBoneReferenceTransform, NULL, &this->matBoneReferenceTransform);
}

ModelAnimationKeyFrames * ModelSkeletonBone::GetAnimationKeyFrames(wstring animationName)
{
	map<wstring, ModelAnimationKeyFrames *>::iterator iter;
	for (iter = animationKeyFrames.begin(); iter != animationKeyFrames.end(); ++iter)
	{
		if (iter->first == animationName)
			return iter->second;
	}

	return NULL;
}

void ModelSkeletonBone::Export(BinaryWriter * bw)
{
	bw->Write(name);

	bw->Write(matBindPoseTransform);
	bw->Write(matInvBindPoseTransform);
	bw->Write(matBoneReferenceTransform);
	bw->Write(matInvBoneReferenceTransform);

	bw->Write(parentBoneIndex);

	bw->Write(animationKeyFrames);
}

void ModelSkeletonBone::Import(BinaryReader * br)
{
	name = br->WString();

	matBindPoseTransform = br->Matrix();
	matInvBindPoseTransform = br->Matrix();
	matBoneReferenceTransform = br->Matrix();
	matInvBoneReferenceTransform = br->Matrix();
	
	parentBoneIndex = br->Int();

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
