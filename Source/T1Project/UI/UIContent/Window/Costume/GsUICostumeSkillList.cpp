// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Window/Costume/GsUICostumeSkillList.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Item/GsItemManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Costume/GsCostumeData.h"

#include "UI/UIContent/Popup/GsUIPopupSkillDetailInfo.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "GsUICostumeItem.h"
#include "GsUICostumeSkillIconItem.h"

#include "PanelWidget.h"
#include "ScrollBox.h"
#include "WrapBox.h"

void UGsUICostumeSkillList::BeginDestroy()
{
	if (nullptr != _passiveScrollHelper)
	{
		_passiveScrollHelper->OnCreateEntry.RemoveDynamic(this, &UGsUICostumeSkillList::OnCreatePassiveSkillIconEntry);
		_passiveScrollHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUICostumeSkillList::OnRefreshPassiveSkillIconEntry);
		_passiveScrollHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICostumeSkillList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (auto userData = GGameData()->GetUserData())
	{
		_genderType = userData->mGender;
		if (const auto weaponInfo = UGsItemManager::GetEquipItemTableDataByTID(userData->mLookInfo.mWeapon))
		{
			_weaponType = weaponInfo->weaponType;
		}
	}

	TArray<UWidget*>  childList = _passiveSkillListPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeSkillIconItem>())
		{
			child->RemoveFromParent();
		}
	}

	_passiveScrollHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_passiveScrollHelper->Initialize(_entryPassiveSkillIcon, _passiveSkillListPanel);
	_passiveScrollHelper->OnCreateEntry.AddDynamic(this, &UGsUICostumeSkillList::OnCreatePassiveSkillIconEntry);
	_passiveScrollHelper->OnRefreshEntry.AddDynamic(this, &UGsUICostumeSkillList::OnRefreshPassiveSkillIconEntry);
}

void UGsUICostumeSkillList::NativeConstruct()
{
	Super::NativeConstruct();

	_passiveScrollHelper->RefreshAll(0);
}

void UGsUICostumeSkillList::RefreshSkillList(const TSharedPtr<FGsCostumeData> costumeData, bool useTotalSkill /*= false*/)
{
	_skillList.Empty();
	if (costumeData)
	{	
		if (useTotalSkill)
		{
			if (const auto activeSkill = costumeData->GetCostumeActiveSkill())
			{
				if (const auto useSkill = activeSkill->userSkillSetId.GetRow())
				{
					const auto skillData = (_genderType == CreatureGenderType::MALE) ?
						useSkill->maleSkillId.GetRow() : useSkill->femaleSkillId.GetRow();

					_skillList.Add({ true, skillData->id, activeSkill->weaponType, useSkill->icon });
				}
			}
		}

		for (auto weaponType : { CreatureWeaponType::ALL, CreatureWeaponType::SWORD, CreatureWeaponType::DAGGER
								,CreatureWeaponType::BOW, CreatureWeaponType::WAND, CreatureWeaponType::ORB, CreatureWeaponType::SHOTGUN,
			CreatureWeaponType::HAND })
		{
			auto selectPassiveSkills = costumeData->GetCostumePassivityList(weaponType);
			for (const auto passiveSkill : selectPassiveSkills)
			{
				_skillList.Add({ false, passiveSkill->id, weaponType, passiveSkill->iconPath });
			}
		}
	}

	_passiveScrollHelper->RefreshAll(_skillList.Num());
}

void UGsUICostumeSkillList::SetWrapSize(float wrapSize)
{
	WrapSize = wrapSize;
	_passiveSkillListPanel->bExplicitWrapSize = true;
}

void UGsUICostumeSkillList::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (_passiveSkillListPanel)
	{	
		_passiveSkillListPanel->WrapSize = WrapSize;
	}
}

void UGsUICostumeSkillList::OnCreatePassiveSkillIconEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeSkillIconItem>(InEntry))
	{
		entry->_OnSelectedIcon.BindLambda([](bool isActiveSkill, int id) {

			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupSkillDetailInfo"));
			if (widget.IsValid())
			{
				if (UGsUIPopupSkillDetailInfo* popup = Cast<UGsUIPopupSkillDetailInfo>(widget.Get()))
				{
					if (isActiveSkill)
						 popup->SetData(id);
					else popup->SetPassivityData(id);
				}
			}
			});
	}
}

void UGsUICostumeSkillList::OnRefreshPassiveSkillIconEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeSkillIconItem>(InEntry))
	{
		if (_skillList.IsValidIndex(index))
		{
			auto& data = _skillList[index];
			entry->SetData(data._isActive, data._id, data._type, data._iconPath);
		}
	}
}

