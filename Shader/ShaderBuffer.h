#pragma once
#include "../stdafx.h"

class ShaderBuffer
{
public:
	void SetVSBuffer(UINT slot)
	{
		D3D::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &buffer);
	}
	
	void SetPSBuffer(UINT slot)
	{
		D3D::GetDeviceContext()->PSSetConstantBuffers(slot, 1, &buffer);
	}

	void SetBuffers() {

		Update();
		if(VS_Buffer !=0)
			D3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &VS_Buffer);

		if (GS_Buffer != 0)
			D3D::GetDeviceContext()->GSSetConstantBuffers(0, 1, &GS_Buffer);

		if (PS_Buffer != 0)
			D3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &PS_Buffer);

		D3D::GetDeviceContext()->VSSetConstantBuffers(13, 1, &worldBuffer);

	}

	virtual void SetWorld(D3DXMATRIX _world) {
		world = _world;
		D3DXMatrixTranspose(&world, &world);
		UpdateWorld();
	}
	

protected:
	ShaderBuffer(UINT dataSize)
		:VS_Buffer(0), GS_Buffer(0), PS_Buffer(0), buffer(0)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = dataSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
	}

	ShaderBuffer(UINT vs_dataSize, UINT gs_dataSize, UINT ps_dataSize)
		:VS_Buffer(0), GS_Buffer(0), PS_Buffer(0), buffer(0)
	{
		if (vs_dataSize != 0) {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = vs_dataSize;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &VS_Buffer);
			assert(SUCCEEDED(hr));
		}
		if (gs_dataSize != 0) {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = gs_dataSize;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &GS_Buffer);
			assert(SUCCEEDED(hr));
		}
		if (ps_dataSize != 0) {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = ps_dataSize;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &PS_Buffer);
			assert(SUCCEEDED(hr));
		}

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}

	~ShaderBuffer()
	{
		SAFE_RELEASE(buffer);
		SAFE_RELEASE(VS_Buffer);
		SAFE_RELEASE(GS_Buffer);
		SAFE_RELEASE(PS_Buffer);

	}

	virtual void Update() = 0;

	void UpdateBuffer(void* data, UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, data, dataSize);

		D3D::GetDeviceContext()->Unmap(buffer, 0);
	}

	void UpdateWorld() {
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = D3D::GetDeviceContext()->Map
		(
			worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, &world, sizeof(D3DXMATRIX));

		D3D::GetDeviceContext()->Unmap(worldBuffer, 0);
	}

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

	D3DXMATRIX world;



private:
	D3D11_BUFFER_DESC desc;
	ID3D11Buffer* buffer;
	ID3D11Buffer* VS_Buffer;
	ID3D11Buffer* GS_Buffer;
	ID3D11Buffer* PS_Buffer;

	
	ID3D11Buffer* worldBuffer;
};