#pragma once
#pragma once
#include "ShaderBuffer.h"
#include "../Render/LightManager.h"

class LightBuffer
{
public:
	LightBuffer(LightManager* light)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(LightData);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));

		this->light = light;
	}


	void SetBuffers() {

		light->GetDirection(&data.lightDirection);
		light->GetView(&data.lightView);
		light->GetProjection(&data.lightProjection);

		


		D3DXMatrixTranspose(&data.lightView, &data.lightView);
		D3DXMatrixTranspose(&data.lightProjection, &data.lightProjection);


		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &data, sizeof(LightData));

		D3D::GetDeviceContext()->Unmap(buffer, 0);

		D3D::GetDeviceContext()->VSSetConstantBuffers(11, 1, &buffer);
		D3D::GetDeviceContext()->GSSetConstantBuffers(11, 1, &buffer);
		D3D::GetDeviceContext()->PSSetConstantBuffers(11, 1, &buffer);
	}


	void Update()
	{

	}

	struct LightData {

		D3DXMATRIX lightView;
		D3DXMATRIX lightProjection;
		D3DXVECTOR3 lightDirection;
		float padding;
	};



private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	LightManager* light;
	LightData data;
};