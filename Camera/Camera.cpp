#include "../stdafx.h"
#include "Camera.h"

#include "iFallowCamera.h"
Camera* Camera::instance = NULL;

const float Camera::screenNear = 0.1f;
const float Camera::screenDepth = 400.0f;

Camera::Camera()
{
	position = D3DXVECTOR3(0, 0, -30);
	forward = D3DXVECTOR3(0, 1, 0);
	right = D3DXVECTOR3(0, 0, 0);
	up = D3DXVECTOR3(0, 0, 0);
	rotate = D3DXVECTOR2(0, 0);
	translationSpeed = 38.0f;
	rotationSpeed = 1.5f;
	targetpos = nullptr;
	angle_pie = 3.141592f / 8 * 3;
	angle_theta = 3.141592f / 2;
	targetDistance = 6;
	targetHeightOffset = 2;



	D3DXMatrixIdentity(&view);


	//OrthoWindow용 기본 카메라 세팅
	D3DXVECTOR3 defaultposition = D3DXVECTOR3(0, 0, -10);
    D3DXVECTOR3 defaultforward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 defaultup = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&defaultView, &defaultposition, &(defaultposition + defaultforward), &defaultup);


	UpdateRotationMatrix();
	UpdateViewMatrix();
}

Camera::~Camera()
{
}

Camera * Camera::Get()
{
	if (instance == NULL)
		instance = new Camera();

	return instance;
}

void Camera::Delete()
{
	SAFE_DELETE(instance);
}


void Camera::GetCameraSpaceMatrix(D3DXMATRIX & matrix)
{
	D3DXMatrixIdentity(&cameraSpace);
	cameraSpace._11 = right.x;
	cameraSpace._12 = right.y;
	cameraSpace._13 = right.z;

	cameraSpace._21 = forward.x;
	cameraSpace._22 = forward.y;
	cameraSpace._23 = forward.z;

	cameraSpace._31 = up.x;
	cameraSpace._32 = up.y;
	cameraSpace._33 = up.z;

	matrix = cameraSpace;
}

void Camera::SetFallowCamera(iFallowCamera * target)
{
	if (target != nullptr) {
		target->GetPosition(targetpos);
		forward= D3DXVECTOR3(0, 0, 1);
		right= D3DXVECTOR3(1, 0, 0);
		up= D3DXVECTOR3(0, 1, 0); 
	}
	else
		targetpos = nullptr;
}

void Camera::UpdateRotationMatrix()
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, rotate.x);
	D3DXMatrixRotationY(&y, rotate.y);
	
	rotation = x * y;

	D3DXVec3TransformCoord(&forward, &D3DXVECTOR3(0, 0, 1), &rotation);
	D3DXVec3TransformCoord(&right, &D3DXVECTOR3(1, 0, 0), &rotation);
	D3DXVec3TransformCoord(&up, &D3DXVECTOR3(0, 1, 0), &rotation);
}

//void Camera::UpdateProjectionMatrix()
//{
//	D3DInfo info;
//	D3D::GetInfo(&info);
//
//	viewport.Width = (float)info.screenWidth;
//	viewport.Height = (float)info.screenHeight;
//	viewport.MinDepth = 0.0f;
//	viewport.MaxDepth = 1.0f;
//	viewport.TopLeftX = 0.0f;
//	viewport.TopLeftY = 0.0f;
//	D3D::GetDeviceContext()->RSSetViewports(1, &viewport);
//
//	fieldOfView = (float)D3DX_PI / 4.0f;
//	screenAspect = (float)info.screenWidth / (float)info.screenHeight;
//
//	D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, screenAspect, screenNear, screenDepth);
//}


//void Camera::UpdateProjection2Orth()
//{
//	D3DXMatrixIdentity(&view);
//	D3DXMatrixOrthoLH(&orthoProjection, 100, 100, screenNear, screenDepth);
//}
//
void Camera::UpdateViewMatrix()
{                                                               
	D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);

	D3DXMATRIX R;
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, 7.7f); // xy plane}
	D3DXMatrixReflect(&R, &plane);
	mirrorView = R * view;

}

void Camera::Update()
{
	if (targetpos != nullptr) {
		//구면좌표계를 사용하여 카메라 위치 세팅
		D3DXVECTOR3 cameraChildPos;
		cameraChildPos.x = targetDistance * sinf(angle_pie) * cosf(angle_theta);
		cameraChildPos.z = -targetDistance * sinf(angle_pie) * sinf(angle_theta) ;
		cameraChildPos.y = targetDistance * cosf(angle_pie);
		position = *targetpos + cameraChildPos;
		position.y += targetHeightOffset;

		forward = - cameraChildPos;
		D3DXVec3Normalize(&forward, &forward);

		D3DXVec3Cross(&right, &up, &forward);
		D3DXVec3Normalize(&right, &right);
		UpdateViewMatrix();
	}

}

void Camera::Move(D3DXVECTOR3 translation)
{
	position += translation * Frames::TimeElapsed();
	UpdateViewMatrix();
}

void Camera::MoveUp()
{
	Move(up * translationSpeed);
}

void Camera::MoveDown()
{
	Move(-up * translationSpeed);
}

void Camera::MoveLeft()
{
	Move(-right * translationSpeed);
}

void Camera::MoveRight()
{
	Move(right * translationSpeed);
}

void Camera::MoveForward()
{
	Move(forward * translationSpeed);
}

void Camera::MoveBackward()
{
	Move(-forward * translationSpeed);
}

void Camera::Rotate(D3DXVECTOR2 rotate)
{
	if (targetpos != nullptr) {
		angle_theta += rotate.y	* 3.141592f / 45 * Frames::TimeElapsed();
		angle_pie -= rotate.x	* 3.141592f / 15 * Frames::TimeElapsed();
	}
	else {
		this->rotate.x += rotate.x * Frames::TimeElapsed() * rotationSpeed;
		this->rotate.y += rotate.y * Frames::TimeElapsed() * rotationSpeed;
		
		UpdateRotationMatrix();
		UpdateViewMatrix();
	}


}

void Camera::RotateUp()
{
	Rotate(D3DXVECTOR2(1, 0));
}

void Camera::RotateDown()
{
	Rotate(D3DXVECTOR2(-1, 0));
}

void Camera::RotateLeft()
{
	Rotate(D3DXVECTOR2(0, 1));
}

void Camera::RotateRight()
{
	Rotate(D3DXVECTOR2(0, -1));
}

void Camera::GetRay(D3DXVECTOR3 * origin, D3DXVECTOR3 * direction, D3DXMATRIX projection)
{
	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();

	D3DInfo info;
	D3D::GetInfo(&info);


	D3DXVECTOR2 point;
	point.x = ((2.0f * mouse.x) / info.screenWidth) - 1.0f;
	point.y = (((2.0f * mouse.y) / info.screenHeight) - 1.0f) * -1.0f;

	point.x = point.x / projection._11;
	point.y = point.y / projection._22;

	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);

	D3DXVECTOR3 dir;
	dir.x = (point.x * invView._11) + (point.y * invView._21) + invView._31;
	dir.y = (point.x * invView._12) + (point.y * invView._22) + invView._32;
	dir.z = (point.x * invView._13) + (point.y * invView._23) + invView._33;

	D3DXVec3Normalize(&dir, &dir);
	
	*origin = position;
	*direction = dir;
}
