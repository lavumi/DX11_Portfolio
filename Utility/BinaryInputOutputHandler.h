#pragma once
#include "BinaryFile.h"

class BinaryInputOutputHandler
{
public:
	virtual ~BinaryInputOutputHandler() {};

	virtual void Export(BinaryWriter* bw) = 0;
	virtual void Import(BinaryReader* br) = 0;
};