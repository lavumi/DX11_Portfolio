#pragma once

class LightManager {

public:

	LightManager();
	~LightManager();

	struct LightData {
		D3DXMATRIX lightViewXProjection;
		D3DXVECTOR3 lightPosition;
		float night;
	};

	void GetDirection(D3DXVECTOR3* view) {
		memcpy(view, this->lightData.lightPosition, sizeof(D3DXVECTOR3));
	}

	void GetViewXProjection(D3DXMATRIX* view) {
		memcpy(view, this->lightData.lightViewXProjection, sizeof(D3DXMATRIX));
	}

	void GetLightData(LightData* view) {
		memcpy(view, &lightData, sizeof(LightData));
	}


	//LightData* GetLightData() {
	//	return &lightData;
	//}

	D3DXCOLOR GetGA() {
		return globalAmbient;
	}

	void Update();

	void Delete();

private:
	LightData lightData;


	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

	float speed;
	float angle_pie, angle_theta;
	float anglePieDir;

	D3DXCOLOR globalAmbient;
};