#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIChatMacroEntry.generated.h"

class UTextBlock;
class UEditableTextBox;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIChatMacroEntry :
	public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FOnSendText);
	DECLARE_DELEGATE_OneParam(FOnChangedText, const FText&);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _macroIndex;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editEnterText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSend;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnChangedText(const FText& OriginalText);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCommitedText(const FText& inputText, ETextCommit::Type InCommitType);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSend();

protected:
	virtual void NativeOnInitialized() override;

	int8 _index;

public:
	void SetData(int8 index, const FText& preText);

	FOnSendText		OnSendText;
	FOnChangedText	OInputText;

};

