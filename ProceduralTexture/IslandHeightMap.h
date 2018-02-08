#pragma once
class RenderTexture;
class IslandHeightMap {

public:

	IslandHeightMap();
	~IslandHeightMap();


	void CreateVoronoiRegion();


	void DrawTexture();


	void Render();


private:

	UINT sizeX, sizeY;
};