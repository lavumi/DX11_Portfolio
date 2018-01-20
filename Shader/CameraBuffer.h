#pragma once
#include "ShaderBuffer.h"

class CameraBuffer : public ShaderBuffer
{
public:
	CameraBuffer()
		: ShaderBuffer(sizeof(Data))
	{
		
	}

	void Update()
	{
		Camera::Get()->GetView(&data.view);
		D3D::Get()->GetProjection(&data.projection);
		Camera::Get()->GetPosition(&data.position);


		Data temp;
		D3DXMatrixTranspose(&temp.view, &data.view);
		D3DXMatrixTranspose(&temp.projection, &data.projection);
		temp.position = data.position;

		UpdateBuffer(&temp, sizeof(Data));
	}


//void MirrorUpdate()
//{
//	Camera::Get()->GetView(&data.view);
//	D3D::Get()->GetProjection(&data.projection);
//	Camera::Get()->GetPosition(&data.position);
//
//
//	D3DXMATRIX R;
//	D3DXPLANE plane(0.0f, 0.0f, 1.0f, -20.0f); // xy plane
//	D3DXMatrixReflect(&R, &plane);
//	data.view =  R*data.view;
//
//	D3DXVec3TransformCoord(&data.position, &data.position, &R);
//
//	Data temp;
//	D3DXMatrixTranspose(&temp.view, &data.view);
//	D3DXMatrixTranspose(&temp.projection, &data.projection);
//	temp.position = data.position;
//
//	UpdateBuffer(&temp, sizeof(Data));
//}


	void OrthoUpdate() {



	
		D3DXVECTOR3 position = D3DXVECTOR3(0, 0, -10);
		D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
		D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtLH(&data.view, &position, &(position + forward), &up);
		D3D::Get()->GetOrthoProjection(&data.projection);
		Camera::Get()->GetPosition(&data.position);

		Data temp;
		D3DXMatrixTranspose(&temp.view, &data.view);
		D3DXMatrixTranspose(&temp.projection, &data.projection);
		temp.position = data.position;

		UpdateBuffer(&temp, sizeof(Data));
	}
	struct Data
	{
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXVECTOR3 position;
		float padding;
	};

private:
	Data data;
};