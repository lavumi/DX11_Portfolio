#include "MirrorShader.h"

MirrorShader::MirrorShader()
	:Shader(L"./Shader/FX/MirrorShader.fx")
{
	CreateInputLayout(VertexTextureNormalTangent::desc, VertexTextureNormalTangent::count);

	CreateBuffers();
	parallexData.scale = 0.1f;
	parallexData.layer = 32;

	material.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	material.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.shininess = 70;
	material.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
}

MirrorShader::~MirrorShader()
{
}

void MirrorShader::Update()
{

}

void MirrorShader::Render(UINT indexCount, D3DXMATRIX world, ID3D11ShaderResourceView * diffuseMap, ID3D11ShaderResourceView * normalMap, ID3D11ShaderResourceView * heightMap, ID3D11ShaderResourceView* lightMap)
{

	//월드의 역행렬 
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

	
	//노말맵
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		parallaxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &parallexData, sizeof(D3DXVECTOR4));

	D3D::GetDeviceContext()->Unmap(parallaxBuffer, 0);



	//머테리얼
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		MaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &material, sizeof(MaterialData));

	D3D::GetDeviceContext()->Unmap(MaterialBuffer, 0);


	//빛
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, LightManager::Get()->GetLightData(), sizeof(LightManager::LightData));

	D3D::GetDeviceContext()->Unmap(LightBuffer, 0);


	//반사된 뷰, 카메라 포지션
	Camera::Get()->GetView(&mirrorCameraData.view);
	D3D::Get()->GetProjection(&mirrorCameraData.projection);
	Camera::Get()->GetPosition(&mirrorCameraData.position);
	
	
	D3DXMATRIX R;
	D3DXPLANE plane(0.0f, -1.0f, 0.0f, -7.80f); // xy plane
	D3DXMatrixReflect(&R, &plane);
	mirrorCameraData.view =  R* mirrorCameraData.view;
	
	D3DXVec3TransformCoord(&mirrorCameraData.position, &mirrorCameraData.position, &R);

	D3DXMatrixTranspose(&mirrorCameraData.view, &mirrorCameraData.view);
	D3DXMatrixTranspose(&mirrorCameraData.projection, &mirrorCameraData.projection);

	
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	D3D::GetDeviceContext()->Map
	(
		mirrorViewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, &mirrorCameraData, sizeof(MirrorCameraData));

	D3D::GetDeviceContext()->Unmap(mirrorViewBuffer, 0);



	worldBuffer->SetWorld(world);
	cameraBuffer->SetVSBuffer(0);
	worldBuffer->SetVSBuffer(1);

	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &diffuseMap);
	D3D::GetDeviceContext()->PSSetShaderResources(1, 1, &normalMap);
	D3D::GetDeviceContext()->PSSetShaderResources(2, 1, &heightMap);
	D3D::GetDeviceContext()->PSSetShaderResources(3, 1, &lightMap);
	

	
	D3D::GetDeviceContext()->VSSetConstantBuffers(2, 1, &LightBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(3, 1, &ExtraBuffer);
	D3D::GetDeviceContext()->VSSetConstantBuffers(4, 1, &mirrorViewBuffer);

	D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &MaterialBuffer);
	D3D::GetDeviceContext()->PSSetConstantBuffers(1, 1, &parallaxBuffer);
	

	D3D::GetDeviceContext()->IASetInputLayout(layout);
	D3D::GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3D::GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);



	D3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	Sampler::Get()->SetDefault();

}

void MirrorShader::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;
	HRESULT hr;
	
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
	desc.ByteWidth = sizeof(MaterialData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &MaterialBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(ParallexData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &parallaxBuffer);
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
	desc.ByteWidth = sizeof(D3DXMATRIX)*2;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &lightMatrixBuffer);
	assert(SUCCEEDED(hr));


	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(MirrorCameraData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &mirrorViewBuffer);
	assert(SUCCEEDED(hr));

}
