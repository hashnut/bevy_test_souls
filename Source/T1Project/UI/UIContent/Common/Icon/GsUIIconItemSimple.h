// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIIconItemSimple.generated.h"

class FGsItem;
class FGsItemEquipment;
class UTextBlock;

class UUserWidget;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedItemIconCoolTime, uint64);

/**
 * 간단한 아이템 아이콘 (HUD 포션 아이콘 표시 등)
 * - 쿨타임 기능
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemSimple : public UGsUIIconItem
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UGsGrayscaleImage* _iconImage;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon")
	class UImage* _emptyStateBG;
	
	// 사용 연출
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon")
	UUserWidget* _toggleOnEffect;

protected:
	float _coolTime = 0.0f;
	float _remainCoolTime = 0.0f;
	bool _isCoolTime = false;

	MsgGameObjHandleArray _messageDelegateList;

	FTimerHandle _timerHandleDissolve;
	TWeakObjectPtr<UWidgetAnimation> _animDissolve;

public:
	FOnFinishedItemIconCoolTime OnFinishedItemIconCoolTime;

protected:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
public:	
	virtual void SetEmptyIcon() override;
	virtual void SetImageGrayscale(const bool InIsGray) override;

public:
	// player 가 소유하지 않는 아이템일 경우
	virtual void SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel = 0, const uint8 InEnchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD, const uint8 InEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX) override;

	// player 가 소유한 아이템일 경우
	virtual void SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType = FindItemStorageType::Storage_Inventory) override;

	virtual void SetIconImageSprite(class UPaperSprite* InSprite) override;

protected:
	virtual void SetCooltimeUI(float InRemainSec, float InRate);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UIIcon")
	void OnChangeCoolTime(float InRemainSec, float InPercent);

protected:
	void SetCoolTime(float InDeltaTime);
	void SetCoolTimeInter();
	

	void ExcuteCoolTime(const struct IGsMessageParam* InParam);
	void FinishedCoolTime(const struct IGsMessageParam* InParam);

	bool IsExcuteWeaponItemCooltime(FGsItemEquipment* itemData);

public:
	void SetEmptyStateBG(bool InActive);
	void SetActiveOnEffect(bool In_isActive);

private:
	void CheckCoolTime();
	void ResetCoolTimeInfo();	

public:
	// Dissolve연출 시 코드에서 이 함수를 부를 것
	void PlayDissolveEffect();

protected:
	void OnTimerDissolve();
	void ResetDissolveTimer();
	void ResetAllAnimation();

	// 이하 함수들은 호출하지 말 것
	UFUNCTION(BlueprintImplementableEvent, Category = "UIIcon")
	void OnPlayDissolveAnimation();
	UFUNCTION(BlueprintImplementableEvent, Category = "UIIcon")
	void OnPlayDefaultAnimation();
	UFUNCTION(BlueprintCallable, Category = "UIIcon")
	void StartDissolveEffect(UWidgetAnimation* InAnimation); // BP에서 OnPlayDissolveAnimation 받으면 호출
};
