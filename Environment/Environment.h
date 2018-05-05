#pragma once

class Skydome;
class Landscape;
class Water;
class Skyplane;
class TerrainGrass;
class Character;
class TreeMaker;



class Frustum;
class Environment {
public:
	Environment();
	~Environment();

	void Initialize();
	void Update();
	

	


	void SetLandTexture(ID3D11ShaderResourceView* diffuse);

	void MakeGrassPosData(Frustum* frustum);

	void RenderSkydome();
	void RenderSkyplane();
	void RenderLand();
	void RenderReflectedLand();
	void RenderLandForShadow();
	void RenderWater();
	void RenderGrass();

	void RenderTree();
	void RenderTreeLeaf();

	/********************************
	TODO : 
	잔디 설정을 위한 캐릭터 위치 입력
	수정 필수
	**********************************/
	void SetCharacter(Character*);

	void GetLandY(D3DXVECTOR3 &position);
private:

	Landscape * landscape;
	Skydome* skydome;
	Skyplane* cloud;

	Water* lake;
	TerrainGrass* grass;

	vector<TreeMaker*> trees;
	


	//QuadTree* quadTree;
	D3DXVECTOR3* characterPos;



	void CreateTree(int count); 
	void DeleteTree();
};