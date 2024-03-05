// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIStatChangePanel.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UMG/Public/Components/ListViewBase.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Item/Data/GsItemStatData.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "GsUIStatChangeListViewEntry.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Data/GsDataContainManager.h"
#include "UTIL/GsText.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "DataSchema/CombatPower/GsSchemaCombatPowerData.h"



void UGsUIStatChangePanel::NativeConstruct()
{
	Super::NativeConstruct();

	_tickEnable = false;
}

void UGsUIStatChangePanel::NativeTick(const FGeometry& inMyGeometry, float InDeltaTime)
{
	Super::NativeTick(inMyGeometry, InDeltaTime);

	if (false == _tickEnable)
	{
		return;
	}

	_time += InDeltaTime;

	if (_time > _statNotifyShowTime)
	{		
		if (CheckFinish())
		{
			return;
		}

		InvalidStatSet();
				
		_time = 0;
	}
}

void UGsUIStatChangePanel::Play(StatUpdateReason inReason, const float inOption, const TMap<StatType, TPair<int, int>>& inStatDiffMap, TFunction<void(void)> inCallbackFunc)
{	
	_reasonType = inReason;
	_callbackFunc = inCallbackFunc;

	_statMap = inStatDiffMap;
	InvalidCombatPower(_statMap);
	_statMap.GenerateKeyArray(_statKeyArray);

	_time = 0;
	_statSetIndex = 0;
	_maxStatSetCount = 0;

	//연출을 위한 값 설정
	if (auto dataManager = GData())
	{
		_statSetShowCount = dataManager->GetGlobalData()->_statNotifyShowCount;
		_statNotifyShowTime = dataManager->GetGlobalData()->_statNotifyShowCount;
		_statNotifyProduceTime = dataManager->GetGlobalData()->_statNotifyShowCount;
	}

	//전투력만 보여야 하는 경우
	if (FGsStatChangeNotifyManager::EGsCombatPowerUI::ShowOnlyCombatPower == (FGsStatChangeNotifyManager::EGsCombatPowerUI)inOption)
	{
		_statKeyArray.Empty();
		StartPlayAnimation();
		return;
	}

	//전투력과 스탯을 보이는 경우
	_preWeaponType = GSStatChangeNotify()->GetPreWeaponType();
	_curWeaponType = FGsStatHelper::GetPlayerWeapon();

#if STAT_CHANGE_NOTIFY_DEBUG
	FString preWeaponText = FGsTextUtil::GetStringFromEnum(TEXT("CreatureWeaponType"), _preWeaponType);
	FString curWeaponText = FGsTextUtil::GetStringFromEnum(TEXT("CreatureWeaponType"), _curWeaponType);
	GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - pre weapon : %s\tcur weapon : %s"), *preWeaponText, *curWeaponText);
#endif

	RemoveShowStatType();

	_maxStatSetCount = ((_statKeyArray.Num() % _statSetShowCount) == 0) ? (_statKeyArray.Num() / _statSetShowCount) : ((_statKeyArray.Num() / _statSetShowCount) + 1);

#if STAT_CHANGE_NOTIFY_DEBUG
	GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - max stat set count : %d\tstat count ; %d"), _maxStatSetCount, _statKeyArray.Num());
#endif
	
	TFunction<bool(StatType, StatType)> sortFunc = [&](StatType sourceStatType, StatType destStatType)->bool {
		//section check
		const auto* sourceStatData = FGsStatHelper::FindCombatPowerStat(sourceStatType);
		const auto* destStatData = FGsStatHelper::FindCombatPowerStat(destStatType);

		//다음과 같은 순서로 스탯을 보여준다.
		//1. 공용 스탯
		//2. 무기 스탯
		//3. uiOrder
		if (sourceStatData && destStatData)
		{
			bool isImportantSource = false;
			bool isImportantDest = false;
			//wepon check
			if (StatUpdateReason::ITEM_EQUIP == _reasonType)
			{
				isImportantSource = (sourceStatData->weaponType.Contains(_preWeaponType) || sourceStatData->weaponType.Contains(_curWeaponType));
				isImportantDest = (sourceStatData->weaponType.Contains(_preWeaponType) || sourceStatData->weaponType.Contains(_curWeaponType));
			}
			else
			{
				isImportantSource = sourceStatData->weaponType.Contains(_curWeaponType);
				isImportantDest = destStatData->weaponType.Contains(_curWeaponType);
			}

			if (isImportantSource && false == isImportantDest)
			{
				return true;
			}
			else if (isImportantDest && false == isImportantSource)
			{
				return false;
			}

			return sourceStatData->uiOrderNum < destStatData->uiOrderNum;
		}

		return false;
	};

	_statKeyArray.Sort(sortFunc);
	StartPlayAnimation();
}

void UGsUIStatChangePanel::OnFinishAnimation()
{
	if (_callbackFunc)
	{
		_callbackFunc();
	}	
}

void UGsUIStatChangePanel::InvalidStatSet()
{
	int32 index = _statSetIndex * _statSetShowCount;
	int32 showCount = 0;
	TArray<UGsUIStatChangeListViewEntryData*> entryDataArray;
	while (_statKeyArray.IsValidIndex(index))
	{
		if (showCount >= _statSetShowCount)
		{
			break;
		}

		UGsUIStatChangeListViewEntryData* entryData = NewObject<UGsUIStatChangeListViewEntryData>();
		const auto statKey = _statKeyArray[index];
		const auto& statValue = _statMap[statKey];
		entryData->Init(statKey, statValue.Key, statValue.Value);
		entryDataArray.Add(entryData);

#if STAT_CHANGE_NOTIFY_DEBUG
		FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), statKey);
		GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - index : %d\tstat type : %s\tshow count : %d"), index, *statTypeText, showCount);
#endif

		++showCount;
		++index;
	}

	_statListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_statListView->SetListItems(entryDataArray);

	++_statSetIndex;
}

bool UGsUIStatChangePanel::CheckFinish()
{
	if (_maxStatSetCount <= _statSetIndex)
	{
		//끝남을 알림 
		OnFinishAnimation();
		_tickEnable = false;
		return true;
	}

	return false;
}

void UGsUIStatChangePanel::RemoveShowStatType()
{
	//combat power는 UI 상단에 보여주므로 제거
	if (_statKeyArray.Contains(StatType::COMBAT_POWER))
	{
		_statKeyArray.Remove(StatType::COMBAT_POWER);
	}

	TArray<StatType> statKeyArray = _statKeyArray;
	for (StatType stat : statKeyArray)
	{
		const auto* statInfo = FGsStatHelper::FindCombatPowerStat(stat);
		if (nullptr == statInfo)
		{
			_statKeyArray.Remove(stat);

#if STAT_CHANGE_NOTIFY_DEBUG
			FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), stat);
			GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - remove stat type : %s"), *statTypeText);
#endif
			continue;
		}

		if (StatUpdateReason::ITEM_EQUIP == _reasonType)
		{
			if(0 == statInfo->weaponType.Num() ||
				(false == statInfo->weaponType.Contains(_curWeaponType)
				&& false == statInfo->weaponType.Contains(_preWeaponType)))
			{
				_statKeyArray.Remove(stat);

#if STAT_CHANGE_NOTIFY_DEBUG
				FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), stat);
				GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - remove stat type : %s"), *statTypeText);
#endif
			}
		}
		else
		{
			if (false == statInfo->weaponType.Contains(_curWeaponType))
			{
				_statKeyArray.Remove(stat);

#if STAT_CHANGE_NOTIFY_DEBUG
				FString statTypeText = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), stat);
				GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - remove stat type : %s"), *statTypeText);
#endif
			}
		}			
	}
}

void UGsUIStatChangePanel::StartPlayAnimation()
{
	StopAllAnimations();
	PlayAnimation(_openAnim, 0.0f, 1);
	InvalidStatSet();
	_tickEnable = true;
}

//전투력 변경시 갱신
void UGsUIStatChangePanel::InvalidCombatPower(const TMap<StatType, TPair<int, int>>& inStatDiffMap)
{
	if (false == inStatDiffMap.Contains(StatType::COMBAT_POWER))
	{
		InitCombatPower();
		return;
	}

	auto data = inStatDiffMap[StatType::COMBAT_POWER];
	int value = FMath::Abs(data.Value - data.Key);

	//신규 값이 이전 값보다 좋으면 
	if (data.Key < data.Value)
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Up);
		_combatPowerTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::UP_COMPARE_STAT_COLOR));
		_combatPowerTextBlock->SetText(FText::AsNumber(value));
	}
	else if (data.Key > data.Value)
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Down);
		_combatPowerTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DOWN_COMPARE_STAT_COLOR));
		_combatPowerTextBlock->SetText(FText::AsNumber(value));
	}
	else
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Same);
		_combatPowerTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));
		_combatPowerTextBlock->SetText(FText::GetEmpty());
	}	
}

void UGsUIStatChangePanel::InitCombatPower()
{
	_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Same);
	_combatPowerTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));

	auto* localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
	if (nullptr == localPlayer)
	{		
		_combatPowerTextBlock->SetText(FText::GetEmpty());
		return;
	}

	auto* combatPowerStat = localPlayer->GetCreatureStatInfoByType(StatType::COMBAT_POWER);
	if (nullptr == combatPowerStat)
	{
		_combatPowerTextBlock->SetText(FText::GetEmpty());
		return;
	}

	_combatPowerTextBlock->SetText(FText::GetEmpty());
}
