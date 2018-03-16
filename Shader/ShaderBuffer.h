#pragma once
#include "../stdafx.h"

class ShaderBuffer
{
public:
	void SetBuffers() {

		Update();
		if(VS_Buffer !=0)
			D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &VS_Buffer);
		if (DS_Buffer != 0)
			D3D::GetDeviceContext()->DSSetConstantBuffers(0, 1, &DS_Buffer);
		if (HS_Buffer != 0)
			D3D::GetDeviceContext()->HSSetConstantBuffers(0, 1, &HS_Buffer);
		if (GS_Buffer != 0)
			D3D::GetDeviceContext()->GSSetConstantBuffers(0, 1, &GS_Buffer);
		if (PS_Buffer != 0)
			D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &PS_Buffer);
	//	D3D::GetDeviceContext()->VSSetConstantBuffers(13, 1, &worldBuffer);
	}

	//virtual void SetWorld(D3DXMATRIX _world) {
	//	world = _world;
	//	D3DXMatrixTranspose(&world, &world);
	//	UpdateWorld();
	//}
	

protected:

	ShaderBuffer(UINT vs_dataSize, UINT hs_dataSize, UINT ds_dataSize, UINT gs_dataSize, UINT ps_dataSize)
		:VS_Buffer(0), HS_Buffer(0), DS_Buffer(0),GS_Buffer(0), PS_Buffer(0)
	{
		CreateBuffer(vs_dataSize, VS_Buffer);
		CreateBuffer(hs_dataSize, HS_Buffer);
		CreateBuffer(ds_dataSize, DS_Buffer);
		CreateBuffer(gs_dataSize, GS_Buffer);
		CreateBuffer(ps_dataSize, PS_Buffer);
	}

	~ShaderBuffer()
	{
		//SAFE_RELEASE(buffer);
		SAFE_RELEASE(VS_Buffer);
		SAFE_RELEASE(GS_Buffer);
		SAFE_RELEASE(PS_Buffer);
		SAFE_RELEASE(DS_Buffer);
		SAFE_RELEASE(HS_Buffer);
	}

	virtual void Update() = 0;



	void UpdateVSBuffer(void* data, UINT dataSize)
	{
		if (dataSize == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			VS_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(VS_Buffer, 0);
	}
	void UpdateGSBuffer(void* data, UINT dataSize)
	{
		if (dataSize == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			GS_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(GS_Buffer, 0);
	}
	void UpdatePSBuffer(void* data, UINT dataSize)
	{
		if (dataSize == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			PS_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(PS_Buffer, 0);
	}

	void UpdateDSBuffer(void* data, UINT dataSize)
	{
		if (dataSize == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			DS_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(DS_Buffer, 0);
	}

	void UpdateHSBuffer(void* data, UINT dataSize)
	{
		if (dataSize == 0)
			return;
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			HS_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(HS_Buffer, 0);
	}

	D3DXMATRIX world;



private:
	D3D11_BUFFER_DESC desc;

	ID3D11Buffer* VS_Buffer;
	ID3D11Buffer* DS_Buffer;
	ID3D11Buffer* GS_Buffer;
	ID3D11Buffer* PS_Buffer;
	ID3D11Buffer* HS_Buffer;
	
	void CreateBuffer(UINT dataSize, ID3D11Buffer*& buffer) {
		if (dataSize == 0)
			return;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = dataSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
		
	}
};