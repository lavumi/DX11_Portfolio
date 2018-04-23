#pragma once

class Frustum;
class QuadTree {
public:
	QuadTree(UINT mapWidth, UINT mapHeight);
	~QuadTree();

	void CreateTree();
	void setIndexByLOD(vector<UINT>& indexData, Frustum* frustum);
	
private:
	D3DXVECTOR3 cameraPos;
	UINT mapWidth, mapHeight;

	struct Node {

		Node* subNodes[4];
		Node *left, *down;
		UINT startX, startY;
		UINT width, height;

		D3DXVECTOR3 center;
		float radius;
		//int index;


		UINT* indices;
		QuadTree* tree;

		int LOD;

		void getIndecies(UINT* & out_indices, UINT & out_width, UINT& out_height, UINT LOD);
		Node() {
			
			subNodes[0] = subNodes[1] = subNodes[2] = subNodes[3] = nullptr;
			left = down = nullptr;
			indices = nullptr;
			startX = startY = 0;//endX = endY = 0;
			//width = height = 0;
		}
		void SetIndices(UINT startX_Idx, UINT startY_Idx, UINT mapWidth);

		~Node() {
				delete[] indices;
		}
	};


	Node * head;

	//이웃 노드 체크를 위한 자료구조
	//없에면 좋겠다
	map<Node*,UINT > nodeIdx;
	map<UINT, Node*> nodeIdx_right;
	map<UINT, Node*> nodeIdx_up;
	
	void DeleteTree(Node* node);


	void Scan(vector<UINT>& out_indexData,Node* node, Frustum* frustum);
	void SetNode(Node* node, UINT startX_Idx, UINT startY_Idx, UINT width, UINT height);

	void ClearLOD(Node * node);
	void SetNeighbor(Node* node);
	
};