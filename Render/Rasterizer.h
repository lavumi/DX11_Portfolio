#pragma once

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void SetSolid();
	void SetWireframe();

	void SetOnCullMode();
	void SetOffCullMode();
	void SetFrontCullMode();


private:




	ID3D11RasterizerState* solidState;
	ID3D11RasterizerState* wireframeState;
	ID3D11RasterizerState* onCullState;
	ID3D11RasterizerState* offCullState;
	ID3D11RasterizerState* frontCullState;
};