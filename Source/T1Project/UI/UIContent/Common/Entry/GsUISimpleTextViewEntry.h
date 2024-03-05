#pragma once
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GsUISimpleTextViewEntry.generated.h"

class UTextBlock;
class UGsButton;

UCLASS()
class UGsUISimpleTextViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	FString _text;	
};

UCLASS()
class T1PROJECT_API UGsUISimpleTextViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _textBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _button;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;
	void OnClickButton();
};
