#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "GsUIPresetFairy.generated.h"

/*
	프리셋 페어리 UI
*/

class UWidgetSwitcher;
class UImage;
class UTextBlock;
class UPanelWidget;

UCLASS()
class UGsUIPresetFairy : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imagePortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEnchantCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEquip;

public:
	void SetFairy(FairyId InFairyId);

protected:
	int32 GetGradeIndex(FairyGrade InGrade);
};