#include "../stdafx.h"
#include "Character.h"

#include "../Model/ModelScene.h"
#include "../Terrain/Landscape.h"
#include "../Camera/Camera.h"
Character::Character()
{
	model = new ModelScene();
	position = D3DXVECTOR3(128, 0, 128);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	speed = 1;
}

Character::~Character()
{
	SAFE_DELETE(model);
}

void Character::Initialize(FBXModelShader* shader, Landscape* land)
{

	model->LoadFbx(L"/Contents/Models/Sword And Shield Idle.fbx");
	model->LoadAniFbx(L"/contents/Models/jump.fbx", L"jump");
	//model->SaveBinary(L"./Contents/Models/", L"PaladinIdle");
	//model->LoadBinary(L"./Contents/Models/", L"PaladinIdle");



	this->land = land;
	this->shader = shader;
	position = D3DXVECTOR3(140, 0, 110);
	land->GetY(position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	speed = 1;

	SetCamera(Camera::Get());
}

void Character::MoveTo(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}

void Character::Move(float x, float z)
{
	return;
	position.x += x * speed;
	position.z += z * speed;
	land->GetY(position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}

void Character::Update()
{
	model->Update(&world);
}

void Character::Render()
{
	model->Render(shader);
}

void Character::SetAni(int index)
{
	model->SetAni(index);
}

void Character::SetCamera(Camera * camera)
{
	camera->SetFallowCamera(this);
}

void Character::GetPosition(D3DXVECTOR3*& pos)
{
	pos = &position;
}
