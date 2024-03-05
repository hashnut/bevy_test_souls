// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildCampBuffInfoEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"
#include "Management/ScopeGame/GsEffectTextManager.h"


void UGsUIGuildCampBuffInfoEntry::BeginDestroy()
{
	if (_slotHelperConstructionBuff)
	{
		_slotHelperConstructionBuff->OnRefreshEntry.RemoveDynamic(this,
			&UGsUIGuildCampBuffInfoEntry::OnRefreshConstructionBuff);
	}
	_slotHelperConstructionBuff = nullptr;

	if (_slotHelperAreaBuff)
	{
		_slotHelperAreaBuff->OnRefreshEntry.RemoveDynamic(this,
			&UGsUIGuildCampBuffInfoEntry::OnRefreshAreaBuff);
	}
	_slotHelperAreaBuff = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildCampBuffInfoEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperConstructionBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperConstructionBuff->Initialize(_entryWidgetClass, _panelConstructionBuffList);
	_slotHelperConstructionBuff->OnRefreshEntry.AddDynamic(this,
		&UGsUIGuildCampBuffInfoEntry::OnRefreshConstructionBuff);

	_slotHelperAreaBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperAreaBuff->Initialize(_entryWidgetClass, _panelAreaBuffList);
	_slotHelperAreaBuff->OnRefreshEntry.AddDynamic(this,
		&UGsUIGuildCampBuffInfoEntry::OnRefreshAreaBuff);
}

void UGsUIGuildCampBuffInfoEntry::SetData(const FGsSchemaCampData* InData, int32 InBuildingLevel)
{
	if (nullptr == InData)
	{
		return;
	}
	
	FString strTitle = FString::Format(TEXT("{0} Lv.{1}"), { *InData->nameText.ToString(), InBuildingLevel});
	_tbTitle->SetText(FText::FromString(strTitle));

	// 버프정보 표시
	_constructionBuffTextList.Empty();
	_areaBuffTextList.Empty();

	// 건물 건설 전에는 정보가 없다
	if (0 < InBuildingLevel)
	{
		if (FGsEffectTextManager* effectTextMgr = GSEffectText())
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);

			int32 levelIndex = (0 < InBuildingLevel) ? InBuildingLevel - 1 : 0;
			if (InData->constructionBuffList.IsValidIndex(levelIndex))
			{
				effectTextMgr->GetEffectTextListPassivity(InData->constructionBuffList[levelIndex], 
					_constructionBuffTextList, effectTextOption);
			}

			if (InData->areaBuffList.IsValidIndex(levelIndex))
			{
				// 레벨 1 고정. 레벨별 abnormality id가 전부 다름
				effectTextMgr->GetEffectTextListAbnormality(InData->areaBuffList[levelIndex], 
					_areaBuffTextList, 1, effectTextOption);
				//effectTextMgr->GetEffectTextListAbnormality(InData->areaBuffList[levelIndex], 
				//	_areaBuffTextList, InBuildingLevel, effectTextOption);
			}
		}
	}

	_slotHelperConstructionBuff->RefreshAll(_constructionBuffTextList.Num());
	_slotHelperAreaBuff->RefreshAll(_areaBuffTextList.Num());
}

void UGsUIGuildCampBuffInfoEntry::OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry)
{
	if (_constructionBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_constructionBuffTextList[InIndex].Key);
			entry->SetTextBack(_constructionBuffTextList[InIndex].Value);
		}
	}
}

void UGsUIGuildCampBuffInfoEntry::OnRefreshAreaBuff(int32 InIndex, UWidget* InEntry)
{
	if (_areaBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_areaBuffTextList[InIndex].Key);
			entry->SetTextBack(_areaBuffTextList[InIndex].Value);
		}
	}
}
