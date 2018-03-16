#include "../stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox()
{

}

BoundingBox::~BoundingBox()
{

}

BoundingBox BoundingBox::CreateAABB(D3DXVECTOR3 vtx[8], D3DXMATRIX matrix)
{


	D3DXVECTOR3 vector[8], vector2[8];
	float minx, miny, minz, maxx, maxy, maxz;
	minx = miny = minz = 9999999.0f;
	maxx = maxy = maxz = -9999999.0f;
	for (UINT i = 0; i < 8; i++) {
		D3DXVec3TransformCoord(&vector[i], &vtx[i], &matrix);
		//maxx = max(maxx, vtx[i].x); minx = min(minx, vtx[i].x);
		//maxy = max(maxy, vtx[i].y); miny = min(miny, vtx[i].y);
		//maxz = max(maxz, vtx[i].z); minz = min(minz, vtx[i].z);
	}


	for (UINT i = 0; i < 8; i++) {
		//D3DXVec3TransformCoord(&vtx[i], &vtx[i], &matrix);
		maxx = max(maxx, vector[i].x); minx = min(minx, vector[i].x);
		maxy = max(maxy, vector[i].y); miny = min(miny, vector[i].y);
		maxz = max(maxz, vector[i].z); minz = min(minz, vector[i].z);
	}
	BoundingBox output;
	output.min = D3DXVECTOR3(minx, miny, minz);
	output.max = D3DXVECTOR3(maxx, maxy, maxz);

	return output;
}
