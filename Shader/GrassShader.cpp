#include "GrassShader.h"

GrassShader::GrassShader()
	:Shader(L"./Shader/FX/GrassShader.fx")
{
	CreateInputLayout();
	CreateGeometryShader();
	CreateBuffers();
}

GrassShader::~GrassShader()
{

}

void GrassShader::Update()
{

}

void GrassShader::Render(UINT instanceCount, D3DXMATRIX view, D3DXMATRIX projection)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);
	D3D::GetDeviceContext()->GSSetShader(geoShader, NULL, 0);

	SetMatrix(view, view, projection);

	//D3D::GetDeviceContext()->VSSetConstantBuffers(10, 1, &wBuffer);
	D3D::GetDeviceContext()->GSSetConstantBuffers(0, 1, &vpBuffer);
	//D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &vpBuffer);

	//D3D::GetDeviceContext()->Draw(3, 0);
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

void GrassShader::CreateBuffers()
{

}
