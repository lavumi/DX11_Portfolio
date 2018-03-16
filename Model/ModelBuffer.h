#pragma once
#include "../Shader/ShaderBuffer.h"

/********************************************************************************
 @brief
 모든 Bone의 현재 Time에서의 Transform Matrix를 저장하는 버퍼
 모든 Vertex에서 같은 값을 사용하므로 ModelClass에서 한 번 업데이트
********************************************************************************/
class ModelBuffer : public ShaderBuffer
{
public:
	ModelBuffer()
		: ShaderBuffer(sizeof(VS_Data), 0,0,0, sizeof(PS_DATA))
	{
		D3DXMatrixIdentity(&data.boneScale);
		
		for(int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		data.isSkinning = false;


		psData.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		psData.diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		psData.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		psData.shininess = 70;
		psData.globalAmbient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		UpdatePSBuffer(&psData, sizeof(PS_DATA));
	}

	void Update()
	{
		UpdateVSBuffer(&data, sizeof(VS_Data));
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for(UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}


	struct VS_Data
	{
		D3DXMATRIX boneScale;
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		D3DXVECTOR3 padding;
	};
	struct PS_DATA
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXCOLOR globalAmbient;
		float shininess;
		D3DXVECTOR3 padding;
	};
private:
	VS_Data data;
	PS_DATA psData;
};