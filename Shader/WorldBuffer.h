#pragma once
#include "ShaderBuffer.h"

class WorldBuffer
{
public:
	WorldBuffer()
	{
		D3DXMatrixIdentity(&data.world);

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));


	}

	void SetBuffer() {
		D3D::GetDeviceContext()->VSSetConstantBuffers(13, 1, &buffer);
		D3D::GetDeviceContext()->GSSetConstantBuffers(13, 1, &buffer);
	}


	void SetWorld(D3DXMATRIX world) {

		D3DXMatrixTranspose(&data.world, &world);

		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &data, sizeof(Data));

		D3D::GetDeviceContext()->Unmap(buffer, 0);


	}

	struct Data
	{
		D3DXMATRIX world;
	};

private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	Data data;
};