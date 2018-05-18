#include "../stdafx.h"
#include "Environment.h"

#include "Skydome.h"
#include "Skyplane.h"
#include "Landscape.h"
#include "Water.h"
#include "TerrainGrass.h"
//#include "QuadTree.h"

#include "../Render/Frustum.h"

#include "../ProceduralGenerator/TreeMaker.h"

#include "../Object/Character.h"
#include "../Object/TexturePlane.h"


Environment::Environment()
{
	//quadTree = 0;
	skydome = new Skydome();
	cloud = new Skyplane();
	landscape = new Landscape();
	lake = new Water();
	grass = new TerrainGrass();


	//quadTree = new QuadTree();
	
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

	//SAFE_DELETE(quadTree);
}

void Environment::Initialize(Frustum* frustum)
{
	landscape->Initialize();
	grass->Initialize(landscape);
	cloud->Initialize();

	this->frustum = frustum;


	CreateTree(23);


	UINT width, height;
	landscape->GetSize(width, height);


	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);
	//quadTree->Initialize(width, height);
	//quadTree->CreateTree();

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
		TreeMaker* treeTest = new TreeMaker();
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

void Environment::MakeGrassPosData(Frustum * frustum)
{
	grass->SetPosByFrustum(frustum);
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
	for (size_t i = 0; i < trees.size(); i++) {
		if(frustum->CheckSphere(landscape->treePos[i], 20))
		trees[i]->Render();
	}
		

}

void Environment::RenderTreeLeaf()
{
	for (size_t i = 0; i < trees.size(); i++) {
		if (frustum->CheckSphere(landscape->treePos[i], 20))
		trees[i]->LeafRender();
	}
}

void Environment::SetCharacter(Character  *character)
{
	character->GetPosition(characterPos);
}

void Environment::GetLandY(D3DXVECTOR3 & position)
{
	landscape->GetY(position);
}
