#include "../stdafx.h"
#include "RenderingManager.h"
#include "RenderTexture.h"
#include "Frustum.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "LightManager.h"


#include "../ProceduralGenerator/GrassTexture.h"
#include "../ProceduralGenerator/PerlinNoise.h"
#include "../ProceduralGenerator/Convert_Normal_Height.h"


#include "../Environment/Environment.h"
#include "../Environment/Skydome.h"
#include "../Environment/Skyplane.h"
#include "../Environment/Landscape.h"
#include "../Environment/Water.h"
#include "../Environment/TerrainGrass.h"



#include "../Object/TestCube.h"
#include "../Object/Mirror.h"
#include "../Object/OrthoWindowPlane.h"
#include "../Object/Character.h"


#include "../Shader/ShaderManager.h"


#include "../Shader/VPBuffer.h"
#include "../Shader/LightBuffer.h"
#include "../Shader/FrustumBuffer.h"
#include "../Shader/GAbuffer.h"


#include "../System/TextManager.h"


RenderingManager::RenderingManager()
{
	shaderManager = new ShaderManager();
	sampler = new Sampler();
	rasterizer = new Rasterizer();
	lightManager = new LightManager();

	depthShadowTexture = new RenderTexture(2048, 2048);
	shadowTexture = new RenderTexture();
	lakeReflectionTexture = new RenderTexture();
	lakeRefractionTexture = new RenderTexture();
	mainRendering = new RenderTexture();
	postRendering = new RenderTexture();
	rainTexture = new RenderTexture();


	grassTexture = new GrassTexture();
	noise = new PerlinNoise();

	orthoWindow = new OrthoWindowPlane();



	

	frustumBuffer = new FrustumBuffer();
	vpBuffer = new VPBuffer();
	lightBuffer = new LightBuffer(lightManager);
	gaBuffer = new GAbuffer();

	frustum = new Frustum();

	//여기가 맞을까?
	//grass = new TerrainGrass();
}

RenderingManager::~RenderingManager()
{
	SAFE_DELETE(depthShadowTexture);
	SAFE_DELETE(shadowTexture);
	SAFE_DELETE(lakeReflectionTexture);
	SAFE_DELETE(lakeRefractionTexture);
	SAFE_DELETE(rainTexture);
	SAFE_DELETE(mainRendering);
	SAFE_DELETE(postRendering);

	SAFE_DELETE(grassTexture);
	SAFE_DELETE(noise);

	SAFE_DELETE(orthoWindow);

	SAFE_DELETE(shaderManager);

	SAFE_DELETE(frustumBuffer);
	SAFE_DELETE(vpBuffer);
	SAFE_DELETE(lightBuffer);
	SAFE_DELETE(gaBuffer);
}

void RenderingManager::Initianlize(Environment* envi)
{
	environment = envi;

	sampler->SetDefault();
	
	depthShadowTexture->IntializeShadowTexture(3);
	shadowTexture->Initialize();
	lakeReflectionTexture->Initialize();
	lakeRefractionTexture->Initialize();
	mainRendering->Initialize(3);
	rainTexture->Initialize();
	postRendering->Initialize();




	frustum->Initialize(mainRendering, lightManager);




	noise->MakePerlinNoise();

	grassTexture->DrawTexture();
	environment->SetLandTexture(grassTexture->diffuse);



	gaBuffer->SetGA(lightManager->GetGA());


	D3D::GetDeviceContext()->PSSetShaderResources(14, 1, noise->GetPerlinNoise());
}


void RenderingManager::AddCharacter(Character *character)
{

	characters.push_back(character);
	environment->SetCharacter(character);
}

void RenderingManager::Test(TestCube * testcube)
{
	this->testcube = testcube;
}

void RenderingManager::Update()
{
	frustum->Update();

	lightManager->Update();
	lightBuffer->SetBuffers();


	for (UINT i = 0; i < 3; i++) {
		cropMatrix[i] = frustum->GetCropMatrix(i);
	}
	Camera::Get()->GetView(&cropMatrix[3]);
	D3DXMatrixTranspose(&cropMatrix[3], &cropMatrix[3]);




	frustumBuffer->UpdateBuffer(cropMatrix, frustum->GetPlanes());



	frustumBuffer->SetBuffers();


	gaBuffer->SetGA(lightManager->GetGA());
	gaBuffer->SetBuffer();
	
}

void RenderingManager::Render()
{

	LightView();
	RenderShadow();
	RenderReflection();
	RenderRefraction();



	RenderMain();

	PostRender();
	
	FinalRender();
}






void RenderingManager::TreeRenderTest()
{

	D3D::Get()->BeginScene();
	D3D::Get()->SetDefaultRenderView();

	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);


	vpBuffer->SetVPMatrix(view, projection);
	D3D::Get()->SetBlender(D3D::BL_state::Off);


	assert(shaderManager->SetShader(L"TextureShader"));
	environment->RenderTree();

	
	assert(shaderManager->SetShader(L"InstanceTextureShader"));
	rasterizer->SetOffCullMode();
	{
		environment->RenderTreeLeaf();
	}
	rasterizer->SetOnCullMode();

	//assert(shaderManager->SetShader(L"NormalMapShader"));
	//testcube->Render();

}

void RenderingManager::LightView()
{
	depthShadowTexture->SetTarget();
	depthShadowTexture->Clear(0, 0, 0, 1);

	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);
	vpBuffer->SetVPMatrix(view, projection);

	assert(shaderManager->SetShader(L"LightViewShader"));
	
	//environment->RenderLandForShadow();

	testcube->Render();
	environment->RenderTree();



	assert(shaderManager->SetShader(L"InstanceTextureLightViewShader"));
	rasterizer->SetOffCullMode();
	environment->RenderTreeLeaf();
	rasterizer->SetOnCullMode();


	assert(shaderManager->SetShader(L"FBXLightViewShader"));
	for each (Character* character in characters)
	{
		character->Render();
	}


	//rasterizer->SetOffCullMode();
	//assert(shaderManager->SetShader(L"GrassLightViewShader"));
	//environment->RenderGrass();
	//rasterizer->SetOnCullMode();
}

void RenderingManager::RenderShadow()
{
	shadowTexture->SetTarget();
	shadowTexture->Clear();

	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);

	vpBuffer->SetVPMatrix(view, projection);

	D3D::GetDeviceContext()->PSSetShaderResources(13, 1, depthShadowTexture->GetShadowResourceView());
	assert(shaderManager->SetShader(L"ShadowShader"));
	environment->RenderLandForShadow();
	testcube->Render();
	environment->RenderTree();


	assert(shaderManager->SetShader(L"InstanceTextureShadowShader"));
	environment->RenderTreeLeaf();



	assert(shaderManager->SetShader(L"FBXShadowShader"));
	for (size_t i = 0; i<characters.size(); i++)
		characters[i]->Render();

	rasterizer->SetOffCullMode();
	assert(shaderManager->SetShader(L"GrassShadowShader"));
	environment->RenderGrass();
	rasterizer->SetOnCullMode();
}

void RenderingManager::RenderReflection()
{
	lakeReflectionTexture->SetTarget();
	lakeReflectionTexture->Clear();


	//반사된 세계는 현실 세계에 대해 뒤집혀있으므로 컬모드가 반대다
	rasterizer->SetFrontCullMode();

	//스카이돔, 구름 그리기
	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);
	vpBuffer->SetVPMatrix(view, projection);


	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
	rasterizer->SetOffCullMode();
	{
		assert(shaderManager->SetShader(L"SkydomeShader"));
		environment->RenderSkydome();

		D3D::Get()->SetBlender(D3D::BL_state::Add);

		assert(shaderManager->SetShader(L"SkyplaneShader"));
		environment->RenderSkyplane();

		D3D::Get()->SetBlender(D3D::BL_state::Off);
	}
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
	rasterizer->SetFrontCullMode();



	//반사된 뷰, 프로젝션 받기
	Camera::Get()->GetMirrorView(&view);
	D3D::Get()->GetProjection(&projection);
	vpBuffer->SetVPMatrix(view, projection);


	assert(shaderManager->SetShader(L"NormalMapShader"));
	D3D::GetDeviceContext()->PSSetShaderResources(13, 1, shadowTexture->GetShadowResourceView());
	testcube->Render();


	assert(shaderManager->SetShader(L"TerrainShader"));
	environment->RenderLand();

	rasterizer->SetOnCullMode();
}

void RenderingManager::RenderRefraction()
{
	lakeRefractionTexture->SetTarget();
	lakeRefractionTexture->Clear();

	//뷰, 프로젝션 받기
	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);
	vpBuffer->SetVPMatrix(view, projection);



	assert(shaderManager->SetShader(L"TerrainShader"));
	//호수 아랫면을 랜더링하게 클립면을 바꿔준다
	environment->RenderReflectedLand();

	rasterizer->SetSolid();
}

void RenderingManager::RenderMain()
{
	mainRendering->SetTarget();
	mainRendering->Clear(0.5f,0.5f,0.5f,1);



	Camera::Get()->GetView(&view);
	D3D::Get()->GetProjection(&projection);
	vpBuffer->SetVPMatrix(view, projection);




	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
	rasterizer->SetOffCullMode();
	{
		assert(shaderManager->SetShader(L"SkydomeShader"));
		environment->RenderSkydome();

		//D3D::Get()->SetBlender(D3D::BL_state::Add);
		//
		//assert(shaderManager->SetShader(L"SkyplaneShader"));
		//environment->RenderSkyplane();
		//
		//
		//D3D::Get()->SetBlender(D3D::BL_state::Off);
	}
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
	rasterizer->SetOnCullMode();



	assert(shaderManager->SetShader(L"NormalMapShader"));
	D3D::GetDeviceContext()->PSSetShaderResources(13, 1, shadowTexture->GetShadowResourceView());
	testcube->Render();

	environment->RenderTree();


	assert(shaderManager->SetShader(L"TerrainShader"));
	environment->RenderLand();
	
	//rasterizer->SetWireframe();
	//assert(shaderManager->SetShader(L"TextureShader"));
	//environment->RenderLandForShadow();
	//rasterizer->SetSolid();

	D3D::GetDeviceContext()->PSSetShaderResources(11, 1, lakeReflectionTexture->GetShadowResourceView());
	D3D::GetDeviceContext()->PSSetShaderResources(10, 1, lakeRefractionTexture->GetShadowResourceView());

	assert(shaderManager->SetShader(L"WaterShader"));
	environment->RenderWater();

	

//	assert(shaderManager->SetShader(L"NormalMapShader"));
	


	rasterizer->SetOffCullMode(); 
	{
		assert(shaderManager->SetShader(L"InstanceTextureShader"));
		environment->RenderTreeLeaf();
		assert(shaderManager->SetShader(L"GrassShader"));
		environment->RenderGrass();
	}
	rasterizer->SetOnCullMode();
	


	assert(shaderManager->SetShader(L"FBXModelShader"));
	for (size_t i = 0; i<characters.size(); i++)
		characters[i]->Render();





}

void RenderingManager::PostRender()
{
	postRendering->SetTarget();
	postRendering->Clear();

	Camera::Get()->GetDefaultView(&view);
	D3D::Get()->GetOrthoProjection(&projection);


	D3D::Get()->SetBlender(D3D::BL_state::Off);
	vpBuffer->SetVPMatrix(view, projection);


	D3D::GetDeviceContext()->PSSetShaderResources(10, 1, mainRendering->GetShadowResourceView());
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, mainRendering->GetDepthBuffer());
	assert(shaderManager->SetShader(L"DepthFogShader"));
	orthoWindow->Render();


}

void RenderingManager::FinalRender()
{
	D3D::Get()->BeginScene();
	D3D::Get()->SetDefaultRenderView();

	Camera::Get()->GetDefaultView(&view);
	D3D::Get()->GetOrthoProjection(&projection);


	D3D::Get()->SetBlender(D3D::BL_state::Off);
	vpBuffer->SetVPMatrix(view, projection);





	D3D::GetDeviceContext()->PSSetShaderResources(10, 1, postRendering->GetShadowResourceView());
	assert(shaderManager->SetShader(L"TextureShader"));
	orthoWindow->Render();


	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);
	D3D::Get()->SetBlender(D3D::BL_state::Linear);
	assert(shaderManager->SetShader(L"FontShader"));
	TextManager::Get()->Render();





	D3D::Get()->SetBlender(D3D::BL_state::Off);
	rasterizer->SetOnCullMode();
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);



}
