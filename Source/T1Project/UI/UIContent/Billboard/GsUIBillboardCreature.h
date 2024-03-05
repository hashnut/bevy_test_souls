// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardCreature.generated.h"

class UTextBlock;
class UProgressBar;
class FGsStatInfo;

/**
 * 크리쳐 기본 빌보드(Abstract)
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIBillboardCreature : public UGsUIBillboard
{
	GENERATED_BODY()

protected:
	// ProgressBar에 바인딩할 변수. 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	float _hpBarValue = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _targetBlockLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _targetBlockRight;

	// 외각 문자 디폴트 컬러값 캐시
	FLinearColor _defaultTargetBlockColor;
	FLinearColor _defaultTextBlockColor;

protected:
	TMap<EGsBillboardUpdateType, TFunction<void()>> _updateFuncMap;
	bool _bIsTargeting = false;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	
	// UGsUIBillboard override
	virtual EGsGameObjectType GetOwnerObjetType() const override { return EGsGameObjectType::Creature; }

public:
	virtual void Reset();

protected:
	virtual void InitFunctionMap();
	virtual void SetHPPercent(float InPercent); // 0 ~ 1 사이의 값
	virtual void SetHPShieldProgressBar(UProgressBar* InBarShieldFront, UProgressBar* InBarShieldBack,
		int InCurrHp, int InMaxHp, int InCurrShield);
	virtual void SetNameColor(const FLinearColor& InColor);
	virtual void SetTargetBlockColor(const FLinearColor& InColor);

public:
	virtual void OnHit(class UGsGameObjectBase* InAttacker) {}
	virtual void OnTargeting(bool InIsTarget, class UGsGameObjectBase* InOwner, bool InIsFirstTarget = true, bool InIsEnemy = true);
	virtual void OnChangedStat(const FGsStatInfo& InStatInfo) {}

	virtual void OnSecondTarget();
	// 스캔 순서 텍스트 입력
	virtual void SetScanOrderText(const FText& In_name) {}
	// 스캔 멤버인지 변경
	virtual void SetScanMember(bool In_isMember) {}
	virtual bool IsScanNumber() const { return false; }
	// 소유자 이름 옵션 갱신, local player 소유에 대한 갱신, 생존 남은 시간 
	virtual void SetOwnMonsterOption(FString ownerName, int32 remainTimeSec = 0, int32 fullTimeSec = 0, bool isLocalPlayerOwn = false) {}
	virtual void SetRemainLifeTime(time_t endTimeTick) {}
	virtual void ChangeOwnMonsterOption() {}
	// 상태이상 컨퓨즈(내 HP/MP 정보 가림)
	virtual void SetAbnormalityConfuse(bool bIsConfuse) {}

public:
	void UpdateByType(EGsBillboardUpdateType InType);

	void SetNameText(const FText& InName);

protected:
	void SetTargetingName(bool InTarget);
	class UGsGameObjectLocalPlayer* GetLocalPlayer() const;

	// 1, 2차 타겟 모두 true, 타겟 없을 경우 false 
	bool IsTargeting() const;
};
