// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyBase.h"
#include "GsUIGuildDiplomacyState.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class FGsGuildDiplomacyDataState;
class UGsSwitcherButton;

/**
 * 길드 외교 탭의 동맹 신청 현황 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyState : public UGsUIGuildDiplomacyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootEmpty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbRequestedCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbRequestCount;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;	

	TArray<const FGsGuildDiplomacyDataState*> _dataList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void Show() override;
	virtual void Hide() override;
	virtual void SendRequestList(bool bInResetCoolTime) override;
	virtual bool IsRedDot() override;

	virtual void OnDiplomacyInvalidateList() override;
	virtual void OnDiplomacyResendList() override;
	virtual void OnDiplomacyAllyStateNotify(const struct FGsGuildMsgParamDiplomacyNotify* InParam) override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

protected:
	void InvalidateList();
	void SortByRemainTime(bool bIsAscending);
};
