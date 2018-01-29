#include "../stdafx.h"
#include "LightManager.h"


LightManager* LightManager::instance = nullptr;

LightManager * LightManager::Get()
{
	if (instance == nullptr)
		instance = new LightManager();
	return instance;
}

void LightManager::Update()
{
	D3DXVECTOR3 position, forward;
	//Camera::Get()->GetPosition(&position, &forward);
	position = D3DXVECTOR3(128, 0,128);

	angley -= (float)D3DX_PI / 180 * speed;

	lightData.lightDirection.x = cosf(angley);
	lightData.lightDirection.y = sinf(angley);
	

	


	D3DXVec3Normalize(&lightData.lightDirection, &lightData.lightDirection);


	D3DXMatrixLookAtLH(&lightData.lightView, &(position - lightData.lightDirection * 300), &(position), &up);
	D3DXMatrixOrthoLH(&lightData.lightProjection,500, 500, 0.1f, 500);
	
	D3DXMatrixTranspose(&lightData.lightView, &lightData.lightView);
	D3DXMatrixTranspose(&lightData.lightProjection, &lightData.lightProjection);
}

void LightManager::Delete()
{
	SAFE_DELETE(instance);
}

LightManager::LightManager()
{
	lightData.lightDirection = D3DXVECTOR3(0.01f, -1, 0.01f);

	anglex = angley = (float)D3DX_PI / 180 * -90;
	speed = 0;
	UserInterface::Get()->AddSun(&lightData.lightDirection, &speed);
}

LightManager::~LightManager()
{
	SAFE_DELETE(instance);
}
