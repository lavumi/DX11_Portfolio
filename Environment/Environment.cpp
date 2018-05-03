#include "../stdafx.h"
#include "Environment.h"

#include "Skydome.h"
#include "Skyplane.h"
#include "Landscape.h"
#include "Water.h"
#include "TerrainGrass.h"

#include "../ProceduralGenerator/TreeMaker.h"

#include "../Object/Character.h"


Environment::Environment()
{
	skydome = new Skydome();
	cloud = new Skyplane();
	landscape = new Landscape();
	lake = new Water();
	grass = new TerrainGrass();
	//treeTest = new TreeMaker();
}

Environment::~Environment()
{
	DeleteTree();

	SAFE_DELETE(skydome);
	SAFE_DELETE(cloud);
	SAFE_DELETE(landscape);
	SAFE_DELETE(lake);
	SAFE_DELETE(grass);




}

void Environment::Initialize()
{
	landscape->Initialize();
	grass->Initialize(landscape);
	cloud->Initialize();

	CreateTree(12);
	//treeTest = new TreeMaker();
	//treeTest->Initialize(10, 0, 10);
}


void Environment::Update()
{
	skydome->Update();

	landscape->Update();

	cloud->Update();

	grass->Update();
	grass->SetCharacterPos(characterPos);
}

void Environment::CreateTree(int count)
{
	DeleteTree();

	for (int i = 0; i < count; i++) {
		treeTest = new TreeMaker();
		treeTest->Initialize(landscape->treePos[i]);

		trees.push_back(treeTest);
	}

}

void Environment::DeleteTree()
{
	for each (TreeMaker* singleTree in trees)
	{
		SAFE_DELETE(singleTree);
	}
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

void Environment::RenderTree()
{

	for each (TreeMaker* singleTree in trees)
	{
		singleTree->Render();
	}

//	treeTest->Render();
}

void Environment::RenderTreeLeaf()
{
	for each (TreeMaker* singleTree in trees)
	{
		singleTree->LeafRender();
	}
	//treeTest->LeafRender();
}

void Environment::SetCharacter(Character  *character)
{
	character->GetPosition(characterPos);

}

void Environment::GetLandY(D3DXVECTOR3 & position)
{
	landscape->GetY(position);
}
