#pragma once

class Sampler
{
public:
	Sampler();
	~Sampler();

	void SetDefault();
	
private:

	ID3D11SamplerState** samplerState;
};