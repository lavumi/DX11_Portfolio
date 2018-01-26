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
	Camera::Get()->GetPosition(&position, &forward);


	angley -= (float)D3DX_PI / 180 / 10;

	lightData.lightDirection.x = cosf(angley);
	lightData.lightDirection.y = sinf(angley);
	



	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&lightData.lightDirection, &lightData.lightDirection);


	D3DXMatrixLookAtLH(&lightData.lightView, &position, &(lightData.lightDirection + position), &up);
	D3DXMatrixOrthoLH(&lightData.lightProjection,500, 500, 0.1f, 500);
	
	D3DXMatrixTranspose(&lightData.lightView, &lightData.lightView);
	D3DXMatrixTranspose(&lightData.lightProjection, &lightData.lightProjection);
}

LightManager::LightManager()
{
	lightData.lightDirection = D3DXVECTOR3(0.01f, -1, 0.01f);

	anglex = angley = (float)D3DX_PI / 180 *90;
//	UserInterface::Get()->AddSun(&lightData.lightDirection, nullptr);
}

LightManager::~LightManager()
{
	SAFE_DELETE(instance);
}
