#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "GsUIFairyComposeRes.generated.h"

/**
 *
 */
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIFairyComposeRes : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textPreGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textPostGrade;

public:
	void SetResult(FairyGrade preGrade, FairyGrade postGrade);
	void SetItemResult(ItemGrade preGrade, ItemGrade posetGrade);
};
