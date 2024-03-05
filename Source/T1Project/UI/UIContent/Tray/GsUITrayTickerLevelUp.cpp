// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerLevelUp.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsGameObjectUtil.h"
#include "LevelUp/GsLevelUpStatListViewEntry.h"
#include "UMG/Public/Components/ListViewBase.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Item/Data/GsItemStatData.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "StatChange/GsUIStatChangePanel.h"
#include "DataSchema/User/Level/GsSchemaLevelStatTable.h"


void UGsUITrayTickerLevelUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_closeButton->OnClicked.AddDynamic(this, &UGsUITrayTickerLevelUp::OnClickClose);

	_animationFinishEvent.Clear();
	_animationFinishEvent.BindUFunction(this, FName(FString(TEXT("OnFinishAnimation"))));
	BindToAnimationFinished(_ani_end, _animationFinishEvent);
}

void UGsUITrayTickerLevelUp::NativeConstruct()
{
	Super::NativeConstruct();

	_closeButton->SetIsEnabled(true);

	_animationFinishEvent.Clear();

	PlayAnimation(_ani_start);
}

void UGsUITrayTickerLevelUp::NativeDestruct()
{
	StopAnimation(_ani_start);
	StopAnimation(_ani_end);
	
	_animationFinishEvent.Clear();

	_closeButton->SetIsEnabled(false);

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_LEVEL_UP);
	}

	Super::NativeConstruct();
}

FReply UGsUITrayTickerLevelUp::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (_closeButton->bIsEnabled)
	{
		OnClickClose();
	}	

	return FReply::Handled();
}

void UGsUITrayTickerLevelUp::CreateStatMap(OUT TMap<StatType, int32>& outStatMap, const FGsSchemaLevelStatTable* inStat)
{	
	outStatMap.Empty();
	if (StatType::NONE != inStat->statType1)
	{
		outStatMap.Emplace(inStat->statType1, inStat->statValue1);
	}
	if (StatType::NONE != inStat->statType2)
	{
		outStatMap.Emplace(inStat->statType2, inStat->statValue2);
	}
	if (StatType::NONE != inStat->statType3)
	{
		outStatMap.Emplace(inStat->statType3, inStat->statValue3);
	}
	if (StatType::NONE != inStat->statType4)
	{
		outStatMap.Emplace(inStat->statType4, inStat->statValue4);
	}
	if (StatType::NONE != inStat->statType5)
	{
		outStatMap.Emplace(inStat->statType5, inStat->statValue5);
	}
	if (StatType::NONE != inStat->statType6)
	{
		outStatMap.Emplace(inStat->statType6, inStat->statValue6);
	}
	if (StatType::NONE != inStat->statType7)
	{
		outStatMap.Emplace(inStat->statType7, inStat->statValue7);
	}
	if (StatType::NONE != inStat->statType8)
	{
		outStatMap.Emplace(inStat->statType8, inStat->statValue8);
	}
	if (StatType::NONE != inStat->statType9)
	{
		outStatMap.Emplace(inStat->statType9, inStat->statValue9);
	}
	if (StatType::NONE != inStat->statType10)
	{
		outStatMap.Emplace(inStat->statType10, inStat->statValue10);
	}
}

void UGsUITrayTickerLevelUp::SetData(uint16 inPreLevel, uint16 inCurrentLevel)
{	
	_textBlockLevel->SetText(FText::AsNumber(inCurrentLevel));
	
	const UGsTable* table = FGsSchemaLevelStatTable::GetStaticTable();
	const UGsTableLevelStatTable* levelStatTable = Cast<UGsTableLevelStatTable>(table);
	if (nullptr == levelStatTable)
	{
		return;
	}

	TArray<const FGsSchemaLevelStatTable*> levelStatArray;
	levelStatTable->GetAllRows(levelStatArray);

	//get different stat point between current level and pre level
	const FGsSchemaLevelStatTable** preStatPtr = levelStatArray.FindByPredicate([&](const FGsSchemaLevelStatTable* inFind){
		return inFind->level == inPreLevel;
	});
	const FGsSchemaLevelStatTable** currentStatPtr = levelStatArray.FindByPredicate([&](const FGsSchemaLevelStatTable* inFind) {
		return inFind->level == inCurrentLevel;
	});
	if (nullptr == preStatPtr || nullptr == currentStatPtr)
	{
		GSLOG(Error, TEXT("stat missing - level : %d"), inCurrentLevel);
		return;
	}

	const FGsSchemaLevelStatTable* preStat = *preStatPtr;
	const FGsSchemaLevelStatTable* currentStat = *currentStatPtr;
	if (nullptr == preStat || nullptr == currentStat)
	{
		GSLOG(Error, TEXT("stat missing - level : %d"), inCurrentLevel);
		return;
	}

	TMap<StatType, int32> preStatMap;
	CreateStatMap(preStatMap, preStat);

	TMap<StatType, int32> newStatMap;
	CreateStatMap(newStatMap, currentStat);

	TMap<StatType, TPair<int, int>> statDiffMap;
	FGsStatChangeInfo::TryDiffStat(preStatMap, newStatMap, statDiffMap);

	if (false == statDiffMap.Contains(StatType::COMBAT_POWER))
	{
		statDiffMap.Emplace(StatType::COMBAT_POWER, GSStatChangeNotify()->GetLevelUpCombatPower());
	}

	float option = 0.f;
	bool isVisible = false;
	GOption()->GetValue(EGsOptionItem::CombatPower, option, isVisible);
	FGsStatChangeNotifyManager::EGsCombatPowerUI optionType = (FGsStatChangeNotifyManager::EGsCombatPowerUI)option;
	_statChangePanel->SetVisibility(FGsStatChangeNotifyManager::EGsCombatPowerUI::Off == optionType ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (FGsStatChangeNotifyManager::EGsCombatPowerUI::Off != optionType)
	{
		_statChangePanel->Play(StatUpdateReason::USER_LEVEL_UP, option, statDiffMap, [this]() {
			PlayAnimation(_ani_end);
			});
	}
}

void UGsUITrayTickerLevelUp::OnClickClose()
{
	StopAnimation(_ani_start);
	_closeButton->SetIsEnabled(false);

	//play close animation
	PlayAnimation(_ani_end);
}

void UGsUITrayTickerLevelUp::OnFinishAnimation()
{
	GUI()->RemoveWidget(this);
}

void UGsUILevelUpStatPanel::SetData(const TMap<StatType, FGsStatDiffInfo>& inStatMap)
{
	UGsLevelUpStatListViewEntryData* statData = nullptr;
	TArray<UGsLevelUpStatListViewEntryData*> dataArray;

	int32 entryNum = (int32)((inStatMap.Num() + 1) / 2);
	int32 dataNum = inStatMap.Num();

	TArray<StatType> statKeyArray;
	TArray<FGsStatDiffInfo> statValueArray;

	inStatMap.GenerateKeyArray(statKeyArray);
	inStatMap.GenerateValueArray(statValueArray);
	int32 index = 0;

	for (int32 i = 0; i < entryNum; ++i)
	{	
		statData = NewObject<UGsLevelUpStatListViewEntryData>();
		dataArray.Add(statData);

		index = i * 2;		
		statData->_statType1 = statKeyArray[index];
		statData->_statDiff1 = statValueArray[index];
		statData->_showStat1 = true;

		index = (i * 2) + 1;
		if (index < dataNum)
		{
			statData->_statType2 = statKeyArray[index];
			statData->_statDiff2 = statValueArray[index];
			statData->_showStat2 = true;			
		}
		else
		{
			statData->_showStat2 = false;
		}				
	}

	_statListView->SetListItems(dataArray);
	_statListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}
