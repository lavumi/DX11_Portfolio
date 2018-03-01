#pragma once
#include "Shader.h"

class FBXModelShader : public Shader {
public:
	FBXModelShader();
	~FBXModelShader();


	virtual void Update();
	virtual void Render(UINT indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);



private:
	void CreateBuffers();





};