#include "../stdafx.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation() 
	: name(L""), keyFrameCount(0), frameRate(0), defaultFrameRate(0)
	, rootType(RootType::RootType_Default), isLoop(true), isEnd(false)
{
}

ModelAnimation::ModelAnimation(wstring name, int frameCount, float defaultFrameRate)
	: name(name), keyFrameCount(frameCount), frameRate(defaultFrameRate), defaultFrameRate(defaultFrameRate)
	, rootType(RootType::RootType_Default), isLoop(true), isEnd(false)
{
	
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Export(BinaryWriter * bw)
{
	bw->Write(name);

	bw->Write((int)rootType);

	bw->Write(keyFrameCount);
	bw->Write(frameRate);
	bw->Write(defaultFrameRate);

	bw->Write(isLoop);
	bw->Write(isEnd);
}

void ModelAnimation::Import(BinaryReader * br)
{
	name = br->WString();

	rootType  = (RootType)br->Int();

	keyFrameCount = br->Int();
	frameRate = br->Float();
	defaultFrameRate = br->Float();

	isLoop = br->Bool();
	isEnd = br->Bool();
}
