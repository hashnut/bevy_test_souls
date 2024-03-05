#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/GsGameObjectHandler.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "Fairy/GsFairyData.h"

class UGsGameObjectPlayer;
class AGsActorFieldFairy;
class UCurveFloat;

class T1PROJECT_API FGsFairyHandlerPlayer : public IGsGameObjectHandler, public TSharedFromThis<FGsFairyHandlerPlayer>
{
protected:
	UGsGameObjectPlayer* _ownPlayer = nullptr;
	TWeakObjectPtr<AGsActorFieldFairy> _fairy = nullptr;

	TWeakPtr<FGsFairyData> _fairyData;

	//FairyId _fairyID = INVALID_FAIRY_ID;
	//SkillLevel _fairyLV = INVALID_SKILL_LEVEL;
	//FairyFeatureId	_fairyFeatureID = INVALID_FAIRY_FEATURE_ID;

	float _springEndLength{ 10.f };

	// 페어리 springArm 위치 정보 백업
	FVector _springRelativeLocation;

	// 페어리 springArm 조절 커브 데이터
	UCurveFloat* _springArmLengthCurve = nullptr;
	float _springArmDefaultLength = 0.f;
	float _springArmCurrentTime = 0.f;
	float _springArmMaxTime = 0.f;

public:
	FGsFairyHandlerPlayer() : IGsGameObjectHandler(GameObjectHandlerType::FAIRY) {}
	virtual ~FGsFairyHandlerPlayer() = default;

public:

	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void InitializeActor(class AActor* Owner) override;
	virtual void Finalize() override;
	virtual void Update(float Delta) override;

	virtual void UpdateEquipFairy(FairyId id, bool instant = true); //level 에 따라 외형이 바뀐다고 하면 필요
	virtual void SetFairySkillEffect();
	virtual void SetFairyVehicleEffect(bool IsActive, bool useTalk = false);
	virtual void SetFairyRelativeLocation(const FVector& RelativeLocation);
	virtual void SetFairyTalkInfo() {};
	
	virtual void ShowFairyWarpEffect();
	virtual void ShowFairySpawnEffect();
	virtual void SetFairyHiddenInGame();

	virtual void OnVehicleEffect(UCurveFloat* curveData, const FVector& reletivePosition, bool useTalk = false);
	virtual void OnRestoreVehicleEffect();
	
public:
	void RestoreEquipFairy();
	bool CheckFairyChange(FairyId id);

private:
	void InternalSpawnActor(class UClass* ObjectClass, const TArray<FGsSchemaFairyEffect>& BaseEffect, const FGsSchemaFairyEffect* SkillEffect);

public:
	AGsActorFieldFairy* GetFairyActor()
	{
		return _fairy.Get(false);
	}

	TWeakPtr<FGsFairyData> GetFairyData()
	{
		return _fairyData;
	}
};

