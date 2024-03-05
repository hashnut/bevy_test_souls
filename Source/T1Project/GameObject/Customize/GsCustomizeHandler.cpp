
#include "GsCustomizeHandler.h"

//#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Util/GsTableUtil.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameObject/Define/EGsPartsType.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "Net/GsNetSendService.h"
//#include "DataSchema/Customize/GsSchemaCustomizeFacePreset.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorPalette.h"
#include "DataSchema/Customize/GsSchemaCustomizeBoneScaleSetting.h"
#include "DataSchema/Customize/GsSchemaCustomizeTattooData.h"
#include "DataSchema/GsSchemaCharacterCreateData.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/User/WeaponSocket/GsSchemaWeaponSocketInfo.h"
#include "DataSchema/GameObject/GsSchemaUserLookInfo.h"

#include "Data/GsResourceManager.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "DataSchema/GsSchemaGameMode.h"

#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "T1Project.h"
#include "Item/GsItemManager.h"
#include "ActorEx/GsWeaponActor.h"
#include "Item/GsSchemaItemCommon.h"
#include "Costume/GsCostumeData.h"

#include "ActorComponentEx/GsCostumeEffectComponent.h"
#include "ActorComponentEx/GsMeshShaderControlcomponent.h"


static const FName TargetNameCheekSize("CUS_cheek");// 볼살 Down, Up
static const FName TargetNameChinLength("CUS_jaw"); // 턱 길이 Short, Long
static const FName TargetNameChinSize("CUS_jaw"); // 턱 넓이 Narrow, Broad
static const FName TargetNameNosePosition("CUS_nose"); // 코 위치 Down, Up
static const FName TargetNameBrowPosition("CUS_eyebrow"); // 눈썹 위치 Down, Up
static const FName TargetNameBrowAngle("CUS_eyebrowRotate"); // 눈썹 각도 Out, In
static const FName TargetNameBrowLength("CUS_eyebrow"); // 눈썹 두께 Thin, Thick
static const FName TargetNameEyePosition("CUS_eye"); // 눈 위치 Down, Up
static const FName TargetNameEyeSpacing("CUS_eye"); // 눈 간격 Narrow, Wide
static const FName TargetNameEyeSize("CUS_eye"); // 눈 크키 Small, Big
static const FName TargetNameMouthPosition("CUS_mouth"); // 입 위치 Down, Up
static const FName TargetNameMouthSize("CUS_mouth"); // 입 길이 Short, Long

static const FName TargetNameHeight("Bip001"); // 키
static const FName TargetNameHead("Bip001-Head"); // 머리 크기
static const FName TargetNameChest("Bip001-Spine2"); // 가슴 크기
static const FName TargetNameWaistR("Bip001-Spine"); // 허리 크기
static const FName TargetNameWaistL("Bip001-Spine1"); // 허리 크기
static const FName TargetNamePelvis("Bip001-Pelvis"); // 골반 크기
static const FName TargetNameLegsR("Bip001-R-Thigh"); // 다리 두께 
static const FName TargetNameLegsL("Bip001-L-Thigh"); // 다리 두께 

static const FName TargetNameHairColor("Custom Base Color"); // 머리 색깔
static const FName TargetNameHairShadowColor("Custom Shadow Color"); // 머리 그림자 색깔
//static const FName TargetNameHairSpecularColor("Spec Color 0"); // 머리 스펙큘러 색깔

static const FName TargetNameSkinColor("Custom Base Color"); // 피부 색깔
static const FName TargetNameSkinShadowColor("Custom Shadow Color"); // 피부 그림자 색깔
//static const FName TargetNameSkinSpecularColor("Spec Color 0"); // 피부 스펙큘러 색깔

static const FName TargetNamePupilColor("Custom Base Color 2"); // 눈동자 색깔
static const FName TargetNamePupilShadowColor("Custom Shadow Color 2"); // 눈동자 그림자 색깔
//static const FName TargetNamePupilSpecularColor("Spec Color 2"); // 눈동자 스펙큘러 색깔

static const FName TargetNameBrowColor("Custom Base Color 1"); // 눈썹 색깔
static const FName TargetNameBrowShadowColor("Custom Shadow Color 1"); // 눈썹 그림자 색깔
//static const FName TargetNameBrowSpecularColor("Spec Color 1"); // 눈썹 스펙큘러 색깔

static const FName SetLightVectorChannel("SetLightVectorChannel");

static const FName OutlineColor0("Outline Color 0");
static const FName OutlineColor1("Outline Color 1");
static const FName OutlineColor2("Outline Color 2");
static const FName OutlineColor3("Outline Color 3");

TMap<FGsCustomizeHandler::EGsTargetNames, FName> FGsCustomizeHandler::_targetNames;

// public 함수들 ///////////////////////////////////////////////////////////////////////////////////////////////////////
void FGsCustomizeHandler::InitializeActor(AActor* Owner)
{
	_characterPlayer = Cast<AGsCharacterPlayer>(Owner);	

	InitializeBoneTargetName();

	_bonesStartScaleVector.Empty();
	_scaleDisplacementPerBone.Empty();

	_facePartsId = -1;
	_hairPartsId = -1;
	_bodyPartsId = -1;
	_weaponPartsId = -1;
	_tattooTextureId = -1;

	_isDirty = false;

	ResetEquipParts();

	// 액터가 초기화되고 나서 설정하는 데이터 인데.. 고려필요
	UpdateCustomizeDiplay();
}

void FGsCustomizeHandler::InitializeBoneTargetName()
{
	// bak1210 
	// static 변수로 선언되어 인스턴스에 공유되는 자원이다 (최초 1회만 초기화 함)
	if (_targetNames.Num() != ((int)EGsTargetNames::All - 1))
	{
		_targetNames.Add(EGsTargetNames::CheekSize, TargetNameCheekSize);
		_targetNames.Add(EGsTargetNames::ChinLength, TargetNameChinLength);
		_targetNames.Add(EGsTargetNames::ChinSize, TargetNameChinSize);
		_targetNames.Add(EGsTargetNames::NosePosition, TargetNameNosePosition);
		_targetNames.Add(EGsTargetNames::EyePosition, TargetNameEyePosition);
		_targetNames.Add(EGsTargetNames::EyeSpacing, TargetNameEyeSpacing);
		_targetNames.Add(EGsTargetNames::EyeSize, TargetNameEyeSize);
		_targetNames.Add(EGsTargetNames::BrowPosition, TargetNameBrowPosition);
		_targetNames.Add(EGsTargetNames::BrowAngle, TargetNameBrowAngle);
		_targetNames.Add(EGsTargetNames::BrowLength, TargetNameBrowLength);
		_targetNames.Add(EGsTargetNames::MouthPosition, TargetNameMouthPosition);
		_targetNames.Add(EGsTargetNames::MouthSize, TargetNameMouthSize);

		_targetNames.Add(EGsTargetNames::Height, TargetNameHeight);
		_targetNames.Add(EGsTargetNames::Head, TargetNameHead);
		_targetNames.Add(EGsTargetNames::Chest, TargetNameChest);
		_targetNames.Add(EGsTargetNames::WaistR, TargetNameWaistR);
		_targetNames.Add(EGsTargetNames::WaistL, TargetNameWaistL);
		_targetNames.Add(EGsTargetNames::Pelvis, TargetNamePelvis);
		_targetNames.Add(EGsTargetNames::LegsR, TargetNameLegsR);
		_targetNames.Add(EGsTargetNames::LegsL, TargetNameLegsL);

		_targetNames.Add(EGsTargetNames::HairColor, TargetNameHairColor);
		_targetNames.Add(EGsTargetNames::HairShadowColor, TargetNameHairShadowColor);
		/*_targetNames.Add(EGsTargetNames::HairSpecularColor, TargetNameHairSpecularColor);*/

		_targetNames.Add(EGsTargetNames::SkinColor, TargetNameSkinColor);
		_targetNames.Add(EGsTargetNames::SkinShadowColor, TargetNameSkinShadowColor);
		/*_targetNames.Add(EGsTargetNames::SkinSpecularColor, TargetNameSkinSpecularColor);*/

		_targetNames.Add(EGsTargetNames::PupilColor, TargetNamePupilColor);
		_targetNames.Add(EGsTargetNames::PupilShadowColor, TargetNamePupilShadowColor);
		//_targetNames.Add(EGsTargetNames::PupilSpecularColor, TargetNamePupilSpecularColor);

		_targetNames.Add(EGsTargetNames::BrowColor, TargetNameBrowColor);
		_targetNames.Add(EGsTargetNames::BrowShadowColor, TargetNameBrowShadowColor);
		_targetNames.Add(EGsTargetNames::SetLightVectorChannel, SetLightVectorChannel);
		//_targetNames.Add(EGsTargetNames::BrowSpecularColor, TargetNameBrowSpecularColor);
	}
}

void FGsCustomizeHandler::InitializeScaleDisplacementAllBone(CreatureGenderType InGenderType)
{
	_genderType = InGenderType;

	const FGsSchemaCustomizeBoneScaleSetting* boneScaleSetting = nullptr;
	const UGsTableCustomizeBoneScaleSetting* table = Cast<UGsTableCustomizeBoneScaleSetting>(FGsSchemaCustomizeBoneScaleSetting::GetStaticTable());
	if (nullptr == table)
		return;

	if (false == table->FindRowById(_genderType, boneScaleSetting))
		return;

	for (int32 i = 0; i < boneScaleSetting->Height.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->Height[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->Height[i], EGsUICustomizeType::HEIGHT);
	}

	for (int32 i = 0; i < boneScaleSetting->Head.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->Head[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->Head[i], EGsUICustomizeType::HEAD_SIZE);
	}

	for (int32 i = 0; i < boneScaleSetting->Chest.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->Chest[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->Chest[i], EGsUICustomizeType::CHEST_SIZE);
	}

	for (int32 i = 0; i < boneScaleSetting->Waist.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->Waist[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->Waist[i], EGsUICustomizeType::WAIST_SIZE);
	}

	for (int32 i = 0; i < boneScaleSetting->Pelvis.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->Pelvis[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->Pelvis[i], EGsUICustomizeType::PELVIS_SIZE);
	}

	for (int32 i = 0; i < boneScaleSetting->LegThickness.Num(); ++i)
	{
		CalcStartScaleVectorPerTable(&boneScaleSetting->LegThickness[i]);
		InitializeScaleDisplacementPerBone(&boneScaleSetting->LegThickness[i], EGsUICustomizeType::LEG_SIZE);
	}
}

void FGsCustomizeHandler::Update(float Delta)
{
	UpdateCustomizeDiplay();
}

void FGsCustomizeHandler::SetActor(AActor* Owner)
{
	_characterPlayer = Cast<AGsCharacterPlayer>(Owner);

	_bonesStartScaleVector.Empty();
	_scaleDisplacementPerBone.Empty();

	_isDirty = false;

	ResetEquipParts();
	InitializeBoneTargetName();
}

AGsCharacterPlayer* FGsCustomizeHandler::GetPlayer()
{
	return _characterPlayer.Get();
}

void FGsCustomizeHandler::InitializeScaleDisplacementPerBone(const FGsSchemaCustomizeBoneScaleGroup* InTable, EGsUICustomizeType InUICustomizeType)
{
	TMap<EGsUICustomizeType, FVector>* BoneScaleVectorPtr = _scaleDisplacementPerBone.Find(InTable->BoneName);
	if (BoneScaleVectorPtr)
	{
		BoneScaleVectorPtr->Add(InUICustomizeType, FVector::ZeroVector);
	}
	else
	{
		TMap<EGsUICustomizeType, FVector> BoneScaleVector;
		BoneScaleVector.Add(InUICustomizeType, FVector::ZeroVector);
		_scaleDisplacementPerBone.Add(InTable->BoneName, BoneScaleVector);
	}
}

void FGsCustomizeHandler::CalcStartScaleVectorPerTable(const FGsSchemaCustomizeBoneScaleGroup* InTable)
{
	FVector* StartVectorPtr = _bonesStartScaleVector.Find(InTable->BoneName);
	if (StartVectorPtr)
	{
		(*StartVectorPtr).X <= InTable->MinVector.X ? (*StartVectorPtr).X : (*StartVectorPtr).X = InTable->MinVector.X;
		(*StartVectorPtr).Y <= InTable->MinVector.Y ? (*StartVectorPtr).Y : (*StartVectorPtr).Y = InTable->MinVector.Y;
		(*StartVectorPtr).Z <= InTable->MinVector.Z ? (*StartVectorPtr).Z : (*StartVectorPtr).Z = InTable->MinVector.Z;
	}
	else
	{
		_bonesStartScaleVector.Add(InTable->BoneName, InTable->MinVector);
	}
}

void FGsCustomizeHandler::Finalize()
{
	_characterPlayer.Reset();
}

void FGsCustomizeHandler::SetByCustomizeUIData(FGsCustomizeUIData* InUICustomData)
{
	if (InUICustomData == nullptr) return;	

	UpdateCustomizeUIData(InUICustomData);

	_hairPartsId	= _feature.faceShape.mHairId;
	_facePartsId	= _feature.faceShape.mFaceId;
	_myFace			= true;
	_myHair			= true;
	_pelvisSize		= DEFAULT_CUSTOM_VALUE;
	_usingBoneScale = true;
	_costumeEffect	= nullptr;
	_isDirty		= true;

	// 케릭터가 nullptr 일때는 비동기 로딩중
	if (false == _characterPlayer.IsValid())
	{
		return;
	}
	UpdateCustomizeDiplay();
}

void FGsCustomizeHandler::UpdateCustomizeUIData(FGsCustomizeUIData* InUICustomData)
{
	if (InUICustomData == nullptr) return;	

	ConvertToServerFace(InUICustomData, &_feature.faceShape);
	ConvertToServerBody(InUICustomData, &_feature.bodyShape);
}

void FGsCustomizeHandler::SetCustomizeDataBonScale(const CustomizeFaceData& InFaceData, const CustomizeBodyData& InBodyData)
{
	SetCustomizeData(InFaceData, InBodyData);

	InitializeScaleDisplacementAllBone(_genderType);

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();

	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEIGHT, InBodyData.mHeight, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEAD_SIZE, InBodyData.mHeadSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::CHEST_SIZE, InBodyData.mChestSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::WAIST_SIZE, InBodyData.mWaistSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::PELVIS_SIZE, InBodyData.mPelvisSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::LEG_SIZE, InBodyData.mLegSize, CustomMeshComponent);
}

void FGsCustomizeHandler::ConvertToServerFace(FGsCustomizeUIData* InUICustomData, CustomizeFaceData* OutFaceData)
{
	if (InUICustomData == nullptr || OutFaceData == nullptr) return;

	OutFaceData->mFaceId = InUICustomData->GetFaceId();
	OutFaceData->mHairId = InUICustomData->GetHairId();
	OutFaceData->mTattooId = InUICustomData->GetTattooId();

	OutFaceData->mHairColorIndex = InUICustomData->GetHairColorIndex();
	OutFaceData->mSkinColorIndex = InUICustomData->GetSkinColorIndex();
	OutFaceData->mBrowColorIndex = InUICustomData->GetBrowColorIndex();
	OutFaceData->mPupilColorIndex = InUICustomData->GetPupilColorIndex();
}


void FGsCustomizeHandler::ConvertToServerBody(FGsCustomizeUIData* InUICustomData, CustomizeBodyData* OutBodyData)
{
	if (InUICustomData == nullptr || OutBodyData == nullptr) return;


	OutBodyData->mSkinColorIndex = InUICustomData->GetSkinColorIndex();

	OutBodyData->mHeight = InUICustomData->GetValue(EGsUICustomizeType::HEIGHT);
	OutBodyData->mHeadSize = InUICustomData->GetValue(EGsUICustomizeType::HEAD_SIZE);
	OutBodyData->mChestSize = InUICustomData->GetValue(EGsUICustomizeType::CHEST_SIZE);
	OutBodyData->mWaistSize = InUICustomData->GetValue(EGsUICustomizeType::WAIST_SIZE);
	OutBodyData->mPelvisSize = InUICustomData->GetValue(EGsUICustomizeType::PELVIS_SIZE);
	OutBodyData->mLegSize = InUICustomData->GetValue(EGsUICustomizeType::LEG_SIZE);
}

// send는 현재 로비에서 UI상에서 밖에 호출되지 않는다.
void FGsCustomizeHandler::SendGatewayReqCreateUser(const FString& Name, FGsCustomizeUIData* InUICustomData)
{
	CustomizeFaceData FaceData;
	CustomizeBodyData BodyData;

	ConvertToServerFace(InUICustomData, &FaceData);
	ConvertToServerBody(InUICustomData, &BodyData);
	
	FGsNetSendService::SendGatewayReqCreateUser(Name, _genderType, FaceData, BodyData);
}

// send는 현재 로비에서 UI상에서 밖에 호출되지 않는다.
void FGsCustomizeHandler::SendGatewayReqCustomizing(FGsCustomizeUIData* InUICustomData)
{
	CustomizeFaceData FaceData;
	CustomizeBodyData BodyData;

	ConvertToServerFace(InUICustomData, &FaceData);
	ConvertToServerBody(InUICustomData, &BodyData);
	
	FGsNetSendService::SendGatewayReqCustomizing(_genderType, FaceData, BodyData);
}

// 머리카락
void FGsCustomizeHandler::SetHairColorChange(int32 InValue)
{
	if (_characterPlayer == nullptr) return;

	InValue = (-1 != InValue) ? InValue : 0;

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentHAIR();
	if (CustomMeshComponent /*&& (_costumeParts.hairCostumeId <= 0 || !_costumeParts.hairIsOn)*/)
	{	
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::HairColor], FLinearColor(GetPaletteColor(TEXT("Base"), InValue)));
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::HairShadowColor], FLinearColor(GetPaletteColor(TEXT("Shadow"), InValue)));
		/*UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::HairSpecularColor], FLinearColor(GetPaletteColor(TEXT("Specular"), InValue)));*/

		SetOutlineParameter(CustomMeshComponent);

		CustomMeshComponent->MarkRenderStateDirty();
		/*CustomMeshComponent->RecreateRenderState_Concurrent();*/
	}
}

// 피부
void FGsCustomizeHandler::SetSkinColorChange(int32 InValue, int8 inMode)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	InValue = (-1 != InValue) ? InValue : 0;

	FLinearColor skinColor = FLinearColor(GetPaletteColor(TEXT("Base"), InValue));
	FLinearColor shadowColor = FLinearColor(GetPaletteColor(TEXT("Shadow"), InValue));
	FLinearColor SpecularColor = FLinearColor(GetPaletteColor(TEXT("Specular"), InValue));
	/*FLinearColor OutlineColor = FLinearColor(GetPaletteColor(TEXT("Outline"), InValue));*/
	
	if (inMode != 2/*body*/)
	{
		USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinColor], skinColor);
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinShadowColor], shadowColor);
		/*UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinSpecularColor], SpecularColor);*/
		UGsMeshShaderControlComponent::SetScalarParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SetLightVectorChannel], 1);
		if (nullptr != CustomMeshComponent)
		{
			SetOutlineParameter(CustomMeshComponent);
			CustomMeshComponent->MarkRenderStateDirty();
			/*CustomMeshComponent->RecreateRenderState_Concurrent();*/
		}
	}
	if (inMode != 1 /*face*/)
	{
		USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinColor], skinColor);
		UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinShadowColor], shadowColor);
		/*UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SkinSpecularColor], SpecularColor);*/
		UGsMeshShaderControlComponent::SetScalarParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::SetLightVectorChannel], 1);
		if (nullptr != CustomMeshComponent)
		{
			SetOutlineParameter(CustomMeshComponent);
			CustomMeshComponent->MarkRenderStateDirty();
			/*CustomMeshComponent->RecreateRenderState_Concurrent();*/
		}
	}
}

void FGsCustomizeHandler::ForceUpdateSkinColor(int8 inMode)
{
	SetSkinColorChange(_feature.bodyShape.mSkinColorIndex, inMode);
}

void FGsCustomizeHandler::ForceFaceAttibuteColor()
{
	SetPupilColorChange(_feature.faceShape.mPupilColorIndex);
	SetBrowColorChange(_feature.faceShape.mBrowColorIndex);
	SetTattooChange(_feature.faceShape.mTattooId);
}

// 눈동자
void FGsCustomizeHandler::SetPupilColorChange(int32 InValue)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	InValue = (-1 != InValue) ? InValue : 0;

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();

	UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::PupilColor], FLinearColor(GetPaletteColor(TEXT("Base"), InValue)));
	UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::PupilShadowColor], FLinearColor(GetPaletteColor(TEXT("Shadow"), InValue)));
	if (nullptr != CustomMeshComponent)
	{
		CustomMeshComponent->MarkRenderStateDirty();
		/*CustomMeshComponent->RecreateRenderState_Concurrent();*/
	}
}

// 눈썹
void FGsCustomizeHandler::SetBrowColorChange(int32 InValue)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	InValue = (-1 != InValue) ? InValue : 0;

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();

	UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::BrowColor], FLinearColor(GetPaletteColor(TEXT("Base"), InValue)));
	UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(CustomMeshComponent, _targetNames[EGsTargetNames::BrowShadowColor], FLinearColor(GetPaletteColor(TEXT("Shadow"), InValue)));
	if (nullptr != CustomMeshComponent)
	{
		CustomMeshComponent->MarkRenderStateDirty();
		/*CustomMeshComponent->RecreateRenderState_Concurrent();*/
	}
}

bool FGsCustomizeHandler::SetFaceChange(int32 InPartsId, bool isMyFace)
{
	if (false == _characterPlayer.IsValid())
	{
		return false;
	}

	if (InPartsId == -100)
	{
		RemoveFaceMeshBySystem();
		_equipFacePartsId = InPartsId;
		return true;
	}
	else if (_equipFacePartsId == InPartsId)
	{
		SetSkinColorChange(_feature.bodyShape.mSkinColorIndex, 1);

		if (isMyFace)
		{
			ForceFaceAttibuteColor();
		}
		return false;
	}

	if (const FGsSchemaPartsShapeInfo* Data = FGsPartsHandlerBase::GetShapeInfo(_genderType, InPartsId))
	{
		if (false == _isAsync)
		{
			USkeletalMesh* mesh = Data == nullptr ? nullptr : GResource()->LoadSync<USkeletalMesh>(Data->shapePath);
			USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
			if (CustomMeshComponent)
			{
				SetSKeletalMesh(CustomMeshComponent, mesh);
				_equipFacePartsId = InPartsId;

				SetSkinColorChange(_feature.bodyShape.mSkinColorIndex, 1);

				if (isMyFace)
				{
					ForceFaceAttibuteColor();
				}				
			}
		}
		else
		{
			UGsGameObjectBase* Owner = GetOwner();
			GResource()->LoadAsync(Data->shapePath, FStreamableDelegateParam::CreateWeakLambda(_owner, [Owner, InPartsId, isMyFace](UObject* inRes)->void
				{
					FGsCustomizeHandler* customize = Owner->GetCustomizeHandler();
					if (nullptr != inRes && nullptr != customize)
					{
						auto ptr = customize;
						ptr->AsyncFaceChangeLoadComplated(inRes);
						ptr->_equipFacePartsId = InPartsId;

						ptr->SetSkinColorChange(ptr->_feature.bodyShape.mSkinColorIndex, 1);

						if (isMyFace)
						{
							ptr->ForceFaceAttibuteColor();
						}			

					}
					
				}));
		}

		return true;
	}
	return false;
}

void FGsCustomizeHandler::AsyncFaceChangeLoadComplated(class UObject* inRes)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	USkeletalMesh* mesh = Cast<USkeletalMesh>(inRes);
	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
	if (CustomMeshComponent)
	{
		SetSKeletalMesh(CustomMeshComponent, mesh);
	}
}

bool FGsCustomizeHandler::SetBodyChange(int32 InPartsId)
{
	_bodyPartsId = InPartsId;
	return SetBodyChange(_bodyPartsId, _pelvisSize, _usingBoneScale);
}

bool FGsCustomizeHandler::SetBodyChange(int32 InPartsId, uint8 sizePelvis, bool usingBoneScale)
{
	if (false == _characterPlayer.IsValid())
	{
		return false;
	}

	if (_equipBodyPartsId == InPartsId)
	{
		//bak1210
		//UI 컨텐츠 상에서 재활용할때 생기는 이슈를 핸들링하기위함
		if (usingBoneScale)
		{
			USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();

			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEAD_SIZE,	_feature.bodyShape.mHeadSize, CustomMeshComponent);
			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEIGHT,		_feature.bodyShape.mHeight, CustomMeshComponent);
			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::CHEST_SIZE,	_feature.bodyShape.mChestSize, CustomMeshComponent);
			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::WAIST_SIZE,	_feature.bodyShape.mWaistSize, CustomMeshComponent);
			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::PELVIS_SIZE, sizePelvis, CustomMeshComponent);
			SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::LEG_SIZE,	_feature.bodyShape.mLegSize, CustomMeshComponent);
		}

		SetSkinColorChange(_feature.bodyShape.mSkinColorIndex, 2);
		return false;
	}

	if (const FGsSchemaPartsShapeInfo* Data = FGsPartsHandlerBase::GetShapeInfo(_genderType, InPartsId))
	{
		if (false == _isAsync)
		{
			USkeletalMesh* mesh = Data == nullptr ? nullptr : GResource()->LoadSync<USkeletalMesh>(Data->shapePath);
			USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();
			if (CustomMeshComponent)
			{
				SetSKeletalMesh(CustomMeshComponent, mesh);
				_equipBodyPartsId = InPartsId;					

				if (usingBoneScale)
				{
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEAD_SIZE, _feature.bodyShape.mHeadSize, CustomMeshComponent);
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEIGHT, _feature.bodyShape.mHeight, CustomMeshComponent);
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::CHEST_SIZE, _feature.bodyShape.mChestSize, CustomMeshComponent);
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::WAIST_SIZE, _feature.bodyShape.mWaistSize, CustomMeshComponent);
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::PELVIS_SIZE, sizePelvis, CustomMeshComponent);
					SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::LEG_SIZE, _feature.bodyShape.mLegSize, CustomMeshComponent);
				}

				SetSkinColorChange(_feature.bodyShape.mSkinColorIndex, 2);

				if (nullptr != _costumeEffect)
				{
					SetCostumeEffect(_costumeEffect);
				}
			}
				
		}
		else
		{
			TWeakObjectPtr<UGsGameObjectBase> Owner = GetOwner();
			GResource()->LoadAsync(Data->shapePath, FStreamableDelegateParam::CreateWeakLambda(_owner, [Owner, InPartsId, sizePelvis, usingBoneScale](UObject* inRes)->void
				{
					FGsCustomizeHandler* costomize = Owner.IsValid() ? Owner->GetCustomizeHandler() : nullptr;
					if (nullptr != inRes && nullptr != costomize)
					{
						auto ptr = costomize;
						ptr->AsyncBodyhangeLoadComplated(inRes);
						ptr->_equipBodyPartsId = InPartsId;
						
						if (ptr->_characterPlayer.IsValid())
						{
							USkeletalMeshComponent* CustomMeshComponent = ptr->_characterPlayer->GetPartsSkeletalComponentBODY();

							if (usingBoneScale)
							{
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::HEAD_SIZE, ptr->_feature.bodyShape.mHeadSize, CustomMeshComponent);
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::HEIGHT, ptr->_feature.bodyShape.mHeight, CustomMeshComponent);
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::CHEST_SIZE, ptr->_feature.bodyShape.mChestSize, CustomMeshComponent);
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::WAIST_SIZE, ptr->_feature.bodyShape.mWaistSize, CustomMeshComponent);
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::PELVIS_SIZE, sizePelvis, CustomMeshComponent);
								ptr->SetBodyBoneScaleChange(ptr->_genderType, EGsUICustomizeType::LEG_SIZE, ptr->_feature.bodyShape.mLegSize, CustomMeshComponent);
							}

							ptr->SetSkinColorChange(ptr->_feature.bodyShape.mSkinColorIndex, 2);

							if (nullptr != ptr->_costumeEffect)
							{
								ptr->SetCostumeEffect(ptr->_costumeEffect);
							}
						}
					}					
				}));

		}
		return true;
	}
	return false;
}

void FGsCustomizeHandler::AsyncBodyhangeLoadComplated(UObject* inRes)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}
	
	USkeletalMesh* mesh = Cast<USkeletalMesh>(inRes);
	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();

	// [BY] 어떤 상황에서인지 CustomMeshComponent 가 제거되었음 크래시 이슈이기 때문에 일단 예외처리
	if (_characterPlayer->IsPendingKill() || CustomMeshComponent->IsPendingKill())
	{
		GSLOG(Error, TEXT("[Customize] _characterPlayer or CustomMeshComponent not ValidLowLevel"));
		return;
	}

	if (CustomMeshComponent)
	{
		SetSKeletalMesh(CustomMeshComponent, mesh);		
	}
}

bool FGsCustomizeHandler::SetHairChange(int32 InPartsId, bool isMyHair)
{
	if (false == _characterPlayer.IsValid())
	{
		return false;
	}

	if (InPartsId == -100)
	{
		RemoveHairMeshBySystem();
		_equipFacePartsId = InPartsId;
		return true;
	}
	else if (_equipHairPartsId == InPartsId)
	{
		//if(true == isMyHair)
		{
			//bak1210
			//UI 컨텐츠 상에서 재활용할때 생기는 이슈를 핸들링하기위함
			SetHairColorChange(_feature.faceShape.mHairColorIndex);
		}
		return false;
	}
		
		
	if (const FGsSchemaPartsShapeInfo* Data = FGsPartsHandlerBase::GetShapeInfo(_genderType, InPartsId))
	{
		if (false == _isAsync)
		{
			USkeletalMesh* mesh = (Data == nullptr) ? nullptr : GResource()->LoadSync<USkeletalMesh>(Data->shapePath);
			USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentHAIR();
			if (CustomMeshComponent)
			{
				SetSKeletalMesh(CustomMeshComponent, mesh);
				_equipHairPartsId = InPartsId;

				//if (true == isMyHair)
				{
					SetHairColorChange(_feature.faceShape.mHairColorIndex);
				}					
			}
				
		}
		else
		{
			UGsGameObjectBase* Owner = GetOwner();
			GResource()->LoadAsync(Data->shapePath, FStreamableDelegateParam::CreateWeakLambda(_owner, [Owner, InPartsId/*, isMyHair*/](UObject* inRes)->void
				{
					FGsCustomizeHandler* costomize = Owner->GetCustomizeHandler();
					if (nullptr != inRes && nullptr != costomize)
					{
						auto ptr = costomize;
						ptr->AsyncHairChangeLoadComplated(inRes);
						ptr->_equipHairPartsId = InPartsId;

						//if (true == isMyHair)
						{
							ptr->SetHairColorChange(ptr->_feature.faceShape.mHairColorIndex);
						}
					}					
				}));
		}
		return true;
	}
	return false;
}

void FGsCustomizeHandler::AsyncHairChangeLoadComplated(UObject* inRes)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	USkeletalMesh* mesh = Cast<USkeletalMesh>(inRes);
	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentHAIR();
	if (CustomMeshComponent)
	{
		SetSKeletalMesh(CustomMeshComponent, mesh);		
	}
}

bool FGsCustomizeHandler::SetWeaponChange(int32 InPartsId, CreatureWeaponType inWeaponType, int32 inWeaponLevel, FGsWeaponLoadCompleteDelegate inAfter)
{
	if (false == _characterPlayer.IsValid())
	{
		return false;
	}

	if (inWeaponType == CreatureWeaponType::NONE)
	{
		_characterPlayer->DestroyWeapon();
		_characterPlayer->DestroyWeaponSub();
		return false;
	}

	int32 enchantLevel = inWeaponLevel;	
	
	const FGsSchemaPartsShapeInfo* shapeData = FGsPartsHandlerBase::GetShapeInfo(_genderType, InPartsId);
	if (nullptr == shapeData)
	{
		return false;
	}	
	
	UChildActorComponent* weaponRightChildActorComponent = _characterPlayer->GetWeaponChildActorComponent();
	UChildActorComponent* weaponLeftChildActorComponent = _characterPlayer->GetWeaponSubChildActorComponent();
	if (nullptr == weaponRightChildActorComponent || nullptr == weaponLeftChildActorComponent)
	{
		return false;
	}

	TArray<AActor*> restoreActors = { weaponRightChildActorComponent->GetChildActor(), weaponLeftChildActorComponent->GetChildActor() };
	_characterPlayer->RestoreAttachedHiddenMesh(restoreActors);
	_characterPlayer->RestoreAttachedActorMaterials(restoreActors);

	bool sameParts = (_equipWeaponPartsId == InPartsId);

	_equipWeaponPartsId = InPartsId;

	if (false == sameParts)
	{
		if (shapeData->shapePath.IsValid())
		{
			TWeakObjectPtr<AGsCharacterPlayer> weakPlayer(_characterPlayer);
			inAfter.AddLambda([weakPlayer, inWeaponType, inWeaponLevel]()->void
				{
					if (weakPlayer.IsValid())
					{
						weakPlayer->SetWeaponEnchantEffect(inWeaponType, inWeaponLevel);
						weakPlayer->UpdateAllChangedMaterials();

						UChildActorComponent* weaponRight = weakPlayer->GetWeaponChildActorComponent();
						UChildActorComponent* weaponLeft = weakPlayer->GetWeaponSubChildActorComponent();

						TArray<AActor*> restoreActors = { weaponRight? weaponRight->GetChildActor() : nullptr, weaponLeft ? weaponLeft->GetChildActor() : nullptr };
						weakPlayer->ApplyAttachedHiddenMesh(restoreActors);
					}
				});
			_characterPlayer->SetWeaponBlueprintClass(shapeData->shapePath, inAfter, _isAsync);
			
		}
		else
		{
			_characterPlayer->DestroyWeapon();
		}

		if (shapeData->subShapePath.IsValid())
		{
			TWeakObjectPtr<AGsCharacterPlayer> weakPlayer(_characterPlayer);
			inAfter.AddLambda([weakPlayer, inWeaponType, inWeaponLevel]()->void
				{
					if (weakPlayer.IsValid())
					{	
						weakPlayer->SetWeaponSubEnchantEffect(inWeaponType, inWeaponLevel);
						weakPlayer->UpdateAllChangedMaterials();

						UChildActorComponent* weaponRight = weakPlayer->GetWeaponChildActorComponent();
						UChildActorComponent* weaponLeft = weakPlayer->GetWeaponSubChildActorComponent();

						TArray<AActor*> restoreActors = { weaponRight ? weaponRight->GetChildActor() : nullptr, weaponLeft ? weaponLeft->GetChildActor() : nullptr };
						weakPlayer->ApplyAttachedHiddenMesh(restoreActors);
					}
				});
			
			_characterPlayer->SetWeaponSubBlueprintClass(shapeData->subShapePath, inAfter, _isAsync);
		}
		else
		{
			_characterPlayer->DestroyWeaponSub();
		}
	}		
	else
	{
		_characterPlayer->SetWeaponEnchantEffect(inWeaponType, inWeaponLevel);
		_characterPlayer->SetWeaponSubEnchantEffect(inWeaponType, inWeaponLevel);

		_characterPlayer->UpdateAllChangedMaterials();
		_characterPlayer->ApplyAttachedHiddenMesh(restoreActors);
	}

	return !sameParts;
}

void FGsCustomizeHandler::AsyncWeaponChangeLoadComplated()
{
	if (_characterPlayer == nullptr) return;
}

// 타투
bool FGsCustomizeHandler::SetTattooChange(int32 InTattooId)
{
	if (_characterPlayer == nullptr) return false;

	
	const FGsSchemaCustomizeTattooData* Data = UGsTableUtil::FindRow<FGsSchemaCustomizeTattooData>(InTattooId);
	if (Data == nullptr) return false;

	if (false == _isAsync)
	{		
		USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
		if (CustomMeshComponent)
		{
			UMaterialInstanceDynamic* DynamicMaterial = GetMaterialInstanceDynamic(CustomMeshComponent, 0);
			if (DynamicMaterial)
			{				
				UTexture* texture = Data == nullptr ? nullptr : GResource()->LoadSync<UTexture>(Data->texturePath);
				DynamicMaterial->SetTextureParameterValue("Tex Tattoo", texture);
			}
		}
	}
	else
	{
		UGsGameObjectBase* Owner = GetOwner();
		GResource()->LoadAsync(Data->texturePath, FStreamableDelegateParam::CreateWeakLambda(_owner, [Owner, InTattooId](UObject* inRes)->void
			{
				FGsCustomizeHandler* costomize = Owner->GetCustomizeHandler();
				if (nullptr != costomize)
				{
					costomize->AsyncTattooChangeLoadComplated(inRes);
				}				
			}));
	}
	return true;
}

void FGsCustomizeHandler::AsyncTattooChangeLoadComplated(UObject* inRes)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
	if (nullptr == CustomMeshComponent)
	{
		return;		
	}

	UMaterialInstanceDynamic* DynamicMaterial = GetMaterialInstanceDynamic(CustomMeshComponent, 0);
	if (nullptr == DynamicMaterial)
	{
		return;
	}
	
	UTexture* texture = Cast<UTexture>(inRes);
	if (nullptr == texture)
	{
		return;
	}

	DynamicMaterial->SetTextureParameterValue("Tex Tattoo", texture);
}



void FGsCustomizeHandler::AsyncFaceAndTatooChangeLoadComplated(UObject* inRes)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	const FGsSchemaPartsShapeInfo* Data = FGsPartsHandlerBase::GetShapeInfo(_genderType, _facePartsId);
	if (Data)
	{
		USkeletalMesh* mesh = Data == nullptr ? nullptr : Cast<USkeletalMesh>(inRes);
		USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
		if (CustomMeshComponent)
			SetSKeletalMesh(CustomMeshComponent, mesh);

		SetTattooChange(_tattooTextureId);
	}
}

void FGsCustomizeHandler::RemoveFaceMeshBySystem()
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}
	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
	if (CustomMeshComponent)
	{
		SetSKeletalMesh(CustomMeshComponent, nullptr);
		CustomMeshComponent->SetSkeletalMesh(nullptr);
	}
}

void FGsCustomizeHandler::RemoveHairMeshBySystem()
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}
	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentHAIR();
	if (CustomMeshComponent)
	{
		SetSKeletalMesh(CustomMeshComponent, nullptr);
		CustomMeshComponent->SetSkeletalMesh(nullptr);
	}
}

void FGsCustomizeHandler::RestoreWeapon()
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	UChildActorComponent* mainChildActorCompo = _characterPlayer->GetWeaponChildActorComponent();
	UChildActorComponent* subChildActorCompo = _characterPlayer->GetWeaponSubChildActorComponent();

	TArray<AActor*> restoreActors = { mainChildActorCompo ? mainChildActorCompo->GetChildActor() : nullptr,
									  subChildActorCompo ? subChildActorCompo->GetChildActor() : nullptr };

	_characterPlayer->RestoreAttachedHiddenMesh(restoreActors);
	_characterPlayer->RestoreAttachedActorMaterials(restoreActors);

	if (mainChildActorCompo)
	{
		mainChildActorCompo->DestroyChildActor();
	}

	if (subChildActorCompo)
	{
		subChildActorCompo->DestroyChildActor();
	}

	_equipWeaponPartsId = -1;
}


// morphtarget
void FGsCustomizeHandler::SetFaceMorphChange(FGsCustomizeHandler::EGsTargetNames InTargetNameType, int32 InValue)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	FName* Name = _targetNames.Find(InTargetNameType);
	float floatValue = InValue * 0.01f;
	if (Name)
	{
		// Down, Up
		if (InTargetNameType == EGsTargetNames::CheekSize ||
			InTargetNameType == EGsTargetNames::NosePosition ||
			InTargetNameType == EGsTargetNames::BrowPosition ||
			InTargetNameType == EGsTargetNames::EyePosition ||
			InTargetNameType == EGsTargetNames::MouthPosition
			)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Up");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Down");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Short, Long
		else
		if (InTargetNameType == EGsTargetNames::ChinLength ||
			InTargetNameType == EGsTargetNames::MouthSize
			)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Long");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Short");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Narrow, Broad
		else
		if (InTargetNameType == EGsTargetNames::ChinSize)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Broad");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Narrow");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Out, In
		else
		if (InTargetNameType == EGsTargetNames::BrowAngle)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("In");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Out");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Thin, Thick
		else
		if (InTargetNameType == EGsTargetNames::BrowLength)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Thick");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Thin");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Narrow, Wide
		else
		if (InTargetNameType == EGsTargetNames::EyeSpacing)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Wide");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Narrow");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
		// Small, Big
		else
		if (InTargetNameType == EGsTargetNames::EyeSize)
		{
			FString NameStr = Name->ToString();

			FString UpNameStr = TEXT("Big");
			FString UpResultName = NameStr + UpNameStr;
			FName UpName = FName(*UpResultName);

			FString DownNameStr = TEXT("Small");
			FString DownResultName = NameStr + DownNameStr;
			FName DownName = FName(*DownResultName);

			if (0.5f < floatValue)
			{
				SetMorphTarget(UpName, FMath::Lerp(0.0f, 1.0f, (floatValue - 0.5f) * 2.0f));
				SetMorphTarget(DownName, 0.0f);
			}
			else
			{
				SetMorphTarget(UpName, 0.0f);
				SetMorphTarget(DownName, FMath::Lerp(1.0f, 0.0f, floatValue * 2.0f));
			}
		}
	}
}

/*
R = Parents Result
x = Origin
dx = X Scale Displacement
cdx = Child Displacement
R = x * dx * cdx
*/
// Displacement => Dis
void FGsCustomizeHandler::CalcScaleDisplacement(const FVector& InBasisScaleVec, const FVector& InDisVec, FVector* InSubDisVec, FVector* OutDisVec, FVector* OutResultVec)
{
	if (OutResultVec == nullptr || OutDisVec == nullptr)
		return;

	float x = InBasisScaleVec.X;
	float dx = InDisVec.X;
	float cdx = InSubDisVec != nullptr ? (*InSubDisVec).X : InBasisScaleVec.X;
	float rx = x * dx * cdx;
	
	float y = InBasisScaleVec.Y;
	float dy = InDisVec.Y;
	float cdy = InSubDisVec != nullptr ? (*InSubDisVec).Y : InBasisScaleVec.Y;
	float ry = y * dy * cdy;
	
	float z = InBasisScaleVec.Z;
	float dz = InDisVec.Z;
	float cdz = InSubDisVec != nullptr ? (*InSubDisVec).Z : InBasisScaleVec.Z;
	float rz = z * dz * cdz;

	(*OutDisVec).Set(dx, dy, dz); // 변위량을 저장
	(*OutResultVec).Set(rx, ry, rz); // R
}

/*
R = Child Result
x = Origin
dx = 1 / X Parents Reciprocal Scale Displacement
cdx = Parent Displacement
R = x * dx * cdx
*/
void FGsCustomizeHandler::CalcScaleReciprocalDisplacement(const FVector& InBasisScaleVec, const FVector& InDisVec, FVector* InSubDisVec, FVector* OutDisVec, FVector* OutResultVec)
{
	float x = InBasisScaleVec.X;
	float dx = (1.0f / InDisVec.X);
	float cdx = InSubDisVec != nullptr ? (*InSubDisVec).X : InBasisScaleVec.X;
	float rx = x * dx * cdx;

	float y = InBasisScaleVec.Y;
	float dy = (1.0f / InDisVec.Y);
	float cdy = InSubDisVec != nullptr ? (*InSubDisVec).Y : InBasisScaleVec.Y;
	float ry = y * dy * cdy;

	float z = InBasisScaleVec.Z;
	float dz = (1.0f / InDisVec.Z);
	float cdz = InSubDisVec != nullptr ? (*InSubDisVec).Z : InBasisScaleVec.Z;
	float rz = z * dz * cdz;

	(*OutDisVec).Set(dx, dy, dz); // 자식의 변위량을 저장
	(*OutResultVec).Set(rx, ry, rz);
}

void FGsCustomizeHandler::CalcCumulativeScaleVector(const FGsSchemaCustomizeBoneScaleGroup* InTable, EGsUICustomizeType InUICustomizeType, FVector* OutScaleDisVector)
{
	if (OutScaleDisVector == nullptr || InTable == nullptr) return;

	FVector* StartScaleVectorPtr = _bonesStartScaleVector.Find(InTable->BoneName);
	TMap<EGsUICustomizeType, FVector>* AllScaleDisPtr = _scaleDisplacementPerBone.Find(InTable->BoneName);
	FVector* SaveBoneScaleVectorPtr = AllScaleDisPtr != nullptr ? AllScaleDisPtr->Find(InUICustomizeType) : nullptr;
	(*SaveBoneScaleVectorPtr) = (*OutScaleDisVector);

	if (StartScaleVectorPtr != nullptr)
	{
		FVector CumulativeScaleVector(FVector::ZeroVector);
		FVector* HeightScale = AllScaleDisPtr->Find(EGsUICustomizeType::HEIGHT);
		if (HeightScale) CumulativeScaleVector += (*HeightScale);

		FVector* HeadScale = AllScaleDisPtr->Find(EGsUICustomizeType::HEAD_SIZE);
		if (HeadScale) CumulativeScaleVector += (*HeadScale);

		FVector* ChestScale = AllScaleDisPtr->Find(EGsUICustomizeType::CHEST_SIZE);
		if (ChestScale) CumulativeScaleVector += (*ChestScale);

		FVector* WaistScale = AllScaleDisPtr->Find(EGsUICustomizeType::WAIST_SIZE);
		if (WaistScale) CumulativeScaleVector += (*WaistScale);

		FVector* PelvisScale = AllScaleDisPtr->Find(EGsUICustomizeType::PELVIS_SIZE);
		if (PelvisScale) CumulativeScaleVector += (*PelvisScale);

		FVector* LegScale = AllScaleDisPtr->Find(EGsUICustomizeType::LEG_SIZE);
		if (LegScale) CumulativeScaleVector += (*LegScale);

		OutScaleDisVector->X = StartScaleVectorPtr->X + CumulativeScaleVector.X;
		OutScaleDisVector->Y = StartScaleVectorPtr->Y + CumulativeScaleVector.Y;
		OutScaleDisVector->Z = StartScaleVectorPtr->Z + CumulativeScaleVector.Z;
	}
}

void FGsCustomizeHandler::SetCalcLerpMinMax(const TArray<FGsSchemaCustomizeBoneScaleGroup>& InTable, EGsUICustomizeType InUICustomizeType, int32 InValue, USkeletalMeshComponent* InApplySkeletalMeshComponent)
{	
	for (int32 i = 0; i < InTable.Num(); ++i)
	{
		FVector ScaleDisVector;

		// 절대값
		ScaleDisVector.X = FMath::Lerp(0.0f, InTable[i].MaxVector.X - InTable[i].MinVector.X, (InValue * 0.01f));
		ScaleDisVector.Y = FMath::Lerp(0.0f, InTable[i].MaxVector.Y - InTable[i].MinVector.Y, (InValue * 0.01f));
		ScaleDisVector.Z = FMath::Lerp(0.0f, InTable[i].MaxVector.Z - InTable[i].MinVector.Z, (InValue * 0.01f));

		// Virtual Parents
		if (InTable[i].VirtualParentsBone != FName("None"))
		{
			InApplySkeletalMeshComponent->SetBridgeBoneIndex(InTable[i].BoneName, InTable[i].VirtualParentsBone);
			continue;
		}

		// R = x * dx * dy
		int32 ParentsBoneIndex = InApplySkeletalMeshComponent->GetBoneIndex(InTable[i].BoneName);
		FVector* ParentsValPtr = _parentsBoneScales.Find(ParentsBoneIndex);
		FVector* ChildValPtr = _childBoneScales.Find(ParentsBoneIndex);
		FVector* ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ParentsBoneIndex);		

		if (ParentsValPtr == nullptr)
		{
			_parentsBoneScales.Add(ParentsBoneIndex, FVector(1.0f));
			ParentsValPtr = _parentsBoneScales.Find(ParentsBoneIndex);		
		}
		if (ResultValPtr == nullptr)
		{
			InApplySkeletalMeshComponent->BoneScales.Add(ParentsBoneIndex, FVector(1.0f));
			ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ParentsBoneIndex);
		}		

		CalcCumulativeScaleVector(&InTable[i], InUICustomizeType, &ScaleDisVector);
		CalcScaleDisplacement(InTable[i].ScaleVector, ScaleDisVector, ChildValPtr, ParentsValPtr, ResultValPtr);	

		TArray<int32> ChildBones;
		ChildBones.Reset();
		if (InApplySkeletalMeshComponent->SkeletalMesh == nullptr) return;

		const int32 NumBones = InApplySkeletalMeshComponent->SkeletalMesh->GetRefSkeleton().GetNum();
		for (int32 ChildIndex = ParentsBoneIndex + 1; ChildIndex < NumBones; ChildIndex++)
		{
			if (ParentsBoneIndex == InApplySkeletalMeshComponent->SkeletalMesh->GetRefSkeleton().GetParentIndex(ChildIndex))
			{
				ChildBones.Add(ChildIndex);
			}
		}
		
		for (int32 ChildIndex = 0; ChildIndex < ChildBones.Num(); ++ChildIndex)
		{
			ChildValPtr = nullptr;
			ParentsValPtr = nullptr;
			ResultValPtr = nullptr;

			ParentsValPtr = _parentsBoneScales.Find(ChildBones[ChildIndex]);
			ChildValPtr = _childBoneScales.Find(ChildBones[ChildIndex]);
			ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ChildBones[ChildIndex]);

			if (ChildValPtr == nullptr)
			{
				_childBoneScales.Add(ChildBones[ChildIndex], FVector(1.0f));
				ChildValPtr = _childBoneScales.Find(ChildBones[ChildIndex]);

			}
			if (ResultValPtr == nullptr)
			{
				InApplySkeletalMeshComponent->BoneScales.Add(ChildBones[ChildIndex], FVector(1.0f));
				ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ChildBones[ChildIndex]);
			}

			CalcScaleReciprocalDisplacement(InTable[i].ScaleVector, ScaleDisVector, ParentsValPtr, ChildValPtr, ResultValPtr);

			ChildValPtr = nullptr;
			ParentsValPtr = nullptr;
			ResultValPtr = nullptr;
		}	
	}

	InApplySkeletalMeshComponent->RecalcRequiredBones(InApplySkeletalMeshComponent->GetPredictedLODLevel());
}

// 패기 예정
void FGsCustomizeHandler::SetCalcLerpMinOrignAndOrignMax(const TArray<FGsSchemaCustomizeBoneScaleGroup>& InTable, EGsUICustomizeType InUICustomizeType, int32 InValue, USkeletalMeshComponent* InApplySkeletalMeshComponent)
{
	for (int32 i = 0; i < InTable.Num(); ++i)
	{
		// 기준 벡터가(ScaleVec) minVec ~ maxVec 기준(maxVec는 기본 벡터보다 당연히 크고, minVec는 당연히 작다는 기준 당연한거다!)
		FVector ScaleDisVector;

		if (InValue > 50) ScaleDisVector.X = FMath::Lerp(InTable[i].ScaleVector.X, InTable[i].MaxVector.X, (InValue * 0.01f));
		else ScaleDisVector.X = FMath::Lerp(InTable[i].MinVector.X, InTable[i].ScaleVector.X, (InValue * 0.01f));

		if (InValue > 50) ScaleDisVector.Y = FMath::Lerp(InTable[i].ScaleVector.Y, InTable[i].MaxVector.Y, (InValue * 0.01f));
		else ScaleDisVector.Y = FMath::Lerp(InTable[i].MinVector.Y, InTable[i].ScaleVector.Y, (InValue * 0.01f));

		if (InValue > 50) ScaleDisVector.Z = FMath::Lerp(InTable[i].ScaleVector.Z, InTable[i].MaxVector.Z, (InValue * 0.01f));
		else ScaleDisVector.Z = FMath::Lerp(InTable[i].MinVector.Z, InTable[i].ScaleVector.Z, (InValue * 0.01f));

		// Virtual Parents
		if (InTable[i].VirtualParentsBone != FName("None"))
		{
			InApplySkeletalMeshComponent->SetBridgeBoneIndex(InTable[i].BoneName, InTable[i].VirtualParentsBone);
			continue;
		}

		// R = x * dx * dy
		int32 ParentsBoneIndex = InApplySkeletalMeshComponent->GetBoneIndex(InTable[i].BoneName);
		FVector* ParentsValPtr = _parentsBoneScales.Find(ParentsBoneIndex);
		FVector* ChildValPtr = _childBoneScales.Find(ParentsBoneIndex);
		FVector* ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ParentsBoneIndex);

		if (ParentsValPtr == nullptr)
		{
			_parentsBoneScales.Add(ParentsBoneIndex, FVector(1.0f));
			ParentsValPtr = _parentsBoneScales.Find(ParentsBoneIndex);

		}
		if (ResultValPtr == nullptr)
		{
			InApplySkeletalMeshComponent->BoneScales.Add(ParentsBoneIndex, FVector(1.0f));
			ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ParentsBoneIndex);
		}

		CalcCumulativeScaleVector(&InTable[i], InUICustomizeType, &ScaleDisVector);
		CalcScaleDisplacement(InTable[i].ScaleVector, ScaleDisVector, ChildValPtr, ParentsValPtr, ResultValPtr);

		TArray<int32> ChildBones;
		ChildBones.Reset();
		const int32 NumBones = InApplySkeletalMeshComponent->SkeletalMesh->GetRefSkeleton().GetNum();
		for (int32 ChildIndex = ParentsBoneIndex + 1; ChildIndex < NumBones; ChildIndex++)
		{
			if (ParentsBoneIndex == InApplySkeletalMeshComponent->SkeletalMesh->GetRefSkeleton().GetParentIndex(ChildIndex))
			{
				ChildBones.Add(ChildIndex);
			}
		}

		for (int32 ChildIndex = 0; ChildIndex < ChildBones.Num(); ++ChildIndex)
		{
			ChildValPtr = nullptr;
			ParentsValPtr = nullptr;
			ResultValPtr = nullptr;

			ParentsValPtr = _parentsBoneScales.Find(ChildBones[ChildIndex]);
			ChildValPtr = _childBoneScales.Find(ChildBones[ChildIndex]);
			ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ChildBones[ChildIndex]);

			if (ChildValPtr == nullptr)
			{
				_childBoneScales.Add(ChildBones[ChildIndex], FVector(1.0f));
				ChildValPtr = _childBoneScales.Find(ChildBones[ChildIndex]);

			}
			if (ResultValPtr == nullptr)
			{
				InApplySkeletalMeshComponent->BoneScales.Add(ChildBones[ChildIndex], FVector(1.0f));
				ResultValPtr = InApplySkeletalMeshComponent->BoneScales.Find(ChildBones[ChildIndex]);
			}

			CalcScaleReciprocalDisplacement(InTable[i].ScaleVector, ScaleDisVector, ParentsValPtr, ChildValPtr, ResultValPtr);

			ChildValPtr = nullptr;
			ParentsValPtr = nullptr;
			ResultValPtr = nullptr;
		}

		InApplySkeletalMeshComponent->RecalcRequiredBones(InApplySkeletalMeshComponent->GetPredictedLODLevel());
	}
}

void FGsCustomizeHandler::SetBodyBoneScaleChange(EGsUICustomizeType InType, int32 InValue)
{
	if (_characterPlayer.IsValid())
	{
		USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();
		SetBodyBoneScaleChange(_genderType, InType, InValue, CustomMeshComponent);
	}
}

void FGsCustomizeHandler::SetBodyBoneScaleChange(CreatureGenderType InGender, EGsUICustomizeType InType, int32 InValue, USkeletalMeshComponent* InCustomMeshComponent)
{
	if (nullptr == InCustomMeshComponent)
	{
		return;
	}

	const FGsSchemaCustomizeBoneScaleSetting* boneScaleSetting = nullptr;
	const UGsTableCustomizeBoneScaleSetting* table = Cast<UGsTableCustomizeBoneScaleSetting>(FGsSchemaCustomizeBoneScaleSetting::GetStaticTable());
	if (nullptr == table)
		return;

	if (false == table->FindRowById(InGender, boneScaleSetting))
		return;	

	const FGsSchemaGameMode* Data = UGsTableUtil::FindRow<FGsSchemaGameMode>(0);
	if (nullptr == Data)
		return;

	if (EGsUICustomizeType::HEIGHT == InType)
	{				
		if (Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->Height, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->Height, InType, InValue, InCustomMeshComponent);
		}
	}
	else
	if (EGsUICustomizeType::HEAD_SIZE == InType)
	{
		if(Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->Head, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->Head, InType, InValue, InCustomMeshComponent);
		}
	}
	else
	if (EGsUICustomizeType::CHEST_SIZE == InType)
	{
		if (Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->Chest, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->Chest, InType, InValue, InCustomMeshComponent);
		}
	}
	else
	if (EGsUICustomizeType::WAIST_SIZE == InType)
	{
		if (Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->Waist, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->Waist, InType, InValue, InCustomMeshComponent);
		}
	}
	else
	if (EGsUICustomizeType::PELVIS_SIZE == InType)
	{
		if (Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->Pelvis, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->Pelvis, InType, InValue, InCustomMeshComponent);
		}
	}
	else
	if (EGsUICustomizeType::LEG_SIZE == InType)
	{
		if (Data->CustomizeBoneScaleCalcMode == false)
		{
			SetCalcLerpMinMax(boneScaleSetting->LegThickness, InType, InValue, InCustomMeshComponent);
		}
		else
		if (Data->CustomizeBoneScaleCalcMode == true)
		{
			SetCalcLerpMinOrignAndOrignMax(boneScaleSetting->LegThickness, InType, InValue, InCustomMeshComponent);
		}
	}
}

// private 함수들 ///////////////////////////////////////////////////////////////////////////////////////////////////////

void FGsCustomizeHandler::SetMorphTarget(FName MorphTargetName, float Value, bool bRemoveZeroWeight)
{
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentFACE();
	if (CustomMeshComponent)
	{
		CustomMeshComponent->SetMorphTarget(MorphTargetName, Value);
	}
}

void FGsCustomizeHandler::SetBoneScale(FName BoneName, float Value)
{
	if (_characterPlayer == nullptr) return;

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetPartsSkeletalComponentBODY();
	if (CustomMeshComponent)
	{
		CustomMeshComponent->SetBoneScale(BoneName, FVector(Value + 0.5f));
	}
}

void FGsCustomizeHandler::SetSKeletalMesh(USkeletalMeshComponent* DestSkeletalMeshComp, USkeletalMesh* ApplySkeletalMesh)
{
	_characterPlayer->RestorePreEquipMaterial(DestSkeletalMeshComp);
	_characterPlayer->RestorePreEquipHiddenMesh(DestSkeletalMeshComp);

	if (DestSkeletalMeshComp != nullptr && ApplySkeletalMesh != nullptr)
	{
		// 현재는 다른 규칙이 없어서 1번을 기본 메터리얼로 생각한다. 스켈레탈 메쉬 0번을 적용하는걸로 없으면 nullptr
		// 원본 메터리얼을 토대로 생성
		UMaterialInstanceDynamic* DynamicMaterial = nullptr;
		if (ApplySkeletalMesh->GetMaterials().Num() > 0)
			DynamicMaterial = CreateMaterialInstanceDynamic(DestSkeletalMeshComp, ApplySkeletalMesh, 0);

		DestSkeletalMeshComp->SetMaterial(0, DynamicMaterial);
		DestSkeletalMeshComp->SetSkeletalMesh(ApplySkeletalMesh, false);

		UMaterialInstanceDynamic* DynOverlayMaterial = nullptr;
		if(UMaterialInterface * overlayMat = DestSkeletalMeshComp->GetOverlayMaterial())
		{
			DynOverlayMaterial = Cast<UMaterialInstanceDynamic>(overlayMat);
			if (nullptr == DynOverlayMaterial)
			{
				DynOverlayMaterial = UMaterialInstanceDynamic::Create(overlayMat, DestSkeletalMeshComp);
			}
		}

		_characterPlayer->RestoreMeshState(DestSkeletalMeshComp);

		DestSkeletalMeshComp->SetOverlayMaterial(DynOverlayMaterial);
		if (_characterPlayer.IsValid() && _characterPlayer->IsMaterialChanged())
		{
			if (_characterPlayer->IsOverlayMaterialChanged())
			{
				_characterPlayer->UpdateOriginalMaterial(DestSkeletalMeshComp, DynOverlayMaterial);
			}
			else
			{
				_characterPlayer->UpdateOriginalMaterial(DestSkeletalMeshComp, DynamicMaterial);
			}
		}
	}
	else if (DestSkeletalMeshComp && ApplySkeletalMesh == nullptr)
	{	
		_characterPlayer->RestoreMeshState(DestSkeletalMeshComp);

		if (_characterPlayer.IsValid() && _characterPlayer->IsMaterialChanged())
		{
			_characterPlayer->UpdateOriginalMaterial(DestSkeletalMeshComp, nullptr);
		}		
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("!!!!!!!!!!!!!!!! [Warning] Costume Data is null !!!!!!!!!!!!!!!!!!!!!!!!!!"));
		}
	}
	
}

UMaterialInstanceDynamic* FGsCustomizeHandler::GetMaterialInstanceDynamic(USkeletalMeshComponent* InSkeletalMeshComp, int32 InIndex)
{	
	/* _characterPlayer.IsValid() ? _characterPlayer->GetMaterialsByMesh(InSkeletalMeshComp) : */
	// tatoo 등은 none overlay material에 적용, overlay material이 changed되어 있어도 상관없다
	const TArray<UMaterialInterface*> MaterialInterfaces = InSkeletalMeshComp->GetMaterials(); 
	if (MaterialInterfaces.Num() > 0)
	{
		UMaterialInterface* MaterialInterface = MaterialInterfaces[InIndex];
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
		if (DynamicMaterial == nullptr)
		{
			UMaterialInstanceConstant* ConstantMaterialInterface = Cast<UMaterialInstanceConstant>(MaterialInterface);
			if (ConstantMaterialInterface)
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(ConstantMaterialInterface, InSkeletalMeshComp);
				InSkeletalMeshComp->SetMaterial(InIndex, DynamicMaterial);

				return DynamicMaterial;
			}
		}
		else
			return DynamicMaterial;
	}

	return nullptr;
}

UMaterialInstanceDynamic* FGsCustomizeHandler::CreateMaterialInstanceDynamic(UObject* InOuter, USkeletalMesh* InSkeletalMesh, int32 InIndex)
{
	if (InSkeletalMesh == nullptr) return nullptr;
	if (InSkeletalMesh->GetMaterials().Num() <= InIndex) return nullptr;
	
	UMaterialInterface* MaterialInterface = InSkeletalMesh->GetMaterials()[InIndex].MaterialInterface;
	if (MaterialInterface == nullptr) return nullptr;
		
	return UMaterialInstanceDynamic::Create(MaterialInterface, InOuter, MaterialInterface->GetFName());	
}

void FGsCustomizeHandler::SetOutlineParameter(USkeletalMeshComponent* InSkeletalMeshComp)
{
	if (nullptr == InSkeletalMeshComp)
	{
		return;
	}
	UMaterialInterface* MaterialInterface = InSkeletalMeshComp->GetMaterial(0);
	if (nullptr == MaterialInterface)
	{
		return;
	}

	FLinearColor Color0, Color1, Color2, Color3;
	if (false == MaterialInterface->GetVectorParameterValue(OutlineColor0, Color0))
	{
		return;
	}
	if (false == MaterialInterface->GetVectorParameterValue(OutlineColor1, Color1))
	{
		return;
	}
	if (false == MaterialInterface->GetVectorParameterValue(OutlineColor2, Color2))
	{
		return;
	}
	if (false == MaterialInterface->GetVectorParameterValue(OutlineColor3, Color3))
	{
		return;
	}

	UMaterialInstanceDynamic* DynamiOverlaycMaterial = Cast<UMaterialInstanceDynamic>(InSkeletalMeshComp->GetOverlayMaterial());
	if (nullptr == DynamiOverlaycMaterial)
	{
		return;
	}

	DynamiOverlaycMaterial->SetVectorParameterValue(OutlineColor0, Color0);
	DynamiOverlaycMaterial->SetVectorParameterValue(OutlineColor1, Color1);
	DynamiOverlaycMaterial->SetVectorParameterValue(OutlineColor2, Color2);
	DynamiOverlaycMaterial->SetVectorParameterValue(OutlineColor3, Color2);
}

FColor FGsCustomizeHandler::GetPaletteColor(FName InKey, int32 InIndex) const
{
	const UGsTable* table = FGsSchemaCustomizeColorPalette::GetStaticTable();
	if (table)
	{
		const FGsSchemaCustomizeColorPalette* row = nullptr;
		if (table->FindRow<FGsSchemaCustomizeColorPalette>(InKey, row))
		{
			if (InIndex < row->colorList.Num())
			{
				return row->colorList[InIndex];
			}
		}
	}

	GSLOG(Error, TEXT("[Customize] Fail to find Color. key: %s, index: %d"), *InKey.ToString(), InIndex);

	// 못찾은 상황. (0,0,0,0) 리턴
	return FColor::Transparent;
}

void FGsCustomizeHandler::ChangeWeaponSocket(const FGsSchemaUserData* InUserData, CreatureWeaponType InWeaponType, bool InBattle)
{
	if (nullptr ==  InUserData) return;

	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	UChildActorComponent* weaponR = _characterPlayer->GetWeaponChildActorComponent();
	UChildActorComponent* weaponL = _characterPlayer->GetWeaponSubChildActorComponent();
	// FVector weaponSocketScaleR = FVector::OneVector;
	// FVector weaponSocketScaleL = FVector::OneVector;

	if (InBattle)
	{
		if (const FGsSchemaWeaponSocketInfo* findInfo = InUserData->weaponSocketList.Find(InWeaponType))
		{
			const FGsSchemaWeaponBattleInfo& battleInfo = findInfo->battle;

			weaponR->AttachToComponent(_characterPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale, battleInfo.socketName_R);
			weaponR->SetRelativeTransform(FTransform::Identity);

			weaponL->AttachToComponent(_characterPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale, battleInfo.socketName_L);
			weaponL->SetRelativeTransform(FTransform::Identity);

// 			const FTransform& rTm = _characterPlayer->GetMesh()->GetSocketTransform(battleInfo.socketName_R);
// 			const FTransform& lTm = _characterPlayer->GetMesh()->GetSocketTransform(battleInfo.socketName_L);
// 			if (InResetScale == false)
// 			{
// 				weaponSocketScaleR = rTm.GetScale3D();
// 				weaponSocketScaleL = lTm.GetScale3D();
// 			}			
		}
	}
	else
	{
		if (const FGsSchemaWeaponSocketInfo* findInfo = InUserData->weaponSocketList.Find(InWeaponType))
		{
			const FGsSchemaWeaponPeaceInfo& peaceInfo = findInfo->peace;

			weaponR->AttachToComponent(_characterPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale, peaceInfo.socketName);
			weaponR->SetRelativeTransform(peaceInfo.socketOffset_R);

			weaponL->AttachToComponent(_characterPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale, peaceInfo.socketName);
			weaponL->SetRelativeTransform(peaceInfo.socketOffset_L);
		}
	}

	// 애니메이션에서 Bone 스케일을 제어 하는 경우가 있으므로
	// 무기의 BP화에 따라 자식들 Scene컴퍼넌트들로 상위정보에맞게 Scale을 동기화 시켜준다.
//	TArray<USceneComponent*> childSceneCompontns;
//	weaponL->GetChildrenComponents(true, childSceneCompontns);
// 	for (USceneComponent* el : childSceneCompontns)
// 	{		
// 		el->SetRelativeScale3D_Direct(weaponSocketScaleL);
// 	}

//	weaponR->GetChildrenComponents(true, childSceneCompontns);
// 	for (USceneComponent* el : childSceneCompontns)
// 	{
// 		el->SetRelativeScale3D_Direct(weaponSocketScaleR);
// 	}

	if (false == weaponR->IsRegistered())
	{
		weaponR->RegisterComponent();
	}

	if (false == weaponL->IsRegistered())
	{
		weaponL->RegisterComponent();
	}
}

void FGsCustomizeHandler::SetCustomizeData(const CustomizeFaceData& orgFace, const CustomizeBodyData& orgBody)
{
	_feature.faceShape = orgFace;
	_feature.bodyShape = orgBody;
}

void FGsCustomizeHandler::SetCustomizeBodyData(const CustomizeBodyData& orgBody)
{
	InitializeScaleDisplacementAllBone(_genderType);

	USkeletalMeshComponent* CustomMeshComponent = _characterPlayer->GetMesh();
	CustomMeshComponent->bBoneScaleMode = true;
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEIGHT, orgBody.mHeight, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::HEAD_SIZE, orgBody.mHeadSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::CHEST_SIZE, orgBody.mChestSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::WAIST_SIZE, orgBody.mWaistSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::PELVIS_SIZE, orgBody.mPelvisSize, CustomMeshComponent);
	SetBodyBoneScaleChange(_genderType, EGsUICustomizeType::LEG_SIZE, orgBody.mLegSize, CustomMeshComponent);
}

void FGsCustomizeHandler::ResetEquipParts()
{
	_equipFacePartsId = -1;
	_equipHairPartsId = -1;
	_equipBodyPartsId = -1;
	_equipWeaponPartsId = -1;

	_costumeParts.costumeId = INVALID_COSTUME_ID;
}

uint32 FindShapeIdByGender(const FGsSchemaPartsShapeData* inData, CreatureGenderType inGender)
{
	for (auto& e : inData->shapeInfo)
	{
		if (e.Key == inGender || e.Key == CreatureGenderType::ALL)
		{
			return	inData->id;
		}
	}
	return INVALID_COSTUME_ID;
}


void FGsCustomizeHandler::SetCostume(CostumeId lookatId, CostumeId costumeId, bool hairIsOn, bool isLookHairApplied, bool isLookBodyApplied,
	bool usingBoneScale, bool usingCustomizeFace, bool usingCostumeEffect)
{
	auto CheckInvalidLookAt = [this](CostumeId lookatId, bool hairApply, bool bodyApply)->bool {
		return _costumeParts.lookatId == lookatId && _costumeParts.lookHairApplied == hairApply && _costumeParts.lookBodyApplied == bodyApply;
	};

	// 이미 장착한 코스튬 검사
	if (CheckInvalidLookAt(lookatId, isLookHairApplied, isLookBodyApplied) && (_costumeParts.costumeId == costumeId) && (_costumeParts.hairIsOn == hairIsOn))
	{
		return;
	}
	
	// 잘못된 코슈튬을 장착하려 할때는 이전데이트는 유지 단순 리턴처리
	if (costumeId == INVALID_COSTUME_ID)
	{
		return;
	}

	_costumeParts.lookatId = lookatId; // Look은 INVALID_COSTUME_ID 일 수 있음
	_costumeParts.lookHairApplied = isLookHairApplied; // Look이 적용된다면 헤어가 적용될지 말지
	_costumeParts.lookBodyApplied = isLookBodyApplied; // Look이 적용된다면 바디가 적용될지 말지

	if (auto costume = FGsCostumeData::loadCostume(costumeId))
	{
		_costumeParts.costumeId = costumeId; //costume은 INVALID_COSTUME_ID 일 수 없음
		_costumeParts.hairIsOn	= hairIsOn;

		//외형적용되어 있고 헤어 파츠가 미적용 으로 set 되어 있으면 코스춤 헤어 적용
		//외형적용되어 있고 헤어 파츠도 적용하도록 set 되었지만, 헤어가 없는 경우는 기본 헤어를 적용// 
		bool bSystemLookInfo = false;
		//바디는 없으면 오류임
		if (lookatId != INVALID_COSTUME_ID)
		{
			// bak1210
			// 외형변경시스템은 코슈튬에 종속된 데이터가 아님 좀더 Low데이터로 구성됨( ex CostumeCommon에 외형정보역시 UserLookInfo로 구성됨)
			if (const FGsSchemaUserLookInfo* lookInfo = UGsTableUtil::FindRowById<UGsTableUserLookInfo, FGsSchemaUserLookInfo>(lookatId))
			{
				bSystemLookInfo = lookInfo->isSystem;
				if (auto hairRow = lookInfo->PartsHair.GetRow())
				{
					_costumeParts.hairCostumeId = FindShapeIdByGender(hairRow, _genderType);
				}
				if (auto bodyRow = lookInfo->PartsBody.GetRow())
				{
					_costumeParts.bodyCostumeId = FindShapeIdByGender(bodyRow, _genderType);
				}
			}
			else
			{
				_costumeParts.hairCostumeId = costume->GetCostumeHairID(_genderType);
				_costumeParts.bodyCostumeId = costume->GetCostumeClothID(_genderType);
			}
		}
		else
		{
			_costumeParts.hairCostumeId = costume->GetCostumeHairID(_genderType);
			_costumeParts.bodyCostumeId = costume->GetCostumeClothID(_genderType);

			_costumeParts.isHiddenFace = 0;
			_costumeParts.isHiddenHair = 0;

			int index = 0;
			TArray<EGsPartsType> hiddenParts;
			if (hairIsOn && _costumeParts.hairCostumeId != INVALID_CUSTOM_PART_ID && 
				costume->GetHiddenPartsForHairCostume(_genderType, hiddenParts))
			{
				_costumeParts.isHiddenFace += hiddenParts.Find(EGsPartsType::FACE, index) ? 1 : 0;
				_costumeParts.isHiddenHair += hiddenParts.Find(EGsPartsType::HAIR, index) ? 1 : 0;
			}

			hiddenParts.Reset();
			if (_costumeParts.bodyCostumeId != INVALID_CUSTOM_PART_ID && 
				costume->GetHiddenPartsForBodyCostume(_genderType, hiddenParts))
			{
				_costumeParts.isHiddenFace += hiddenParts.Find(EGsPartsType::FACE, index) ? 1 : 0;
				_costumeParts.isHiddenHair += hiddenParts.Find(EGsPartsType::HAIR, index) ? 1 : 0;
			}
		}

		if (bSystemLookInfo)
		{
			_hairPartsId = _costumeParts.hairCostumeId == 0 ? _feature.faceShape.mHairId : _costumeParts.hairCostumeId;
			_facePartsId = -100;
		}
		else
		{
			_hairPartsId = _costumeParts.isHiddenHair > 0 ? -100 : 
				(_costumeParts.hairCostumeId == 0 || !hairIsOn ? _feature.faceShape.mHairId : _costumeParts.hairCostumeId);
			_facePartsId = _costumeParts.isHiddenFace > 0 ? -100 : _feature.faceShape.mFaceId;
		}

		
		_bodyPartsId = _costumeParts.bodyCostumeId;

		_myFace				= (_facePartsId == _feature.faceShape.mFaceId) && (usingCustomizeFace == true);
		//_myHair				= (_costumeParts.hairCostumeId != 0) ? bSystemLookInfo ? false : (false == hairIsOn) : true;
		// 조건이 변경되어 무조건 커마 적용되도록 변경
		_pelvisSize			= _feature.bodyShape.mPelvisSize; //costume->IsCustomizedPelvisSizeApply() ? _feature.bodyShape.mPelvisSize : DEFAULT_CUSTOM_VALUE;
		_usingBoneScale		= usingBoneScale;
		_costumeEffect		= usingCostumeEffect ? &costume->_costumeData->effectInfo : nullptr;
		_isDirty			= true;

		costume.Reset();
	} 
	else // 코스튬은 기본복식이라도 무조건 있어야 하는 상황임으로 외형변경 예외처리는 하지 않음
	{
		return;
	}

	// 케릭터가 nullptr 일때는 비동기 로딩중
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	UpdateCustomizeDiplay();

}

void FGsCustomizeHandler::SetAppearance(CostumeId lookatId, bool isHairApplied, bool isBodyApplied, 
	bool isHairOptionApplied, bool usingBoneScale /*= true*/, bool usingCustomizeFace /*= true*/, bool usingCostumeEffect /*= true*/)
{
	auto CheckInvalidLookAt = [this](CostumeId lookatId, bool hairApply, bool bodyApply)->bool {
		return _costumeParts.lookatId == lookatId && _costumeParts.lookHairApplied == hairApply && _costumeParts.lookBodyApplied == bodyApply;
	};

	if (CheckInvalidLookAt(lookatId, isHairApplied, isBodyApplied) || _costumeParts.costumeId == INVALID_COSTUME_ID)
	{
		return;
	}

	auto costumeId = _costumeParts.costumeId;
	if (auto costume = FGsCostumeData::loadCostume(costumeId)) //costume없는 외형변경은 없다
	{
		_costumeParts.lookatId = lookatId; //INVALID_COSTUME_ID라도 set
		_costumeParts.lookHairApplied = isHairApplied;
		_costumeParts.lookBodyApplied = isBodyApplied;

		if (lookatId != INVALID_COSTUME_ID)
		{
			// bak1210
			// 외형변경시스템은 코슈튬에 종속된 데이터가 아님 좀더 Low데이터로 구성됨( ex CostumeCommon에 외형정보역시 UserLookInfo로 구성됨)
			if (const FGsSchemaUserLookInfo* lookInfo = UGsTableUtil::FindRowById<UGsTableUserLookInfo, FGsSchemaUserLookInfo>(lookatId))
			{
				if (auto hairRow = lookInfo->PartsHair.GetRow())
				{
					_costumeParts.hairCostumeId = FindShapeIdByGender(hairRow, _genderType);
				}
				if (auto bodyRow = lookInfo->PartsBody.GetRow())
				{
					_costumeParts.bodyCostumeId = FindShapeIdByGender(bodyRow, _genderType);
				}
			}
			else
			{
				_costumeParts.hairCostumeId = costume->GetCostumeHairID(_genderType);
				_costumeParts.bodyCostumeId = costume->GetCostumeClothID(_genderType);
			}
		}
		else
		{
			_costumeParts.hairCostumeId = costume->GetCostumeHairID(_genderType);
			_costumeParts.bodyCostumeId = costume->GetCostumeClothID(_genderType);
		}

		_hairPartsId = (_costumeParts.hairCostumeId == 0 || (isHairOptionApplied && !_costumeParts.hairIsOn)) ? _feature.faceShape.mHairId : _costumeParts.hairCostumeId;
		_facePartsId = _feature.faceShape.mFaceId;
		_bodyPartsId = _costumeParts.bodyCostumeId;
		//_myHair = (_costumeParts.hairCostumeId != 0) ? isHairOptionApplied && !_costumeParts.hairIsOn : true;
		_pelvisSize = _feature.bodyShape.mPelvisSize ;
		_usingBoneScale = usingBoneScale;
		_costumeEffect = usingCostumeEffect ? &costume->_costumeData->effectInfo : nullptr;
		_isDirty = true;

	}

	// 케릭터가 nullptr 일때는 비동기 로딩중
	if (false == _characterPlayer.IsValid())
	{
		return;
	}

	UpdateCustomizeDiplay();
}

bool FGsCustomizeHandler::CheckCostumeChange(CostumeId lookatId, CostumeId costumeId)
{
	return (_costumeParts.lookatId != lookatId) || (_costumeParts.costumeId != costumeId);
}

void FGsCustomizeHandler::SetCostumeEffect(const FGsSchemaCostumeEffect* InEffectInfo)
{
	if (nullptr == InEffectInfo
	|| false == _characterPlayer.IsValid())
	{
		return;
	}

	_characterPlayer->SetCostumeEffectBlueprintClass(InEffectInfo, _isAsync);
}

void FGsCustomizeHandler::UpdateCustomizeDiplay()
{
	if (false == _isDirty)
	{
		return;
	}

	SetHairChange(_hairPartsId, true/*_myHair*/);
	SetFaceChange(_facePartsId, _myFace);
	SetBodyChange(_bodyPartsId, _pelvisSize, _usingBoneScale);
	
	_isDirty = false;
}



