#pragma once

enum class RootType
{
	RootType_Default = 0,
	RootType_Fixed,
	RootType_FirstFrame,
};

enum class FBXLoadType
{
	FBXLoadType_Mesh = 0,
	FBXLoadType_Skeleton,
	FBXLoadType_Material,
	FBXLoadType_Animation
};

enum class FBXExportType
{
	FBXExportType_Mesh = 0,
	FBXExportType_Skeleton,
	FBXExportType_Material,
	FBXExportType_Animation
};