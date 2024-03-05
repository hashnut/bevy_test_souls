// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "GsUIPopupMultiLevelRankStat.generated.h"


class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UWidgetSwitcher;
class UGsButton;
/**
 * 승급 시스템 누적 스탯 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMultiLevelRankStat : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperPassivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfPassivityEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxPassivityList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	TArray<TPair<StatType, int32>> _refreshStatBuffer;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnRefreshPassivityEntryItem(int32 InIndex, class UWidget* InEntry);

public:
	void InvalidateStat();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
