#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/EGsPartsType.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/GsGameObjectHandler.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "DataSchema/Customize/GsSchemaCustomizeBoneScaleGroup.h"
#include "Runtime/AnimationCore/Public/BoneIndices.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "Classes/GsDelegate.h"


class AGsCharacterPlayer;
class AGsCharacterBase;
class USkeletalMesh;
class USkeletalMeshComponent;
class UMaterialInstanceDynamic;
class UObject;

struct FGsCustomizeUIData;
struct FGsSchemaCostumeEffect;

class T1PROJECT_API FGsCustomizeHandler : public IGsGameObjectHandler
{
public:
	enum class EGsTargetNames : uint8
	{
		// 얼굴 조정값
		CheekSize, // 볼살
		ChinLength,	// 턱 길이
		ChinSize,	// 턱 크기
		NosePosition, // 코 위치
		EyePosition, //눈 위치
		EyeSpacing, // 눈 간격
		EyeSize, // 눈 사이즈		
		BrowPosition, // 눈썹 위치
		BrowAngle, // 눈썹 각도
		BrowLength, // 눈썹 길이
		MouthPosition, // 입 위치
		MouthSize, // 입 길이		

		HairColor, // 머리 색깔
		HairShadowColor, // 머리 그림자 색깔
		HairSpecularColor, // 머리 스펙큘러
		
		SkinColor, // 피부 색깔
		SkinShadowColor, // 피부 그림자 색깔
		SkinSpecularColor, // 피부 스펙큘러
		
		PupilColor, // 눈동자 색깔
		PupilShadowColor, // 눈동자 그림자 색깔
		//PupilSpecularColor, // 눈동자 스펙큘러
		
		BrowColor, // 눈썹 색깔
		BrowShadowColor, // 눈썹 그림자 색깔
		//BrowSpecularColor, // 눈썹 스펙큘러		
		SetLightVectorChannel,

		FaceDataCnt,

		// 몸 조정 값
		Height, // 키
		Head, // 머리 크기
		Chest, // 가슴 크기
		WaistR, // 허리 크기
		WaistL, // 허리 크기
		Pelvis, // 골반 크기
		LegsR, // 다리 두께
		LegsL, // 다리 두께

		All
	};

private:	
	TWeakObjectPtr<class AGsCharacterPlayer> _characterPlayer;

	static TMap<EGsTargetNames, FName> _targetNames;
	TMap<FBoneIndexType, FVector> _childBoneScales;		// 자식일떄의 Scale 변위량을 저장
	TMap<FBoneIndexType, FVector> _parentsBoneScales;	// 부모일떄의 Scale 변위량을 저장
	TMap<FName, FVector> _bonesStartScaleVector;		// 같은 bone들중에 최소값 == 시작값
	TMap<FName, TMap<EGsUICustomizeType, FVector>> _scaleDisplacementPerBone;

	int32 _facePartsId = -1;
	int32 _hairPartsId = -1;
	int32 _bodyPartsId = -1;
	int32 _weaponPartsId = -1;
	int32 _tattooTextureId = -1;

	int32 _equipFacePartsId = -1;
	int32 _equipHairPartsId = -1;
	int32 _equipBodyPartsId = -1;
	int32 _equipWeaponPartsId = -1;

	uint8 _pelvisSize = DEFAULT_CUSTOM_VALUE;
	bool  _myFace = true;
	bool  _myHair = true;
	bool  _usingBoneScale = false;
	const FGsSchemaCostumeEffect* _costumeEffect = nullptr;

	CreatureGenderType _genderType = CreatureGenderType::MALE;

	struct FeatureData
	{
		CustomizeFaceData faceShape;
		CustomizeBodyData bodyShape;
	};

	struct CostumePart
	{
		CostumeId lookatId = INVALID_COSTUME_ID;			// 코스튬 메인키와 동일
		CostumeId costumeId = INVALID_COSTUME_ID;			// FGsSchemaCostumeCommon Key 값 (코스튬 메인키)
		int32 hairCostumeId = INVALID_COSTUME_ID;			// FGsSchemaPartsShapeInfo Hair (Gender에 의해 갈림)
		int32 bodyCostumeId = INVALID_COSTUME_ID;			// FGsSchemaPartsShapeInfo body (Gender에 의해 갈림)

		bool hairIsOn = false;
		bool lookHairApplied{ false };			// lookInfo 정보가 valid 할때, 적용할지,말지 유무임 :헤어 정보가 있는가 없는가가 아님
		bool lookBodyApplied{ false };			// lookInfo 정보가 valid 할때, 적용할지,말지 유무임 :바디 정보가 있는가 없는가가 아님

		// 2024.01.09 적용 body는 기본이라 hidden 적용 될 일이 없으므로 face/hair에 대해서만 counting
		uint8 isHiddenFace{ 0 };				// lookInfo 정보가 valid 하고, look이 적용되어 있을 때, face를 감추도록 한 part의 수
		uint8 isHiddenHair{ 0 };				// lookInfo 정보가 valid 하고, look이 적용되어 있을 때, hair를 감추는록 한 part의 수 
	}; 

	FeatureData _feature; 
	CostumePart _costumeParts;

	bool _isDirty = false;
	bool _isAsync = false;
	class UGsGameObjectBase* _owner;

public:
	FGsCustomizeHandler() : IGsGameObjectHandler(GameObjectHandlerType::CUSTOMIZE) {};
	virtual ~FGsCustomizeHandler() = default;


public:
	void InitializeActor(class AActor* Owner) override;	
	static void InitializeBoneTargetName();
	void InitializeScaleDisplacementAllBone(CreatureGenderType inGenderType);
	void Update(float Delta) override;

	// 직접액터를 세팅할때 쓰임
	void SetActor(class AActor* Owner);
	AGsCharacterPlayer* GetPlayer();
	UGsGameObjectBase* GetOwner() { return _owner; }

private:
	void CalcScaleDisplacement(const FVector& InBasisScaleVec, const FVector& InDisVec, FVector* InSubDisVec, FVector* OutDisVec, FVector* OutResultVec);
	void CalcScaleReciprocalDisplacement(const FVector& InBasisScaleVec, const FVector& InDisVec, FVector* InSubDisVec, FVector* OutDisVec, FVector* OutResultVec);
	void CalcCumulativeScaleVector(const FGsSchemaCustomizeBoneScaleGroup* InTable, EGsUICustomizeType InUICustomizeType, FVector* OutScaleDisVector);
	void CalcStartScaleVectorPerTable(const FGsSchemaCustomizeBoneScaleGroup* InTable);

	void InitializeScaleDisplacementPerBone(const FGsSchemaCustomizeBoneScaleGroup* InTable, EGsUICustomizeType InUICustomizeType);

	void ConvertToServerFace(FGsCustomizeUIData* InUICustomData, CustomizeFaceData* OutFaceData);
	void ConvertToServerBody(FGsCustomizeUIData* InUICustomData, CustomizeBodyData* OutBodyData);

	void SetSKeletalMesh(USkeletalMeshComponent* DestSkeletalMeshComp, USkeletalMesh* ApplySkeletalMesh);
	void SetBoneScale(FName BoneName, float Value);
	void SetMorphTarget(FName MorphTargetName, float Value, bool bRemoveZeroWeight = true);

	UMaterialInstanceDynamic* GetMaterialInstanceDynamic(USkeletalMeshComponent* InSkeletalMeshComp, int32 InIndex);
	UMaterialInstanceDynamic* CreateMaterialInstanceDynamic(UObject* InOuter, USkeletalMesh* InSkeletalMesh, int32 InIndex);

	void SetOutlineParameter(USkeletalMeshComponent* InSkeletalMeshComp);

	void SetCalcLerpMinMax(
		const TArray<FGsSchemaCustomizeBoneScaleGroup>& InTable, EGsUICustomizeType InUICustomizeType, 
		int32 InValue, USkeletalMeshComponent* InApplySkeletalMeshComponent
	);

	// 패기 예정
	void SetCalcLerpMinOrignAndOrignMax(
		const TArray<FGsSchemaCustomizeBoneScaleGroup>& InTable, EGsUICustomizeType InUICustomizeType, 
		int32 InValue, USkeletalMeshComponent* InApplySkeletalMeshComponent
	);

	FColor GetPaletteColor(FName InKey, int32 InIndex) const;

	void AsyncFaceChangeLoadComplated(class UObject* inRes);
	void AsyncHairChangeLoadComplated(class UObject* inRes);
	void AsyncBodyhangeLoadComplated(class UObject* inRes);
	void AsyncWeaponChangeLoadComplated();
	void AsyncTattooChangeLoadComplated(class UObject* inRes);
	void AsyncFaceAndTatooChangeLoadComplated(class UObject* inRes);
public:
	void RemoveFaceMeshBySystem();
	void RemoveHairMeshBySystem();

public:
	// bak1210 : 인터페이스 통합을위한 함수변경
	// 기존 함수는 IniailizeActor, InitializeScaleDisplacementAllBone 으로 분리 / 함수이름 변경	
	virtual void Initialize(UGsGameObjectBase* owner) override
	{
		_owner = owner;
	}
	virtual void Finalize() override;

public:
	void SetByCustomizeUIData(FGsCustomizeUIData* InUICustomData); // UI에 셋팅 되있는 데이터로 셋팅
	void UpdateCustomizeUIData(FGsCustomizeUIData* InUICustomData);
	// 서버에서 받은 데이터로 셋팅
	void SetCustomizeDataBonScale(const struct CustomizeFaceData& InFaceData, const struct CustomizeBodyData& InBodyData);
	
	void SetCustomizeData(const CustomizeFaceData& orgFace, const CustomizeBodyData& orgBody);
	void SetCustomizeBodyData(const CustomizeBodyData& orgBody);
	void SetBodyParts(int32 InPartsId)	{ _bodyPartsId = InPartsId; }
	void ResetEquipParts();
	void SetAsyncMode()					{ _isAsync = true; }
	void SendGatewayReqCreateUser(const FString& Name, FGsCustomizeUIData* InUICustomData);
	void SendGatewayReqCustomizing(FGsCustomizeUIData* InUICustomData);

public:
	void SetHairColorChange(int32 InValue);

	// Mode
	// 0 - 전체
	// 1 - Face
	// 2 - Body
	void SetSkinColorChange(int32 InValue, int8 InMode = 0);
	// Mode
	// 0 - 전체
	// 1 - Face
	// 2 - Body
	void ForceUpdateSkinColor(int8 InMode = 2);	
	void SetPupilColorChange(int32 InValue);
	void SetBrowColorChange(int32 InValue);

	void ForceFaceAttibuteColor();

public:
	// 굳이 비동기 요청시 확인할 필요가 없으면 콜백 함수(InLoadedDelegate)를 셋팅할 필요는 없다.
	bool SetHairChange(int32 InPartsId, bool isMyHair = true);
	bool SetFaceChange(int32 InPartsId, bool isMyFace = true);
	bool SetBodyChange(int32 InPartsId);
	bool SetWeaponChange(int32 InPartsId = 0, CreatureWeaponType inWeaponType = CreatureWeaponType::NONE, int32 inWeaponLevel = 0, FGsWeaponLoadCompleteDelegate inAfter = FGsWeaponLoadCompleteDelegate());
	bool SetTattooChange(int32 InTattooId);

public:
	void SetBodyBoneScaleChange(CreatureGenderType InGender, EGsUICustomizeType InType, int32 InValue, USkeletalMeshComponent* InCustomMeshComponent);	// (Async SetBoneScale)	
	void SetBodyBoneScaleChange(EGsUICustomizeType InType, int32 InValue);	// (Async SetBoneScale)	
	

public:
	void SetGenderType(CreatureGenderType InGenderType) { _genderType = InGenderType; }

public:
	void RestoreWeapon();	
	// GsPartHandler클래스에 있던 함수 이관
	// MeshComponent Socket에 Attach
	void ChangeWeaponSocket(const struct FGsSchemaUserData* InUserData, CreatureWeaponType InWeaponType, bool InBattle);	

public:
	// CostumeId lookatId : 보여지는 코스튬 정보
	// CostumeId costumeId : 코스튬 ID
	// bool hairIsOn = true : 코스튬 헤어장착여부
	// bool isLookHairApplied = true : lookatId가 valid 할 때, 헤어 적용 여부
	// bool isLookBodyApplied = true : lookatId가 valid 할 때, 바디 적용 여부
	// bool usingBoneScale = true : 본스케일 여부
	// bool usingCustomizeFace = true : 얼굴에 타투 눈섭, 눈알 색상적용여부
	// bool usingCostumeEffect = true : 코스튬 부착 이펙트를 사용할지 여부 (시네마틱에서는 출력하지 않음)
	void SetCostume(CostumeId lookatId, CostumeId costumeId, bool hairIsOn = true, bool isLookHairApplied = true, bool isLookBodyApplied = true, bool usingBoneScale = true, bool usingCustomizeFace = true, bool usingCostumeEffect = true);

	// 코스춤이 적용되어 있을 때 외형만 변경처리 SetCostume이 먼저 되어 있지 않으면 처리되지 않음
	// CostumeId lookatId : 보여지는 코스튬 정보
	// bool isHairOptionApplied = true : 코스튬 헤어장착여부를 적용할 것인가 (UI 사용을 위한 처리)
	// bool isLookHairApplied = true : lookatId가 valid 할 때, 헤어 적용 여부
	// bool isLookBodyApplied = true : lookatId가 valid 할 때, 바디 적용 여부
	// bool usingBoneScale = true : 본스케일 여부
	// bool usingCustomizeFace = true : 얼굴에 타투 눈섭, 눈알 색상적용여부
	// bool usingCostumeEffect = true : 코스튬 부착 이펙트를 사용할지 여부 (시네마틱에서는 출력하지 않음)
	void SetAppearance(CostumeId lookatId, bool isHairApplied, bool isBodyApplied, bool isHairOptionApplied = false, bool usingBoneScale = true, bool usingCustomizeFace = true, bool usingCostumeEffect = true);

	bool CheckCostumeChange(CostumeId lookatId, CostumeId costumeId);

	const CustomizeBodyData& GetCustomizeBodyData()
	{
		return _feature.bodyShape;
	}

private:
	void SetCostumeEffect(const FGsSchemaCostumeEffect* InEffectInfo);
	void UpdateCustomizeDiplay();
	bool SetBodyChange(int32 InPartsId, uint8 sizePelvis, bool usingBoneScale);

	/*deprecated*/
	void SetFaceMorphChange(EGsTargetNames InTargetNameType, int32 InValue);	
};