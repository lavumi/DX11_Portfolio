#pragma once
#include "Shader.h"

class WaterBuffer;
class WaterShader : public Shader {
public:
	WaterShader();
	
	~WaterShader();


	virtual void Update();

	virtual void Render();





};