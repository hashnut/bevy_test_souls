// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotSetEquip.h"
#include "Components/WidgetSwitcher.h"
#include "Management/GsScopeHolder.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotEquip.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Skill/GsSkill.h"
#include "Net/GsNetSendServiceWorld.h"


void UGsUISkillSlotSetEquip::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 이 타이밍에 가능한지 체크해볼 필요가 있음
	for (int32 i = 0; i <= MAX_COSTUME_SKILL_SLOT_ID; ++i)
	{
		// C_Skill_0 ~  세팅
		FString widgetName = TEXT("C_Skill_");
		widgetName.AppendInt(i);

		UGsUISkillSlotEquip* slot = Cast<UGsUISkillSlotEquip>(GetWidgetFromName(*widgetName));
		if (slot)
		{
			slot->InitializeSlot(i);
			slot->OnClickSkillSlot.BindUObject(this, &UGsUISkillSlotSetEquip::OnClickSlot);
			_slotList.Add(slot);
		}
	}

	_btnRemoveAll->OnClicked.AddDynamic(this, &UGsUISkillSlotSetEquip::OnClickRemoveAll);
	_btnInfo->OnPressed.AddDynamic(this, &UGsUISkillSlotSetEquip::OnPressInfo);
}

void UGsUISkillSlotSetEquip::NativeDestruct()
{
	_weaponType = CreatureWeaponType::MAX;

	Super::NativeDestruct();
}

void UGsUISkillSlotSetEquip::SetData(CreatureWeaponType InType, bool bInForced)
{
	// 현재와 같은 무기타입이면 갱신 안함(최적화 위함)
	if (bInForced ||
		_weaponType != InType)
	{
		_weaponType = InType;

		InvalidateSlotSet();
	}
}

void UGsUISkillSlotSetEquip::SetSkillCategory(int32 InIndex)
{
	_switcherSlotSet->SetActiveWidgetIndex(InIndex);

	// 패시브 페이지일 때 버튼 가림
	if (2 == InIndex)
	{
		_btnRemoveAll->SetVisibility(ESlateVisibility::Hidden);
		_btnInfo->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_btnRemoveAll->SetVisibility(ESlateVisibility::Visible);
		_btnInfo->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUISkillSlotSetEquip::SetEnableEquip(bool bEnable, bool bIsEquipMode, bool bIsCostume, const FGsSkill* InSelectedSkill)
{
	_bEquipMode = bEnable;
	_bIsCostume = bIsCostume;
	int32 costumeIndex = MAX_COSTUME_SKILL_SLOT_ID - 1;

	if (_bIsCostume)
	{
		for (int32 i = 0; i < _slotList.Num(); ++i)
		{
			if (UGsUISkillSlotEquip* slot = _slotList[i])
			{
				if (costumeIndex == i)
				{
					slot->SetEnableEquip(true, true, InSelectedSkill);
				}
				else
				{
					slot->SetEnableEquip(false, false, InSelectedSkill);
				}
			}
		}

		_btnRemoveAll->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		for (int32 i = 0; i < _slotList.Num(); ++i)
		{
			if (UGsUISkillSlotEquip* slot = _slotList[i])
			{
				if (costumeIndex == i)
				{
					slot->SetEnableEquip(false, false, InSelectedSkill);
				}
				else
				{
					slot->SetEnableEquip(bEnable, bIsEquipMode, InSelectedSkill);
				}
			}
		}

		_btnRemoveAll->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUISkillSlotSetEquip::InvalidateSlotSet()
{
	// 슬롯 정보 가져오기. 실패시 모두 비우기.
	const TMap<SkillSlotId, const FGsSkill*>* skillSlotMap = GSkill()->GetSkillSlotMap(_weaponType);
	if (nullptr == skillSlotMap)
	{
		for (UGsUISkillSlotEquip* slot : _slotList)
		{
			// nullptr넘기면 empty 처리됨
			slot->SetData(nullptr);
		}

		return;
	}

	// 슬롯 세팅
	for (UGsUISkillSlotEquip* slot : _slotList)
	{
		int32 slotId = slot->GetSlotId();
		const FGsSkill* skill = nullptr;

		// 편집창엔 평타스킬 없음
		if (0 == slotId)
		{
			skill = GSkill()->FindNormalSkill(_weaponType);
		}
		else
		{
			if (skillSlotMap)
			{
				if (const FGsSkill* const* skillPtr = skillSlotMap->Find(slotId))
				{
					if (const FGsSkill* tempSkill = *skillPtr)
					{
						// 체인스킬일 경우 강제로 1단계로 설정
						if (tempSkill->IsChainSkill())
						{
							skill = RecursiveFindFirstChainSkill(tempSkill);
						}
						else
						{
							skill = tempSkill;
						}
					}
				}
			}
		}

		// skill이 nullptr이면 empty 처리됨
		slot->SetData(skill);
	}
}

void UGsUISkillSlotSetEquip::InvalidateSlotBySlotId(SkillSlotId InSlotId)
{
	if (UGsUISkillSlotEquip* slot = FindSlotById(InSlotId))
	{		
		if (const TMap<SkillSlotId, const FGsSkill*>* skillSlotMap = GSkill()->GetSkillSlotMap(_weaponType))
		{
			if (const FGsSkill* const* skillPtr = skillSlotMap->Find(InSlotId))
			{
				slot->SetData(*skillPtr);
				return;
			}
		}

		slot->SetData(nullptr);
	}
}

const FGsSkill* UGsUISkillSlotSetEquip::RecursiveFindFirstChainSkill(const FGsSkill* InSkill) const
{
	if (1 == InSkill->GetChainStep())
	{
		return InSkill;
	}

	if (INVALID_SKILL_ID == InSkill->GetPrevSkillId())
	{
		return InSkill;
	}

	const FGsSkill* prevSkill = GSkill()->FindSkill(InSkill->GetPrevSkillId(), CreatureWeaponType::MAX, true);
	if (nullptr == prevSkill)
	{
		return InSkill;
	}

	return RecursiveFindFirstChainSkill(prevSkill);
}

void UGsUISkillSlotSetEquip::OnClickSlot(UGsUISkillSlotEquip* InSlot)
{
	OnClickSkillSlot.ExecuteIfBound(InSlot);
}

void UGsUISkillSlotSetEquip::OnClickRemoveAll()
{
	// 전부 비어있으면 보내지 않음(보낼 필요가 없음)
	if (IsAllEmpty())
	{
		return;
	}

	if (_bIsCostume)
	{
		return;
	}

	// 개선: R3 연타방지 필요
	SkillCategorySet categoryType = static_cast<SkillCategorySet>(_switcherSlotSet->GetActiveWidgetIndex());
	if (SkillCategorySet::PASSIVE == categoryType)
	{
		return;
	}

	FGsNetSendServiceWorld::SendSkillSlotClear(_weaponType, categoryType);
}

void UGsUISkillSlotSetEquip::OnPressInfo()
{
	if (_tooltipInfo->IsOpen())
	{
		_tooltipInfo->Close();
		return;
	}

	// 글자가 길어져 화면 밖으로 못나가게 강제 줄바꿈 설정
	_tooltipInfo->SetWrapTextOption(500.f);

	FText textDesc;
	FText::FindText(TEXT("SkillUIText"), TEXT("PopUp_Guide_SkillSetting"), textDesc);
	_tooltipInfo->SetDescText(textDesc);

	_tooltipInfo->Open();
}

UGsUISkillSlotEquip* UGsUISkillSlotSetEquip::FindSlotById(SkillSlotId InSlotId) const
{
	for (UGsUISkillSlotEquip* slot : _slotList)
	{
		if (slot->GetSlotId() == InSlotId)
		{
			return slot;
		}
	}

	return nullptr;
}

const FGsSkill* UGsUISkillSlotSetEquip::GetSkill(SkillSlotId InSlotId) const
{
	if (UGsUISkillSlotEquip* slot = FindSlotById(InSlotId))
	{
		return slot->GetSkill();
	}
	
	return nullptr;
}

bool UGsUISkillSlotSetEquip::IsAllEmpty() const
{
	for (UGsUISkillSlotEquip* slot : _slotList)
	{
		if (false == slot->IsEmpty())
		{
			return false;
		}
	}

	return true;
}
