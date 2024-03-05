#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIItemCollectionStatTypeCheckbox.generated.h"

enum class ECheckBoxState : uint8;
class UWidgetSwitcher;
class UTextBlock;
class UGsButton;

/**
 * GsUIPopupItemCollectionSelectStatType 에서 사용할 스탯 선택용 체크박스
 */
UCLASS()
class T1PROJECT_API UGsUIItemCollectionStatTypeCheckbox : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNameOn;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNameOff;

public:
	FSimpleDelegate _onClickDelegate;

public:
	void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBtn();

public:
	void SetTitle(const FText& InText);
	void SetState(const ECheckBoxState InCheckState);
	ECheckBoxState GetState() const;
};