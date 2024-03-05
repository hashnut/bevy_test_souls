#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMultiLevelRankStatEntry.generated.h"


class UTextBlock;
/**
 * ╫б╠ч ╫ц╫╨еш ╫╨ех Entry
 */
UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankStatEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockValue;

public:
	virtual void SetData(const FText& InName, const FText& InValue);
};
