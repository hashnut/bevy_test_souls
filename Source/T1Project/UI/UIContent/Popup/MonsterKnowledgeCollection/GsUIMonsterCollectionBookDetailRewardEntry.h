#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMonsterCollectionBookDetailRewardEntry.generated.h"


class UTextBlock;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionBookDetailRewardEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textKnowledgeLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRequredKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRewardPoint;

public:
	void SetData(const int32 InLevel, const int32 InKnowledge, const int32 InPoint);
};
