#pragma once

class ModelScene;
class Shader;
class Environment;

#include "../Camera/iFallowCamera.h"
class Character  : public iFallowCamera
{
public:

	Character();
	~Character();

	void Initialize(Environment* );
	void MoveTo(float x, float y, float z);
	

	
	void Update();
	void Render();


	void SetAni(int index);

	void GetPosition(D3DXVECTOR3*& pos);


	//ifCamera
	virtual void SetFallowCamera();
private:
	void Move(float x, float z);
	void MoveControl();

	bool moveState;
	ModelScene * model;
	D3DXMATRIX world;
	D3DXVECTOR3 position;
	float speed;
	Environment* land;

	UINT currentAnime;


	UINT textID;
	char text[64];
	char frametxt[5];
	
	void PrintCharacterPos();

	//방향 각도
	float* forwardAngle;
};