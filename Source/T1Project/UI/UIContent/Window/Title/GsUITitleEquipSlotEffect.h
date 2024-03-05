#pragma once

/**
* file		GsUITitleEquipSlotEffect.h
* @brief	window title ¿¡ ÀåÂø ½½·Ô È¿°ú
* @author	PKT
* @date		2022/01/24
**/


#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "GsUITitleEquipSlotEffect.generated.h"

class FGsTitleData;
class UGsSwitcherButton;
class UGsUITitleEquipSlotStatInfo;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUITitleEquipSlotEffect : public UUserWidget
{
	GENERATED_BODY()

	const static int32 FOCUS_IN_SWITCER_INDEX = 0;
	const static int32 FOCUS_OUT_SWITCER_INDEX = 1;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITitleEquipSlotStatInfo* _frontSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITitleEquipSlotStatInfo* _backSlot;
public:
	void SetToggle(TitleEquipSlot InType);

	void SetData(const FGsTitleData* InFrontTitleData, const FGsTitleData* InBackTitleData, bool InIsShowEffect);
};
