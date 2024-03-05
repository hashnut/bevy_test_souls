// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISanctumNexusInfoByLevel.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "Management/ScopeGame/GsEffectTextManager.h"


void UGsUISanctumNexusInfoByLevel::BeginDestroy()
{
	if (_slotHelperConstructionBuff)
	{
		_slotHelperConstructionBuff->OnRefreshEntry.RemoveDynamic(this,
			&UGsUISanctumNexusInfoByLevel::OnRefreshConstructionBuff);
	}
	_slotHelperConstructionBuff = nullptr;

	Super::BeginDestroy();
}

void UGsUISanctumNexusInfoByLevel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperConstructionBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperConstructionBuff->Initialize(_entryWidgetClass, _panelConstructionBuffList);
	_slotHelperConstructionBuff->OnRefreshEntry.AddDynamic(this,
		&UGsUISanctumNexusInfoByLevel::OnRefreshConstructionBuff);
}

void UGsUISanctumNexusInfoByLevel::SetData(const FGsSchemaSanctumNexusData* InData, int32 InLevel)
{
	if (nullptr == InData)
	{
		return;
	}

	// TEXT: 수호탑 Lv.{0}
	FText textFormat;
	if (FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Occupy_NeedLevel"), textFormat))
	{		
		FText textNexusLevel = FText::Format(textFormat, InLevel);

		_tbTitle->SetText(textNexusLevel);
	}

	// 버프정보 표시
	_constructionBuffTextList.Empty();

	// 건물 건설 전에는 정보가 없다
	if (0 < InLevel)
	{
		if (FGsEffectTextManager* effectTextMgr = GSEffectText())
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);

			int32 levelIndex = (0 < InLevel) ? InLevel - 1 : 0;
			if (InData->constructionBuffList.IsValidIndex(levelIndex))
			{
				if (const FGsSchemaPassivitySet* passivityData = InData->constructionBuffList[levelIndex].GetRow())
				{
					effectTextMgr->GetEffectTextListPassivity(passivityData->id,
						_constructionBuffTextList, effectTextOption);
				}
			}
		}
	}

	_slotHelperConstructionBuff->RefreshAll(_constructionBuffTextList.Num());
}

void UGsUISanctumNexusInfoByLevel::OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry)
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
