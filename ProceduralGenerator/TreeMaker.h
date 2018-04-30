#pragma once

class NormalShaderBuffer;
class TreeMaker {
public:
	TreeMaker();
	~TreeMaker();

	void Initialize(D3DXVECTOR3);
	
	void Render();
	void LeafRender();

private:


	//필요 변수들
	float branchingChanceByGrade[4][3] = {
		{ 1.0f, 0.00f, 0.19f },
		{ 0.6f, 0.0f, 0.19f },
		{ 0.1f, 0.5f, 0.4f },
		{ 0.1f, 0.5f, 0.4f }
	};
	
	int branchingMMAngle[4][2] = { 
		{7, 8},
		{3, 5},
		{7, 8},
		{ 7, 8 }
	};
	int curveMMAngle[4][2] = {
		{ 0, 2 },
		{ 0, 3 },
		{ 0, 3 },
		{ 2, 5 }
	};

	int widthVariation[4][2] = {
		{ 8, 9 },
		{ 7, 9 },
		{ 8, 9 },
	{ 8, 9 }
	};






	struct TreeNode {
		D3DXVECTOR3 point;
		D3DXVECTOR3 direction;
		float width, height;
		//float endWidth;
		TreeNode* child[2];
		//bool leaf;
		TreeNode(D3DXVECTOR3 point, D3DXVECTOR3 direction, float width) {
			this->direction = direction;
			this->point = point; 
			this->width = width;
			//this->endWidth = 0;
			this->height = 1;
			child[0] = nullptr;
			child[1] = nullptr;
			//leaf = false;
		};
	};



	void CreateTree(float x, float y, float z);


	void Branching(TreeNode*);
	void DeleteTree(TreeNode* node);
	void CreateMesh(TreeNode* node, UINT, float);


	void LeafPosotionSetting(TreeNode* node);


	vector<VertexTextureNormalTangent> vertexData;
	vector<UINT> indexData;

	TreeNode* root;




	D3DXVECTOR3 treePosition;

	void CreateBuffer();
	//void CreateBufferTest();
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* leafVertexBuffer;
	ID3D11Buffer* instanceBuffer;
	vector<D3DXMATRIX> leafInstanceData;
	UINT leafCount;

	
	D3DXMATRIX world;
	WorldBuffer* wBuffer;
	NormalShaderBuffer* buffer;

	UINT nodeCount;
	UINT vertexCount;
	UINT indexCount;



	ID3D11ShaderResourceView* diffuseMap;
	ID3D11ShaderResourceView* normalMap;

};