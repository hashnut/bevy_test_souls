#pragma once
#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Components/EditableTextBox.h"
#include "GsEditableTextBox.generated.h"

UCLASS()
class T1PROJECT_API UGsEditableTextBox : public UEditableTextBox
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateColor TextColor;

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetColorAndOpacity(FSlateColor color);

	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetColorAndOpacity"))
	FSlateColor GetColorAndOpacity();

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;

	void SetMaximumLength(int maxByteLength);
	void EnableInput();

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void HandleOnTextChanged(const FText& Text) override;
	virtual void HandleOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod) override;

protected:
	int _maxLength{ 0 };
};