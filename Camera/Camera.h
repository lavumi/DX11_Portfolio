#pragma once

class Camera
{
public:
	Camera();
	~Camera();

	static Camera* Get();
	static void Delete();




	void GetPosition(D3DXVECTOR3* position, D3DXVECTOR3* forward = nullptr)
	{
		*position = this->position;
		if(forward != nullptr)
			*forward = this->forward;
	}


	void GetCameraSpaceMatrix(D3DXMATRIX& matrix);


	void UpdateRotationMatrix();
	//void UpdateProjectionMatrix();
	//void UpdateProjection2Orth();
	void UpdateViewMatrix();
	//void Update();

	void Move(D3DXVECTOR3 translation);
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();

	void Rotate(D3DXVECTOR2 rotate);
	void RotateUp();
	void RotateDown();
	void RotateLeft();
	void RotateRight();


	void GetView(D3DXMATRIX* view)
	{
		memcpy(view, this->view, sizeof(D3DXMATRIX));
	}

	void GetMirrorView(D3DXMATRIX* view) {
		memcpy(view, this->mirrorView, sizeof(D3DXMATRIX));
	}

	void GetDefaultView(D3DXMATRIX* view) {
		memcpy(view, this->defaultView, sizeof(D3DXMATRIX));
	}
	//void GetProjection(D3DXMATRIX* projection)
	//{
	//	memcpy(projection, this->projection, sizeof(D3DXMATRIX));
	//}
	//
	//void GetOrthProjection(D3DXMATRIX* projection)
	//{
	//	memcpy(projection, this->orthoProjection, sizeof(D3DXMATRIX));
	//}


	void GetRay(D3DXVECTOR3* origin, D3DXVECTOR3* direction, D3DXMATRIX projection);

	const static float screenNear; //거리
	const static float screenDepth; //깊이

	float fieldOfView; //시야각

private:
	static Camera* instance;



	D3DXVECTOR3 position; //위치
	D3DXVECTOR3 forward; //전방
	D3DXVECTOR3 right; //우측
	D3DXVECTOR3 up; //위쪽
	D3DXVECTOR2 rotate; //회전

	D3DXMATRIX view;
	D3DXMATRIX mirrorView;
	D3DXMATRIX rotation;

	D3DXMATRIX defaultView;


	D3DXMATRIX cameraSpace;

	float translationSpeed;
	float rotationSpeed;


	//D3D11_VIEWPORT viewport;
	//
	//float screenAspect; //화면비율
	//D3DXMATRIX projection;
	//D3DXMATRIX orthoProjection;
};