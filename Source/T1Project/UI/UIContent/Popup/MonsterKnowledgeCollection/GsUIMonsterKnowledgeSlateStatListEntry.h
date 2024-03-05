#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMonsterKnowledgeSlateStatListEntry.generated.h"


class UTextBlock;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIMonsterKnowledgeSlateStatListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textValue;

public:
	void SetData(const FText& InName, const FText& InValue);
};
