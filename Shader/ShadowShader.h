#pragma once
#include "Shader.h"


class ShadowShader : public Shader {
public:
	ShadowShader();
	~ShadowShader();


	virtual void Update();
	virtual void Render();



	//void CreateBuffers();
	//
	//ID3D11Buffer* LightBuffer;




};