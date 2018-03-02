#include "../stdafx.h"
#include "ModelScene.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelBuffer.h"
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelSkeleton.h"
#include "ModelSkeletonBone.h"
#include "ModelBoneWeights.h"

wstring ModelScene::TexturePath = L"";

ModelScene::ModelScene()
	: manager(NULL), scene(NULL), converter(NULL), importer(NULL)	, skeleton(NULL)
{

	//manager = FbxManager::Create();
	// TexturePath의 상대경로 저장
	WCHAR folder[255];
	GetCurrentDirectory(255, folder);
	
	TexturePath = folder;
	TexturePath += L"\\Contents\\Models\\Textures\\";
	
	modelBuffer = new ModelBuffer();
	animationController = new ModelAnimationController();

	D3DXMatrixScaling(&scaleBias, 0.01f, 0.01f, 0.01f);
}

ModelScene::~ModelScene()
{
	for each(ModelMaterial* temp in materials)
		SAFE_DELETE(temp);
	
	for each(Model* temp in models)
		SAFE_DELETE(temp);
	
	SAFE_DELETE(modelBuffer);
	
	if (scene != NULL)scene->Destroy();
	if (manager != NULL)manager->Destroy();
}

void ModelScene::SetAni(int index)
{
	animationController->SetCurrentAnimation(index);
}

/****************************************************************************************************
@brief
Fbx SDK 의 메모리 관리를 위한 Manager 생성 
Fbx File을 Impoter를 통하여 import시킨 후 initialize
File의 Scene정보(Material, Skeleton, Mesh, Animation) 로드
****************************************************************************************************/
void ModelScene::LoadFbx(wstring filePath)
{
	this->filePath = filePath;

	manager = FbxManager::Create();
	assert(manager != NULL);

	scene = FbxScene::Create(manager, "");
	assert(scene != NULL);

	string tempFile = String::WStringToString(filePath);

	//SDK의 버전을 얻어온다.
	int sdkMajor, sdkMinor, sdkRevision;
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);
	
	//임포터 생성
	importer = FbxImporter::Create(manager, "");
	bool status = importer->Initialize(tempFile.c_str(), -1, NULL);
	assert(status == true);
	
	status = importer->Import(scene);
	assert(status == true);

	converter = new FbxGeometryConverter(manager);
	{
		ProcessScene();
	}
	SAFE_DELETE(converter);

	importer->Destroy();
}

void ModelScene::LoadAniFbx(wstring filePath, wstring aniName)
{
	this->filePath = filePath;

	manager = FbxManager::Create();
	assert(manager != NULL);

	scene = FbxScene::Create(manager, "");
	assert(scene != NULL);

	string tempFile = String::WStringToString(filePath);

	//SDK의 버전을 얻어온다.
	int sdkMajor, sdkMinor, sdkRevision;
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

	//임포터 생성
	importer = FbxImporter::Create(manager, "");
	bool status = importer->Initialize(tempFile.c_str(), -1, NULL);
	assert(status == true);

	status = importer->Import(scene);
	assert(status == true);

	converter = new FbxGeometryConverter(manager);
	{
		ProcessAnimations(aniName);
	}
	SAFE_DELETE(converter);

	importer->Destroy();
}

void ModelScene::SaveBinary(wstring filePath, wstring fileName)
{
	wstring totalFileName = filePath + fileName;

	ExportMaterials(totalFileName + L"_material.bin");
	ExportSkeleton(totalFileName + L"_skeleton.bin");
	ExportModels(totalFileName + L"_models.bin");
	ExportAnimations(totalFileName + L"_animations.bin");
}

void ModelScene::LoadBinary(wstring filePath, wstring fileName)
{
	wstring totalFileName = filePath + fileName;

	ImportMaterials(totalFileName + L"_material.bin");
	ImportSkeleton(totalFileName + L"_skeleton.bin");
	ImportModels(totalFileName + L"_models.bin");
	ImportAnimations(totalFileName + L"_animations.bin");
}

ModelAnimation * ModelScene::GetAnimation(wstring animName)
{
	map<wstring, ModelAnimation*>::iterator iter;
	iter = animations.find(animName);
	if (iter != animations.end())
		return iter->second;

	return NULL;
}

void ModelScene::Update(D3DXMATRIX* worldPos)
{
	bool isAnimated = false;

	//if (animationController != NULL)
	{
		isAnimated = animationController->GetAnimationCount() > 0;

		if (isAnimated == true)
		{
			animationController->Update();

			// Skeleton 정보 갱신
			if (skeleton != NULL)
			{
				skeleton->UpdateAnimation(animationController);
			}

			// Skeleton 정보에 따라 Mesh정보 갱신
			for each(Model* model in models)
				model->UpdateAnimation(animationController);
		}
	}

	if (skeleton != NULL)
	{
		D3DXMATRIX* bonesArray = skeleton->GetSkinTransforms();
		if (bonesArray != NULL)
		{
			UINT boneCount = skeleton->GetBoneCount();
			boneCount = boneCount < ModelBoneWeights::MaxBonesPerMax ? boneCount : ModelBoneWeights::MaxBonesPerMax;

			modelBuffer->SetBoneArray(bonesArray, boneCount);
		}
	}

	modelBuffer->SetSkinning(isAnimated);
	modelBuffer->Update();

	for each(Model* model in models)
		model->Update(isAnimated, worldPos);
}

void ModelScene::Render(FBXModelShader* shader)
{
	modelBuffer->SetVSBuffer(2);

	for each(Model* model in models)
		model->Render(shader);
}



/****************************************************************************************************
@brief
Material, Skelton, Mesh, Animation 정보를 Top->Down 방식의 재귀적 순회로 읽어들임
****************************************************************************************************/
void ModelScene::ProcessScene()
{
	ProcessMaterial();
	ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eSkeleton);
	ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eMesh);
	ProcessAnimations();
	
}

/****************************************************************************************************
@brief
Scene안의 모든 Material 정보를 읽어, 각각의 Material 정보를 ModelMaterial Class에 저장
****************************************************************************************************/
void ModelScene::ProcessMaterial()
{
	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* temp = scene->GetMaterial(i);
		
		bool isExist = false;
		for (size_t i = 0; i < fbxMaterials.size(); i++)
		{
			if (*temp == *fbxMaterials[i]) isExist = true;
		}

		if (isExist == false)
		{
			ModelMaterial* material = new ModelMaterial(i, temp);

			materials.push_back(material);
			fbxMaterials.push_back(temp);
		}
	}
}

/****************************************************************************************************
@brief
Node의 정보를 로드
각 Node의 속성(Skeelton, Mesh 외의 나머지 속성은 불필요하므로 로드하지 않음)별로 순회
함수 내에서 재귀적으로 모든 자식 Node에 대하여 수행

@param FbxNode* node 최초 실행시 Root Node로 한번 실행, 이후 재귀적으로 자식 노드를 받음
@param FbxNodeAttribute::EType attribute 노드의 속성(Skeleton, Mesh...)
****************************************************************************************************/
void ModelScene::ProcessNode(FbxNode * node, FbxNodeAttribute::EType attribute)
{	
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == attribute)
		{
			switch (nodeAttribute->GetAttributeType())
			{
				case FbxNodeAttribute::eSkeleton:
				{
					ProcessSkeleton(node);
				}	
				break;

				case FbxNodeAttribute::eMesh:
				{
					ProcessMesh(node);
				}	
				break;

				case FbxNodeAttribute::eMarker:
				case FbxNodeAttribute::eNurbs:
				case FbxNodeAttribute::ePatch:
				case FbxNodeAttribute::eCamera:
				case FbxNodeAttribute::eLight:
				case FbxNodeAttribute::eNull:
					break;
			}//switch
		}//if
	}//if(nodeAttribute)

	for (int i = 0; i < node->GetChildCount(); i++)
		ProcessNode(node->GetChild(i), attribute);
}

/****************************************************************************************************
@brief
Bone의 계층구조를 가지는 Skeleton 정보를 로드하여 ModelSkeleton Class에 저장
****************************************************************************************************/
void ModelScene::ProcessSkeleton(FbxNode * node)
{
	FbxSkeleton* fbxSkeleton = node->GetSkeleton();
	if (fbxSkeleton == NULL)
		return;

	if (skeleton == NULL)
		skeleton = new ModelSkeleton();


	int parentBoneIndex = -1;
	FbxNode* parentNode = node->GetParent();
	if (parentNode != NULL)
	{
		wstring boneName = String::StringToWString(parentNode->GetName());
		parentBoneIndex = skeleton->FindBoneIndex(boneName);
	}

	// ModelSkeleton Class에 각 Bone의 정보를 ModelSkeletonBone Class로 저장
	wstring boneName = String::StringToWString(node->GetName());
	ModelSkeletonBone* skeletonBone = new ModelSkeletonBone(boneName, parentBoneIndex);
	skeleton->AddSkeletonBone(skeletonBone);
}

/****************************************************************************************************
@brief
Mesh 정보를 로드하여 Model Class에 저장
각 Mesh 정보(Global Transform, Geomatric Offset)를 Model Class에
각 Vertex의 정보(Pos, Normal, Uv, BoneWeight)는 Material 별로 나뉘어 Model Class안의 Part Class에 저장
****************************************************************************************************/
void ModelScene::ProcessMesh(FbxNode * node)
{
	// Mesh 정보를 읽어올 때, 기본 구성 단위를 삼각형으로 읽기
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	converter->Triangulate(attribute, true, true);

	FbxMesh* mesh = node->GetMesh();
	assert(mesh != NULL);

	int vertexCount = mesh->GetControlPointsCount();
	assert(vertexCount > 0);

	// Vertex의 수만큼 ModelBoneWeights Class를 할당
	vector<ModelBoneWeights> boneWeights(vertexCount, ModelBoneWeights());
	ProcessBoneWeights(mesh, boneWeights);

	// BoneWeights의 정보를 담고있는 ModelBuffer는 Model -> Part까지 전달
	Model* model = new Model(this, String::StringToWString(node->GetName()));

	// Control Point는 Vertex의 물리적인 정보, (i.e. 육면체의 경우 4개)
	FbxVector4* controlPoints = mesh->GetControlPoints();

	// Mesh의 Polygon(삼각형)수만큼 반복
	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		ModelMaterial* material = NULL;

		// Polygon의 세 점
		for (int vi = 0; vi < 3; vi++)
		{
			// i번째 Polygon의 vi(0~2)번째 Vertex의 Vertex Index를 구함
			int vertexIndex = mesh->GetPolygonVertex(i, vi);
			if (vertexIndex < 0 || vertexIndex >= vertexCount)
				continue;

			if (material == NULL)
				material = LinkMaterialWithPolygon(mesh, 0, i, 0, vertexIndex);

			FbxVector4 fbxPos = controlPoints[vertexIndex];
			FbxVector4 fbxNormal;
			mesh->GetPolygonVertexNormal(i, vi, fbxNormal);
			fbxNormal.Normalize();

			D3DXVECTOR3 vecPos = ModelUtility::ToVector3(fbxPos);
			D3DXVECTOR3 vecNormal = ModelUtility::ToVector3(fbxNormal);

			D3DXVECTOR2 vecUv = GetUV(mesh, 0, i, vi, vertexIndex);

			// Model 에 Vertex정보를 추가(Model에서 Material별로 나누어 각 Part에 추가)
			model->AddVertex(material, vecPos, vecNormal, vecUv, boneWeights[vertexIndex]);
		}//for(vi)
	}//for(i)

	 // Mesh의 Global TransForm(루트->모델) 을 AbsoluteTransform에 저장
	D3DXMATRIX temp = GetAbsoluteTransformFromCurrentTake(node, FbxTime(0));
	model->SetAnimationTransform(temp);
	// Mesh의 GeobetrixOffset을 추가(대부분의 경우 Identity Matrix)
	model->SetGeometricOffset(scaleBias * GetGeometricOffset(node));

	model->CreateData();
	model->CreateBuffer();

	models.push_back(model);
}

/****************************************************************************************************
@brief
Animation 정보를 로드하여 ModelAnimationController Class 에 저장
****************************************************************************************************/
void ModelScene::ProcessAnimations(wstring aniName)
{
	FbxNode* rootNode = scene->GetRootNode();
	if (rootNode == NULL) return;

	// Fbx File 내에 설정된 Frame Rate(초당 실행할 Frame수)를 로드
	float frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

	// animationArray에 Animation 이름을 로드하여 저장
	FbxArray<FbxString *> animationArray;
	FbxDocument* document = dynamic_cast<FbxDocument *>(scene);
	if (document != NULL)
		document->FillAnimStackNameArray(animationArray);

	UINT animationCount = importer->GetAnimStackCount();
	// importer로 부터 FBX에 저장된 모든 Animation 정보를 로드
	for (UINT i = 0; i < animationCount; i++)
	{
		FbxTakeInfo* animationInfo = importer->GetTakeInfo(i);
		wstring animationName;
		if (aniName == L"")
			animationName = String::StringToWString(animationInfo->mImportName.Buffer());
		else
			animationName = aniName;

		// FbxTimeSpan: 구간 시간을 저장하는 Class
		FbxTimeSpan span = animationInfo->mLocalTimeSpan;
	
		// 초 단위로 시작, 끝 시간을 로드하여 할당
		double startTime = span.GetStart().GetSecondDouble();
		double endTime = span.GetStop().GetSecondDouble();

		if (startTime < endTime)
		{
			// KeyFrames의 수 = Animation 실행 시간(초) * 초당 Frmae 수
			int keyFrameCount = (int)((endTime - startTime) * (double)frameRate);

			// AnimationController에 ModelAnimation Class로 정보를 저장
			ModelAnimation* animation = new ModelAnimation(animationName, keyFrameCount, frameRate);
			AddAnimation(animation);

			animationController->AddAnimation(animation);
			
			// 기본 Animation 정보를 이용하여, Skeleton, Mesh의 Animation 정보 계산
			ProcessAnimation(rootNode, animationName, frameRate, (float)startTime, (float)endTime);
		}
	}
	animationArray.Clear();

	// 마지막에 추가된 애니메이션을 현재 애니메이션으로 설정 후 플레이
	animationController->SetCurrentAnimation(animationCount-1);
	animationController->Play();

	//TODO: defualt 스택 세팅
}


/****************************************************************************************************
@brief
해당 Animation의 각 Bone과 Mesh의 모든 KeyFrame 정보를 계산하여 animationKeyFrames에 할당하여
각 Bone과 Mesh에 해당하는 ModelSkeletonBone, Model Class에 저장
****************************************************************************************************/
void ModelScene::ProcessAnimation(FbxNode * node, wstring animationName, float frameRate, float start, float stop)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			if (skeleton != NULL)
			{
				wstring boneName = String::StringToWString(node->GetName());
				ModelSkeletonBone* bone = skeleton->FindBone(boneName);

				if (bone != NULL)
				{
					ModelAnimationKeyFrames* animationKeyFrames = new ModelAnimationKeyFrames(animationName);

					double time = 0;
					// Animation의 실행 시간 안에서, Frame Time 마다 KeyFrame을 추가
					// 각 KeyFrame Class에 해당 Bone의 해당 시간에서의 Local Transform Matrix(부모 본-> 자신 본)를 로드하여 저장

					while (time <= (double)stop)
					{
						FbxTime animationTime;
						animationTime.SetSecondDouble(time);
						
						//// Bone 자신의 Global Transform(자신 본 -> 루트)을 저장
						//D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, animationTime);
						//// 부모 Bone 의 Global Transform(부모 본 -> 루트)을 저장
						//D3DXMATRIX matParentAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node->GetParent(), animationTime);
						//
						//// 부모 Bone 의 Global Transform(부모 본 -> 루트)의 Inverse Transform(루트 -> 부모 본)를 저장
						//D3DXMATRIX matInvParentAbsoluteTransform;
						//D3DXMatrixInverse(&matInvParentAbsoluteTransform, NULL, &matParentAbsoluteTransform);
						//
						//// matTransform = (자신 본 -> 루트) * (루트 -> 부모 본) == (자신 본 -> 부모 본)
						//// 부모 Bone 좌표계에서의 Bone의 Transform Matrix
						//D3DXMATRIX localTransform = matAbsoluteTransform * matInvParentAbsoluteTransform;
						

						// Local Transform = 부모 Bone의 Global Trasforn 의 Inverse Transform * 자신 Bone의 Global Transform
						D3DXMATRIX localTransform = GetLocalTransformFromCurrentTake(node, animationTime);
						animationKeyFrames->AddKeyFrame(localTransform);

						time += 1.0f / frameRate;
					}

					bone->AddAnimationKeyFrames(animationKeyFrames);
				}//if(bone)
			}//if(skeleton)
		}
		else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			Model* model = NULL;
			for each(Model* tempModel in models)
			{
				wstring modelName = String::StringToWString(node->GetName());
				if (modelName == tempModel->GetName())
				{
					model = tempModel;

					break;
				}
			}

			if (model != NULL)
			{
				ModelAnimationKeyFrames* animationKeyFrames = new ModelAnimationKeyFrames(animationName);

				double time = 0;
				while (time <= (double)stop)
				{
					FbxTime animationTime;
					animationTime.SetSecondDouble(time);

					// Mesh는 모두 GlobalTransform Matrix(루트->매쉬) 정보로 저장
					D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, animationTime);
					animationKeyFrames->AddKeyFrame(matAbsoluteTransform);

					time += 1.0f / frameRate;
				}

				model->AddAnimationKeyFrames(animationKeyFrames);
			}
		}//if(nodeAttribute->GetAttributeType())
	}//if(nodeAttribute)

	// 자식 node(Bone)을 재귀적으로 순회
	for (int i = 0; i < node->GetChildCount(); ++i)
		ProcessAnimation(node->GetChild(i), animationName, frameRate, start, stop);
}

/****************************************************************************************************
@brief
Mesh의 Skin 정보를 로드하여, 해당 Mesh가 각 Bone에 대하여 얼만큼의 영향을 받는지(Weight)를 로드
****************************************************************************************************/
void ModelScene::ProcessBoneWeights(FbxMesh * mesh, vector<ModelBoneWeights>& meshBoneWeights)
{
	if (skeleton == NULL)
		return;

	// Deformer는 주로 Scene에서 하나만 존재
	for (int i = 0; i < mesh->GetDeformerCount(); ++i)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == NULL)
			continue;

		// Deformer의 타입이 Skin일 경우, 즉 Skinned Mesh일 경우
		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			
			FbxSkin* skin = (FbxSkin *)deformer;

			if (skin == NULL)
				continue;

			ProcessBoneWeights(skin, meshBoneWeights);
		}
	}
}

/****************************************************************************************************
 @brief
 Skin 정보를 로드하여, Mesh의 해당 Vertex가 각 Bone에 대하여 얼만큼의 영향을 받는지(Weight)를
 ModelBoneWeights Class의 Vector에 저장(크기: VertexCount, Index: VertexIndex)
****************************************************************************************************/
void ModelScene::ProcessBoneWeights(FbxSkin * skin, vector<ModelBoneWeights>& meshBoneWeights)
{
	FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;

	int clusterCount = skin->GetClusterCount();
	vector<ModelBoneWeights> skinBoneWeights(meshBoneWeights.size(), ModelBoneWeights());
	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);
		if (cluster == NULL)
			continue;

		linkMode = cluster->GetLinkMode();
		FbxNode* pLinkNode = cluster->GetLink();

		if (pLinkNode == NULL)
			continue;

		wstring lineNodeName = String::StringToWString(pLinkNode->GetName());
		int nBoneIndex = skeleton->FindBoneIndex(lineNodeName);
		if (nBoneIndex < 0)
			continue;

		ModelSkeletonBone* skeletonBone = skeleton->GetSkeletonBone(nBoneIndex);

		FbxAMatrix matClusterTransformMatrix;
		FbxAMatrix matClusterLinkTransformMatrix;
		cluster->GetTransformMatrix(matClusterTransformMatrix); // 매쉬의 Transform Matrix
		cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix); // 본의 Transform Matrix

																		//TODO: 체크
		skeletonBone->SetBindPoseTransform(ModelUtility::ToMatrix(matClusterLinkTransformMatrix)); // 본->루트
		skeletonBone->SetBoneReferenceTransform(ModelUtility::ToMatrix(matClusterTransformMatrix)); // 매쉬->루트

		int* indices = cluster->GetControlPointIndices(); //해당 Bone에 영향을 받는 모든 Vertex Index값의 배열
		double* weights = cluster->GetControlPointWeights(); // 각 Vertex Index가 갖는 Weight값의 배열

															 // indices[j]의 Index를 갖는 Vertex에 영향을 주는 Bone의 Idx와, 그 수치인 Weights를
															 // ModelBoneWeights Class로 형식으로 추가
		for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
			skinBoneWeights[indices[j]].AddBoneWeight(nBoneIndex, (float)weights[j]);
	}

	// Weight의 모드
	switch (linkMode)
	{
		case FbxCluster::eNormalize: // 모든 Weight의 합이 1 로 정규화
			{
				for (int i = 0; i < (int)skinBoneWeights.size(); ++i)
					skinBoneWeights[i].Normalize();
			}
			break;

		case FbxCluster::eAdditive: // 그대로 합산
			break;

		case FbxCluster::eTotalOne: // 모든 Weight의 합이 1 이어야 함(정규화 되지 않음)
			break;
	}

	for (size_t i = 0; i < meshBoneWeights.size(); i++)
		meshBoneWeights[i].AddBoneWeights(skinBoneWeights[i]);
}

/****************************************************************************************************
 @biref
 해당 Vertex Index의 Material 정보를 ModelMaterial Class 형식으로 반환
****************************************************************************************************/
ModelMaterial * ModelScene::LinkMaterialWithPolygon(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	if (layerIndex < 0 || layerIndex > mesh->GetLayerCount())
		return NULL;

	
	FbxNode* node = mesh->GetNode();
	if (node == NULL) return NULL;

	FbxLayerElementMaterial* fbxMaterial = mesh->GetLayer(layerIndex)->GetMaterials();
	if (fbxMaterial == NULL) return NULL;

	int mappingIndex = GetMappingIndex
	(
		fbxMaterial->GetMappingMode()
		, polygonIndex
		, 0
		, vertexIndex
	);
	if (mappingIndex < 0) return NULL;

	// Material이 Vertex Index에 적용된 모드를 로드한 후,
	// 이에 맞도록 mappingIndex를 계산 후, GetMaterial로 해당 Vertex의 Material 을 반환
	FbxLayerElement::EReferenceMode refMode = fbxMaterial->GetReferenceMode();
	if (refMode == FbxLayerElement::eDirect)
	{
		if (mappingIndex < node->GetMaterialCount())
			return GetMaterial(node->GetMaterial(mappingIndex));
	}
	else if (refMode == FbxLayerElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<int>& indexArr = fbxMaterial->GetIndexArray();

		if (mappingIndex < indexArr.GetCount())
		{
			int tempIndex = indexArr.GetAt(mappingIndex);

			if (tempIndex < node->GetMaterialCount())
				return GetMaterial(node->GetMaterial(tempIndex));
		}//if(mappingIndex)
	}//if(refMode)

	return NULL;
}

/****************************************************************************************************
@biref
MappingMode에 따른 Index를 계산하여 반환
****************************************************************************************************/
int ModelScene::GetMappingIndex(FbxLayerElement::EMappingMode mode, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	switch (mode)
	{
		case FbxLayerElement::eAllSame: 
			return 0;
		case FbxLayerElement::eByControlPoint: 
			return vertexIndex;
		case FbxLayerElement::eByPolygonVertex: 
			return polygonIndex * 3 + polygonVertexIndex;
		case FbxLayerElement::eByPolygon:
			return polygonIndex;
	}
	
	return -1;
}

/****************************************************************************************************
@biref
해당 Vertex Index의 UV값을 로드하여 반환
****************************************************************************************************/
D3DXVECTOR2 ModelScene::GetUV(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	int layerCount = mesh->GetLayerCount();
	if (layerIndex >= layerCount) return D3DXVECTOR2();

	FbxLayer* layer = mesh->GetLayer(layerIndex);
	if(layer == NULL) return D3DXVECTOR2();

	// Diffuse Texture의 Uv로드
	FbxLayerElementUV* uv = layer->GetUVs(FbxLayerElement::eTextureDiffuse);
	if(uv == NULL) return D3DXVECTOR2();


	FbxLayerElement::EMappingMode mappingMode = uv->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = uv->GetReferenceMode();

	const FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uv->GetDirectArray();
	const FbxLayerElementArrayTemplate<int>& uvIndexArray = uv->GetIndexArray();

	int mappingIndex = 0;
	if (mappingMode == FbxLayerElement::eByControlPoint)
	{
		mappingIndex = vertexIndex;

		if (refMode == FbxLayerElement::eDirect)
		{
			if(mappingIndex < uvArray.GetCount())
				return ModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}
		else if (refMode == FbxLayerElement::eIndexToDirect)
		{
			if (mappingIndex < uvArray.GetCount())
			{
				int tempIndex = uvIndexArray.GetAt(mappingIndex);

				if(tempIndex < uvIndexArray.GetCount())
					return ModelUtility::ToUv(uvArray.GetAt(tempIndex));
			}
		}//if(refMode)
	}
	else if (mappingMode == FbxLayerElement::eByPolygonVertex)
	{
		mappingIndex = mesh->GetTextureUVIndex
		(
			polygonIndex
			, polygonVertexIndex
			, FbxLayerElement::eTextureDiffuse
		);

		switch (refMode)
		{
			case FbxLayerElement::eDirect:
			case FbxLayerElement::eIndexToDirect:
				if (mappingIndex < uvArray.GetCount())
					return ModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}//switch(refMode)
	}//if(mappingMode)

	return D3DXVECTOR2();
}

/****************************************************************************************************
 @biref
 FbxSurfaceMaterial 으로 저장된 값을 ModelMaterial Class로 변환
****************************************************************************************************/
ModelMaterial * ModelScene::GetMaterial(FbxSurfaceMaterial * fbxMaterial)
{
	for (size_t i = 0; i < fbxMaterials.size(); i++)
	{
		if (fbxMaterials[i] == fbxMaterial)
			return materials[i];
	}

	return NULL;
}

/****************************************************************************************************
 @biref
 Node의 Offset Transform Matrix를 반환
****************************************************************************************************/
D3DXMATRIX ModelScene::GetGeometricOffset(FbxNode * node)
{
	if (node == NULL)
	{
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);

		return matrix;
	}

	FbxVector4 T = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 R = node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 S = node->GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix offset = FbxAMatrix(T, R, S);

	return ModelUtility::ToMatrix(offset);
}

/****************************************************************************************************
 @biref
 임의의 time에서 node의 Global Transform Matrix(노드->루트)를 로드하여 반환
****************************************************************************************************/
D3DXMATRIX ModelScene::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}
	
	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);
	
	return ModelUtility::ToMatrix(matrix);
}

/****************************************************************************************************
 @biref
 임의의 time에서 node의 Local Transform Matrix(자신->부모)를 로드하여 반환
****************************************************************************************************/
D3DXMATRIX ModelScene::GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if( node == NULL )
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	FbxAMatrix matrix = node->EvaluateLocalTransform(time);

	return ModelUtility::ToMatrix(matrix);
}

void ModelScene::AddAnimation(ModelAnimation * animation)
{
	wstring animName = animation->GetName();
	if (animations.count(animName) > 0)
		return;
	else
		animations.insert({ animName, animation });
}


// Binary Export 관련
void ModelScene::ExportMaterials(wstring fileName)
{
	BinaryWriter bMaterials = BinaryWriter();
	bMaterials.Open(fileName);
	{
		bMaterials.Write(materials);
	}
	bMaterials.Close();
}

void ModelScene::ExportSkeleton(wstring fileName)
{
	BinaryWriter bSkeleton = BinaryWriter();
	bSkeleton.Open(fileName);
	{
		bSkeleton.Write(skeleton);
	}
	bSkeleton.Close();
}

void ModelScene::ExportModels(wstring fileName)
{
	BinaryWriter bModels = BinaryWriter();
	bModels.Open(fileName);
	{
		bModels.Write(models);
	}
	bModels.Close();
}

void ModelScene::ExportAnimations(wstring fileName)
{
	BinaryWriter bAnimations = BinaryWriter();
	bAnimations.Open(fileName);
	{
		bAnimations.Write(animations);
	}
	bAnimations.Close();
}

void ModelScene::ImportMaterials(wstring fileName)
{
	BinaryReader bMaterials = BinaryReader();
	bMaterials.Open(fileName);
	{
		UINT materialsCount = bMaterials.UInt();
		materials.clear();
		materials.reserve(materialsCount);
		for (size_t i = 0; i < materialsCount; i++)
		{
			ModelMaterial* material = new ModelMaterial();
			material->Import(&bMaterials);
			materials.push_back(material);
		}
	}
	bMaterials.Close();
}

void ModelScene::ImportSkeleton(wstring fileName)
{
	BinaryReader bSkeleton = BinaryReader();
	bSkeleton.Open(fileName);
	{
		skeleton = new ModelSkeleton();
		skeleton->Import(&bSkeleton);
	}
	bSkeleton.Close();
}

void ModelScene::ImportModels(wstring fileName)
{
	BinaryReader bModels = BinaryReader();
	bModels.Open(fileName);
	{
		UINT modelsCount = bModels.UInt();
		models.clear();
		models.reserve(modelsCount);
		for (size_t i = 0; i < modelsCount; i++)
		{
			Model* model = new Model(this);
			model->Import(&bModels);
			models.push_back(model);
		}
	}
	bModels.Close();
}

void ModelScene::ImportAnimations(wstring fileName)
{
	BinaryReader bAnimations = BinaryReader();
	bAnimations.Open(fileName);
	{
		animations.clear();
		UINT animationKeyFramesCount = bAnimations.UInt();
		
		if (animationKeyFramesCount > 0 && animationController == NULL)
			animationController = new ModelAnimationController();

		for (size_t i = 0; i < animationKeyFramesCount; i++)
		{
			wstring name = bAnimations.WString();
			ModelAnimation* modelAnimation = new ModelAnimation();
			modelAnimation->Import(&bAnimations);
			animations.insert(make_pair(name, modelAnimation));

			animationController->AddAnimation(modelAnimation);
		}

		animationController->SetCurrentAnimation(animationKeyFramesCount-1);
		animationController->Play();
	}
	bAnimations.Close();
}
