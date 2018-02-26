#include "InstanceTextureShader.h"

InstanceTextureShader::InstanceTextureShader()
	:Shader(L"./Shader/FX/InstanceTextureShader.fx")
{
	CreateInputLayout();

	CreateBuffers();
}

InstanceTextureShader::~InstanceTextureShader()
{

}

void InstanceTextureShader::Update()
{

}

void InstanceTextureShader::Render(UINT indexCount, UINT instanceCount, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* diffuse)
{

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	SetMatrix(view, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(10, 1, &wBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(11, 1, &vpBuffer);

	if(diffuse != nullptr)
		D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuse);

	D3D::GetDeviceContext()->DrawIndexedInstanced(6, instanceCount, 0, 0, 0);
}

void InstanceTextureShader::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layoutdesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTMATRIX",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0 , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTMATRIX",1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTMATRIX",2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTMATRIX",3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	HRESULT hr = D3D::GetDevice()->CreateInputLayout
	(
		layoutdesc
		, 6
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &layout
	);
	assert(SUCCEEDED(hr));
}

void InstanceTextureShader::CreateBuffers()
{

}
