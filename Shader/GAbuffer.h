#pragma once
#include "ShaderBuffer.h"


//Global Ambient buffer
class GAbuffer
{
public:
	GAbuffer()
	{
		//psData.globalAmbient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1);


		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(PS_DATA);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
	}
	~GAbuffer() {
		SAFE_RELEASE(buffer);
	}
	void SetBuffer() {
		D3D::GetDeviceContext()->PSSetConstantBuffers(2, 1, &buffer);
	}

	void SetGA(D3DXCOLOR color) {
		psData.globalAmbient = color;

		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &psData, sizeof(PS_DATA));

		D3D::GetDeviceContext()->Unmap(buffer, 0);
	}



	struct PS_DATA
	{
		D3DXCOLOR globalAmbient;
	};


private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	PS_DATA psData;
};
