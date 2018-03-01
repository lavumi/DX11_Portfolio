#pragma once

enum class AnimationMode
{
	Play = 0, Pause, Stop,
};

/********************************************************************************
 @breif
 Time에 따른 Animatiion의 Frame을 계산하는 Class
 
 현재 사용되는 Animation 을 조정하고, 그 Animation의 현재 Time에 해당하는 Key Frame(current, next)을 계산
 useQuaternionKeyFrames이 Ture일때 Time에 따른 Frame 보간값을 계산
********************************************************************************/
class ModelAnimation;
class ModelAnimationController
{
public:
	ModelAnimationController();
	~ModelAnimationController();

	void AddAnimation(ModelAnimation* animation);

	void SetIsLoop(bool isLoop) { this->isLoop = isLoop; }

	ModelAnimation* GetCurrentAnimation();
	void SetCurrentAnimation(wstring animationName);
	void SetCurrentAnimation(int index);

	UINT GetAnimationCount();
	ModelAnimation* GetAnimation(int index);
	ModelAnimation* GetAnimation(wstring animName);

	int GetCurrentKeyFrame() { return currentKeyFrame; }
	void SetCurrentKeyFrame(int keyFrame);
	
	int GetNextKeyFrame() { return nextKeyFrame; }
	float GetKeyFrameFactor() { return keyFrameFactor; }

	AnimationMode GetAnimationMode() { return animationMode; }

	void UseQuaternionKeyFrames(bool use) { useQuaternionKeyFrames = use; }
	bool UseQuaternionKeyFrames() { return useQuaternionKeyFrames; }

	void Play();
	void Pause();
	void Stop();

	void Update();
private:
	AnimationMode animationMode; /// 재생 모드
	ModelAnimation* currentAnimation; /// 현재 Animation
	ModelAnimation* prevAnimation; /// 현재 Animation

	typedef pair<wstring, ModelAnimation *> Pair;
	vector<Pair> animations; /// Animation 벡터
	
	float frameTimer; /// 현재 KeyFrame으로 변경 후 경과 시간

	int currentKeyFrame; /// 현재 KeyFrame
	int nextKeyFrame; /// 다음 KeyFrame
	float keyFrameFactor; /// 현재 KeyFrame과 다음 KeyFrame사이에서의 보간 값
	bool useQuaternionKeyFrames; /// 보간 사용 유무 flag

	bool isLoop;
};