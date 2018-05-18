#include "../stdafx.h"
#include "LightManager.h"


//LightManager* LightManager::instance = nullptr;

//LightManager * LightManager::Get()
//{
//	if (instance == nullptr)
//		instance = new LightManager();
//	return instance;
//}

void LightManager::Update()
{
	D3DXVECTOR3 position;
	position = D3DXVECTOR3(256, 0,256);


	//구면좌표계를 통해 광원의 위치 설정
	angle_theta += 3.141592f / 180 * speed;


	if (angle_pie < 3.141592f / 180 * 30)
		anglePieDir = 0.666666f;
	else if (angle_pie > 3.141592f / 180 * 150)
		anglePieDir = -0.666666f;
	
	angle_pie += 3.141592f / 180 * speed * anglePieDir;
	

	lightData.lightPosition.x = sinf(angle_pie) * cosf(angle_theta);
	lightData.lightPosition.z = sinf(angle_pie) * sinf(angle_theta);
	lightData.lightPosition.y = cosf(angle_pie);


	if (angle_pie > 3.141592f / 180 * 90)
		lightData.night = 1;
	else
		lightData.night = 0;



	
	//데이터 입력
	D3DXVec3Normalize(&lightData.lightPosition, &lightData.lightPosition);

	D3DXMATRIX view, proj;

	if(lightData.night == 1)
		D3DXMatrixLookAtLH(&view, &(position - lightData.lightPosition * 300), &(position), &up);
	else
		D3DXMatrixLookAtLH(&view, &(position + lightData.lightPosition * 300), &(position), &up);
	D3DXMatrixOrthoLH(&proj, 500, 500, 0.1f, 1000);

	D3DXMatrixMultiply(&lightData.lightViewXProjection, &view, &proj);

	


	//빛의 위치에 따른 global ambient 설정
	float r, g, b;

	r = sinf(angle_theta);
	b = D3DXVec3Dot(&lightData.lightPosition, &D3DXVECTOR3(0, 0.8660f, 0.5f));
	g = (r + b) / 2;

	r = max(r, 0.2f);
	g = max(g, 0.2f);
	b = max(b, 0.2f);

	globalAmbient = D3DXCOLOR(r, g, b, 1);
}

void LightManager::Delete()
{
//	SAFE_DELETE(instance);
}

LightManager::LightManager()
{
	//lightData.lightPosition = D3DXVECTOR3(0.01f, -1, 0.01f);

	angle_pie = 3.141592f / 6;
	angle_theta = 3.141592f / 2;
	anglePieDir = -0.333333f;

	lightData.lightPosition.x = sinf(angle_pie) * cosf(angle_theta);
	lightData.lightPosition.z = sinf(angle_pie) * sinf(angle_theta);
	lightData.lightPosition.y = cosf(angle_pie);

	speed = 0;
	UserInterface::Get()->AddSun(&lightData.lightPosition, &speed);
}

LightManager::~LightManager()
{
//	SAFE_DELETE(instance);
}
