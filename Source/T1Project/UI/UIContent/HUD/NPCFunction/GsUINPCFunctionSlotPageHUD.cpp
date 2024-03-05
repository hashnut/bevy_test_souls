#include "GsUINPCFunctionSlotPageHUD.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionButton.h"

void UGsUINPCFunctionSlotPageHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int npcFunctionSlotMaxCount = GData()->GetGlobalData()->_npcFunctionSlotMaxCount;

	for (int32 i = 1; i <= npcFunctionSlotMaxCount; ++i)
	{
		// C_Skill_0 ~ 
		FString widgetName = TEXT("C_NPCFunction_");
		widgetName.AppendInt(i);

		UGsUINPCFunctionButton* slot = Cast<UGsUINPCFunctionButton>(GetWidgetFromName(*widgetName));
		if (slot)
		{
			slot->InitializeSlot(i);

			_slotList.Add(slot);
			_slotMap.Emplace(i, slot);
		}
	}
}

void UGsUINPCFunctionSlotPageHUD::SetIsFakePage(bool bIsFakePage)
{
	_bIsFakePage = bIsFakePage;

	for (UGsUINPCFunctionButton* slot : _slotList)
	{
		slot->SetIsFakePage(bIsFakePage);
	}
}

UGsUINPCFunctionButton* UGsUINPCFunctionSlotPageHUD::GetSlot(int32 InSlotId) const
{
	if (UGsUINPCFunctionButton* const* slot = _slotMap.Find(InSlotId))
	{
		return *slot;
	}

	return nullptr;
}