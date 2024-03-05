// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupResolutionList.generated.h"

class URichTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupResolutionList : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _scrollPanel;

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _panelSlotHelper;

	TFunction<void(int)> _selectCallback;
	TFunction<void(int)> _destructCallback;

	FGsToggleGroup _toggleGroup;
	int _index = 0;
	int _initIndex = 0;
	int _preIndex = -1;

	//TArray<FIntPoint> _resolutionList;
	TArray<FText> _resolutionTextList;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

public:
	virtual void NativeOnInitialized() override;

protected:
	virtual void NativeDestruct() override;

public:
	void SetData(TArray<FText>& InResolutionTextList, int InMyIndex, TFunction<void(int)> InCallback = nullptr,
		TFunction<void(int)> InDestructCallback = nullptr);

public:
	void ApplyPreIndexButton();

protected:
	// 닫기 클릭
	UFUNCTION()
	void OnClickClose();

	// 슬롯 클릭 시
	UFUNCTION()
	void OnClickSlot(int32 InIndex, bool bIsSelected);

	// 리스트 앤트리 갱신 시
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
