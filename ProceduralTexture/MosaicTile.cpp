#include "../stdafx.h"
#include "MosaicTile.h"
#include "../Render/RenderTexture.h"


MosaicTile::MosaicTile()
	:vertexBlob(nullptr), pixelBlob(nullptr), vertexShader(nullptr), pixelShader(nullptr)
	, vertexBuffer(nullptr), indexBuffer(nullptr),
	finalResult(NULL)
{

	MosaicTileSingle=0;
	TangentMap = 0;
	Patterns = 0;
	DiffuseMap = 0;
	rndNoise = 0;


	
	basePath = L"./Shader/FX/";

	ShaderList.push_back(L"BaseTile.fx");
	ShaderList.push_back(L"CreateTangentMap.fx");
	ShaderList.push_back(L"SquarePattern.fx");
	ShaderList.push_back(L"DrawPatternInTile.fx");
	ShaderList.push_back(L"perlin.fx");
	ShaderList.push_back(L"finalDraw.fx");


	layout = nullptr;
	CreateBuffer();

	patternData._LineCount = 12;
	patternData.LineWidth = 12;
	patternData.color1 = D3DXCOLOR(1, 1, 1, 1);
	patternData.color2 = D3DXCOLOR(0, 0,0, 1);
	UserInterface::AddPattern(&patternData.LineWidth, &patternData._LineCount, &patternData.color1, &patternData.color2);

}

MosaicTile::~MosaicTile()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_RELEASE(layout);

	SAFE_RELEASE(vertexBlob);
	SAFE_RELEASE(vertexShader);

	SAFE_RELEASE(pixelBlob);
	SAFE_RELEASE(pixelShader);
}

void MosaicTile::DrawTexture()
{

	SAFE_DELETE(MosaicTileSingle);
	SAFE_DELETE(TangentMap);
	SAFE_DELETE(Patterns);
	SAFE_DELETE(DiffuseMap);
	SAFE_DELETE(rndNoise);


	MosaicTileSingle = new RenderTexture();
	TangentMap = new RenderTexture();
	Patterns = new RenderTexture();
	DiffuseMap = new RenderTexture();
	rndNoise = new RenderTexture(720, 720);



	//기본 렌더링 셋팅
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Sampler::Get()->SetDefault();



	DrawPatterns();

	tileBase();
	CreateTangentMap();
	DrawTile();


	DrawNoise();


	//최종 그릴 이미지
	SetFinalResult(rndNoise);
	//SetFinalResult(directionalWarp);

	//렌더링 타겟 되돌리기
	D3D::Get()->SetDefaultRenderView();


	//rndNoise->SaveTexture(L"testPNG.png");

}




void MosaicTile::tileBase()
{

	MosaicTileSingle->SetTarget();
	MosaicTileSingle->Clear(1,1,1,1);


    CreateShader(0);
    
    D3D::GetDeviceContext()->IASetInputLayout(layout);
    D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
    D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

    
    D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
	
}

void MosaicTile::CreateTangentMap()
{
	TangentMap->SetTarget();
	TangentMap->Clear(1, 1, 1, 1);

	CreateShader(1);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);


	ID3D11ShaderResourceView* test;
	D3DX11CreateShaderResourceViewFromFile(
		D3D::GetDevice(), L"./Heightmap.png", NULL, NULL, &test, NULL
	);
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, MosaicTileSingle->GetShadowResourceView());;

	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

	normal = *(TangentMap->GetShadowResourceView());

}

void MosaicTile::DrawPatterns()
{
	Patterns->SetTarget();
	Patterns->Clear(1, 1, 1, 1);

	CreateShader(2);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	



	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(PatternData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = &patternData;

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &subResource, &buffer);
	assert(SUCCEEDED(hr));

	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &buffer);


	
	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);


	

}

void MosaicTile::DrawTile()
{
	DiffuseMap->SetTarget();
	DiffuseMap->Clear(1, 1, 1, 1);

	CreateShader(3);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, MosaicTileSingle->GetShadowResourceView());;
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, Patterns->GetShadowResourceView());;


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);
	diffuse = *(DiffuseMap->GetShadowResourceView());

}

void MosaicTile::DrawNoise()
{
	rndNoise->SetTarget();
	rndNoise->Clear(1, 1, 1, 1);

	CreateShader(4);

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);

}



void MosaicTile::Render()
{
	D3D::Get()->SetBlender_Off();
	CreateShader((int)ShaderList.size() - 1);
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

	if (finalResult != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &finalResult);;

	Sampler::Get()->SetDefault();


	D3D::GetDeviceContext()->DrawIndexed(6, 0, 0);


}
void MosaicTile::SetFinalResult(RenderTexture * renderTexture)
{
	finalResult = *(renderTexture->GetShadowResourceView());
}


void MosaicTile::CreateShader(int shaderIdx)
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

void MosaicTile::CreateBuffer()
{
	VertexTexture* rect = new VertexTexture[4];
	float size = 720.0f / 1280.0f;
	rect[0].position = D3DXVECTOR3(-size, -1, 0.5f);
	rect[1].position = D3DXVECTOR3(-size, 1, 0.5f);
	rect[2].position = D3DXVECTOR3(size, -1, 0.5f);
	rect[3].position = D3DXVECTOR3(size, 1, 0.5f);

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


