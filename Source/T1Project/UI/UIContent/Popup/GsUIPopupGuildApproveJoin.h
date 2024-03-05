// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupGuildApproveJoin.generated.h"

class UGsButton;
struct IGsMessageParam;

/**
 * 길드 가입 승인 창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildApproveJoin : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelScrollRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelEmptyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _panelSlotHelper;

protected:
	// 리스트의 포인터만 가져다 쓰므로, 리스트를 비우거나 수정하지 말 것
	TArray<struct FGsGuildJoinWaitingUser>* _dataList;

	TArray<MsgGuildHandle> _msgGuildHandleList;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void RegisterMessages();
	void DeregisterMessages();

	void InvalidateList();

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();
	
	// 승인/거절 결과 받음.
	void OnReceiveJoinApproveResult(const IGsMessageParam*);
};
