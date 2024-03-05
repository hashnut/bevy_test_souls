// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Animation/GsCompleteCampAnimInstance.h"
#include "GsCampBase.generated.h"


class AGsCharacterLocalPlayer;
class UWidgetComponent;
class UGsUIBillboardInteractionButton;
class UGsUIBillboardCamp;
/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsCampBase : public AGsCharacterBase
{
	GENERATED_BODY()

protected:
	FVector _navigationAddOffset;
	TWeakObjectPtr<AGsCharacterLocalPlayer> _localPlyerPtr;

public:
	AGsCampBase();

protected:
	bool FindLocalPlayerPtr();

public:
	virtual void DestoryPorc() {}

public:
	virtual void FinishedAnim() {}

public:
	virtual void SetHpBarWidgetProgress(const float InValue) {}
	virtual void SetCampName(const FText InText) {}
	virtual void SetCampEmblemIcon(const uint8 InEmblemId) {}
	virtual void SetHpBarAndNameWidgetVisibility(const bool InActive) {}
	virtual void BeginDestroyAnim() {}
	virtual void BeginHitAnim() {}
	virtual void SetCurCampType(const EGsCampType InCampType) {}
	virtual void SetNavigationAddOffsetValue(FVector InVec) {}

public:
	virtual void SetUseInteractionButton(bool In_isUse) {}
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) {}

public:
	virtual FVector GetActorMeshBoundsExtends() { return FVector(0.f, 0.f, 0.f); }
	virtual FVector GetActorBoundsExtends() { return FVector(0.f, 0.f, 0.f); }
	virtual float GetHpBarValue() { return 0.f; }
	virtual UWidgetComponent* GetInteractionWidgetComponent() { return nullptr; }
	virtual UGsUIBillboardInteractionButton* GetInteractionButton() const { return nullptr; } 	
	virtual UGsUIBillboardCamp* GetCampNameplateWidget() const { return nullptr; }
	virtual FVector GetNavModifierSizeVec() { return FVector(0.f, 0.f, 0.f); }

//----------------------------------------------------------------------------------------------------------------------
	// 성소 구현을 위해 임시로 추가한 항목들
protected:
	bool _bIsSanctum = false;

protected:
	virtual void SetUseInteractionButtonSanctum(bool InUse) {}

public:
	virtual void PlayAnimationHit() {}
	virtual void PlayAnimationDestroy() {}
	virtual void PlayAnimationConstructionStep(int32 InStep, bool bIsNormal) {}

	// 0~1사이의 값
	virtual void SetDammageEffect(float InRate) {}

public:
	void SetSanctumName(const FText& InText);
	void SetShowSanctumName(bool InShow);
	void SetShowNamePanel(bool InShow);

	void SetIsSanctum(bool bIsSanctum) { _bIsSanctum = bIsSanctum; }
	bool IsSanctum() const { return _bIsSanctum; }

	void SetNameplateHpBar(bool bIsShow, float InHpRate);
};
