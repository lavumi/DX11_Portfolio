#pragma once

class Camera;
class iFallowCamera {
public:
	virtual ~iFallowCamera() {
	};
	virtual void SetFallowCamera() = 0;
	virtual void GetPosition(D3DXVECTOR3*& pos)=0;
};
