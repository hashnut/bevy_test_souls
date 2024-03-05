// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICostumeDetail.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "UI/UIContent/Window/Costume/GsUICostumeSkillList.h"
#include "UI/UIContent/Window/Costume/GsUICostumeActiveSkill.h"
#include "UI/UIContent/Window/Costume/GsUICostumeStat.h"
#include "UI/UIContent/Window/Costume/GsUICostumeSkillIconItem.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Costume/GsCostumeData.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Components/PanelWidget.h"

void UGsUICostumeDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_itemMainStatList = { _mainStat1, _mainStat2, _mainStat3, _mainStat4, _mainStat5, _mainStat6 };
	_itemSubStatList = { _subStat1, _subStat2, _subStat3, _subStat4, _subStat5 };

	if (_skillList) _skillList->SetWrapSize(WrapSize);
	if (_activeSkillPanel) _activeSkillPanel->SetVisibility(UseActiveWidget == UseSkillPanel::ActiveSkillDev ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (_skillListPanel) _skillListPanel->SetVisibility(UseActiveWidget == UseSkillPanel::NONE ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (_emptyPanel) _emptyPanel->SetVisibility(UseActiveWidget == UseSkillPanel::NONE ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if(UseActiveWidget == UseSkillPanel::AllSkillList)
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_SummonConfirm_Skill"), _textSkillTitle);
}

void UGsUICostumeDetail::SetSelectedCostumeDetail(TWeakPtr<FGsCostumeData> data)
{
	TArray<UGsUICostumeStat*> statCompos = { _identityStat };
	for (auto statList : { statCompos, _itemMainStatList, _itemSubStatList })
	{
		for (auto elem : statList)
		{
			elem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	bool isPassiveSkillData = false;
	bool isActiveSkillData = false;
	
	_activeSkill->SetData(nullptr);
	_skillList->RefreshSkillList(nullptr);

	if (auto costumeData = data.Pin())
	{
		static TFunction<void(UGsUICostumeStat* entry, const FGsCostumeData::FGsCostumeStatData* data)> SetStatEntry =
			[](UGsUICostumeStat* entry, const FGsCostumeData::FGsCostumeStatData* data) {
			if (entry && data && data->type != StatType::NONE)
			{
				entry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				entry->SetData(data->type, data->value);
			}
		};

		const auto& identityStat = costumeData->GetIdentityStat();
		const auto& mainStatList = costumeData->GetMainStats();
		const auto& subStatList = costumeData->GetSubStats();

		TArray<FGsCostumeData::FGsCostumeStatData> statData = { identityStat };

		statCompos.Append(_itemMainStatList.GetData(), mainStatList.Num() > _itemMainStatList.Num() ? _itemMainStatList.Num() : mainStatList.Num());
		statData.Append(mainStatList.GetData(), mainStatList.Num() > _itemMainStatList.Num() ? _itemMainStatList.Num() : mainStatList.Num());

		statCompos.Append(_itemSubStatList.GetData(), subStatList.Num() > _itemSubStatList.Num() ? _itemSubStatList.Num() : subStatList.Num());
		statData.Append(subStatList.GetData(), subStatList.Num() > _itemSubStatList.Num() ? _itemSubStatList.Num() : subStatList.Num());

		auto index = 0;
		for (auto e : statCompos)
		{
			SetStatEntry(e, &statData[index++]);
		}

		for (auto weaponType : { CreatureWeaponType::ALL, CreatureWeaponType::SWORD, CreatureWeaponType::DAGGER
								,CreatureWeaponType::BOW, CreatureWeaponType::WAND, CreatureWeaponType::ORB, CreatureWeaponType::SHOTGUN,
			CreatureWeaponType::HAND })
		{
			auto selectPassiveSkills = costumeData->GetCostumePassivityList(weaponType);

			if (selectPassiveSkills.Num() > 0) 
			{
				isPassiveSkillData = true;
				break;
			}
		}

		isActiveSkillData = costumeData->GetCostumeActiveSkill() != nullptr;

		if (UseActiveWidget == UseSkillPanel::ActiveSkillDev)
		{
			_activeSkill->SetData(costumeData); 
			_skillList->RefreshSkillList(costumeData, false);
		}
		else if(UseActiveWidget == UseSkillPanel::AllSkillList)
		{
			_skillList->RefreshSkillList(costumeData, true);
		}
	}

	_activeSkillPanel->SetVisibility(UseActiveWidget == UseSkillPanel::ActiveSkillDev && isActiveSkillData ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_skillListPanel->SetVisibility(
		(UseActiveWidget == UseSkillPanel::AllSkillList && (isPassiveSkillData || isActiveSkillData)) ||
		(UseActiveWidget == UseSkillPanel::ActiveSkillDev && isPassiveSkillData) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_emptyPanel->SetVisibility(UseActiveWidget == UseSkillPanel::NONE || (!isPassiveSkillData && !isActiveSkillData) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_deco->SetVisibility((UseActiveWidget == UseSkillPanel::ActiveSkillDev && isPassiveSkillData) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUICostumeDetail::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if(_skillList) _skillList->SetWrapSize(WrapSize);
	if(_activeSkillPanel) _activeSkillPanel->SetVisibility(UseActiveWidget == UseSkillPanel::ActiveSkillDev ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if(_title) _title->SetVisibility(UseActiveWidget == UseSkillPanel::ActiveSkillDev ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (_skillListPanel) _skillListPanel->SetVisibility(UseActiveWidget == UseSkillPanel::NONE ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (_emptyPanel) _emptyPanel->SetVisibility(UseActiveWidget == UseSkillPanel::NONE ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

CostumeId UGsUICostumeDetail::GetRecoverCostumeId()
{
	return _activeSkill->GetRecoverCostumeId();
}
