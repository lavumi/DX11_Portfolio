#pragma once
#include "ShaderBuffer.h"

class Frustum;
class FrustumBuffer 
{
public:
	FrustumBuffer()
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
	~FrustumBuffer() {
		SAFE_RELEASE(buffer);
	}





	void UpdateBuffer(D3DXMATRIX cropMatrix[4], D3DXPLANE* plane) {



		for (UINT i = 0; i < 4; i++) {
			D3DXMatrixTranspose(&gsData.cropMatrix[i] , &cropMatrix[i]);
		}
		memcpy(&gsData.frustumPlane, plane, sizeof(D3DXPLANE) * 3);


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
		D3DXPLANE frustumPlane[3];
	};



private:
	//TODO : ¹öÆÛ¿Í ÇÁ·¯½ºÅÒ°£ÀÇ Ä¸½¶È­. ÀÌ´ë·Î ±¦ÂúÀº°¡???!!!
	friend class Frustum;

	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	GS_DATA gsData;

};