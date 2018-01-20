#pragma once

class Texture
{
public:
	static void LoadTexture(wstring fileName, ID3D11Texture2D** texture);
	static void LoadPixel(ID3D11Texture2D* texture, UINT** colors);
};