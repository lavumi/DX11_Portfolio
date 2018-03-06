#pragma once
#include "ShaderBuffer.h"

class VPBuffer
{
public:
	VPBuffer()
	{
		D3DXMatrixIdentity(&data.view);
		//D3DXMatrixIdentity(&data.projection);

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));


	}

	void SetVPMatrix(D3DXMATRIX view,D3DXMATRIX projection, UINT shader = 0) {

		D3DXMATRIX _view, _projection;
		_view = view;
		_projection = projection;


		D3DXMatrixTranspose(&_view, &view);
		D3DXMatrixTranspose(&_projection, &projection);

		data.view = _view * _projection;
		
		D3DXMatrixTranspose(&data.view, &(view*projection));
		//D3DXMatrixTranspose(&data.projection, &projection);

		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &data, sizeof(Data));

		D3D::GetDeviceContext()->Unmap(buffer, 0);
		if(shader == 0)
			D3D::GetDeviceContext()->VSSetConstantBuffers(12, 1, &buffer);
		else if (shader == 1)
			D3D::GetDeviceContext()->GSSetConstantBuffers(12, 1, &buffer);
	}




	void Update()
	{
		
	}

	struct Data
	{
		D3DXMATRIX view;
		//D3DXMATRIX projection;
	};

private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	Data data;
};