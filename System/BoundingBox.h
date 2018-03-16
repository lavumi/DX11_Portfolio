#pragma once
class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	static BoundingBox CreateAABB(D3DXVECTOR3 vtx[8],  D3DXMATRIX matrix );

	D3DXVECTOR3 min, max;
};