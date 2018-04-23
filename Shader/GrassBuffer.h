#pragma once
#include "ShaderBuffer.h"

class Character;
class GrassBuffer : public ShaderBuffer
{
public:
	GrassBuffer()
		: ShaderBuffer(0,0,0, sizeof(GS_DATA), 0)
	{	
		gsData._grassHeight = 1.4f;
		gsData._grassWidth = 0.04f;
		gsData._timer = 0;
		gsData.padding = 2;
		D3DXVec2Normalize(&gsData._wind, &D3DXVECTOR2(1, 0));
		//gsData.padding2 = D3DXVECTOR2(0, 0);
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
		inverse = false;
	}

	void SetGrassData(float height, float width, D3DXVECTOR3 wind) {
		gsData._grassHeight = height;
		gsData._grassWidth = width;
		//D3DXVec3Normalize(&gsData._wind, &wind);
		//UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}

	void SetCharacter(D3DXVECTOR4 pos ) {
		gsData.characterPos = pos;
	}
	struct GS_DATA
	{
		float _grassHeight;
		float _grassWidth;
		float _timer;
		float padding;
		D3DXVECTOR4 characterPos;
		D3DXVECTOR2 _wind;
		D3DXVECTOR2 padding2;
		
	};
	void Update()
	{
		DWORD time = timeGetTime();
		time %= 1000;
		float timer =  (float)(time) / 1000;
		
		gsData._timer = timer;
		UpdateGSBuffer(&gsData, sizeof(GS_DATA));
	}


private:
	bool inverse;
	GS_DATA gsData;
};