#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "GsUIPresetCostume.generated.h"

/*
	프리셋 코스튬 UI
*/

class UWidgetSwitcher;
class UImage;
class UTextBlock;
class UPanelWidget;

UCLASS()
class UGsUIPresetCostume : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imagePortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEquip;

public:
	void SetCostume(CostumeId InCostumeId);

protected:
	int32 GetGradeIndex(CostumeGrade InGrade);
};