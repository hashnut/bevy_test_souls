// Fill out your copyright notice in the Description page of Project Settings.
#include "GsDivFairyEnchant.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "DataSchema/Fairy/GsSchemaFairyEnchant.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivity.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"

#include "Fairy/GsFairyData.h"
#include "Sound/GsSoundPlayer.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "UI/UIContent/Popup/GsUIPopupFairyCollection.h"
#include "UI/UIContent/Popup/GsUIPopupFairySkillTooltip.h"

#include "GsUIFairySkillItem.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemIngredient.h"

#include "TextBlock.h"
#include "Components/Image.h"
#include "WidgetSwitcher.h"
#include "RichTextBlock.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Algo/Transform.h"
#include "Algo/ForEach.h"

void UGsDivFairyEnchant::BeginDestroy()
{
	if (_curScrollHelper)
	{
		_curScrollHelper->OnCreateEntry.RemoveDynamic(this, &UGsDivFairyEnchant::OnCreateFairySkillEntry);
		_curScrollHelper->OnRefreshEntry.RemoveDynamic(this, &UGsDivFairyEnchant::OnRefreshFairySkillEntry);

		_curScrollHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsDivFairyEnchant::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _curSkillListPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairySkillItem>())
		{
			child->RemoveFromParent();
		}
	}

	_btnUpgrade->OnClickCurrencyButton.BindUObject(this, &UGsDivFairyEnchant::OnStartFairyUpgrade);
	_btnCollection->OnClicked.AddDynamic(this, &UGsDivFairyEnchant::OnClickFairyCollection);
	_btnCancel->OnClicked.AddDynamic(this, &UGsDivFairyEnchant::OnCancelUpgrade);

	_curScrollHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_curScrollHelper->Initialize(_entrySkillIcon, _curSkillListPanel);
	_curScrollHelper->OnCreateEntry.AddDynamic(this, &UGsDivFairyEnchant::OnCreateFairySkillEntry);
	_curScrollHelper->OnRefreshEntry.AddDynamic(this, &UGsDivFairyEnchant::OnRefreshFairySkillEntry);

	FText Do_Enchant;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Growth_Grow"), Do_Enchant);
	_btnUpgrade->SetButtonName(Do_Enchant);
}

void UGsDivFairyEnchant::InitEnchantDivision(TWeakPtr<FGsFairyData> data)
{
	_isEnableEnchant = true;
	_fairyData = data;
	_fairyLevelData.Reset();

	_btnUpgrade->SetIsEnabled(false);
	_btnUpgrade->SetData(CurrencyType::GOLD, 0);

	SetFairyEnchantBaseInfo();
}

void UGsDivFairyEnchant::OnEnchantResult(FairyEnchantResult result)
{
	_isEnableEnchant = true;
	_btnCancel->SetIsEnabled(true);
}

void UGsDivFairyEnchant::CancelEnchant()
{
	_isEnableEnchant = true;
	_btnCancel->SetIsEnabled(true);
}

void UGsDivFairyEnchant::OnCreateFairySkillEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySkillItem>(InEntry))
	{
		entry->OnSkillTooltip.BindUObject(this, &UGsDivFairyEnchant::OnTooltipFairySkill);
	}
}

void UGsDivFairyEnchant::OnRefreshFairySkillEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySkillItem>(InEntry))
	{
		if (_fairyLevelData.IsValidIndex(index))
		{
			entry->SetData(_fairyLevelData[index]);
			entry->RefreshUI();
		}
	}
}

void UGsDivFairyEnchant::OnStartFairyUpgrade()
{
	if (CheckEnableFairyUpgrade())
	{
		_isEnableEnchant = true;
		_btnCancel->SetIsEnabled(false);
		OnStartEnchant.ExecuteIfBound(_fairyData);
	}
}

void UGsDivFairyEnchant::OnCancelUpgrade()
{
	_isEnableEnchant = false;
	OnCancelEnchant.ExecuteIfBound();
}

void UGsDivFairyEnchant::OnClickFairyCollection()
{
	if (_fairyData.IsValid() == false)
		return;

	if (auto PopupCollection = Cast<UGsUIPopupFairyCollection>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCollection"))))
	{
		UGsUIPopupFairyCollection::PopupInitData param = { _fairyData.Pin()->_id, _fairyData.Pin()->GetUpgradeLevel() };
		PopupCollection->InitializeData(&param);
	}
}

void UGsDivFairyEnchant::OnTooltipFairySkill(TWeakPtr<FGsFairyPassivityInfo> tooltip)
{
	if (tooltip.IsValid() == false || _fairyData.IsValid() == false) return;

	TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillList;
	if (tooltip.Pin()->isVehicle == false)
	{
		int32 passivityIndex = INDEX_NONE;
		TArray<const FGsSchemaPassivitySet*> upSkillPassivities;
		if (_fairyData.Pin()->GetFairyLevelSkillPassivityList(tooltip.Pin()->level, upSkillPassivities))
		{
			passivityIndex = upSkillPassivities.IndexOfByKey(tooltip.Pin()->passivity);
		}

		if (passivityIndex != INDEX_NONE)
		{
			const FGsSchemaPassivitySet* baseStatPassivity;
			TArray<FGsFairyLevelInfo> passivityList;
			_fairyData.Pin()->GetFairyLevelPassivityList(baseStatPassivity, passivityList);

			Algo::TransformIf(passivityList, skillList, [passivityIndex](auto& e) { return e._skillPassivities.IsValidIndex(passivityIndex);
				}, [passivityIndex](auto& e) {
					return TPair<FairyLevelId, const FGsSchemaPassivitySet*>(e._level, e._skillPassivities[passivityIndex]);
				});
		}
	}
	else
	{
		skillList.Add(TPair<FairyLevelId, const FGsSchemaPassivitySet*>(tooltip.Pin()->level, tooltip.Pin()->passivity));
	}

	if (auto popup = Cast<UGsUIPopupFairySkillTooltip>(GUI()->OpenAndGetWidget(TEXT("PopupFairySkillTooltip")).Get()))
	{
		UGsUIPopupFairySkillTooltip::PopupInitData param{ _fairyData, skillList, tooltip.Pin()->isVehicle ? INVALID_FAIRY_LEVEL : tooltip.Pin()->level, tooltip.Pin()->isVehicle };
		popup->InitializeData(&param);
	}
}

void UGsDivFairyEnchant::SetFairyEnchantBaseInfo()
{
	if (auto fairyData = _fairyData.Pin())
	{
		FairyLevelId curLevel = fairyData->GetCurrentLevel();
		FairyLevelId nextLevel = fairyData->GetUpgradeLevel();
		auto enableUpgrade = fairyData->IsEnableUpgade();

		if (const auto enchantData = GSFairy()->GetFairyEnchantData(fairyData->_id))
		{
			auto enchantLevel = enchantData->enchantLevelList.FindByPredicate([curLevel](FGsSchemaFairyEnchantLevel& e) { return e.level == curLevel; });

			Currency currGold = GGameData()->GetCurrency(CurrencyType::GOLD);
			auto needGold = enchantLevel ? (*enchantLevel).costGold : 0;

			_btnUpgrade->SetData(CurrencyType::GOLD, needGold);
			_btnUpgrade->SetAmountTextColor((currGold >= needGold) ? EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST);
		}

		_btnUpgrade->SetIsEnabled(enableUpgrade);
		
		_textCurLevel = FText::FromString(FString::Format(TEXT("Lv.{0}"), { curLevel }));
		_textNxtLevel = FText::FromString(FString::Format(TEXT("Lv.{0}"), { nextLevel }));

		_maxLevel->SetActiveWidgetIndex(enableUpgrade ? 0 : 1);

		SetFairyUpgradeSkillInfo(fairyData, curLevel, nextLevel);
		SetFairyUnlockStatInfo(fairyData, curLevel, nextLevel);
		SetAttainableFairyCollectionStatInfo(fairyData, curLevel, nextLevel);
	}
}

void UGsDivFairyEnchant::SetFairyUpgradeSkillInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel)
{
	///// 페어리 skill 설명 
	auto enableUpgrade = fairyData->IsEnableUpgade();
	auto level = enableUpgrade ? nextLevel : curLevel;

	TArray<const FGsSchemaPassivitySet*> upSkillPassivities;
	if (fairyData->GetFairyLevelSkillPassivityList(level, upSkillPassivities))
	{
		Algo::Transform(upSkillPassivities, _fairyLevelData, [level, enableUpgrade](const auto passivity) {
			return MakeShared<FGsFairyPassivityInfo>(level, passivity, false, enableUpgrade); });
	}

	if(const auto byVehiclePassivity = fairyData->GetByVehiclePassivity(curLevel))
	{
		_fairyLevelData.Add(MakeShared<FGsFairyPassivityInfo>(curLevel, byVehiclePassivity, true, false));
	}
	//_curScrollHelper->SetEmptyListPanel(_curSkillListPanel);
	_curScrollHelper->RefreshAll(_fairyLevelData.Num());
}

void UGsDivFairyEnchant::SetFairyUnlockStatInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel)
{
	///// 페어리 stat 설명 
	FText FairyUI_OpenStat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_OpenStat"), FairyUI_OpenStat);
	FText FairyUI_PreOpenedStat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_PreOpenedStat"), FairyUI_PreOpenedStat);

	auto curLevelStatPassivity = fairyData->GetFairyLevelStatPassivity(curLevel);
	auto nxtLevelStatPassivity = fairyData->GetFairyLevelStatPassivity(nextLevel);

	auto selectIndex = nxtLevelStatPassivity ? (fairyData->IsCurrentLevelOpen() ? 1 : 0) : 2;
	_switchFairyNextStat->SetActiveWidgetIndex(selectIndex);

	FText fairyNameLevel = GetFairySkillName(fairyData->GetFairyName(), fairyData->GetFairyGrade(), nextLevel);

	_richTextFairyOpenStat->SetText(FText::Format(FairyUI_OpenStat, fairyNameLevel));
	_richTextFairyPreOpenedStat->SetText(FText::Format(FairyUI_PreOpenedStat, fairyNameLevel));

	if (nxtLevelStatPassivity)
	{
		FString strOpenStat;
		GSEffectText()->GetEffectTextPassivity(nxtLevelStatPassivity->id, strOpenStat);
		_openStat = FText::FromString(strOpenStat);
	}
}

void UGsDivFairyEnchant::SetAttainableFairyCollectionStatInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel)
{
	static auto GetStringsSumPassivities = [](IN FairyId fairyId, IN FairyLevelId level, OUT TArray<FString>& strings) {

		TArray<TWeakPtr<FGsFairyCollection>> collections;
		GSFairy()->GetOwnItemCollectionList(collections, fairyId, level);

		TArray<PassivityId> passivities;
		Algo::ForEach(collections, [&passivities](auto collection) {
			if (collection.IsValid()) passivities.Append(collection.Pin()->GetRewardPassivity()); });

		TMap<StatType, int32> statTextResData;
		Algo::ForEachIf(passivities, [](auto passivityId) {
			return UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(passivityId) != nullptr;
			}, [&statTextResData](auto passivityId) {

				const auto tableRow = UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(passivityId);

				TArray<FGsSchemaEffectTextData> allEffectData;
				Algo::ForEach(tableRow->effectList, [&allEffectData](const auto& Effect) { allEffectData.Append(Effect.dataList); });

				Algo::ForEachIf(allEffectData, [](const auto& effectData) {
					return effectData.intParams.IsValidIndex(0) &&
						((effectData.intParams[0] == 0 && effectData.intParams.IsValidIndex(2)) || effectData.intParams.IsValidIndex(4));

					}, [&statTextResData](const auto& effectData) {

						auto stat = static_cast<StatType>(effectData.intParams.Last(1));
						if (statTextResData.Find(stat) == nullptr) statTextResData.Add(stat, 0);
						statTextResData[stat] += effectData.intParams.Last(0);
					});
			});

		statTextResData.KeyStableSort([](auto le, auto re) { return static_cast<uint16>(le) < static_cast<uint16>(re); });

		Algo::Transform(statTextResData, strings, [](auto& statPair) {
			FString strStat;
			FGsStatHelper::GetDesplayStatNameAndValue(statPair.Key, statPair.Value, strStat);
			return strStat;
			});
	};

	///// 컬렉션스탯
	_btnCollection->SetIsEnabled(fairyData->IsEnableUpgade());

	FText collectionStat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_CollectionStatSum"), collectionStat);

	FText fairyNameLevel = GetFairySkillName(fairyData->GetFairyName(), fairyData->GetFairyGrade(), nextLevel);
	_collectionSkillDetail->SetText(FText::Format(collectionStat, fairyNameLevel));

	TArray<FString> statStrings;
	GetStringsSumPassivities(fairyData->_id, nextLevel, statStrings);

	FString devStatString;
	for (auto str : statStrings)
	{
		if (!devStatString.IsEmpty()) devStatString += TEXT("\n");
		devStatString += str;
	}

	_collectionEffectText = FText::FromString(devStatString);
}


bool UGsDivFairyEnchant::CheckEnableFairyUpgrade()
{
	if (!_fairyData.IsValid() || _fairyData.Pin()->IsEnableUpgade() == false) return false;

	if (auto fairyData = _fairyData.Pin())
	{
		auto lv = fairyData->GetCurrentLevel();

		if (const auto enchantData = GSFairy()->GetFairyEnchantData(fairyData->_id))
		{
			auto lvIter = enchantData->enchantLevelList.FindByPredicate([lv](FGsSchemaFairyEnchantLevel& e) { return e.level == lv; });

			int32 ownItemCount = 0;
			int32 useableItemCount = 0;
			auto itemList = GItem()->FindByTID(enchantData->materialItemId, ItemType::MAX);
			for (auto item : itemList)
			{
				if (false == item.IsValid())
					continue;

				if (auto ingredientItem = item.Pin()->GetCastItem<FGsItemIngredient>())
				{
					if (ingredientItem->GetLock() == false)
						useableItemCount += ingredientItem->GetAmount();
					ownItemCount += ingredientItem->GetAmount();
				}
			}
			if (useableItemCount < (*lvIter).materialItemCount)
			{
				FText msg;
				if (FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Levelup_Notice_NotEnoughMaterial"), msg)) /*성장 재료가 부족합니다*/
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
				}
				return false;
			}

			Currency currGold = GGameData()->GetCurrency(CurrencyType::GOLD);
			auto needGold = lvIter ? (*lvIter).costGold : 0;
			if (currGold < needGold)
			{
				FText msg;
				if (FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Levelup_Notice_NotEnoughGold"), msg)) /*골드가 부족합니다*/
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
				}
				return false;
			}
		}
	}

	return true;
}

FText UGsDivFairyEnchant::GetFairySkillName(FText fairyName, FairyGrade grade, FairyLevelId applyLevel)
{
	FText FairyUI_SkillName;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_SkillName"), FairyUI_SkillName);
	FText fairyNameLevel = FText::Format(FairyUI_SkillName, fairyName, applyLevel); //FairyUI_SkillName

	FString strFairySkillName = fairyNameLevel.ToString();
	switch (grade)
	{
	case FairyGrade::NORMAL:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_NORMAL>"), ESearchCase::CaseSensitive);
		break;
	case FairyGrade::MAGIC:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_MAGIC>"), ESearchCase::CaseSensitive);
		break;
	case FairyGrade::RARE:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_RARE>"), ESearchCase::CaseSensitive);
		break;
	case FairyGrade::EPIC:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_EPIC>"), ESearchCase::CaseSensitive);
		break;
	case FairyGrade::UNIQUE:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_UNIQUE>"), ESearchCase::CaseSensitive);
		break;
	case FairyGrade::LEGEND:
		strFairySkillName = strFairySkillName.Replace(TEXT("<Grade>"), TEXT("<GRADE_LEGEND>"), ESearchCase::CaseSensitive);
		break;
	}

	return FText::FromString(strFairySkillName);
}
