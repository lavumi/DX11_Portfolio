#pragma once
#include "Shader.h"
class BaseBuffer;
class FBXModelShader : public Shader {
public:
	FBXModelShader();
	~FBXModelShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world);

	BaseBuffer* buffer;


private:
	void CreateBuffers();





};