// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowGuildJoin.generated.h"

class UGsButton;
class UPanelWidget;
class FGsGuildData;
struct IGsMessageParam;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowGuildJoin : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIComboBox* _comboBoxFilter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _inputBoxSearch;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelOption;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSearch;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyList;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUpdate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroupTab;
	TArray<const FGsGuildData*> _guildList;
	TArray<MsgGuildHandle> _msgGuildHandleList;
	int32 _prevTabIndex = 0;

	int32 _currTabIndex = 0;
	int32 _currComboBoxIndex = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool SyncContentState() override;

private:
	void BindMessages();
	void UnbindMessages();

protected:
	void OnChangeTab(int32 InIndex);
	void OnSelectComboBox(int32 InIndex);

	UFUNCTION()
	void OnClickSearch();
	UFUNCTION()
	void OnTextChangedSearch(const FText& InText);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnClickUpdateList();

protected:
	// 길드 목록을 받음(검색 결과도 동일하게 이걸로 받음)
	void OnReceiveGuildList(const IGsMessageParam*);
	// 신청중 길드 목록을 받음
	void OnReceiveJoinWaitingGuildList(const IGsMessageParam*);	
	// 리스트 갱신 메시지 받음 
	void OnInvalidateGuildJoinList(const IGsMessageParam*);

protected:
	void InvalidateGuildList();
	void SortGuildList();	
};