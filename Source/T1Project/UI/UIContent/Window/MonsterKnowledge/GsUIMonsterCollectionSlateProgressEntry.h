#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "GsUIMonsterCollectionSlateProgressEntry.generated.h"


UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateProgressEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _progressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textCategoryName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textPercent;

public:
	void SetData(EMonsterKnowledgeSlateCategory InCategory, float InPercent);
};