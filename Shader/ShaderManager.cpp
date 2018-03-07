#include "../stdafx.h"
#include "ShaderManager.h"
#include "Shader.h"
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	auto iter = shaders.begin();
	for (; iter != shaders.end();iter++) {
		SAFE_DELETE(iter->second);
	}
}

bool ShaderManager::CreateShader(wstring fxName, D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	fxName += L".fx";
	Shader* shader = new Shader(fxName);
	bool vs, ps, gs, layout;
	//vs = shader->CreateVertexShader();
	//ps = shader->CreatePixelShader();
	//gs = shader->CreateGeometryShader();
	//
	//
	//if ((vs || ps || layout) == false) 
	//	assert(0);
	layout = shader->CreateInputLayout(desc, count);
	if (shaders.find(fxName) != shaders.end()) {
		shaders[fxName] = shader;
		return true;
	}
	else
		return false;
}

Shader * ShaderManager::GetShader(wstring fxName)
{
	if (shaders.find(fxName) != shaders.end())
		return shaders[fxName];
	else
		return nullptr;
}
