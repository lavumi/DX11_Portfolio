#include "../stdafx.h"
#include "Character.h"

#include "../Model/ModelScene.h"
#include "../Model/ModelAnimation.h"

#include "../Environment/Environment.h"
#include "../Camera/Camera.h"
Character::Character()
{
	model = new ModelScene();
	position = D3DXVECTOR3(208, 0, 208);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	moveState = false;
	UserInterface::AddViewValue(&position.x, &position.z);
	forwardAngle = 0;
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
	position = D3DXVECTOR3(160, 0, 160);
	land->GetLandY(position);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	speed = 0.03f;

	textID = TextManager::Get()->AddText(D3DXVECTOR2(10, 32), "Position");

}


Character::~Character()
{
	SAFE_DELETE(model);
}


void Character::MoveTo(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}


void Character::Move(float x, float z)
{
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
	if (Keyboard::Get()->KeyPress('W')) {
		Move(0, 1);
		SetAni(6);
	}
	else if (Keyboard::Get()->KeyPress('S')) {
		Move(0, -1);
		SetAni(0);
	}
	else if (Keyboard::Get()->KeyPress('A')) {
		Move(-1, 0);
		SetAni(5);
	}
	else if (Keyboard::Get()->KeyPress('D')) {
		Move(1, 0);
		SetAni(4);
	}
	else
		SetAni(2);
}

void Character::PrintCharacterPos()
{
	strcpy_s(text, "Position : ");

	_itoa_s((int)position.x, frametxt, 10);
	strcat_s(text, frametxt);
	strcat_s(text, ", ");

	_itoa_s((int)position.y, frametxt, 10);
	strcat_s(text, frametxt);
	strcat_s(text, ", ");

	_itoa_s((int)position.z, frametxt, 10);
	strcat_s(text, frametxt);
	strcat_s(text, ", ");
	TextManager::Get()->ChangeText(textID, text);
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
	PrintCharacterPos();
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
