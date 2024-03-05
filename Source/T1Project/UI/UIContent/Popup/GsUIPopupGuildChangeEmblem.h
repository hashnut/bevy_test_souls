// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupGuildChangeEmblem.generated.h"

class UGsButton;

/**
 * 길드 문장 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildChangeEmblem : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelScrollRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _currencyBtnChangeEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroup;

	TArray<const struct FGsSchemaGuildEmblem*> _dataList;
	TFunction<void(int32)> _selectCallback;

	// 생성 화면에서의 변경 창인지, 길드에서의 변경창인지 구분 위함
	bool _bIsGuildExist = false;

	// 바뀌었을 경우에만 콜백을 호출하기 위해 저장
	int32 _prevEmblemId = 0;
	
	// 미리 한번 찾아서 저장해 둠
	int32 _changeEmblemCost = 0;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(bool bInIsGuildExist, int32 InCurrEmblemId, TFunction<void(int32)> InSelectCallback = nullptr);
	int32 FindEmblemIndex(int32 InEmblemId) const;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	void OnClickListEntry(int32 InIndex);

	UFUNCTION()
	void OnClickChangeEmblem();

	UFUNCTION()
	void OnClickCancel();
};
