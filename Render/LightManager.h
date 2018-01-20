#pragma once
class LightManager {

public:
	struct LightData {
		D3DXMATRIX lightView;
		D3DXMATRIX lightProjection;
		D3DXVECTOR3 lightDirection;
		float padding;
	};



	static LightManager* Get();


	LightData* GetLightData() {
		return &lightData;
	}

	void Update();

private:
	LightManager();
	~LightManager();
	LightData lightData;
	static LightManager* instance;


	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
};