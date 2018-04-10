#pragma once
#include "ShaderBuffer.h"

class CascadeShadowBuffer 
{
public:
	CascadeShadowBuffer()
	{
		D3DXMatrixIdentity(&gsData.cropMatrix[0]);
		D3DXMatrixIdentity(&gsData.cropMatrix[1]);
		D3DXMatrixIdentity(&gsData.cropMatrix[2]);
		D3DXMatrixIdentity(&gsData.cropMatrix[3]);

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(GS_DATA);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
	}
	~CascadeShadowBuffer() {
		SAFE_RELEASE(buffer);
	}

	void UpdateMatrix(D3DXMATRIX cropMatrix[4]) {



		for (UINT i = 0; i < 4; i++) {
			D3DXMatrixTranspose(&gsData.cropMatrix[i] , &cropMatrix[i]);
		}

		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &gsData, sizeof(GS_DATA));

		D3D::GetDeviceContext()->Unmap(buffer, 0);
	}


	void SetBuffers() {
		D3D::GetDeviceContext()->VSSetConstantBuffers(10, 1, &buffer);
		D3D::GetDeviceContext()->GSSetConstantBuffers(10, 1, &buffer);
	}


	struct GS_DATA
	{
		D3DXMATRIX cropMatrix[4];
	};



private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	GS_DATA gsData;

};