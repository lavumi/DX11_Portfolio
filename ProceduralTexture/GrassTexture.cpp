#include "../stdafx.h"
#include "GrassTexture.h"
#include "../Render/RenderTexture.h"
#include "../ProceduralTexture/PerlinNoise.h"


GrassTexture::GrassTexture()
	:vertexBlob(nullptr), pixelBlob(nullptr), vertexShader(nullptr), pixelShader(nullptr)
	,vertexBuffer(nullptr), indexBuffer(nullptr)
	,grassSingle(nullptr)
	, gradient(nullptr)
	, directionalWarp(nullptr)
	,grassTexture(nullptr)
	, rndDraw(nullptr) , finalResult(NULL)
	 {

	basePath = L"./Shader/FX/";

	ShaderList.push_back(L"singleGrass.fx");
	ShaderList.push_back(L"midPointGrad.fx");
	ShaderList.push_back(L"directionalWarp.fx");
	ShaderList.push_back(L"randomSRT.fx");
	ShaderList.push_back(L"rndNoise.fx");
	ShaderList.push_back(L"randomSRTInstance.fx");
	ShaderList.push_back(L"finalDraw.fx");
	



	layout = nullptr;
	CreateBuffer();

	width = 5;

	banding = 0.1f;
	banding2 = 0.333f;

	count = 400;
	rotation = 180;
	minSize = 0.3f;
	maxSize = 0.5f;
	color1 = D3DXCOLOR(0.20784f, 0.48431f, 0.26666f,	1);
	color2 = D3DXCOLOR(0.88f,	0.87f,	 0.6f,		1);




	//UserInterface::AddGrassCreationMenu(
	//	&count,  &banding, &banding2, &rotation, &width
	//	,&minSize ,&maxSize ,&color1, &color2);
}	

GrassTexture::~GrassTexture()
{

	
	SAFE_DELETE(grassTexture);
	SAFE_DELETE(grassSingle);
	SAFE_DELETE(gradient);
	SAFE_DELETE(directionalWarp);
	SAFE_DELETE(rndDraw);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_RELEASE(layout);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);

}

void GrassTexture::DrawTexture()
{
	SAFE_DELETE(grassSingle);
	SAFE_DELETE(gradient);
	SAFE_DELETE(directionalWarp);
	SAFE_DELETE(rndDraw);
	SAFE_DELETE(grassTexture);

	grassSingle = new RenderTexture(720, 720);
	gradient = new RenderTexture(720, 720);
	directionalWarp = new RenderTexture(720, 720);
	rndDraw = new RenderTexture(720, 720);
	grassTexture = new RenderTexture(256, 256);

	//기본 렌더링 셋팅
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Sampler::Get()->SetDefault();

	D3D::Get()->SetDepthStencilState(D3D::DS_state::offState);



	//D3D::Get()->SetBlender_alphaCoverage();
	//그리기
	DrawGrassSingle();
	DirectionalWarp();
	//RND_SRT();
	RND_SRT_Instancing();
	DrawGrassTexture();
	D3D::Get()->SetDepthStencilState(D3D::DS_state::onState);
	//최종 그릴 이미지
	SetFinalResult(rndDraw);
	

	//grassSingle		->SaveTexture(L"grassSingle.png");
	//gradient		->SaveTexture(L"gradient.png");
	directionalWarp ->SaveTexture(L"directionalWarp.png");
	//rndDraw			->SaveTexture(L"rndDraw.png");
	grassTexture	->SaveTexture(L"grassTexture.png");
	//SetFinalResult(directionalWarp);

	//렌더링 타겟 되돌리기
	D3D::Get()->SetDefaultRenderView();


}

void GrassTexture::Render()
{
	D3D::Get()->SetBlender_Off();
	CreateShader((int)ShaderList.size()-1);
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferFinal, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//여기까지 모델에서







	//여기부턴 쉐이더에서 하자
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	if(finalResult != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &finalResult);;

	Sampler::Get()->SetDefault();


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);


}
ID3D11ShaderResourceView * GrassTexture::getGrassTexture()
{
	return *(grassTexture->GetShadowResourceView());
}
/*
void Grass::Render2()
{
	Blender::Get()->SetLinear();
	CreateShader(ShaderList.size() - 1);






	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferFinal, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &textalpha);;

	Sampler::Get()->SetDefault();


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
}
void Grass::Render3()
{
	CreateShader(ShaderList.size() - 1);





	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferFinal, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &textalpha2);;

	Sampler::Get()->SetDefault();


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
}
*/

void GrassTexture::DrawGrassSingle()
{

	grassSingle->SetTarget();
	grassSingle->Clear(0,0,0,0);


	struct remp {
		float widthd;
		D3DXVECTOR3 padding;
	} tempee;
	
	tempee.widthd = (float)width;


	ID3D11Buffer* buffer;
	
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(float)*4;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = &tempee;
	
	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &buffer);
	assert(SUCCEEDED(hr));
	

	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &buffer);
	

	CreateShader(0);
	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);



	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
	SAFE_RELEASE(buffer);
}
void GrassTexture::DirectionalWarp()
{

	{
		gradient->SetTarget();
		gradient->Clear(0, 0, 0, 0);

		struct remp {
			float banding;
			float banding2;
			D3DXVECTOR2 padding;
		} tempee;

		tempee.banding = banding;
		tempee.banding2 = banding2;

		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(float) * 4;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;


		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = &tempee;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &buffer);
		assert(SUCCEEDED(hr));

		D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &buffer);




		CreateShader(1);
		D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
		D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


		D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

		SAFE_RELEASE(buffer);
	}






	directionalWarp->SetTarget();
	directionalWarp->Clear(0, 0, 0, 0);


	CreateShader(2);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, grassSingle->GetShadowResourceView());;
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, gradient->GetShadowResourceView());;




	D3D::Get()->SetBlender_MaxBlend();
	
	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);


}
void GrassTexture::RND_SRT()
{
	rndDraw->SetTarget();
	rndDraw->Clear(0, 0, 0, 1);

	if (maxSize <= minSize + 0.01f) {
		maxSize += 0.01f;
	}


	//지면 그리기
	CreateShader(4);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);



	////////////////////////////////////////////////////////////////////


	


	


	CreateShader(3);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, directionalWarp->GetShadowResourceView());



	//랜덤 그리기용 버퍼
	struct RndMatrix {
		D3DXMATRIX scale, rotation, translation;
	}rndmatrix;

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(RndMatrix);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ID3D11Buffer* buffer;
	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
	assert(SUCCEEDED(hr));


	//색상 버퍼
	struct ColorBuffer {
		D3DXCOLOR color1;
		D3DXCOLOR color2;
		float minScale;
		float maxScale;
		D3DXVECTOR2 padd;
	}colorbuffer;

	colorbuffer.color1 = color1;
	colorbuffer.color2 = color2;
	colorbuffer.minScale = minSize;
	colorbuffer.maxScale = maxSize;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = &colorbuffer;

	desc.ByteWidth = sizeof(ColorBuffer);

	ID3D11Buffer* bufferColor;
	hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &bufferColor);
	assert(SUCCEEDED(hr));
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &bufferColor);


	//랜덤용 변수
	float scalernd; 
	float rotationrnd; 
	float translationRndX, translationRndY, translationRndZ;


	
	for (int i = 0; i <count; i++) {
		 
		//랜덤값 셋팅
		scalernd			= (float)(rand() % (int)((maxSize - minSize)*100)) / 100 + minSize;
		rotationrnd			= (float)(rand() % rotation);
		translationRndX		= (float)(rand() % 200) / 100 - 1.0f;
		translationRndY		= (float)(rand() % 200) / 100 - 1.0f;
		translationRndZ		= (float)(500-i) / 500;


		D3DXMatrixScaling(&rndmatrix.scale, scalernd, scalernd, 1);
		D3DXMatrixRotationZ(&rndmatrix.rotation, (float)D3DX_PI / 180 * rotationrnd);
		D3DXMatrixTranslation(&rndmatrix.translation, translationRndX, translationRndY, translationRndZ);


		D3D11_MAPPED_SUBRESOURCE subResource;
		hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);
		memcpy(subResource.pData, &rndmatrix, sizeof(RndMatrix));
		D3D::GetDeviceContext()->Unmap(buffer, 0);
		D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &buffer);
		//세팅 종료

		
		D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

		//화면 가장자리에서 벗어난것 보정해주기
		{
			bool InBorderX = false;
			bool InBorderY = false;

			float tempX = translationRndX;
			if (translationRndX > 0.5f) {
				tempX -= 2;
			}
			else if (translationRndX < -0.5f) {
				tempX += 2;
			}
			else
				InBorderX = true;

			if (!InBorderX) {//!(InBorderX*InBorderY)) {
				D3DXMatrixTranslation(&rndmatrix.translation, tempX, translationRndY, translationRndZ);

				D3D11_MAPPED_SUBRESOURCE subResource;
				hr = D3D::GetDeviceContext()->Map
				(
					buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
				);
				memcpy(subResource.pData, &rndmatrix, sizeof(RndMatrix));
				D3D::GetDeviceContext()->Unmap(buffer, 0);
				D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &buffer);

				D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
			}

			float tempY = translationRndY;
			if (translationRndY > 0.5f) {
				tempY -= 2;
			}
			else if (translationRndY < -0.5f) {
				tempY += 2;
			}
			else
				InBorderY = true;


			if (!InBorderY) {//!(InBorderX*InBorderY)) {
				D3DXMatrixTranslation(&rndmatrix.translation, translationRndX, tempY, translationRndZ);
		
				D3D11_MAPPED_SUBRESOURCE subResource;
				hr = D3D::GetDeviceContext()->Map
				(
					buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
				);
				memcpy(subResource.pData, &rndmatrix, sizeof(RndMatrix));
				D3D::GetDeviceContext()->Unmap(buffer, 0);
				D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &buffer);
				D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
			}

			if (!(InBorderY*InBorderX)) {//!(InBorderX*InBorderY)) {
				D3DXMatrixTranslation(&rndmatrix.translation, tempX, tempY, translationRndZ);

				D3D11_MAPPED_SUBRESOURCE subResource;
				hr = D3D::GetDeviceContext()->Map
				(
					buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
				);
				memcpy(subResource.pData, &rndmatrix, sizeof(RndMatrix));
				D3D::GetDeviceContext()->Unmap(buffer, 0);
				D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &buffer);

				D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
			}
		}
	}

	SAFE_RELEASE(buffer);
	SAFE_RELEASE(bufferColor);
}

void GrassTexture::RND_SRT_Instancing()
{
	rndDraw->SetTarget();
	rndDraw->Clear(0, 0, 0, 0);

	if (maxSize <= minSize + 0.01f) {
		maxSize = minSize + 0.01f;
	}


	//지면 그리기
	CreateShader(4);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);




	////////////////////////////////////////////////////////////////////

	//쉐이더 만들기
	

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);

	ID3D10Blob* error;
	wstring filePath = basePath + L"randomSRTInstance.fx";
	HRESULT hr = D3DX10CompileFromFile
	(
		filePath.c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));


	hr = D3DX10CompileFromFile
	(
		filePath.c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));









	SAFE_RELEASE(layout);
	D3D11_INPUT_ELEMENT_DESC layoutdesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTVECTOR",0, DXGI_FORMAT_R32G32B32_FLOAT, 1,0 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTVECTOR",1, DXGI_FORMAT_R32_FLOAT, 1,12 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTVECTOR",2, DXGI_FORMAT_R32_FLOAT, 1,16 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	hr = D3D::GetDevice()->CreateInputLayout
	(
		layoutdesc
		, 5
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));

	







	
	//인스턴스 데이터 생성
	struct InstanceData {
		D3DXVECTOR3 position;
		float scale;
		float rotation;
	};
	ID3D11Buffer* instanceBuffer;
	

	vector<InstanceData> instances;
//	instances = new InstanceData[count];

	

	//데이터 입력
	for (int i = 0; i < count; i++) {
		//랜덤값 셋팅
		InstanceData temp;
		temp.scale = (float)(rand() % (int)((maxSize - minSize) * 100)) / 100 + minSize;
		temp.rotation =  (float)D3DX_PI / 180 * (float)(rand() % (rotation + 1));
		temp.position.x = (float)(rand() % 200) / 100 - 1.0f;
		temp.position.y = (float)(rand() % 200) / 100 - 1.0f;
		temp.position.z = (float)(500 - i) / 500;
		instances.push_back(temp);

		{
			bool InBorderX = false;
			bool InBorderY = false;

			D3DXVECTOR3 tempVector = temp.position;

			float tempX = tempVector.x;
			if (temp.position.x > 0.5f) {
				tempX -= 2;
			}
			else if (temp.position.x < -0.5f) {
				tempX += 2;
			}
			else
				InBorderX = true;
			
			float tempY = tempVector.y;
			if (temp.position.y > 0.5f) {
				tempY -= 2;
			}
			else if (temp.position.y < -0.5f) {
				tempY += 2;
			}
			else
				InBorderY = true;
			
			if (!InBorderY && !InBorderX) {
				temp.position = tempVector;
				temp.position.x = tempX;
				temp.position.y = tempY;
				instances.push_back(temp);
			}
		   	
			if (!InBorderX) {
				temp.position = tempVector;
				
				temp.position.x = tempX;
				instances.push_back(temp);
			}
			
			if (!InBorderY) {
				temp.position = tempVector;
				temp.position.y = tempY;
				instances.push_back(temp);
			}
		}
	}



	UINT totalcount = (UINT)instances.size();
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(InstanceData) * totalcount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &instances[0];
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;


		
	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
	assert(SUCCEEDED(hr));

	//SAFE_DELETE_ARRAY(instances);


	//색상 버퍼
	ID3D11Buffer* bufferColor;
	
	struct ColorBuffer {
		D3DXCOLOR color1;
		D3DXCOLOR color2;
		float minScale;
		float maxScale;
		D3DXVECTOR2 padd;
	}colorbuffer;

	colorbuffer.color1 = color1;
	colorbuffer.color2 = color2;
	colorbuffer.minScale = minSize;
	colorbuffer.maxScale = maxSize;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = &colorbuffer;

	
	desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ColorBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	
	 hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &bufferColor);
	assert(SUCCEEDED(hr));
	




	
	UINT stride[2];
	stride[0] = sizeof(VertexTexture);
	stride[1] = sizeof(InstanceData);


	UINT offset[2];
	offset[0] = 0;
	offset[1] = 0;

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;
	


	
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, directionalWarp->GetShadowResourceView());
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &bufferColor);



	D3D::GetDeviceContext()->DrawIndexedInstanced(6, totalcount, 0, 0, 0);
	
	SAFE_RELEASE(bufferColor);
	SAFE_RELEASE(instanceBuffer);
}

void GrassTexture::DrawGrassTexture()
{
	grassTexture->SetTarget();
	grassTexture->Clear(0, 0, 0, 0);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);

	ID3D10Blob* error;
	wstring filePath = basePath + L"randomSRTInstance.fx";
	HRESULT hr = D3DX10CompileFromFile
	(
		filePath.c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));


	hr = D3DX10CompileFromFile
	(
		filePath.c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));









	SAFE_RELEASE(layout);
	D3D11_INPUT_ELEMENT_DESC layoutdesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTVECTOR",0, DXGI_FORMAT_R32G32B32_FLOAT, 1,0 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTVECTOR",1, DXGI_FORMAT_R32_FLOAT, 1,12 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTVECTOR",2, DXGI_FORMAT_R32_FLOAT, 1,16 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	hr = D3D::GetDevice()->CreateInputLayout
	(
		layoutdesc
		, 5
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));










	//인스턴스 데이터 생성
	struct InstanceData {
		D3DXVECTOR3 position;
		float scale;
		float rotation;
	};
	ID3D11Buffer* instanceBuffer;


	vector<InstanceData> instances;
	//	instances = new InstanceData[count];






	//데이터 입력
	for (int i = 0; i < 40; i++) {
		//랜덤값 셋팅
		InstanceData temp;
		temp.scale = (float)(rand() % (int)((maxSize - minSize) * 100)) / 100 + minSize;
		temp.rotation = (float)D3DX_PI / 180 * (float)(rand() % (10 + 1));
		temp.position.x =  -1.0f + (float)i / (float)40 *2;//(float)(rand() % 200) / 100;
		temp.position.y = -0.0f;// (float)(rand() % 200) / 200;
		temp.position.z = (float)(500 - i) / 500;
		instances.push_back(temp);
		{
			bool InBorderX = false;
			bool InBorderY = false;

			D3DXVECTOR3 tempVector = temp.position;

			float tempX = tempVector.x;
			if (temp.position.x > 0.5f) {
				tempX -= 2;
			}
			else if (temp.position.x < -0.5f) {
				tempX += 2;
			}
			else
				InBorderX = true;

			float tempY = tempVector.y;
			if (temp.position.y > 0.5f) {
				tempY -= 2;
			}
			else if (temp.position.y < -0.5f) {
				tempY += 2;
			}
			else
				InBorderY = true;

			if (!InBorderY && !InBorderX) {
				temp.position = tempVector;
				temp.position.x = tempX;
				temp.position.y = tempY;
				instances.push_back(temp);
			}

			if (!InBorderX) {
				temp.position = tempVector;

				temp.position.x = tempX;
				instances.push_back(temp);
			}

			if (!InBorderY) {
				temp.position = tempVector;
				temp.position.y = tempY;
				instances.push_back(temp);
			}
		}
	}



	UINT totalcount = (UINT)instances.size();
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(InstanceData) * totalcount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &instances[0];
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;



	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &instanceBuffer);
	assert(SUCCEEDED(hr));

	//SAFE_DELETE_ARRAY(instances);


	//색상 버퍼
	ID3D11Buffer* bufferColor;

	struct ColorBuffer {
		D3DXCOLOR color1;
		D3DXCOLOR color2;
		float minScale;
		float maxScale;
		D3DXVECTOR2 padd;
	}colorbuffer;

	colorbuffer.color1 = color1;
	colorbuffer.color2 = color2;
	colorbuffer.minScale = minSize;
	colorbuffer.maxScale = maxSize;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = &colorbuffer;


	desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ColorBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;



	hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &bufferColor);
	assert(SUCCEEDED(hr));






	UINT stride[2];
	stride[0] = sizeof(VertexTexture);
	stride[1] = sizeof(InstanceData);


	UINT offset[2];
	offset[0] = 0;
	offset[1] = 0;

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;




	D3D::GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, directionalWarp->GetShadowResourceView());
	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &bufferColor);



//	D3D::Get()->SetBlender_MaxBlend();

	D3D::GetDeviceContext()->DrawIndexedInstanced(6, totalcount, 0, 0, 0);

	SAFE_RELEASE(bufferColor);
	SAFE_RELEASE(instanceBuffer);
}
































void GrassTexture::CreateShader(int shaderIdx)
{
	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);
	ID3D10Blob* error;
	HRESULT hr = D3DX10CompileFromFile
	(
		(basePath + ShaderList[shaderIdx]).c_str(), NULL, NULL, "VS", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &vertexBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreateVertexShader
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, NULL
		, &vertexShader
	);
	assert(SUCCEEDED(hr));

	hr = D3DX10CompileFromFile
	(
		(basePath + ShaderList[shaderIdx]).c_str(), NULL, NULL, "PS", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelBlob, &error, NULL
	);

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

	hr = D3D::GetDevice()->CreatePixelShader
	(
		pixelBlob->GetBufferPointer()
		, pixelBlob->GetBufferSize()
		, NULL
		, &pixelShader
	);
	assert(SUCCEEDED(hr));



	SAFE_RELEASE(layout);
	hr = D3D::GetDevice()->CreateInputLayout
	(
		VertexTexture::desc
		, VertexTexture::count
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	if (!SUCCEEDED(hr))
		assert(0);
}

void GrassTexture::CreateBuffer()
{
	VertexTexture* rect = new VertexTexture[4];
	float size = 720.0f / 1280.0f;
	rect[0].position = D3DXVECTOR3(-size, -1, 0.5f);
	rect[1].position = D3DXVECTOR3(-size, 1,  0.5f);
	rect[2].position = D3DXVECTOR3(size, -1,  0.5f);
	rect[3].position = D3DXVECTOR3(size, 1,   0.5f);

	rect[0].uv = D3DXVECTOR2(0, 1);
	rect[1].uv = D3DXVECTOR2(0, 0);
	rect[2].uv = D3DXVECTOR2(1, 1);
	rect[3].uv = D3DXVECTOR2(1, 0);


	UINT* index = new UINT[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = rect;

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBufferFinal);
	assert(SUCCEEDED(hr));



	rect[0].position = D3DXVECTOR3(-1, -1, 0);
	rect[1].position = D3DXVECTOR3(-1, 1, 0);
	rect[2].position = D3DXVECTOR3(1, -1, 0);
	rect[3].position = D3DXVECTOR3(1, 1, 0);


	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = rect;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));







	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(rect);
	SAFE_DELETE_ARRAY(index);





}

//void Grass::DrawTexture(ID3D11ShaderResourceView* texture)
//{
//	UINT stride = sizeof(VertexTexture);
//	UINT offset = 0;
//
//	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
//	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	D3D::GetDeviceContext()->IASetInputLayout(layout);
//	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
//	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
//	if(texture != nullptr)
//		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);;
//
//	DepthStencil::Get()->SetOffState();
//	Rasterizer::Get()->SetOffCullMode();
//	{
//		D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
//	}
//	DepthStencil::Get()->SetOnState();
//	Rasterizer::Get()->SetOnCullMode();
//}

void GrassTexture::SetFinalResult(RenderTexture * renderTexture)
{
	finalResult = *(renderTexture->GetShadowResourceView());
	diffuse = *(renderTexture->GetShadowResourceView());
}
