#pragma once
#include "../Utility/BinaryInputOutputHandler.h"
/********************************************************************************
* @brief
* KeyFrame에 저장될 Quaternion 정보 Class
*
* Quaternion(회전축, 회전각), Translation(회전축 위치)를 저장
********************************************************************************/
class ModelAnimationQuaternionKeyFrame : public BinaryInputOutputHandler
{
public:
	ModelAnimationQuaternionKeyFrame() {};
	ModelAnimationQuaternionKeyFrame(const D3DXMATRIX& transform)
	{
		// Trnasform Matrix를 Quaternion으로 변환
		D3DXQuaternionRotationMatrix(&q, &transform);
		// Translation정보는 따로 저장
		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}

	const D3DXQUATERNION& GetQuaternion() const { return q; }
	const D3DXVECTOR3& GetTranslation() const { return translation; }

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	D3DXQUATERNION q; /// 회전축(Vector)과 회전각에 대한 정보를 갖는 Quaternion
	D3DXVECTOR3 translation; /// 회전축의 위치(Position) 정보
};

/********************************************************************************
 @brief
 AnimationName에 해당하는 Animation의 모든 KeyFrmae 정보를 저장하는 Class
 
 (KeyFrmae 정보 => Transform Matrix와 Quaternion을 Pair로 저장)
********************************************************************************/
class ModelAnimationKeyFrames : public BinaryInputOutputHandler
{
public:
	ModelAnimationKeyFrames() {};
	ModelAnimationKeyFrames(wstring animationName);
	ModelAnimationKeyFrames(ModelAnimationKeyFrames& other);
	~ModelAnimationKeyFrames();
	
	void AddKeyFrame(const D3DXMATRIX& transform);

	wstring GetAnimationName();
	void SetAnimationName(wstring animationName);

	const D3DXMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const ModelAnimationQuaternionKeyFrame& GetQuaternionKeyFrame(int keyFrame) const;

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	wstring animationName;

	typedef pair<D3DXMATRIX, ModelAnimationQuaternionKeyFrame> Pair;
	vector<Pair> keyFrames; ///모든 KeyFrame의 정보를 저장하는 벡터
};