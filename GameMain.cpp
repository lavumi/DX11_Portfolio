#include "stdafx.h"
#include "GameMain.h"

#include "./Mesh/ObjectMesh.h"
#include "./Render\RenderTexture.h"
#include "./Render/Frustum.h"

#include "./ProceduralTexture/Grass.h"
#include "./ProceduralTexture/MosaicTile.h"
#include "./ProceduralTexture/RNDNoise.h"

#include "./Terrian/Skydome.h"
#include "./Terrian/Skyplane.h"
#include "./Terrian/Landscape.h"
#include "./Terrian/Water.h"


#include "./Object/TestCube.h"
#include "./Object/Mirror.h"
#include "./Object/OrthoWindowPlane.h"

#include "../Shader/NormalMapShader.h"
#include "../Shader/DepthShadowShader.h"
#include "../Shader/TextureShader.h"
#include "../Shader/SkydomeShader.h"
#include "../Shader/ShadowShader.h"
#include "../Shader/BlurShader.h"
#include "../Shader/MirrorShader.h"
#include "../Shader/TerrianShader.h"
#include "../Shader/ColorShader.h"
#include "../Shader/SkyplaneShader.h"



bool GameMain::landscapeWireFrame = false;

void GameMain::Initialize()
{
	depthShadowTexture = new RenderTexture(1280, 1280);
	shadowTexture = new RenderTexture();
	blurShadowTexture = new RenderTexture();


	UserInterface::Get();
	LightManager::Get();
	Shader::CreateBuffer();
	frustum = new Frustum();

	testcube = new TestCube();
	//testplane = new Mirror();
	skydome = new Skydome();
	cloud = new Skyplane();
	landscape = new Landscape();
	lake = new Water();

	grass = new Grass();
	//mosaicTile = new MosaicTile();
	//noise = new RNDNoise();
	


	shadowtestPlane = new OrthoWindowPlane();


	normalMapShader = new NormalMapShader();
	depthShadowShader = new DepthShadowShader();
	textureShader = new TextureShader();
	skydomeShader = new SkydomeShader();
	shadowShader = new ShadowShader();
	blurShader = new BlurShader();
	mirrorShader = new MirrorShader();
	terrianShader = new TerrianShader();
	colorShader = new ColorShader();
	skyplaneShader = new SkyplaneShader();





	grass->DrawTexture();
	landscape->SetTexture(grass->diffuse, nullptr, nullptr);
	
}

void GameMain::Destroy()
{
	SAFE_DELETE(skydome);
	SAFE_DELETE(landscape);
	Shader::DeleteBuffer();
	UserInterface::Delete();
}

void GameMain::Update()
{
	ControlCamera();


	LightManager::Get()->Update();;
	Camera::Get()->Update();
	Shader::bufferupdate();

	D3DXMATRIX view, projection;

	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);
	frustum->SetFrustum(Camera::screenDepth, view, projection);




	skydome->Update();
	landscape->Update();
	cloud->Update();

	
	testcube->Update();



	

	//if (Keyboard::Get()->KeyUp(VK_SPACE)) {
		landscape->changeLOD(frustum);
//	}

	if (Keyboard::Get()->KeyUp('1')) {
		landscapeWireFrame = !landscapeWireFrame;
	}

}

void GameMain::PreRender()
{
	{
		depthShadowTexture->SetTarget();
		depthShadowTexture->Clear(0, 0, 0, 1);

		//testplane->Render();
		//depthShadowShader->Render(testplane->indexCount, testplane->world);


		testcube->Render();
		depthShadowShader->Render(testcube->indexCount, testcube->world[0]);
		depthShadowShader->Render(testcube->indexCount, testcube->world[1]);
		depthShadowShader->Render(testcube->indexCount, testcube->world[2]);

		landscape->Render();
		depthShadowShader->Render(landscape->getIndexCount(), landscape->getWorld());
	}
	{
		
		shadowTexture->SetTarget();
		shadowTexture->Clear();

		//testplane->Render();
		//shadowShader->Render(testplane->indexCount, testplane->world, *depthShadowTexture->GetShadowResourceView());
		
		testcube->Render();
		shadowShader->Render(testcube->indexCount, testcube->world[0], *depthShadowTexture->GetShadowResourceView());
		shadowShader->Render(testcube->indexCount, testcube->world[1], *depthShadowTexture->GetShadowResourceView());
		shadowShader->Render(testcube->indexCount, testcube->world[2], *depthShadowTexture->GetShadowResourceView());
		
		
		landscape->Render();
		shadowShader->Render(landscape->getIndexCount(), landscape->getWorld(), *depthShadowTexture->GetShadowResourceView());
	
	}
	{
		blurShadowTexture->SetTarget(true);
		blurShadowTexture->Clear(0,0,1,1);
		
		shadowtestPlane->Render();
		blurShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, *shadowTexture->GetShadowResourceView());
	}
	D3D::Get()->SetDefaultRenderView();
	
}

void GameMain::Render()
{

	D3D::Get()->SetDepthStencilOffState();
	Rasterizer::Get()->SetOffCullMode();
	{
		skydome->Render();
		skydomeShader->Render(skydome->getIndexCount(), skydome->getWorld());

		Blender::Get()->SetBlendAdd();
		cloud->Render();
		skyplaneShader->Render(cloud->getIndexCount(), cloud->getWorld(), cloud->getDiffuseMap(), cloud->getPerlinMap());
		Blender::Get()->SetOff();
	}
	D3D::Get()->SetDepthStencilOnState();
	Rasterizer::Get()->SetOnCullMode();


	
	testcube->Render();
	normalMapShader->Render(testcube->indexCount, testcube->world[0], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	normalMapShader->Render(testcube->indexCount, testcube->world[1], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	normalMapShader->Render(testcube->indexCount, testcube->world[2], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());

	
	if(landscapeWireFrame)
		Rasterizer::Get()->SetWireframe();


	landscape->Render();
	terrianShader->Render(landscape->getIndexCount(), landscape->getWorld(), landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView());
	Rasterizer::Get()->SetSolid();

	//testplane->Render();
	//simpleShader->Render(testplane->indexCount, testplane->world, nullptr);



	//MIRROR RENDER
	{
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_STENCIL,0, 0);
		D3D::Get()->SetDepthStencilMirrorPreState();
	
		Blender::Get()->SetLinear();
		lake->Render();
		colorShader->Render(lake->indexCount, lake->world, D3DXCOLOR(0.2f,0.5f,1,0.5f));



		D3D::Get()->SetDepthStencilMirrorState();
		
		

	
		//Rasterizer::Get()->SetOffCullMode();
		//{
		//	skydome->Render();
		//	skydomeShader->Render(skydome->indexCount, skydome->world);
		//}
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_DEPTH, 1, 0);
		
		
		//Blender::Get()->SetBlendAdd();
		
		cloud->Render();
		//mirrorShader->Render(cloud->indexCount, skydome->world);

		Blender::Get()->SetOff();



		Rasterizer::Get()->SetFrontCullMode();
		testcube->Render();
		mirrorShader->Render(testcube->indexCount, testcube->world[0], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		mirrorShader->Render(testcube->indexCount, testcube->world[1], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		mirrorShader->Render(testcube->indexCount, testcube->world[2], testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	
	
		
		//landscape->Render();
		//mirrorShader->Render(landscape->getIndexCount(), landscape->getWorld(), landscape->getDiffuseMap(), landscape->getNormalMap(), nullptr, *blurShadowTexture->GetShadowResourceView());

		Blender::Get()->SetOff();
		Rasterizer::Get()->SetOnCullMode();
	
	
	
		
	
	}

	

	D3D::Get()->SetDepthStencilOnState();
}

void GameMain::ControlCamera()
{
	if (Keyboard::Get()->KeyPress('W'))
		Camera::Get()->MoveForward();
	else if (Keyboard::Get()->KeyPress('S'))
		Camera::Get()->MoveBackward();

	if (Keyboard::Get()->KeyPress('A'))
		Camera::Get()->MoveLeft();
	else if (Keyboard::Get()->KeyPress('D'))
		Camera::Get()->MoveRight();

	if (Keyboard::Get()->KeyPress('E'))
		Camera::Get()->MoveUp();
	else if (Keyboard::Get()->KeyPress('Q'))
		Camera::Get()->MoveDown();

	if (Mouse::Get()->ButtonPress(1))
	{
		D3DXVECTOR3 move = Mouse::Get()->GetMoveValue();

		Camera::Get()->Rotate(D3DXVECTOR2(move.y, move.x));
	}
}

