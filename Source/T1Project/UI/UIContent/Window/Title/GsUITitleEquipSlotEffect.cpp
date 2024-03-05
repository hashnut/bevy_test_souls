
#include "GsUITitleEquipSlotEffect.h"
#include "Title/GsTitleData.h"
#include "UI/UIContent/Window/Title/GsUITitleEquipSlotStatInfo.h"
#include "../Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project/T1Project.h"
#include "Components/WidgetSwitcher.h"


void UGsUITitleEquipSlotEffect::SetToggle(TitleEquipSlot InType)
{
	_frontSlot->PlayEffect(TitleEquipSlot::FRONT == InType);
	_backSlot->PlayEffect(TitleEquipSlot::BACK == InType);
}


void UGsUITitleEquipSlotEffect::SetData(const FGsTitleData* InFrontTitleData, const FGsTitleData* InBackTitleData, bool InIsShowEffect)
{
	_frontSlot->SetData(InFrontTitleData, InIsShowEffect);
	_backSlot->SetData(InBackTitleData, InIsShowEffect);
}