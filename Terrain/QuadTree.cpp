#include "../stdafx.h"
#include "QuadTree.h"
#include "../Render/Frustum.h"


#define MAX_VERTEXCOUNT 255


QuadTree::QuadTree(UINT mapWidth, UINT mapHeight)
{
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;


}

QuadTree::~QuadTree()
{
	DeleteTree(head);
}

void QuadTree::CreateTree()
{
	//TODO : 여기서 하는것들의 결과물을 미리 저장하고 실행시 읽어오는 걸로 바꾸자
	head = new Node();
	SetNode(head, 0, 0, this->mapWidth, this->mapHeight);
	SetNeighbor(head);
	nodeIdx.clear();
	nodeIdx_right.clear();
	nodeIdx_up.clear();
}



void QuadTree::setIndexByLOD(vector<UINT>& indexData, Frustum* frustum)
{
	Camera::Get()->GetPosition(&cameraPos);
	Scan(indexData, head, frustum);
}




void QuadTree::Scan(vector<UINT>& indexData, Node * node, Frustum* frustum)
{
	bool  _isVisible = frustum->CheckSphere(node->center, node->radius);
	if (!_isVisible)
		return;
	if (node->subNodes[0] != 0) {
		for (UINT i = 0; i < 4; i++) {
				Scan(indexData, node->subNodes[i], frustum);
		}
	}
	else
	{
		//UINT centerX = (node->endX - node->startX)/2 + node->startX;
		//UINT centerZ = (node->endY - node->startY) / 2 + node->startY;

		UINT centerX = node->width / 2 + node->startX;
		UINT centerZ = node->height / 2 + node->startY;

		int deltaX = (int)cameraPos.x - (int)centerX;
		int deltaZ = (int)cameraPos.z - (int)centerZ;

		UINT distance = deltaX * deltaX + deltaZ * deltaZ;


		//TODO : 정수 제곱근 구하기 수정해보자
		distance = sqrt(distance);

		distance /= 50;

		if (distance > 3)
			distance = 3;

		node->LOD = distance;


		UINT* data;
		UINT width, height;
		node->getIndecies(data, width, height, distance);
		if (height % 2 == 0)
			int a = 10;
		//내 왼쪽, 아래 노드둘다 나보다 LOD가 작을때
		if (node->LOD > node->left->LOD && node->LOD > node->down->LOD) { 
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {

					if (i == 0 && j == 0) {
						UINT center = (data[j + (i + 1) * width] + data[j + 1 + i * width]) / 2;
						UINT down = (data[j + 1 + i * width] + data[j + i * width]) / 2;
						UINT left = (data[j + (i + 1) * width] + data[j + i * width]) / 2;

						indexData.push_back(data[j + i * width]);
						indexData.push_back(left);
						indexData.push_back(center);

						indexData.push_back(center);
						indexData.push_back(left);
						indexData.push_back(data[j + (i + 1) * width]);


						indexData.push_back(data[j + i * width]);
						indexData.push_back(center);
						indexData.push_back(down);

						indexData.push_back(down);
						indexData.push_back(center);
						indexData.push_back(data[j + 1 + i * width]);


						indexData.push_back(center);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);

						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
						indexData.push_back(center);
					}
					else if (i == 0) {
						UINT center = (data[j + (i + 1) * width] + data[j + 1 + i * width]) / 2;
						UINT down = (data[j + 1 + i * width] + data[j + i * width]) / 2;

						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(center);

						indexData.push_back(data[j + i * width]);
						indexData.push_back(center);
						indexData.push_back(down);

						indexData.push_back(down);
						indexData.push_back(center);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(center);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);

						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
						indexData.push_back(center);
					}
					else if (j == 0) {
						UINT center = (data[j + (i + 1) * width] + data[j + 1 + i * width]) / 2;
						UINT left = (data[j + (i + 1) * width] + data[j + i * width]) / 2;

						indexData.push_back(data[j + i * width]);
						indexData.push_back(left);
						indexData.push_back(center);

						indexData.push_back(center);
						indexData.push_back(left);
						indexData.push_back(data[j + (i + 1) * width]);

						indexData.push_back(data[j + i * width]);
						indexData.push_back(center);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(center);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);

						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
						indexData.push_back(center);
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//내 왼쪽 노드만 나보다 LOD가 작을때
		else if (node->LOD > node->left->LOD && node->LOD <= node->down->LOD) { 
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {
					if (j == 0) {
						UINT blockHeight = (data[j + (i + 1) * width] - data[j + i * width]) / mapWidth;
						blockHeight /= 2;
						UINT center = data[j + (i + 1) * width] - mapWidth* blockHeight + blockHeight;//(data[j + (i + 1) * width] + data[j + 1 + i * width]) / 2;
						UINT left = data[j + (i + 1) * width] - mapWidth* blockHeight;//(data[j + (i + 1) * width] + data[j + i * width]) / 2;

						indexData.push_back(data[j + i * width]);
						indexData.push_back(left);
						indexData.push_back(center);

						indexData.push_back(center);
						indexData.push_back(left);
						indexData.push_back(data[j + (i + 1) * width]);

						indexData.push_back(data[j + i * width]);
						indexData.push_back(center);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(center);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);

						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
						indexData.push_back(center);
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//내 아래 노드만 나보다 LOD가 작을때
		else if (node->LOD <= node->left->LOD && node->LOD > node->down->LOD) { 
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {

					if (i == 0) {
						UINT blockWidth = (data[j + 1 + i * width] - data[j + i * width]);
						blockWidth /= 2;
						UINT center = data[j + 1 + i * width] - blockWidth + mapWidth * blockWidth ;// (data[j + (i + 1) * width] + data[j + 1 + i * width]) / 2;
						UINT down = data[j + 1 + i * width] - blockWidth ;// (data[j + 1 + i * width] + data[j + i * width]) / 2;

						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(center);

						indexData.push_back(data[j + i * width]);
						indexData.push_back(center);
						indexData.push_back(down);

						indexData.push_back(down);
						indexData.push_back(center);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(center);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);

						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
						indexData.push_back(center);
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//내 왼쪽, 아래 노드둘다 나보다 LOD가 클때
		else if (node->LOD < node->left->LOD && node->LOD < node->down->LOD) {
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {
					if (i == 0 && j == 0)
						continue;
					else if (j == 0) {
						if (i % 2 == 0) {
							indexData.push_back(data[j + i * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
						else {
							UINT lowerPoint = data[j + i * width] * 2 - data[j + (i + 1) * width];

							indexData.push_back(lowerPoint);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);

							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
					}
					else if (i == 0) {
						if (j % 2 == 0) {
							indexData.push_back(data[j + i * width]);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
						}
						else {
							UINT leftPoint = data[j + i * width] * 2 - data[j + 1 + i * width];

							indexData.push_back(leftPoint);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);

							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//내 왼쪽 노드만 나보다 LOD가 클때
		else if (node->LOD < node->left->LOD && node->LOD >= node->down->LOD) { 
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {
					if (j == 0) {
						if (i % 2 == 0) {
							indexData.push_back(data[j + i * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
						else {
							UINT lowerPoint = data[j + i * width] * 2 - data[j + (i + 1) * width];

							indexData.push_back(lowerPoint);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);

							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//내 아래 노드만 나보다 LOD가 클때
		else if (node->LOD >= node->left->LOD && node->LOD < node->down->LOD) {
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {

					if (i == 0) {
						if (j % 2 == 0) {
							indexData.push_back(data[j + i * width]);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
						}
						else {
							UINT leftPoint = data[j + i * width] * 2 - data[j + 1 + i  * width];

							indexData.push_back(leftPoint);
							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);

							indexData.push_back(data[j + (i + 1) * width]);
							indexData.push_back(data[j + 1 + (i + 1) * width]);
							indexData.push_back(data[j + 1 + i * width]);
						}
					}
					else {
						indexData.push_back(data[j + i * width]);
						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);

						indexData.push_back(data[j + (i + 1) * width]);
						indexData.push_back(data[j + 1 + (i + 1) * width]);
						indexData.push_back(data[j + 1 + i * width]);
					}
				}
			}
		}
		//같을때
		else {//if (node->LOD <= node->left->LOD && node->LOD <= node->down->LOD) {
			for (UINT i = 0; i < height - 1; i++) {
				for (UINT j = 0; j < width - 1; j++) {
					indexData.push_back(data[j + i * width]);
					indexData.push_back(data[j + (i + 1) * width]);
					indexData.push_back(data[j + 1 + i * width]);

					indexData.push_back(data[j + (i + 1) * width]);
					indexData.push_back(data[j + 1 + (i + 1) * width]);
					indexData.push_back(data[j + 1 + i * width]);

				}
				//prUINTf("\n");
			}
		}
		delete[] data;
	}

}


void QuadTree::SetNode(Node* node, UINT startX_Idx, UINT startY_Idx, UINT width, UINT height)
{
	node->tree = this;

	UINT totalVertexCount = width * height;

	node->startX = startX_Idx;
	node->startY = startY_Idx;

	node->width = width;
	node->height = height;


	//TODO 이 부분 수정할 필요 있음 (직접 버텍스에서 받아오던가 높이도 추가하던가
	node->center = D3DXVECTOR3((float)(startX_Idx + width / 2), 0, (float)(startY_Idx + height / 2));
	node->radius = (float)max(width, height)*1.2f;

	//node->endX = startX_Idx + width - 1;
	//node->endY = startY_Idx + height - 1;


	//포함된 버텍스가 없는 경우 ( width, height둘중 하나가 0이라는 소리. 있으면 안된다)
	if (totalVertexCount == 0) {
		//_asm UINT 3; // 여기 걸리면 안된다
		return;
	}
	//설정된 최대 버텍스 갯수보다 포함 버텍스가 많으면 4분할 한다
	else if (totalVertexCount > MAX_VERTEXCOUNT) {
		for (UINT i = 0; i < 4; i++) {
			Node* subNode = new Node();
			node->subNodes[i] = subNode;


			//버텍스가 짝수일 경우, 홀수일 경우 나누는 방식이 다름
			//짝수일 경우 ex) width = 6 , startX = 0
			// {0, 1, 2}, {2, 3, 4, 5} 이렇게 3, 4개로 나누어주어야 한다.		(6+1)/2,	(6+1)/2 + 1   <-- 이 1을 셋팅해주는 과정
			//홀수일 경우 ex) height = 7, startY = 0
			// {0, 1, 2, 3}, {3, 4, 5, 6} 4, 4개로 나눈다.					(7+1)/2,	(7+1)/2
			bool evenX = false, evenY = false;
			if (width % 2 == 0)
				evenX = true;
			if (height % 2 == 0)
				evenY = true;


			SetNode(subNode
				, startX_Idx + (width - 1) / 2 * (i % 2)
				, startY_Idx + (height - 1) / 2 * (i / 2)
				, (width + 1) / 2 + (UINT)evenX * (i % 2)
				, (height + 1) / 2 + (UINT)evenY * (i / 2)
			);
		}
	}
	else {
		node->SetIndices(startX_Idx, startY_Idx, mapWidth);
	}

}

void QuadTree::ClearLOD(Node * node)
{
	if (node->subNodes[0] != nullptr) {
		for (UINT i = 0; i < 4; i++) {
			DeleteTree(node->subNodes[i]);
		}
	}
	else {
		node->LOD = -1;
	}
}

void QuadTree::SetNeighbor(Node * node)
{
	if (node->subNodes[0] != nullptr) {
		for (UINT i = 0; i < 4; i++) {
			SetNeighbor(node->subNodes[i]);
		}
	}
	else {

		if (nodeIdx_right.count(nodeIdx[node]) == 1)
			node->left = nodeIdx_right[nodeIdx[node]];
		else
			node->left = node;
		if (nodeIdx_up.count(nodeIdx[node]) == 1)
			node->down = nodeIdx_up[nodeIdx[node]];
		else
			node->down = node;
	}
}

void QuadTree::DeleteTree(Node * node)
{
	if (node->subNodes[0] != nullptr) {
		for (UINT i = 0; i < 4; i++) {
			DeleteTree(node->subNodes[i]);
		}
	}
	else {
		delete node;
	}
}










/**/


void QuadTree::Node::getIndecies(UINT* & out_indices, UINT & out_width, UINT& out_height, UINT LOD)
{

	//LOD 레벨에 따라 지정된 인덱스 정리
	UINT level = 1;
	for (UINT i = 0; i < LOD; i++) {
		level *= 2;
	}

	//각 레벨의 가로, 세로 버텍스 갯수
	out_width = ceil((float)(width - 1) / (float)level) + 1;
	out_height = ceil((float)(height - 1) / (float)level) + 1;



	UINT out_count = out_width * out_height;
	out_indices = new UINT[out_count];

	//그려야 할 버텍스 셋팅
	UINT indicesCnt = 0;
	for (UINT i = 0; i < height; i++) {
		for (UINT j = 0; j < width; j++) {
			if (j == width - 1 && i == height - 1) {
				out_indices[indicesCnt++] = indices[j + i * width];
			}
			else if (j % level != 0) {
				if (j == width - 1 && i % level == 0)
					out_indices[indicesCnt++] = indices[j + i * width];
				else
					continue;
			}
			else if (i % level != 0) {
				if (i == height - 1 && j % level == 0)
					out_indices[indicesCnt++] = indices[j + i * width];
				else
					continue;
			}
			else
				out_indices[indicesCnt++] = indices[j + i * width];
		}
	}

}

void QuadTree::Node::SetIndices(UINT startX_Idx, UINT startY_Idx,  UINT mapWidth)
{
	//("StartX : %d\nStartY : %d\nWidth : %d\nHeight : %d\n\n", startX_Idx, startY_Idx, width, height);
	indices = new UINT[width*height];


	
	UINT count = 0;
	for (UINT i = 0; i < height; i++) {
		for (UINT j = 0; j < width; j++) {
			indices[count++] = startX_Idx + j + (startY_Idx + i) * mapWidth;
			//prUINTf("%3d ", startX_Idx + j + (startY_Idx + i) * mapWidth);
		}
		//prUINTf("\n");
	}
	//prUINTf("\n");
	//index = startX_Idx + startY_Idx * mapWidth;
	tree->nodeIdx[this] = startX_Idx + startY_Idx * mapWidth;
	tree->nodeIdx_right[startX_Idx + width - 1 +startY_Idx * mapWidth] = this;
	tree->nodeIdx_up[startX_Idx + (startY_Idx + height - 1) * mapWidth] = this;
}


