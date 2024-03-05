// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupGuildAcquisition.generated.h"


class UGsButton;
class UGsUIPartySettingTab;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UUserWidget;
class UPanelWidget;
struct FGsSchemaGuildAcquisition;

/**
 * 기사단 획득 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAcquisition : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChange;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleAcquisitionOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxOption;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

protected:
	TArray<const FGsSchemaGuildAcquisition*> _acquisitionInfoList;

	GuildAcquisitionSettings _cachedAcquisitionSettings;
	GuildAcquisitionSettings _currentAcquisitionSettings;

	bool _hasAuthority = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickChange();
	UFUNCTION()
	void OnClickCancel();

	void OnClickedOption(const FGsSchemaGuildAcquisition* InAcquisitionInfo, int32 InValue);

	void OnToggleAcquisition(bool InIsSelected);

protected:
	UFUNCTION()
	void OnRefreshEntryOption(int32 InIndex, class UWidget* InEntry);

	void InvalidateChangeButton();

};
