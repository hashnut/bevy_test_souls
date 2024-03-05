// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GsUITraySpaceCrackResult.generated.h"

class UTextBlock;
class UGsUICurrencySlot;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconItem;
class UGsUIIconBase;

/**
 * 시공의 틈새 결과창
 */
UCLASS()
class T1PROJECT_API UGsUITraySpaceCrackResult : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyGoldSlot;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencySlot* _currencyMileageSlot;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textMonsterCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEliteMonsterCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _backBtn;	

	TMap<uint32, int> _itemList;
	FTimerHandle _timerHandle;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	// 아이콘 생성
	UFUNCTION()
	void OnCreateIcon(class UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnClickBack();

protected:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	void SetData(uint32 inPlayTime, Money inGold, Money inMileage, 
		SpaceCrackEnemyKill inZakoCount, SpaceCrackEnemyKill inEliteCount,
		TMap<uint32, int> inItemList);

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY; }
};
