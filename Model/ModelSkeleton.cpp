#include "../stdafx.h"
#include "ModelSkeleton.h"
#include "ModelSkeletonBone.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"

ModelSkeleton::ModelSkeleton()
{
	skinTransforms = NULL;
	boneAnimationTransforms = NULL;

	boneCount = 0;
}

ModelSkeleton::ModelSkeleton(const ModelSkeleton & otherSkeleton)
{
	boneCount = otherSkeleton.boneCount;

	skinTransforms = NULL;
	boneAnimationTransforms = NULL;
	/*if (otherSkeleton.skinTransforms != NULL)
	{
		skinTransforms = new D3DXMATRIX[boneCount];
		memcpy(skinTransforms, otherSkeleton.skinTransforms, sizeof(D3DXMATRIX) * boneCount);
	}
	if (otherSkeleton.boneAnimationTransforms != NULL)
	{
		boneAnimationTransforms = new D3DXMATRIX[boneCount];
		memcpy(boneAnimationTransforms, otherSkeleton.boneAnimationTransforms, sizeof(D3DXMATRIX) * boneCount);
	}
*/
	size_t skeletonBoneCount = (size_t)boneCount;
	skeletonBones.reserve(skeletonBoneCount);
	for (size_t i = 0; i < skeletonBoneCount; i++)
	{
		wstring tempName = otherSkeleton.skeletonBones[i].first;
		ModelSkeletonBone tempSkeletonBone = *(otherSkeleton.skeletonBones[i].second);

		skeletonBones.push_back({ tempName, new ModelSkeletonBone(tempSkeletonBone) });
	}
}

ModelSkeleton::~ModelSkeleton()
{
	for each(Pair temp in skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	SAFE_DELETE_ARRAY(skinTransforms);
}

void ModelSkeleton::AddSkeletonBone(ModelSkeletonBone * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));
	
	boneCount++;
}

ModelSkeletonBone * ModelSkeleton::FindBone(wstring boneName)
{
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}
	
	return NULL;
}

int ModelSkeleton::FindBoneIndex(wstring boneName)
{
	int index = 0;
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return index;

		index++;
	}

	return -1;
}

D3DXMATRIX & ModelSkeleton::GetBoneAnimationTransform(int boneIdx)
{
	return boneAnimationTransforms[boneIdx];
}

ModelSkeletonBone * ModelSkeleton::GetSkeletonBone(int index)
{
	return skeletonBones[index].second;
}

void ModelSkeleton::AddAnimationFromOtherSkeleton(wstring srcAnimName, wstring dstAnimName, ModelSkeleton & srcSkeleton)
{
	vector<Pair> srcSkeletonBones = srcSkeleton.skeletonBones;

	vector<Pair>::const_iterator it = srcSkeletonBones.begin();
	for (; it != srcSkeletonBones.end(); ++it)
	{
		ModelSkeletonBone* srcBone = it->second;
		ModelSkeletonBone* dstBone = NULL;

		wstring srcBoneName = it->first;
		dstBone = FindBone(srcBoneName);

		if (dstBone != NULL)
		{
			ModelAnimationKeyFrames* srcAnimKeyFrames = srcBone->GetAnimationKeyFrames(srcAnimName);
			ModelAnimationKeyFrames* dstAnimKeyFrames = new ModelAnimationKeyFrames(*srcAnimKeyFrames);
			dstAnimKeyFrames->SetAnimationName(dstAnimName);
			dstBone->AddAnimationKeyFrames(dstAnimKeyFrames);
		}
	}
}

/****************************************************************************************************
 @brief
 현재 Animation의 Final Transform 정보를 업데이트
****************************************************************************************************/
void ModelSkeleton::UpdateAnimation(ModelAnimationController * animationController)
{
	ModelAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	RootType rootType = currentAnimation->GetRootType();
	BuildBoneTransforms(animationController, currentAnimation->GetName(), rootType);
}

void ModelSkeleton::Export(BinaryWriter * bw)
{
	bw->Write(boneCount);

	UINT skeletonBonesCount = skeletonBones.size();
	bw->Write(skeletonBonesCount);
	for (size_t i = 0; i < skeletonBonesCount; i++)
	{
		bw->Write(skeletonBones[i].first);
		(skeletonBones[i].second)->Export(bw);
	}

	bw->Write(skinTransforms, sizeof(D3DXMATRIX), boneCount);
	bw->Write(boneAnimationTransforms, sizeof(D3DXMATRIX), boneCount);
}

void ModelSkeleton::Import(BinaryReader * br)
{
	boneCount = br->Int();

	UINT skeletonBonesCount = br->UInt();
	skeletonBones.clear();
	skeletonBones.reserve(skeletonBonesCount);
	for (size_t i = 0; i < skeletonBonesCount; i++)
	{
		wstring name = br->WString();
		ModelSkeletonBone* modelSkeletonBone = new ModelSkeletonBone();
		modelSkeletonBone->Import(br);
		skeletonBones.push_back(make_pair(name, modelSkeletonBone));
	}

	SAFE_DELETE_ARRAY(skinTransforms);
	skinTransforms = new D3DXMATRIX[boneCount];
	br->Read(skinTransforms, sizeof(D3DXMATRIX), boneCount);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	boneAnimationTransforms = new D3DXMATRIX[boneCount];
	br->Read(boneAnimationTransforms, sizeof(D3DXMATRIX), boneCount);
}

/****************************************************************************************************
 @brief
 해당 Animation의 현재 Time에대한 KeyFrame의 정보(현재 KeyFrame, 다음 KeyFrmae, KeyFrame Factor)를 얻어와 
 각 Bone에 보간된 Tranform을 계산하여, 그로부터 FinalTransform 을 적용
****************************************************************************************************/
void ModelSkeleton::BuildBoneTransforms(ModelAnimationController* animationController, wstring animationName, RootType rootType)
{
	if (skinTransforms == NULL)
		skinTransforms = new D3DXMATRIX[boneCount];

	if (boneAnimationTransforms == NULL)	
		boneAnimationTransforms = new D3DXMATRIX[boneCount];


	int keyFrame = animationController->GetCurrentKeyFrame();
	int nextKeyFrame = animationController->GetNextKeyFrame();
	float keyframeFactor = animationController->GetKeyFrameFactor();
	bool useQuaterniokeyFrames = animationController->UseQuaternionKeyFrames(); // 보간 유무 결정

	int index = 0;
	vector<Pair>::const_iterator it = skeletonBones.begin();
	for (; it != skeletonBones.end(); ++it)
	{
		ModelSkeletonBone* bone = it->second;
				
		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform(); // 루트->본
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform(); // ???
		D3DXMATRIX matAnimation; // Local Transform(부모 본->자신 본)을 보간될 Transform
		D3DXMATRIX matParentAnimation; // 부모 본의 Local Transform(부모의 부모 본->부모 본)을 보간될 Transform

		D3DXMATRIX matFirstRootTransform;

		ModelAnimationKeyFrames* animatiokeyFrames = bone->GetAnimationKeyFrames(animationName);

		if (useQuaterniokeyFrames == true)
		{
			// 현재 KeyFrame과 다음 KeyFrame의 Quaternion 값 로드
			const ModelAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const ModelAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();
			const D3DXQUATERNION& minus_q2 = -q2;

			D3DXQUATERNION q; // 보간될 Quaternion

			if(D3DXQuaternionLengthSq(&(q1-q2)) > D3DXQuaternionLengthSq(&(q1+q2)))
				D3DXQuaternionSlerp(&q, &q1, &minus_q2, keyframeFactor); // Quaternion(회전축, 회전각) 선형 구면 보간
			else
				D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor); // Quaternion(회전축, 회전각) 선형 구면 보간

			D3DXMatrixRotationQuaternion(&matAnimation, &q); // Quaternion을 Trnsform Matrix로 변환

			D3DXVECTOR3 t1 = qKeyFrame1.GetTranslation();
			D3DXVECTOR3 t2 = qKeyFrame2.GetTranslation();
			
			
			D3DXVECTOR3 vTranslation; // 보간된 본의 위치
			D3DXVec3Lerp(&vTranslation, &t1, &t2, keyframeFactor); // 위치의 선형 보간

			// 회전 축, 회전각이 구면보간된 Matrix에 선형 보간된 위치를 추가
			matAnimation._41 = vTranslation.x;
			matAnimation._42 = vTranslation.y;
			matAnimation._43 = vTranslation.z;
		}
		else
		{
			// 보간 하지 않을경우 그냥 KeyFrmae Trnsform을 사용
			matAnimation = animatiokeyFrames->GetKeyFrameTransform(keyFrame);
		}


		int parentBoneIndex = bone->GetParentBoneIndex();

		if (parentBoneIndex < 0)
		{
			// 루트의 경우 부모 본 TrsnformAnimation은 Idenetity Transform으로 적용
			D3DXMatrixIdentity(&matParentAnimation);

			// 루트 모션을 첫번째 프레임 값으로 고정
			if (rootType == RootType::RootType_FirstFrame) matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);
			// 루트 모션을 변경하지 않음
			else if (rootType == RootType::RootType_Fixed) matAnimation = bone->GetBindPoseTransform();
		}
		else
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		// boneAnimationTransforms = (자식 본->부모 본) * (부모 본-부모의 >부모 본) * ... (부모의 보무의...부모 본->루트 본) = (자신 본->루트본)
		boneAnimationTransforms[index] = matAnimation * matParentAnimation;

		// Mesh상의 Vertex는 Root 공간에 존재 하므로, matInvBindPose(Bone의 초기 값->루트)를 적용 후
		// boneAnimationTransforms(루트 본-> 자신 본) 으로 Animation 된 Bone의 위치로 적용
		skinTransforms[index] = matInvBindPose *  boneAnimationTransforms[index];

		index++;
	}
}
