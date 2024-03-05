// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyBase.h"
#include "GsUIGuildDiplomacyAlly.generated.h"

class UScrollBox;
class UPanelWidget;
class UTextBlock;
class UGsButton;
class FGsGuildDiplomacyDataBase;
class FGsGuildDiplomacyDataAlly;
class FGsGuildDataMine;

/**
 * 길드 외교 탭의 동맹 기사단 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyAlly : public UGsUIGuildDiplomacyBase
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
	UTextBlock* _tbGuildCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAlly;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

	const FGsGuildDataMine* _myGuildData = nullptr;
	int32 _maxAllyCount = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void Show() override;
	virtual void Hide() override;
	virtual void SendRequestList(bool bInResetCoolTime) override;
	virtual bool IsRedDot() override;

	virtual void OnChangeMyAuthority() override;
	virtual void OnDiplomacyInvalidateList() override;
	virtual void OnDiplomacyResendList() override;
	virtual void OnDiplomacyAllyStateNotify(const struct FGsGuildMsgParamDiplomacyNotify* InParam) override;

protected:
	UFUNCTION()
	void OnClickAlly();
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

protected:
	void InvalidateList();
	bool CanAlly() const;
};
