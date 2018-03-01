#include "../stdafx.h"
#include "ModelAnimationKeyFrames.h"

ModelAnimationKeyFrames::ModelAnimationKeyFrames(wstring animationName)
	: animationName(animationName)
{
	
}

ModelAnimationKeyFrames::ModelAnimationKeyFrames(ModelAnimationKeyFrames & other)
{
	animationName = other.animationName;
	keyFrames = other.keyFrames;
}

ModelAnimationKeyFrames::~ModelAnimationKeyFrames()
{

}

void ModelAnimationKeyFrames::AddKeyFrame(const D3DXMATRIX & transform)
{
	keyFrames.push_back(make_pair(transform, ModelAnimationQuaternionKeyFrame(transform)));
}

wstring ModelAnimationKeyFrames::GetAnimationName()
{
	return animationName;
}

void ModelAnimationKeyFrames::SetAnimationName(wstring animationName)
{
	this->animationName = animationName;
}

const D3DXMATRIX & ModelAnimationKeyFrames::GetKeyFrameTransform(int keyFrame) const
{
	return keyFrames[keyFrame].first;
}

const ModelAnimationQuaternionKeyFrame & ModelAnimationKeyFrames::GetQuaternionKeyFrame(int keyFrame) const
{
	return keyFrames[keyFrame].second;
}

void ModelAnimationKeyFrames::Export(BinaryWriter * bw)
{
	bw->Write(animationName);

	UINT keyFramesCount = keyFrames.size();
	bw->Write(keyFramesCount);
	for (size_t i = 0; i < keyFramesCount; i++)
	{
		bw->Write(keyFrames[i].first);
		keyFrames[i].second.Export(bw);
	}
}

void ModelAnimationKeyFrames::Import(BinaryReader * br)
{
	animationName = br->WString();
	
	UINT keyFramesCount = br->UInt();
	keyFrames.clear();
	keyFrames.reserve(keyFramesCount);
	for (size_t i = 0; i < keyFramesCount; i++)
	{
		D3DXMATRIX transform = br->Matrix();
		ModelAnimationQuaternionKeyFrame quternion;
		quternion.Import(br);
		keyFrames.push_back(make_pair(transform, quternion));
	}
}

void ModelAnimationQuaternionKeyFrame::Export(BinaryWriter * bw)
{
	bw->Write(q);
	bw->Write(translation);
}

void ModelAnimationQuaternionKeyFrame::Import(BinaryReader * br)
{
	q = br->Quaternion();
	translation = br->Vector3();
}
