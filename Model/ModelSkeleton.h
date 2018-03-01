#pragma once
#include "../Utility/BinaryInputOutputHandler.h"
#include "ModelEnums.h"

/********************************************************************************
 @brief
 모든 Skeleton Bone의 정보를 계산하여 저장하는 Class

 각 Bone을 (Name, Index)로 구분하여 입출력

********************************************************************************/

class ModelSkeletonBone;
class ModelAnimationController;
class ModelSkeleton : public BinaryInputOutputHandler
{
public:
	ModelSkeleton();
	ModelSkeleton(const ModelSkeleton& otherSkeleton);
	~ModelSkeleton();

	void AddSkeletonBone(ModelSkeletonBone* skeletonBone);
	ModelSkeletonBone* FindBone(wstring boneName);
	int FindBoneIndex(wstring boneName);

	D3DXMATRIX& GetBoneAnimationTransform(int boneIdx);

	ModelSkeletonBone* GetSkeletonBone(int index);
	const vector<pair<wstring, ModelSkeletonBone *>>& GetSkeletonBones() { return skeletonBones; }
	
	D3DXMATRIX* GetSkinTransforms() { return skinTransforms; }

	int GetBoneCount() { return boneCount; }

	void AddAnimationFromOtherSkeleton(wstring srcAnimName, wstring dstAnimName, ModelSkeleton& srcSkeleton);

	void UpdateAnimation(ModelAnimationController* animationController); ///매 프레임마다 모든 본의 Transform 정보를 갱신

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	void BuildBoneTransforms(ModelAnimationController* animationController, wstring animationName, RootType rootType);

	int boneCount;

	typedef pair<wstring, ModelSkeletonBone *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms; /// 최종적으로 Vertex에 적용될 Transform Matrix 배열
	D3DXMATRIX* boneAnimationTransforms; /// 각 Bone의 Transform Matrix 배열
	 
};
