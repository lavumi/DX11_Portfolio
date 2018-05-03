#include "stdafx.h"
#include "GameMain.h"

#include "../Environment/Environment.h"

#include "./Object/TestCube.h"
#include "../Object/Character.h"

#include "../Render/RenderingManager.h"
#include "../System/TextManager.h"

void GameMain::Initialize()
{
	renderManager = new RenderingManager();
	txtManager = new TextManager();


	char frametxt[5];
	int frame = Frames::Get()->getFrame();
	_itoa_s(frame, frametxt, 5);


	char text[16];
	strcpy_s(text, "FPS : ");
	strcat_s(text, frametxt);
	txtManager->AddText(D3DXVECTOR2(10, 10), text);

	Camera::Get();
	UserInterface::Get();






	environment = new Environment();

	player = new Character();
	testcube = new TestCube();



	renderManager->Initianlize(txtManager, environment);
	renderManager->AddCharacter(player);
	renderManager->Test(testcube);



	environment->Initialize();

	player->Initialize(environment);
	player->SetFallowCamera();

}

void GameMain::Destroy()
{
	SAFE_DELETE(renderManager);
	
	
	UserInterface::Delete();

	SAFE_DELETE(testcube);
	SAFE_DELETE(environment);
	SAFE_DELETE(player);
}

void GameMain::Update()
{
	environment->Update();
	testcube->Update();
	player->Update();


	if (Keyboard::Get()->KeyUp(VK_SPACE)) {

	}

	ControlCamera();
	Camera::Get()->Update();

	renderManager->Update();




	char frametxt[5];
	int frame = (int)Frames::Get()->getFrame();
	_itoa_s(frame, frametxt, 10);

	

	char text[16];
	strcpy_s(text, "FPS : ");
	strcat_s(text, frametxt);
	txtManager->ChangeText(0, text);
}



void GameMain::Render()
{
	renderManager->Render();
	/*
	//WATER REFLECTION OLD ver.
	{

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



	}
	*/
}


void GameMain::ControlCamera()
{
	if (Camera::Get()->Fallowing()) {

	}
	else {
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
	}


	if (Mouse::Get()->ButtonPress(1))
	{
		D3DXVECTOR3 move = Mouse::Get()->GetMoveValue();

		Camera::Get()->Rotate(D3DXVECTOR2(move.y, move.x));
	}
}


