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
#include "../Shader/WaterShader.h"
#include "../Shader/SkyplaneShader.h"



bool GameMain::landscapeWireFrame = false;

void GameMain::Initialize()
{
	depthShadowTexture = new RenderTexture(1280, 1280);
	shadowTexture = new RenderTexture();
	blurShadowTexture = new RenderTexture();
	lakeReflectionTexture = new RenderTexture();


	UserInterface::Get();
	LightManager::Get();
	frustum = new Frustum();

	testcube = new TestCube();
	testplane = new Mirror();
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
	waterShader = new WaterShader();





	grass->DrawTexture();
	landscape->SetTexture(grass->diffuse, nullptr, nullptr);
	
}

void GameMain::Destroy()
{
	SAFE_DELETE(skydome);
	SAFE_DELETE(landscape);
	UserInterface::Delete();
}

void GameMain::Update()
{
	ControlCamera();


	LightManager::Get()->Update();;
//	Camera::Get()->Update();

	D3DXMATRIX view, projection;

	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);
	frustum->SetFrustum(Camera::screenDepth, view, projection);




	skydome->Update();
	landscape->Update();
	cloud->Update();

	
	testcube->Update();



	

	if (Keyboard::Get()->KeyUp(VK_SPACE)) {
		//depthShadowTexture->SaveTexture(L"depthShadow.png");
		//shadowTexture->SaveTexture(L"shadow.png");
		//blurShadowTexture->SaveTexture(L"blur.png");
		lakeReflectionTexture->SaveTexture(L"Mirror.png");
		}
		landscape->changeLOD(frustum);


	if (Keyboard::Get()->KeyUp('1')) {
		landscapeWireFrame = !landscapeWireFrame;
	}

}

void GameMain::PreRender()
{

	D3DXMATRIX view, projection;
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	{

		depthShadowTexture->SetTarget();
		depthShadowTexture->Clear(0, 0, 0, 1);

		testplane->Render();
		depthShadowShader->Render(testplane->indexCount, testplane->world, view, projection);


		testcube->Render();
		depthShadowShader->Render(testcube->indexCount, testcube->world[0],view, projection);
		depthShadowShader->Render(testcube->indexCount, testcube->world[1],view, projection);
		depthShadowShader->Render(testcube->indexCount, testcube->world[2],view, projection);

		landscape->Render();
		depthShadowShader->Render(landscape->getIndexCount(), landscape->getWorld(),view, projection);


	}


	{
		shadowTexture->SetTarget();
		shadowTexture->Clear();

		Camera::Get()->GetView(&view);
		D3D::Get()->GetProjection(&projection);

		//testplane->Render();
		//shadowShader->Render(testplane->indexCount, testplane->world,  view, projection,*depthShadowTexture->GetShadowResourceView());
		
		testcube->Render();
		shadowShader->Render(testcube->indexCount, testcube->world[0], view, projection, *depthShadowTexture->GetShadowResourceView());
		shadowShader->Render(testcube->indexCount, testcube->world[1], view, projection, *depthShadowTexture->GetShadowResourceView());
		shadowShader->Render(testcube->indexCount, testcube->world[2], view, projection, *depthShadowTexture->GetShadowResourceView());
		
		
		landscape->Render();
		shadowShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, *depthShadowTexture->GetShadowResourceView());
	}

	{
		blurShadowTexture->SetTarget(true);
		blurShadowTexture->Clear(0,0,0,1);


		Camera::Get()->GetDefaultView(&view);
		D3D::Get()->GetOrthoProjection(&projection);
		
		shadowtestPlane->Render();
		blurShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, view, projection, *shadowTexture->GetShadowResourceView());
	}
	
	{
		lakeReflectionTexture->SetTarget();
		lakeReflectionTexture->Clear();


		//반사된 세계는 현실 세계에 대해 뒤집혀있으므로 컬모드가 반대다
		Rasterizer::Get()->SetFrontCullMode();


		//반사된 뷰, 프로젝션 받기
		Camera::Get()->GetMirrorView(&view);
		D3D::Get()->GetProjection(&projection);

	


		//스카이돔, 구름 그리기
		D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
		Rasterizer::Get()->SetOffCullMode();
		{
			//반사된 평면에 맞춰서 카메라의 위치, 스카이 플레인의 위치까지 보정해주어야 함
			D3DXMATRIX world;
			D3DXVECTOR3 camPos;
			Camera::Get()->GetPosition(&camPos);
			camPos.y = -camPos.y + (lake->GetWaterHeigh() * 2.0f);
			D3DXMatrixTranslation(&world, camPos.x, camPos.y, camPos.z);


			skydome->Render();
			skydomeShader->Render(skydome->getIndexCount(), world, view, projection);
			D3D::Get()->SetBlender_AddBlend();
			cloud->Render();
			skyplaneShader->Render(cloud->getIndexCount(), world, view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());
			D3D::Get()->SetBlender_Off();
		}
		D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
		Rasterizer::Get()->SetOnCullMode();


		testcube->Render();
		normalMapShader->Render(testcube->indexCount, testcube->world[0], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		normalMapShader->Render(testcube->indexCount, testcube->world[1], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		normalMapShader->Render(testcube->indexCount, testcube->world[2], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());


		landscape->Render();
		terrianShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView());
		Rasterizer::Get()->SetSolid();

		testplane->Render();
		colorShader->Render(testplane->indexCount, testplane->world, view, projection, D3DXCOLOR(0, 0, 0, 1));
	}

	
}

void GameMain::Render()
{
	

	D3DXMATRIX view, projection;


	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);


	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
	Rasterizer::Get()->SetOffCullMode();
	{
		skydome->Render();
		skydomeShader->Render(skydome->getIndexCount(), skydome->getWorld(),view, projection );



		D3D::Get()->SetBlender_AddBlend();
		cloud->Render();
		skyplaneShader->Render(cloud->getIndexCount(), cloud->getWorld(), view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());
		D3D::Get()->SetBlender_Off();
	}
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
	Rasterizer::Get()->SetOnCullMode();
	

	testcube->Render();
	normalMapShader->Render(testcube->indexCount, testcube->world[0],view, projection,testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	normalMapShader->Render(testcube->indexCount, testcube->world[1],view, projection,testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	normalMapShader->Render(testcube->indexCount, testcube->world[2],view, projection,testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());

	
	if(landscapeWireFrame)
		Rasterizer::Get()->SetWireframe();


	landscape->Render();
	terrianShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView());
	Rasterizer::Get()->SetSolid();


	//WATER REFLECTION
	{
		//스텐실 버퍼 삭제
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_STENCIL,0, 0);

		//그려야 할 곳(물)을 스텐실 버퍼에 기록
		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorPlaneRenderState);
		D3D::Get()->SetBlender_Linear();
		lake->Render();
		//colorShader->Render(lake->indexCount, lake->world, view, projection, D3DXCOLOR(0, 1, 0, 1));
		waterShader->Render(lake->indexCount, lake->world, view, projection, nullptr, lake->getNormalTexture(), cloud->getPerlinMap());


		//반사 예전 버전
		
		//물 위에 그리는 셋팅
	
		//깊이버퍼를 삭제하여 다시 그릴수 있게 만듬
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_DEPTH, 1, 0);


		//반사된 세계는 현실 세계에 대해 뒤집혀있으므로 컬모드가 반대다
		Rasterizer::Get()->SetFrontCullMode();


		//반사된 뷰, 프로젝션 받기
		Camera::Get()->GetMirrorView(&view);
		D3D::Get()->GetProjection(&projection);

		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorSkyplaneState);
	

		D3DXMATRIX world;
		D3DXVECTOR3 camPos;
		Camera::Get()->GetPosition(&camPos);

		camPos.y = -camPos.y + (lake->GetWaterHeigh() * 2.0f);

		D3DXMatrixTranslation(&world, camPos.x, camPos.y, camPos.z);


		cloud->Render();
		skyplaneShader->Render(cloud->getIndexCount(), world, view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());
		

		D3D::Get()->SetBlender_AddBlend();

		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorObjectRenderState);
		testcube->Render();
		normalMapShader->Render(testcube->indexCount, testcube->world[0], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		normalMapShader->Render(testcube->indexCount, testcube->world[1], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		normalMapShader->Render(testcube->indexCount, testcube->world[2], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());

		
		landscape->Render();
		terrianShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView());

		
		
	}

	D3D::Get()->SetBlender_Off();
	Rasterizer::Get()->SetOnCullMode();
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
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

