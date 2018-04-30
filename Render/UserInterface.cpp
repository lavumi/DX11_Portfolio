#include "../stdafx.h"
#include "UserInterface.h"

UserInterface* UserInterface::instance = NULL;
TwBar* UserInterface::bar = NULL;

UserInterface * UserInterface::Get()
{
	if (instance == NULL)
		instance = new UserInterface();

	return instance;
}

void UserInterface::Delete()
{
	SAFE_DELETE(instance);
}

void UserInterface::AddSun(D3DXVECTOR3 * lightDir, float* base)
{
	//TwAddVarRW(bar, "Light Direction", TW_TYPE_DIR3F, lightDir, "group='Light'");
	TwAddVarRW(bar, "Speed", TW_TYPE_FLOAT, base, "min=0 max=1 step=0.1 group='Light'");
}

void UserInterface::AddMaterial(string groupName, D3DXCOLOR * ambient, D3DXCOLOR * diffuse)
{
	//string option = "group='" + groupName + "'";
	//
	//TwAddVarRW(bar, "Ambient", TW_TYPE_COLOR3F, ambient, option.c_str());
	//TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, diffuse, option.c_str());
}

void UserInterface::AddSkydome(D3DXCOLOR * apex, D3DXCOLOR * center)
{
	TwAddVarRW(bar, "Apex", TW_TYPE_COLOR3F, apex, "group='Sky'");
	TwAddVarRW(bar, "Center", TW_TYPE_COLOR3F, center, "group='Sky'");
}

void UserInterface::AddSkyplane(float* scale, float* speed)
{
	TwAddVarRW(bar, "speed", TW_TYPE_FLOAT, speed, "min=0 max=1 step=0.0001 group='Cloud'");
	TwAddVarRW(bar, "scale", TW_TYPE_FLOAT, scale, "min=0 max=1 step=0.1 group='Cloud'");

}

void UserInterface::AddViewValue(float * value, float* value2)
{
	TwAddVarRO(bar, "CharPosx", TW_TYPE_FLOAT, value, "group='Charac'");
	TwAddVarRO(bar, "CharPosz", TW_TYPE_FLOAT, value2, "group='Charac'");
}

void UserInterface::AddWave(float * translation, float * size, float* angle)
{
	TwAddVarRW(bar, "translation", TW_TYPE_FLOAT, translation, "min=0 max=1 step=0.001 group='Water'");
	TwAddVarRW(bar, "size", TW_TYPE_FLOAT, size, "min=0 max=50 group='Water'");
	TwAddVarRW(bar, "angle", TW_TYPE_FLOAT, angle, "min=0 max=360 step=5 group='Water'");
}

void UserInterface::AddBrushMenu(int * type, int * size, float * type2, int* imagenum)
{
	TwAddVarRW(bar, "Brush Type", TW_TYPE_INT32, type, "min=0 max=1 step=1 group='Brush'");
	TwAddVarRW(bar, "Flat Height", TW_TYPE_FLOAT, type2, "min=0 max=40 group='Brush'");
	TwAddVarRW(bar, "Size", TW_TYPE_INT32, size, "min=0 max=20 group='Brush'");
	TwAddVarRW(bar, "Splating Image", TW_TYPE_INT32, imagenum, "min=1 max=4 group='Brush'");
}

void UserInterface::AddGrassCreationMenu(
	int * count,  float * banding, float * banding2, int * rotation,
	int* width , float * minSize, float * maxSize, 
	D3DXCOLOR * color1, D3DXCOLOR * color2)
{
	TwAddVarRW(bar, "Count", TW_TYPE_INT32, count, "min=0 step=10 group='Grass'");
	TwAddVarRW(bar, "Width", TW_TYPE_INT32, width, "min=1 group='Grass'");
	TwAddVarRW(bar, "Banding", TW_TYPE_FLOAT, banding, "min=0 max=0.5 step=0.1 group='Grass'");
	TwAddVarRW(bar, "Banding2", TW_TYPE_FLOAT, banding2, "min=0 max=1 step=0.01 group='Grass'");
	TwAddVarRW(bar, "Rotation RND Max", TW_TYPE_INT32, rotation, "min=0 max=360 step=1 group='Grass'");
	TwAddVarRW(bar, "Size RND Min", TW_TYPE_FLOAT, minSize, "min=0 step=0.01 group='Grass'");
	TwAddVarRW(bar, "Size RND Max", TW_TYPE_FLOAT, maxSize, "min=0 step=0.01 group='Grass'");
	TwAddVarRW(bar, "Color1", TW_TYPE_COLOR3F, color1, "group='Grass'");
	TwAddVarRW(bar, "Color2", TW_TYPE_COLOR3F, color2, "group='Grass'");
}

void UserInterface::AddParallexBias(float * biasX, float * biasY)
{
	TwAddVarRW(bar, "DepthBias", TW_TYPE_FLOAT, biasX, "min=0 max=0.1 step=0.01 group='Parallex'");
	TwAddVarRW(bar, "num_layers", TW_TYPE_FLOAT, biasY, "min=1 max=32 step=1 group='Parallex'");
	//TwAddVarRW(bar, "rotation", TW_TYPE_FLOAT, speed, "min=0 max=0.1 step=0.01 group='Parallex'");
}

void UserInterface::AddMaterialData(D3DXCOLOR *ambient, D3DXCOLOR *diffuse, D3DXCOLOR *specular,  float *shininess, float* drawTexture)
{
	TwAddVarRW(bar, "Texture", TW_TYPE_FLOAT, drawTexture, "min=0 max=1 step=1 group='Material'");
	TwAddVarRW(bar, "ambient", TW_TYPE_COLOR3F, ambient, "group='Material'");
	TwAddVarRW(bar, "diffuse", TW_TYPE_COLOR3F, diffuse, "group='Material'");
	TwAddVarRW(bar, "specular", TW_TYPE_COLOR3F, specular, "group='Material'");
	TwAddVarRW(bar, "shininess", TW_TYPE_FLOAT, shininess, "min=1 max=90 step=5 group='Material'");
}

void UserInterface::AddRainParameter(float * data)
{
	TwAddVarRW(bar, "RainData", TW_TYPE_FLOAT, data, "min=1 max=50 step=1 group='Rain'");
}

void UserInterface::AddPattern(float * width, float * count, D3DXCOLOR* color1, D3DXCOLOR* color2)
{
	TwAddVarRW(bar, "PatternCount", TW_TYPE_FLOAT, count, "min=1 max=14 step=1 group='Pattern'");
	TwAddVarRW(bar, "PatternWidth", TW_TYPE_FLOAT, width, "min=1 max=14 step=1 group='Pattern'");
	TwAddVarRW(bar, "Color1", TW_TYPE_COLOR3F, color1, "group='Pattern'");
	TwAddVarRW(bar, "Color2", TW_TYPE_COLOR3F, color2, "group='Pattern'");

}


UserInterface::UserInterface()
	: fillMode(0)
{

	
	bar = TweakBar::Get()->GetBar();
	return;
	function<void(void *)> f = bind
	(
		&UserInterface::FillMode
		, this
		, placeholders::_1
	);
	TweakBar::Add(&fillMode, f);

	TwAddButton(bar, "Fill Mode", Notified, &fillMode, "");
}

UserInterface::~UserInterface()
{
}

void UserInterface::FillMode(void * value)
{
	UINT* mode = (UINT *)value;

	if (*mode == 0)
	{
		*mode = 1;
		//Rasterizer::Get()->SetWireframe();
	}
	else if(*mode == 1)
	{
		*mode = 0;
		//Rasterizer::Get()->SetSolid();
	}
}
