#pragma once
#include "Shader.h"

class SkydomeBuffer;
class SkydomeShader : public Shader {
public:
	SkydomeShader();
	~SkydomeShader();


	virtual void Update();
	void Render(UINT indexCount, D3DXMATRIX world);



private:

	SkydomeBuffer* buffer;







};