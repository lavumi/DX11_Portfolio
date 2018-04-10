#pragma once
#include "../Utility/BinaryInputOutputHandler.h"

/********************************************************************************
 각 Vertex에 영향을 미치는 Bone Index와 영향을 미치는 정도인 Wieght를 저장하는 Class
 각 Vertex는 최대 4개의 BoneWeight를 가질수 있으며,
 Bone Index와 Bone Weight는 vector4의 형태로 Vertex Data에 저장된다.
 최종적인 Vertex 정보에 이 Weights값 들이 Blending 되어 적용된다.
********************************************************************************/
struct ModelBlendWeights
{
	D3DXVECTOR4 BlendIndices;
	D3DXVECTOR4 BlendWeights;

	ModelBlendWeights()
	{
		BlendIndices = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		BlendWeights = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void SetBlendWeight(int nIndex, int nBoneIndex, float fWeight)
	{
		switch (nIndex)
		{
			case 0:
				BlendIndices.x = (float)nBoneIndex;
				BlendWeights.x = fWeight;
				break;
			case 1:
				BlendIndices.y = (float)nBoneIndex;
				BlendWeights.y = fWeight;
				break;
			case 2:
				BlendIndices.z = (float)nBoneIndex;
				BlendWeights.z = fWeight;
				break;
			case 3:
				BlendIndices.w = (float)nBoneIndex;
				BlendWeights.w = fWeight;
				break;
		}
	}
};

class ModelBoneWeights
{
public:
	ModelBoneWeights();
	ModelBoneWeights(const ModelBoneWeights& otherModelBoneWeights);
	~ModelBoneWeights();

	static const UINT MaxBonesPerVertex; /// 한 Vertex에 영향을 미치는 최대 Bone의 수
	static const UINT MaxBonesPerMax; /// 최대 Bone의 수

	void AddBoneWeight(int boneIndex, float boneWeight);
	void AddBoneWeight(pair<int, float> boneWeightPair);
	void AddBoneWeights(const ModelBoneWeights& boneWeights);

	void Validate();
	void Normalize();

	int GetBoneWeightCount() const { return boneWeights.size(); }
	pair<int, float> GetBoneWeight(int index) const { return boneWeights[index]; }
	ModelBlendWeights GetBlendWeights();

private:
	float sumWeight; /// 한 Vertex에 영향을 주는 모든 Wieght의 합

	vector<pair<int, float>> boneWeights; /// Bone Index와 Weight값을 Pair를 저장하는 벡터
};