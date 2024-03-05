// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageUI.h"
#include "GsUIPopupChannelList.generated.h"

/**
 * 버튼이 하나 있는 팝업.
 */
UCLASS()
class T1PROJECT_API UGsUIPopupChannelList : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 스크롤 엔트리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIChannel")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scroll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelChannel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (MultiLine = true))
	FText _textDesc;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	// 값만 가져다 쓰므로 편집하지 말 것
	const TArray<struct FGsMapChannelData>* _dataList = nullptr;
	int32 _currChannel = 0;
	FText _textRemainTimeFormat;
	FTimerHandle _timerHandle;
	MsgUIHandle _recieveChannelListHandler;

protected:
	virtual void BeginDestroy() override;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void OnRecieveChannelList(const struct IGsMessageParam*);
	
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();

	void OnTimer();

	void SetUIDescText(float InTime);
};
