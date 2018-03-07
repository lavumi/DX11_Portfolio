#pragma once
class LightManager {

public:
	struct LightData {

		D3DXMATRIX lightView;
		D3DXMATRIX lightProjection;
		//D3DXMATRIX lightViewXProjection;
		D3DXVECTOR3 lightDirection;
		float padding;
	};



	static LightManager* Get();

	void GetView(D3DXMATRIX* view) {
		memcpy(view, this->lightData.lightView, sizeof(D3DXMATRIX));
	}
	void GetProjection(D3DXMATRIX* view) {
		memcpy(view, this->lightData.lightProjection, sizeof(D3DXMATRIX));
	}

	//void GetViewXProjection(D3DXMATRIX* view) {
	//	memcpy(view, this->lightData.lightViewXProjection, sizeof(D3DXMATRIX));
	//}

	//void GetLightDirection(D3DXVECTOR3* out_lightDir) {
	//	memcpy(out_lightDir, this->lightData.lightDirection, sizeof(D3DXVECTOR3));
	//
	//}

	LightData* GetLightData() {
		return &lightData;
	}

	void Update();

	void Delete();

private:
	LightManager();
	~LightManager();
	LightData lightData;
	static LightManager* instance;


	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

	float speed;
	float anglex, angley;
};