#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIItemFusionRefineStat.generated.h"


UCLASS()
class T1PROJECT_API UGsUIItemFusionRefineStat : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _name;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _value;

public:
	void SetData(const FText& InName, const FText& InValue, const FLinearColor& InColor);
};
