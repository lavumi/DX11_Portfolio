#include "stdafx.h"
#include "GameMain.h"

#include "./Mesh/ObjectMesh.h"
#include "./Render\RenderTexture.h"
#include "./Render/Frustum.h"

#include "./ProceduralTexture/GrassTexture.h"
#include "./ProceduralTexture/MosaicTile.h"
#include "./ProceduralTexture/PerlinNoise.h"

#include "./Terrain/Skydome.h"
#include "./Terrain/Skyplane.h"
#include "./Terrain/Landscape.h"
#include "./Terrain/Water.h"
#include "./Terrain/TerrainGrass.h"
#include "./Terrain/Cloud.h"


#include "./Object/TestCube.h"
#include "./Object/Mirror.h"
#include "./Object/OrthoWindowPlane.h"

#include "../Shader/NormalMapShader.h"
#include "../Shader/DepthShadowShader.h"
#include "../Shader/TextureShader.h"
#include "../Shader/SkydomeShader.h"
#include "../Shader/ShadowShader.h"
#include "../Shader/BlurShader.h"
#include "../Shader/TerrainShader.h"
#include "../Shader/ColorShader.h"
#include "../Shader/WaterShader.h"
#include "../Shader/SkyplaneShader.h"
#include "../Shader/InstanceTextureShader.h"
#include "../Shader/RainShader.h"



bool GameMain::landscapeWireFrame = false;

void GameMain::Initialize()
{
	depthShadowTexture			= new RenderTexture(1280, 1280);
	shadowTexture				= new RenderTexture();
	blurShadowTexture			= new RenderTexture();
	lakeReflectionTexture		 = new RenderTexture();
	lakeRefractionTexture		= new RenderTexture();

	UserInterface::Get();
	LightManager::Get();
	frustum			= new Frustum();
	testcube		= new TestCube();
	testplane		= new Mirror();
	skydome			 = new Skydome();
	cloud			= new Skyplane();
	landscape		 = new Landscape();
	lake			 = new Water();
	grass			= new TerrainGrass();
	volumeCloud = new Cloud();

	grassTexture = new GrassTexture();
	//mosaicTile = new MosaicTile();
	noise = new PerlinNoise();



	shadowtestPlane			= new OrthoWindowPlane();

	normalMapShader		 = new NormalMapShader();
	depthShadowShader	 = new DepthShadowShader();
	textureShader		= new TextureShader();
	skydomeShader		 = new SkydomeShader();
	shadowShader		 = new ShadowShader();
	blurShader			= new BlurShader();
	terrainShader		 = new TerrainShader();
	colorShader			= new ColorShader();
	skyplaneShader		 = new SkyplaneShader();
	waterShader			 = new WaterShader();
	instanceShader		 = new InstanceTextureShader();
	rainShader				= new RainShader();


	
	landscape->Initialize();
	grass->Initialize(landscape);
	cloud->Initialize();
	noise->MakePerlinNoise();
	volumeCloud->Initialize();

	grassTexture->DrawTexture();
	landscape->SetTexture(grassTexture->diffuse, nullptr, nullptr);
	grass->setDiffuseMap(grassTexture->getGrassTexture());
	LodOff = false;
}

void GameMain::Destroy()
{
	SAFE_DELETE(skydome);
	SAFE_DELETE(landscape);
	UserInterface::Delete();
	
	SAFE_DELETE(depthShadowTexture);
	SAFE_DELETE(shadowTexture);
	SAFE_DELETE(blurShadowTexture);
	SAFE_DELETE(lakeReflectionTexture);
	SAFE_DELETE(lakeRefractionTexture);

	SAFE_DELETE(frustum);

	SAFE_DELETE(testcube);
	SAFE_DELETE(testplane);
	SAFE_DELETE(skydome);
	SAFE_DELETE(cloud);
	SAFE_DELETE(landscape);
	SAFE_DELETE(lake);
	SAFE_DELETE(grass);
	SAFE_DELETE(volumeCloud);

	SAFE_DELETE(grassTexture);
	SAFE_DELETE(noise);

	SAFE_DELETE(shadowtestPlane);

	SAFE_DELETE(normalMapShader);
	SAFE_DELETE(depthShadowShader);
	SAFE_DELETE(textureShader);
	SAFE_DELETE(skydomeShader);
	SAFE_DELETE(shadowShader);
	SAFE_DELETE(blurShader);
	SAFE_DELETE(terrainShader);
	SAFE_DELETE(colorShader);
	SAFE_DELETE(skyplaneShader);
	SAFE_DELETE(waterShader);
	SAFE_DELETE(instanceShader);
	SAFE_DELETE(rainShader);

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
	volumeCloud->Update();

	testcube->Update();





	if (Keyboard::Get()->KeyUp(VK_SPACE)) {
		//depthShadowTexture->SaveTexture(L"depthShadow.png");
		//shadowTexture->SaveTexture(L"shadow.png");
		//blurShadowTexture->SaveTexture(L"blur.png");
		//lakeRefractionTexture->SaveTexture(L"Mirror.png");
		//LodOff = !LodOff;
		//noise->MakePerlinNoise();
		D3D::Get()->TestMultiTexture(0);
	}
	if (!LodOff)		landscape->changeLOD(frustum);


	if (Keyboard::Get()->KeyUp('P')) {
		landscapeWireFrame = !landscapeWireFrame;
	}

}

void GameMain::PreRender()
{

	D3DXMATRIX view, projection;
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	//광원의 시점에서 물체까지의 거리(depth)기록
	{

		depthShadowTexture->SetTarget();
		depthShadowTexture->Clear(0, 0, 0, 1);

		//testplane->Render();
		//depthShadowShader->Render(testplane->indexCount, testplane->world, view, projection);


		testcube->Render();
		for (int i = 0; i < 6; i++) {
			depthShadowShader->Render(testcube->indexCount, testcube->world[i], view, projection);
		}

		landscape->Render();
		depthShadowShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection);


	}
	//기록된 depth를 바탕으로 그림자 연산
	//TODO 여기서 텍스쳐를 여러개 쓰고 각 텍스쳐별로 bias를 따로 설정해주어야 한다.
	{
		shadowTexture->SetTarget();
		shadowTexture->Clear();

		Camera::Get()->GetView(&view);
		D3D::Get()->GetProjection(&projection);

		//testplane->Render();
		//shadowShader->Render(testplane->indexCount, testplane->world,  view, projection,*depthShadowTexture->GetShadowResourceView());

		testcube->Render();
		for (int i = 0; i < 6; i++) {
			shadowShader->Render(testcube->indexCount, testcube->world[i], view, projection, *depthShadowTexture->GetShadowResourceView());
		}


		landscape->Render();
		shadowShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, *depthShadowTexture->GetShadowResourceView());
	}
	//연산된 그림자를 blur 처리
	{
		blurShadowTexture->SetTarget(true);
		blurShadowTexture->Clear(1, 0, 0, 1);


		Camera::Get()->GetDefaultView(&view);
		D3D::Get()->GetOrthoProjection(&projection);

		shadowtestPlane->Render();
		blurShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, view, projection, *shadowTexture->GetShadowResourceView());
	}

	//호수의 반사평면 그리기
	{
		lakeReflectionTexture->SetTarget();
		lakeReflectionTexture->Clear();


		//반사된 세계는 현실 세계에 대해 뒤집혀있으므로 컬모드가 반대다
		Rasterizer::Get()->SetFrontCullMode();

		D3DXPLANE waterPlane = lake->getwaterPlane();
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
		

			D3DXVECTOR3 lightDir;
			LightManager::Get()->GetLightDirection(&lightDir);


			skydome->Render();
			skydomeShader->Render(skydome->getIndexCount(), world, view, projection, lightDir);
			D3D::Get()->SetBlender(D3D::BL_state::Add);
			cloud->Render();
			skyplaneShader->Render(cloud->getIndexCount(), world, view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());
			D3D::Get()->SetBlender(D3D::BL_state::Off);
		}
		D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
		Rasterizer::Get()->SetFrontCullMode();

		testcube->Render();
		for (int i = 0; i < 6; i++) {
			normalMapShader->Render(testcube->indexCount, testcube->world[i], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		}

		if (landscapeWireFrame)
			Rasterizer::Get()->SetWireframe();

		landscape->Render();
		terrainShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView(), waterPlane);
		Rasterizer::Get()->SetOnCullMode();
	}

	//호수의 굴절 평면 그리기 
	//이건 굴절이 아니라 그냥 투시된 형태
	//TODO : 굴절을 표현하기 위해서 어떻게 해야할까?
	{
		lakeRefractionTexture->SetTarget();
		lakeRefractionTexture->Clear();

		//뷰, 프로젝션 받기
		Camera::Get()->GetView(&view);
		D3D::Get()->GetProjection(&projection);

		//testcube->Render();
		//for (int i = 0; i < 6; i++) {
		//	normalMapShader->Render(testcube->indexCount, testcube->world[i], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		//}


		//호수 아랫면을 랜더링하게 클립면을 바꿔준다
		D3DXPLANE clipPlane = lake->getwaterPlane();
		clipPlane *= -1;


		if (landscapeWireFrame)
			Rasterizer::Get()->SetWireframe();
		landscape->Render();
		terrainShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView(), clipPlane);
		Rasterizer::Get()->SetSolid();
	}
}

void GameMain::Render()
{
	//D3D::Get()->SetBlender_Off();
	//grassTexture->Render();
	//return;
	D3DXMATRIX world, view, projection;




	//Camera::Get()->GetDefaultView(&view);
	//D3D::Get()->GetOrthoProjection(&projection);
	//
	//
	//shadowtestPlane->Render();
	//textureShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, view, projection, *noise->GetPerlinNoise());
	//
	//return;

	//return;

	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);

	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
	Rasterizer::Get()->SetOffCullMode();
	{

		D3DXVECTOR3 lightDir;

		LightManager::Get()->GetLightDirection(&lightDir);

		skydome->Render();
		skydomeShader->Render(skydome->getIndexCount(), skydome->getWorld(), view, projection, lightDir);


		//volumeCloud->Render();
		//colorShader->Render(volumeCloud->getIndexCount(), volumeCloud->getWorld(), view, projection, D3DXCOLOR(1, 1, 0, 1));
		D3D::Get()->SetBlender(D3D::BL_state::Add);
		cloud->Render();
		skyplaneShader->Render(cloud->getIndexCount(), cloud->getWorld(), view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());
		D3D::Get()->SetBlender(D3D::BL_state::Off);
	}
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
	Rasterizer::Get()->SetOnCullMode();





	testcube->Render();
	for (int i = 0; i < 6; i++) {
		normalMapShader->Render(testcube->indexCount, testcube->world[i], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
	}

	if (landscapeWireFrame)
		Rasterizer::Get()->SetWireframe();


	landscape->Render();
	terrainShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView(), lake->getwaterPlane());
	Rasterizer::Get()->SetSolid();



	lake->Render();
	waterShader->Render(lake->indexCount, lake->world, view, projection, 
		lake->getNormalTexture(), cloud->getPerlinMap(), 
		*lakeReflectionTexture->GetShadowResourceView(), 
		*lakeRefractionTexture->GetShadowResourceView());



	//Rain Rendering
	Camera::Get()->GetDefaultView(&view);
	D3D::Get()->GetOrthoProjection(&projection);


	shadowtestPlane->Render();
	// 


	ID3D11Texture2D* depthTexture;

	depthTexture = D3D::Get()->GetBackBufferSubRenderTexture2D(0);
	ID3D11ShaderResourceView* depthSRV;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(depthTexture, &viewDesc, &depthSRV);
	assert(SUCCEEDED(hr));

	D3D::Get()->SetBlender(D3D::BL_state::Off);
	textureShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, view, projection, depthSRV);

	//hr = D3DX11SaveTextureToFile(
	//	D3D::GetDeviceContext(),
	//	depthTexture,
	//	D3DX11_IFF_PNG,
	//	L"MultiRenderTarget_1.png"
	//);
	assert(SUCCEEDED(hr));


	SAFE_RELEASE(depthSRV);
	//rainShader->Render(shadowtestPlane->indexCount, shadowtestPlane->world, view, projection, *D3D::Get()->GetBackBufferSubRenderTexture(0), D3DXCOLOR(0, 1, 1, 1));


	//testing test


	//D3D::Get()->SetBlender_alphaCoverage();
	//Rasterizer::Get()->SetOffCullMode();
	//grass->Render();
	//instanceShader->Render(grass->getIndexCount(), grass->getInstanceCount(), view, projection, grass->getDiffuseMap());
	//Rasterizer::Get()->SetOnCullMode();


	//WATER REFLECTION OLD ver.
	{
		/*
		//스텐실 버퍼 삭제
		//1.스텐실 버퍼를 초기화 한 후 그 위에다 거울면을 그린다(깊이 버퍼는 남겨 있기에 적절히 z버퍼로 가려짐)
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_STENCIL,0, 0);

		//그려야 할 곳(물)을 스텐실 버퍼에 기록
		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorPlaneRenderState);
		D3D::Get()->SetBlender_Linear();
		lake->Render();
		//colorShader->Render(lake->indexCount, lake->world, view, projection, D3DXCOLOR(0, 1, 0, 1));
		waterShader->Render(lake->indexCount, lake->world, view, projection, nullptr, lake->getNormalTexture(), cloud->getPerlinMap());
		//호수 모양만 스텐실 버퍼가 기록되어있음


		//깊이버퍼를 삭제하여 다시 호수 위에 그릴수 있게 만듬
		D3D::Get()->ClearDepthStencil(D3D11_CLEAR_DEPTH, 1, 0);


		//반사된 세계는 현실 세계에 대해 뒤집혀있으므로 컬모드가 반대다
		Rasterizer::Get()->SetFrontCullMode();


		//반사된 뷰, 프로젝션 받기
		Camera::Get()->GetMirrorView(&view);
		D3D::Get()->GetProjection(&projection);


		//호수면에만 그리도록 뎁스 스텐실 변경해준다
		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorSkyplaneState);



		//스카이뷰를 그릴때 카메라 위치도 다시 셋팅해줘야 한다.
		D3DXMATRIX world;
		D3DXVECTOR3 camPos;
		Camera::Get()->GetPosition(&camPos);

		camPos.y = -camPos.y + (lake->GetWaterHeigh() * 2.0f);

		D3DXMatrixTranslation(&world, camPos.x, camPos.y, camPos.z);


		cloud->Render();
		skyplaneShader->Render(cloud->getIndexCount(), world, view, projection, cloud->getDiffuseMap(), cloud->getPerlinMap());



		//물과의 색상혼합을 해줌
		D3D::Get()->SetBlender_AddBlend();

		D3D::Get()->SetDepthStencilState(D3D::DS_state::mirrorObjectRenderState);
		testcube->Render();
		for (int i = 0; i < 6; i++) {
		normalMapShader->Render(testcube->indexCount, testcube->world[i], view, projection, testcube->diffuseMap, testcube->normalMap, testcube->heightMap, *blurShadowTexture->GetShadowResourceView());
		}


		landscape->Render();
		terrainShader->Render(landscape->getIndexCount(), landscape->getWorld(), view, projection, landscape->getDiffuseMap(), landscape->getNormalMap(), *blurShadowTexture->GetShadowResourceView());


		*/
	}

	D3D::Get()->SetBlender(D3D::BL_state::Off);
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

