#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUISimpleTwoTextEntry.generated.h"

class UTextBlock;

/**
 * 텍스트 두개를 앞뒤로 찍는 간단한 리스트 아이템
 */
UCLASS()
class T1PROJECT_API UGsUISimpleTwoTextEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBack;

public:
	void SetTextFront(const FText& InText);
	void SetTextBack(const FText& InText);
};
