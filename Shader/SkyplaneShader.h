#pragma once
#include "Shader.h"


class SkyplaneShader : public Shader {
public:
	SkyplaneShader();
	~SkyplaneShader();


	void Update();
	void Render();



private:



	float speed;

};