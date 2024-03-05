// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUITrayMarketSearch.generated.h"

class UEditableTextBox;
class UGsButton;
class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;

/**
 * 거래소 스트링 검색
 */
UCLASS()
class UGsUITrayMarketSearch : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editableText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDelete;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelHistoryListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyRoot;
	
protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;
	
protected:
	const int32 MAX_TEXT_LENGTH = 50; // 기획서 정의 값
	TArray<FString> _dataList;
	
public:
	virtual void BeginDestroy() override;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 윈도우처럼 취급. 시네마틱 등에서 꺼지지 않을까봐 변경
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_DEFAULT; }
	
protected:
	UFUNCTION()
	void OnClickDelete();
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnCreateEntry(class UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnTextChanged(const FText& InText);
	
	void OnClickListEntry(int32 InIndex, bool bIsRemove);
};
