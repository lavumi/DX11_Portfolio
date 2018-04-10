#pragma once

class ModelScene;
class Shader;
class Landscape;

#include "../Camera/iFallowCamera.h"
class Character  : public iFallowCamera
{
public:

	Character();
	~Character();

	void Initialize(Landscape* );
	void MoveTo(float x, float y, float z);
	void Move(float x, float z);
	void Update();
	void Render();


	void SetAni(int index);
	void SetCamera(Camera* camera);
	void GetPosition(D3DXVECTOR3*& pos);
private:

	ModelScene * model;
	D3DXMATRIX world;
	D3DXVECTOR3 position;
	float speed;
	Landscape* land;
};