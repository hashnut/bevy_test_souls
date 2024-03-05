// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPayIncrease.h"
#include "BattlePass/GsBattlePassLevelData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIPopupBattlePassBuyLevel.generated.h"

class UTextBlock;
class UGsButton;
class UGsUICurrencySlot;
class UGsWrapBoxIconSelector;
class FGsBattlePassData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupBattlePassBuyLevel : public UGsUIPopupCommonPayIncrease
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPreLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNxtLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _listHelper;
	
protected:
	TWeakPtr<FGsBattlePassData> _curPassData;
	TArray<FGsBattlePassOpenPay> _rewards;

public:
	virtual void BeginDestroy() override;

protected:
	// 아이콘 갱신
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnClickYes() override;
	void OnLongPressIcon(UGsUIIconItem& InIcon);

private:
	int GetLevelupRewards();

public:
	struct InitParam
	{
		TWeakPtr<FGsBattlePassData> curPassData;
	};

	void SetInit(InitParam* param, TFunction<void(bool, bool, Level)> callback = nullptr);

	virtual void RefreshUI() override;
	
};
