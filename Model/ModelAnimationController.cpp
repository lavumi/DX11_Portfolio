#include "../stdafx.h"
#include "ModelAnimationController.h"
#include "ModelAnimation.h"

ModelAnimationController::ModelAnimationController()
	: currentAnimation(NULL), prevAnimation(NULL)
	, currentKeyFrame(0), nextKeyFrame(0), keyFrameFactor(0.0f), frameTimer(0.0f)
	, isLoop(false)
{
	useQuaternionKeyFrames = true;

	animationMode = AnimationMode::Stop;
}

ModelAnimationController::~ModelAnimationController()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);
	
	currentAnimation = NULL;
}

/****************************************************************************************************
 @brief
 animations에 ModelAnimation을 Animation 이름 별로 추가
****************************************************************************************************/
void ModelAnimationController::AddAnimation(ModelAnimation * animation)
{
	bool isExist = false;
	for (size_t i = 0; i < animations.size(); i++)
	{
		// 중복 확인
		if (animations[i].first == animation->GetName())
		{
			isExist = true;
			
			break;
		}
	}
	assert(isExist == false);

	animations.push_back(Pair(animation->GetName(), animation));
}

ModelAnimation * ModelAnimationController::GetCurrentAnimation()
{
	if(currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

/****************************************************************************************************
 @brief
 Animation의 이름으로 현재 Animation을 설정
****************************************************************************************************/
void ModelAnimationController::SetCurrentAnimation(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}
	
	if (count != -1)
	{
		if (currentAnimation == animations[count].second)
			return;

		// 현재 애니메이션 교체, 이전 애니메이션 저장
		prevAnimation = currentAnimation;
		currentAnimation = animations[count].second;
	}
	else
		return;

	// 현재 Animation이 바뀌면 Animation 실행 중지
	Stop();
}

/****************************************************************************************************
 @brief
 Animation의 Index로 현재 Animation을 설정
****************************************************************************************************/
void ModelAnimationController::SetCurrentAnimation(int index)
{
	if( index < 0 || index >= (int)animations.size() )
		currentAnimation = NULL;
	else
	{
		prevAnimation = currentAnimation;
		currentAnimation = animations[index].second;
	}
	
	Stop();
}

UINT ModelAnimationController::GetAnimationCount()
{
	return animations.size();
}

ModelAnimation * ModelAnimationController::GetAnimation(int index)
{
	if( index < 0 || index >= (int)animations.size() )
		return NULL;
	else
		return animations[index].second;
}

ModelAnimation * ModelAnimationController::GetAnimation(wstring animName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animName)
		{
			count = i;
			break;
		}
	}

	if (count != -1)
	{
		return animations[count].second;
	}
	else
		return NULL;
}

void ModelAnimationController::Play()
{
	currentAnimation->SetIsEnd(false);
	animationMode = AnimationMode::Play;
}

void ModelAnimationController::Pause()
{
	animationMode = AnimationMode::Pause;
}

void ModelAnimationController::Stop()
{
	animationMode = AnimationMode::Stop;
	frameTimer = 0.0f;

	SetCurrentKeyFrame(0);
}

/****************************************************************************************************
 @brief
 현재 KeyFrame, 다음 KeyFrame을 설정
****************************************************************************************************/
void ModelAnimationController::SetCurrentKeyFrame(int keyFrame)
{
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrameCount();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();
	
	keyFrameFactor = 0.0f;
}

/****************************************************************************************************
 @brief
 frameTimer를 증가시키며 그에 따른 현재 KeyFrame과 다음 KeyFrame 정보를 갱신
 frameTimer가 한 Frame시간보다 길어지면 KyeFrame을 다음으로 이동하고 frameTimer를 초기화
 KeyFrame은 Animation의 전체 Frame 수를 넘을 수 없으도록 설정
 보간을 위해, 시간 경과에 따른 현재 KeyFrame과 다음 KeyFrame의 상대적 위치를 나타내는 FrameFactor를 계산
****************************************************************************************************/
void ModelAnimationController::Update()
{
	if (currentAnimation == NULL || animationMode != AnimationMode::Play)
		return;

	frameTimer += Frames::TimeElapsed(); //이전 Frmae이 실행 된 후 경과 시간

	float secPerFrame = 1.0f / currentAnimation->GetFrameRate(); // Frame 하나의 소요 시간

	if (frameTimer > secPerFrame)
	{
		currentKeyFrame = nextKeyFrame;

		int frameCount = 0;
		while (frameTimer > secPerFrame)
		{
			//currentKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();
			nextKeyFrame = (nextKeyFrame + 1) % currentAnimation->GetKeyFrameCount();

			frameTimer -= secPerFrame;
			frameCount++;

			// Loop가 아닌 Animation이 종료될 때
			if (currentAnimation->GetIsLoop() == false && nextKeyFrame == 0)
			{
				currentAnimation->SetIsEnd(true);

				/*if (prevAnimation != NULL)
				{
				currentAnimation = prevAnimation;
				prevAnimation = NULL;
				currentKeyFrame = 0;
				}*/
				animationMode = AnimationMode::Stop;

				currentKeyFrame = currentAnimation->GetKeyFrameCount() - 1;
				nextKeyFrame = currentKeyFrame;
			}
		}
		keyFrameFactor = (frameTimer + frameCount * secPerFrame) / ((frameCount + 1) *secPerFrame);
	}
	else
	{
		// KeyFrame의 보간값을 계산 [0,1]
		// 현재 Frame와 다음 Frame 사이에서 어느 정도 시간이 경과되었는지를 나타냄
		keyFrameFactor = frameTimer / secPerFrame;
	}
}
