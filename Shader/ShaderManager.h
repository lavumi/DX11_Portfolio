#pragma once
class Shader;
class ShaderManager {
public :
	ShaderManager();
	~ShaderManager();

	bool CreateShader(wstring fxName, D3D11_INPUT_ELEMENT_DESC * desc, UINT count);
	Shader* GetShader(wstring fxName);
	
	bool SetShader(wstring fxName);
private:
	map<wstring, Shader*> shaders;
};