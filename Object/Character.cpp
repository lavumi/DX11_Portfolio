#include "../stdafx.h"
#include "Character.h"

#include "../Model/ModelScene.h"
#include "../Model/ModelAnimation.h"

#include "../Environment/Environment.h"
#include "../Camera/Camera.h"
Character::Character()
{
	model = new ModelScene();
	position = D3DXVECTOR3(128, 0, 128);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	moveState = false;
	UserInterface::AddViewValue(&position.x, &position.z);
	forwardAngle = 0;
}

Character::~Character()
{
	SAFE_DELETE(model);
}

void Character::Initialize(Environment* land)
{
	//model->LoadFbx(L"/Contents/Models/Sword And Shield Idle.fbx");
	//
	//model->LoadAniFbx(L"/contents/Models/Anime/back.fbx", L"back");
	//model->LoadAniFbx(L"/contents/Models/Anime/death.fbx", L"death");
	//model->LoadAniFbx(L"/contents/Models/Anime/idle.fbx", L"idle");
	//model->LoadAniFbx(L"/contents/Models/Anime/slash.fbx", L"slash");
	//model->LoadAniFbx(L"/contents/Models/Anime/strafe1.fbx", L"strafe1");
	//model->LoadAniFbx(L"/contents/Models/Anime/strafe3.fbx", L"strafe3");
	//model->LoadAniFbx(L"/contents/Models/Anime/walk.fbx", L"walk");
	
	
	//model->GetAnimation(L"walk")->SetRootType(RootType::RootType_FirstFrame);
	
	//model->SaveBinary(L"./Contents/Models/", L"Paladin");

	model->LoadBinary(L"./Contents/Models/", L"Paladin");



	this->land = land;
	position = D3DXVECTOR3(140, 0, 110);
	land->GetLandY(position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	speed = 0.13f;

 	
}

void Character::MoveTo(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}


void Character::Move(float x, float z)
{
	//model->SetAni(0);


	if (forwardAngle == 0) {
		return;
		position.x += x * speed;
		position.z += z * speed;
	}
	else
	{
		D3DXVECTOR3 forward2 = D3DXVECTOR3(-sinf(*forwardAngle + 3.141592f / 2), 0, -cosf(*forwardAngle + 3.141592f / 2));
		D3DXVECTOR3 right2 = D3DXVECTOR3(sinf(*forwardAngle), 0, cosf(*forwardAngle));

		position += z * speed * forward2;
		position += x * speed * right2;

		D3DXMATRIX rotation;

		D3DXMatrixRotationY(&rotation, (*forwardAngle - 3.141592f / 2));

		land->GetLandY(position);
		D3DXMatrixTranslation(&world, position.x, position.y, position.z);

		D3DXMatrixMultiply(&world, &rotation, &world);
	}


}

void  Character::MoveControl()
{
	//UINT moving = 0;
	if (Keyboard::Get()->KeyPress('W')) {
		Move(0, 1);
//		moving = 1;
		SetAni(6);
	}
	else if (Keyboard::Get()->KeyPress('S')) {
		Move(0, -1);
//		moving = 0;
		SetAni(0);
	}
	else if (Keyboard::Get()->KeyPress('A')) {
		Move(-1, 0);
//		moving = 3;
		SetAni(5);
	}
	else if (Keyboard::Get()->KeyPress('D')) {
		Move(1, 0);
//		moving = 4;
		SetAni(4);
	}
	else
		SetAni(2);
	
}

void Character::SetFallowCamera()
{
	Camera::Get()->SetFallowCamera(this);
	Camera::Get()->GetCamAngle(forwardAngle);
}

void Character::Update()
{
	model->Update(&world);
	MoveControl();
	//bool curMove = MoveControl();
	//if (moveState != curMove) {
	//	if (moveState == true) {
	//		SetAni(0);
	//		moveState = false;
	//	}
	//	else
	//	{
	//		SetAni(1);
	//		moveState = true;
	//	}
	//		
	//}

}

void Character::Render()
{
	model->Render();
}

void Character::SetAni(int index)
{
	if (currentAnime != index) {
		model->SetAni(index);
		currentAnime = index;
	}
		
	
}

void Character::GetPosition(D3DXVECTOR3*& pos)
{
	pos = &position;
}
