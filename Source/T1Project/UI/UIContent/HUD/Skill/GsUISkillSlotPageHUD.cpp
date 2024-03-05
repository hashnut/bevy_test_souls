// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotPageHUD.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBox.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonBase.h"


void UGsUISkillSlotPageHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (int32 i = 1; i <= MAX_COSTUME_SKILL_SLOT_ID; ++i)
	{
		// C_Skill_0 ~ 
		FString widgetName = TEXT("C_Skill_");
		widgetName.AppendInt(i);

		UGsUISkillButtonBase* slot = Cast<UGsUISkillButtonBase>(GetWidgetFromName(*widgetName));
		if (slot)
		{
			slot->InitializeSlot(i);

			_slotList.Add(slot);
			_slotMap.Emplace(i, slot);
		}
	}
}

void UGsUISkillSlotPageHUD::InitializePage(int32 InPageIndex)
{
	_pageIndex = InPageIndex;
}

void UGsUISkillSlotPageHUD::SetIsFakePage(bool bIsFakePage)
{
	_bIsFakePage = bIsFakePage;

	for (UGsUISkillButtonBase* slot : _slotList)
	{
		slot->SetIsFakePage(bIsFakePage);
	}
}

void UGsUISkillSlotPageHUD::UpdateManagerTick(float InDeltaTime)
{
	for (UGsUISkillButtonBase* slot : _slotList)
	{
		slot->UpdateManagerTick(InDeltaTime);
	}
}

UGsUISkillButtonBase* UGsUISkillSlotPageHUD::GetSlot(int32 InSlotId) const
{
	if (const TWeakObjectPtr<UGsUISkillButtonBase>* slot = _slotMap.Find(InSlotId))
	{
		if (slot->IsValid())
		{
			return slot->Get();
		}
	}

	return nullptr;
}

UGsUISkillButtonBase* UGsUISkillSlotPageHUD::GetSlotBySkillId(SkillId InSkillId) const
{
	for (UGsUISkillButtonBase* slot : _slotList)
	{
		if (slot->GetCachedSkillId() == InSkillId)
		{
			return slot;
		}
	}

	return nullptr;
}
