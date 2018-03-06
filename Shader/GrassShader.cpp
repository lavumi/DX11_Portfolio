#include "GrassShader.h"
#include "GrassBuffer.h"
GrassShader::GrassShader()
	:Shader(L"./Shader/FX/GrassShader.fx")
{
	CreateInputLayout();
	CreateGeometryShader();
	//CreateBuffers();
	//data.x = 0.7f;
	//data.y = 0.04f;
	//data.z = 0;
	//data.w = 0.05f;
	buffer = new GrassBuffer();
}

GrassShader::~GrassShader()
{

}

void GrassShader::Update()
{

}

void GrassShader::Render(UINT instanceCount, ID3D11ShaderResourceView* perlin)
{

	buffer->SetBuffers();




	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->GSSetShader(geoShader, NULL, 0);


	D3D::GetDeviceContext()->GSSetShaderResources(0, 1, &perlin);

	D3D::GetDeviceContext()->DrawInstanced(1, instanceCount, 0, 0);



	D3D::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}

void GrassShader::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layoutdesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTMATRIX",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INSTMATRIX",1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INSTMATRIX",2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INSTMATRIX",3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	HRESULT hr = D3D::GetDevice()->CreateInputLayout
	(
		layoutdesc
		, 5
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));
}
//
//void GrassShader::CreateBuffers()
//{
//
//	D3D11_BUFFER_DESC desc;
//	HRESULT hr;
//
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.Usage = D3D11_USAGE_DYNAMIC;
//	desc.ByteWidth = sizeof(D3DXVECTOR4);
//	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	desc.MiscFlags = 0;
//	desc.StructureByteStride = 0;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
//	assert(SUCCEEDED(hr));
//
//}
