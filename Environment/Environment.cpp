#include "../stdafx.h"
#include "Environment.h"

#include "../Environment/Skydome.h"
#include "../Environment/Skyplane.h"
#include "../Environment/Landscape.h"
#include "../Environment/Water.h"
#include "../Environment/TerrainGrass.h"

#include "../Object/Character.h"


Environment::Environment()
{
	skydome = new Skydome();
	cloud = new Skyplane();
	landscape = new Landscape();
	lake = new Water();
	grass = new TerrainGrass();
}

Environment::~Environment()
{
}

void Environment::Initialize()
{
	landscape->Initialize();
	grass->Initialize(landscape);
	cloud->Initialize();



}


void Environment::Update()
{
	skydome->Update();
	landscape->Update();
	cloud->Update();
	

	grass->SetCharacterPos(characterPos);
}

void Environment::SetLandTexture(ID3D11ShaderResourceView * diffuse)
{
	landscape->SetTexture(diffuse);
}

void Environment::RenderSkydome()
{
	skydome->Render();
}

void Environment::RenderSkyplane()
{
	cloud->Render();
}

void Environment::RenderLand()
{
	landscape->SetPlane(lake->getwaterPlane());
	landscape->Render();
}

void Environment::RenderReflectedLand()
{
	landscape->SetPlane(lake->getwaterPlane()*-1);
	landscape->Render();
}

void Environment::RenderLandForShadow()
{
	landscape->RenderShadow();
}

void Environment::RenderWater()
{
	lake->Render();
}

void Environment::RenderGrass()
{
	grass->Render();
}

void Environment::SetCharacter(Character  *character)
{
	character->GetPosition(characterPos);

}

void Environment::GetLandY(D3DXVECTOR3 & position)
{
	landscape->GetY(position);
}
