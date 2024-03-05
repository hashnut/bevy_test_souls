#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUINPCFunctionButton.generated.h"

class UPaperSprite;
class UGsButton;
class UImage;
class UWidgetSwitcher;
class UTextBlock;

struct FGsSchemaNpcFunctionData;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUINPCFunctionButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSoftObjectPtr<UPaperSprite> _emptyIconRef;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNPCFunction;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconDimmed;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSlotName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSlotName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSlotNameDimmed;

	// 0: on
	// 1: dimmed
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBtnState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _effectMoveButton;

protected:
	int32 _slotId = 0;
	
	TArray<int> _arrayNpcId;

	TFunction<void(UUserWidget* In_target, TArray<int> In_arrayNpcTblId)> _callbackClickMultiNpc;

	// True일 경우, 무한스크롤 연출을위해 추가된 페이지. 메시지를 보내지 않도록 막을 것.
	bool _bIsFakePage = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	void InitializeSlot(int32 InSlotId) { _slotId = InSlotId; }
	void UpdateShowSlotNameOption();

protected:
	UFUNCTION()
	void OnClickDimmedButton();

	UFUNCTION()
	void OnClickNPCFunctionButton();
	UFUNCTION()
	void OnPressedNPCFunctionButton();

public:
	void OnKeyboardPress();
	void OnKeyboardRelease();

public:
	void SetEnableButton(bool InEnable);
	void SetEmptySlot();

	void SetData(TArray<int> In_arrayNpcId, const FGsSchemaNpcFunctionData* In_functionData, 
		bool In_isUseable,
		bool In_isMoveButton,
		TFunction<void(UUserWidget* In_target, TArray<int> In_arrayNpcTblId)> In_callbackClickMultiNpc);

	// 무한스크롤 연출을 위한 가짜 페이지(SendEvent하는것은 막고 연출만 보이게 하기 위함)
	void SetIsFakePage(bool bIsFake) { _bIsFakePage = bIsFake; }

	UPaperSprite* GetEmptyIcon();

	void SetMoveEffect(bool In_isMoveButton);
};