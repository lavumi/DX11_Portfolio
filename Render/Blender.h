#pragma once


class Blender
{
public:
	static Blender* Get();
	static void Delete();

	void SetLinear();
	void SetMaxBlender();
	void SetOff();
	void SetNone();
	void SetBlendAdd();
	void SetBlandFacter(float factor);
private:
	static Blender* instance;

	Blender();
	~Blender();

	ID3D11BlendState** d3dBlendState;
};