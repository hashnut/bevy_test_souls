// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupGuildDiplomacySearch.generated.h"

class UEditableTextBox;
class UTextBlock;
class UGsButton;
class UScrollBox;
class UWidgetSwitcher;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
struct IGsMessageParam;
class FGsGuildData;

/**
 * 길드 외교 검색창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildDiplomacySearch : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editableText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMessage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSearch;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	TArray<MsgGuildHandle> _msgGuildHandleList;

	bool _bIsAlly = false;
	TArray<const FGsGuildData*> _dataList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

private:
	// TODO: 리스트 받고 갱신 처리 필요
	void BindMessages();
	void UnbindMessages();

public:
	void SetData(bool bInIsAlly);

protected:
	UFUNCTION()
	void OnTextChanged(const FText& InText);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnClickSearch();
	UFUNCTION()
	void OnClickCancel();

private:
	void OnReceiveGuildList(const IGsMessageParam*);
};
