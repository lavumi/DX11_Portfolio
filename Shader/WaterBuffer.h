#pragma once
#include "ShaderBuffer.h"

class WaterBuffer : public ShaderBuffer
{
public:
	WaterBuffer()
		: ShaderBuffer(sizeof(VS_DATA), 0,0,0, sizeof(PS_DATA))
	{
		D3DXMATRIX world;
		D3DXMatrixTranslation(&world, 0.0f, -7.9f, 0.0f);

		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, nullptr, &world);
		D3DXMatrixTranspose(&vsData.invTransWorld, &invWorld);



		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		psData.shininess = 200;
		//psData.globalAmbient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);


		angle = 90;
		speed = 0.001f;

		psData.translation = 0;
		psData.scale = 6;
		psData.angle = (float)D3DX_PI / 180 * angle;

		UpdateVSBuffer(&vsData, sizeof(VS_DATA));
		UpdatePSBuffer(&psData, sizeof(PS_DATA));


		UserInterface::AddWave(&speed, &psData.scale, &angle);
	}

	void SetMaterial(D3DXCOLOR& ambient, D3DXCOLOR& diffuse) {
		psData.ambient = ambient;
		psData.diffuse = diffuse;
	//	psData.globalAmbient = globalAmbient;

		//UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void SetWater(float speed, float angle, float scale) {
		this->speed = speed;
		this->angle = angle;
		this->psData.scale = scale;
	}

	void SetWorld(D3DXMATRIX& world) {
		//ShaderBuffer::SetWorld(world);

		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, nullptr, &world);
		D3DXMatrixTranspose(&vsData.invTransWorld, &invWorld);

		//UpdateVSBuffer(&vsData, sizeof(VS_DATA));
	}

	void Update()
	{
		Camera::Get()->GetPosition(&(vsData.cameraPos));
		UpdateVSBuffer(&vsData, sizeof(VS_DATA));



		psData.translation += speed;
		psData.angle = (float)D3DX_PI / 180 * angle;
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	struct VS_DATA
	{
		D3DXVECTOR3 cameraPos;
		float padding;
		D3DXMATRIX invTransWorld;
	};
	struct PS_DATA
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		//D3DXCOLOR globalAmbient;
		float shininess;
		float translation;
		float scale;
		float angle;
	};


private:
	float speed;
	float angle;
	VS_DATA vsData;
	PS_DATA psData;
};