#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMultiLevelRankQuestEntry.generated.h"

class UTextBlock;
class UGsUIProgressBar;
class UWidgetSwitcher;
class UImage;
/**
 * ½Â±Þ ½Ã½ºÅÛ Äù½ºÆ® Ç×¸ñ
 */
UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankQuestEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textParm1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textParm2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _widgetSign;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _progressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCheck;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageType;

	/************************************************************************/
	/* Refresh                                                              */
	/************************************************************************/
public:
	void InitializeData(const FText& InTitle, const FText& InDesc, bool InOnlyTitle, const FSoftObjectPath* InIconPath);
	void SetProgressData(int32 InDynamicValue, int32 InObjectiveValue);
	void SetColor(const FLinearColor& InColor);
};
