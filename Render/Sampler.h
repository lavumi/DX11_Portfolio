#pragma once

class Sampler
{
public:
	static Sampler* Get();
	static void Delete();

	void SetDefault();


private:
	static Sampler* instance;

	Sampler();
	~Sampler();

	ID3D11SamplerState** samplerState;
};