#include "TerrainShader.h"
#include "TerrainBuffer.h"
TerrainShader::TerrainShader()
	:Shader(L"./Shader/FX/TerrainShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);


	buffer = new TerrainBuffer();
	//D3DXPLANE plane = D3DXPLANE(0, 1, 0, 7.9f);




	//CreateBuffers();
	//UserInterface::AddParallexBias(&parallexData.scale, &parallexData.layer);


	//material.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//material.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	//material.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
}

TerrainShader::~TerrainShader()
{
	SAFE_DELETE(buffer);
	//SAFE_RELEASE(LightBuffer);
	//SAFE_RELEASE(MaterialBuffer);
	//SAFE_RELEASE(ExtraBuffer);

}

void TerrainShader::Update()
{

}


void TerrainShader::Render(UINT indexCount, D3DXMATRIX world,
	ID3D11ShaderResourceView ** diffuseMap, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* lightMap) {

	buffer->SetWorld(world);
	buffer->SetBuffers();

	D3D::GetDeviceContext()->PSSetShaderResources(10, 3, diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &lightMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &normalMap);








	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->GSSetShader(NULL, NULL, 0);






	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();
	D3D::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}

void TerrainShader::SetPlane(D3DXPLANE & plane)
{
	buffer->SetPLANE(plane);
}




/*
void TerrainShader::Render(UINT indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	ID3D11ShaderResourceView ** diffuseMap, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* lightMap,
	D3DXPLANE clipPlane)
{
	SetMatrix(world, view, projection);

	D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &wBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(1, 1, &vpBuffer);


	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };



	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, &world);

	D3DXMatrixTranspose(&extraData.invTransWorld, &invWorld);



	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		ExtraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &extraData, sizeof(ExtraData));
	D3D::GetDeviceContext()->Unmap(ExtraBuffer, 0);



	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		MaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &material, sizeof(MaterialData));

	D3D::GetDeviceContext()->Unmap(MaterialBuffer, 0);



	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, LightManager::Get()->GetLightData(), sizeof(LightManager::LightData));

	D3D::GetDeviceContext()->Unmap(LightBuffer, 0);




	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		cameraSpaceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, view*projection, sizeof(D3DXMATRIX));

	D3D::GetDeviceContext()->Unmap(cameraSpaceBuffer, 0);






	D3DXVECTOR4 clipPlaneData = D3DXVECTOR4(clipPlane.a, clipPlane.b, clipPlane.c, clipPlane.d);

	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		clipPlaneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &clipPlaneData, sizeof(D3DXVECTOR4));

	D3D::GetDeviceContext()->Unmap(clipPlaneBuffer, 0);



	D3D::GetDeviceContext()->PSSetShaderResources(10, 3, diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &lightMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &normalMap);







	D3D::GetDeviceContext()->VSSetConstantBuffers(2, 1, &LightBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(3, 1, &ExtraBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(11, 1, &clipPlaneBuffer);


	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &MaterialBuffer);
	D3D::GetDeviceContext()->PSSetConstantBuffers(1, 1, &cameraSpaceBuffer);


	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	D3D::GetDeviceContext()->GSSetShader(NULL, NULL, 0);






	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();
	D3D::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}




void TerrainShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(MaterialData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &MaterialBuffer);
	assert(SUCCEEDED(hr));

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(LightManager::LightData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &LightBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(ExtraData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &ExtraBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXVECTOR4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &clipPlaneBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(D3DXMATRIX);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &cameraSpaceBuffer);
	assert(SUCCEEDED(hr));
}
*/