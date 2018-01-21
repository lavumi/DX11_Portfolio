#pragma once

class UserInterface
{
public:
	static UserInterface* Get();
	static void Delete();

	static void AddSun(D3DXVECTOR3* lightDir, float* base);
	static void AddMaterial(string groupName, D3DXCOLOR* ambient, D3DXCOLOR* diffuse);
	
	static void AddSkydome(D3DXCOLOR* apex, D3DXCOLOR* center);
	static void AddSkyplane(float* scale, float* brightness);
	static void AddBrushMenu(int* type, int* size, float* type2, int*  imagenum);
	static void AddGrassCreationMenu(int * count, float * banding, float * banding2, int * rotation, int* width, float * minSize, float * maxSize, D3DXCOLOR * color1, D3DXCOLOR * color2);

	static void AddParallexBias(float* biasX, float* biasY);
	static void AddMaterialData(D3DXCOLOR *ambient, D3DXCOLOR *diffuse, D3DXCOLOR *specular, float *shininess, float* drawTexture);


	static void AddPattern(float * width, float * count, D3DXCOLOR* color1, D3DXCOLOR* color2);
private:
	static UserInterface* instance;
	static TwBar* bar;

	UserInterface();
	~UserInterface();

	void FillMode(void* value);
	UINT fillMode;
};