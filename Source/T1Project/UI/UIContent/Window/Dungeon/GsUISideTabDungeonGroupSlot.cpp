// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISideTabDungeonGroupSlot.h"

#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonGroupNormal.h"

#include "Image.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "WidgetSwitcher.h"
#include "CanvasPanel.h"

#include "../UTIL/GsCronUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Icon//GsUIIconAbnormality.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UMG/Public/Components/PanelWidget.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/GsSchemaResetTimeTicketConfig.h"



void UGsUISideTabDungeonGroupSlot::BeginDestroy()
{
	if (_btnSlot)
	{
		_btnSlot->OnClicked.RemoveDynamic(this, &UGsUISideTabDungeonGroupSlot::OnClickSlot);
		_btnSlot = nullptr;
	}	

	Super::BeginDestroy();
}

void UGsUISideTabDungeonGroupSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnSlot->OnClicked.AddDynamic(this, &UGsUISideTabDungeonGroupSlot::OnClickSlot);
}

void UGsUISideTabDungeonGroupSlot::SetData(const FGsDungeonGroupBase* InDungeonGroup)
{
	if (nullptr == InDungeonGroup)
	{
		GSLOG(Error, TEXT("nullptr == InDungeonGroup"));
		return;
	}

	_data = InDungeonGroup;

	const FGsSchemaDungeonGroupData* schemaData = _data->GetSchemaData();

	// 2022/08/17 PKT - Dungeon Image	
	UPaperSprite* dungeonDBImg = FGsUIHelper::LoadSpriteFromPath(schemaData->dungeonGroupSpriteImg);
	if (dungeonDBImg)
	{
		_imgDungeonGroup->SetBrushFromAtlasInterface(dungeonDBImg);
	}

	// 2022/08/17 PKT - 던전 그룹 이름.
	_textName->SetText(schemaData->dungeonGroupName);

	// 2023/2/22 PKT - 환경 효과 아이콘
	if (_data->GetEnvPassivity())
	{
		_iconSelectorEnv->SetVisibility(ESlateVisibility::Visible);

		UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(_iconSelectorEnv->GetChildAt(0));
		if (nullptr == icon)
		{
			icon = Cast<UGsUIIconAbnormality>(_iconSelectorEnv->CreateIcon());
			_iconSelectorEnv->AddChild(icon);
		}
		icon->SetData(_data->GetEnvPassivity());
	}
	else
	{
		_iconSelectorEnv->SetVisibility(ESlateVisibility::Collapsed);
	}	

	DrawPKMark(_data);

	_markEvent->SetVisibility(_data->IsEventGroup() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	Invalidate();
}

void UGsUISideTabDungeonGroupSlot::Invalidate()
{
	if (nullptr == _data)
	{
		return;
	}

	SetTime(_data);
	
	/**
	 * UnLock Contents
	 */
	ESlateVisibility visibility = _data->IsUnLockContents() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible;
	_canvasPanelLockContents->SetVisibility(visibility);

	if (ESlateVisibility::HitTestInvisible == visibility)
	{
		FText textCondition;
		_data->LockContentsReason(textCondition);

		_textLockReason->SetText(textCondition);
	}
}

const FGsDungeonGroupBase* UGsUISideTabDungeonGroupSlot::GetData() const
{
	return _data;
}

void UGsUISideTabDungeonGroupSlot::OnClickSlot()
{
	if (_data && false == GetIsSelected() && _OnClickedSlotDelegate.IsBound())
	{
		_OnClickedSlotDelegate.ExecuteIfBound(_data);
	}
}

void UGsUISideTabDungeonGroupSlot::SetIsSelected(bool bInIsSelected)
{
	if (nullptr != _selectedEdgeImg)
	{	// 2023/2/21 PKT - Set Toggle Image
		_selectedEdgeImg->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

bool UGsUISideTabDungeonGroupSlot::GetIsSelected() const
{
	return (nullptr != _selectedEdgeImg && ESlateVisibility::SelfHitTestInvisible == _selectedEdgeImg->GetVisibility());
}

void UGsUISideTabDungeonGroupSlot::DrawPKMark(const FGsDungeonGroupBase* InData)
{
	_swicherPk->SetVisibility(ESlateVisibility::Collapsed);

	if (nullptr == InData || DungeonCategory::PARTY == InData->GetCategory())
	{
		return;
	}

	const static int32 ENTRY_DUNGEON_INDEX = 0;
	const FGsDungeonData* dungeonData = InData->FindDungeonByIndex(ENTRY_DUNGEON_INDEX);
	if (nullptr == dungeonData)
	{
		GSLOG(Error, TEXT("nullptr == dungeonData"));
		return;
	}

	const FGsSchemaMapData* schemaMapData = dungeonData->GetSchema()->mapId.GetRow();

	// 2023/2/22 PKT - 정예 던전에서는 PvP 표기 안함.( 왜??? )
	if (DungeonCategory::ELITE != InData->GetCategory())
	{
		_swicherPk->SetActiveWidgetIndex((true == schemaMapData->balanceId.GetRow()->EnablePK) ? 1 : 0);
		_swicherPk->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UGsUISideTabDungeonGroupSlot::SetTime(const FGsDungeonGroupBase* InDungeonGroup)
{
	_panelTimeInfo->SetVisibility(ESlateVisibility::Collapsed);
	_textBaseTime->SetVisibility(ESlateVisibility::Collapsed);
	_swicherChargeTime->SetVisibility(ESlateVisibility::Collapsed);
	_textResetTime->SetVisibility(ESlateVisibility::Collapsed);
	
	if (DungeonCategory::PARTY == InDungeonGroup->GetCategory() || DungeonCategory::ARENA == InDungeonGroup->GetCategory())
	{
		return;
	}

	const FGsDungeonGroupNormal* normalDungeonGroupData = StaticCast<const FGsDungeonGroupNormal*>(InDungeonGroup);
	const FGsSchemaDungeonGroupData* schemaDungeonGroupData = normalDungeonGroupData->GetSchemaData();

	_panelTimeInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	/**
	 * 1. 던전 기본 제공 시간
	 */
	_textBaseTime->SetVisibility(ESlateVisibility::HitTestInvisible);

	const static int32 UNLIMIT_TIME = -1;
	if (UNLIMIT_TIME >= normalDungeonGroupData->GetNormalTime())
	{
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("DungeonBaseTimeUnlimit"), findText);
		_textBaseTime->SetText(findText);
	}
	else
	{
		FString textBaseTime = FGsTimeStringHelper::GetMinuteString(normalDungeonGroupData->GetNormalTime() / 60);
		_textBaseTime->SetText(FText::FromString(textBaseTime));
	}

	/**
	 * 2. 던전 추가 시간
	 */
	_swicherChargeTime->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (UNLIMIT_TIME >= normalDungeonGroupData->GetNormalTime())
	{
		_swicherChargeTime->SetActiveWidgetIndex(1);
	}
	else
	{
		int chargeTime = normalDungeonGroupData->GetChargeTime() / 60;
		if (0 < chargeTime)
		{
			static const int32 SHOW_CHARGE_TIME_TEXT = 0;
			_swicherChargeTime->SetActiveWidgetIndex(SHOW_CHARGE_TIME_TEXT);

			FString textChargeTime = FGsTimeStringHelper::GetMinuteString(chargeTime);
			_textChargeTime->SetText(FText::FromString(textChargeTime));
		}
		else
		{
			static const int32 BLANK_CHARGE_TIME_TEXT = 1;
			_swicherChargeTime->SetActiveWidgetIndex(BLANK_CHARGE_TIME_TEXT);
		}
	}

	static const int32 USER_INDEX = 0;
	static const int32 ACCOUNT_INDEX = 1;
	_contentsBindType->SetActiveWidgetIndex(ContentsBindingType::ACCOUNT == normalDungeonGroupData->ContentsBindType() ? ACCOUNT_INDEX : USER_INDEX);

	
	/**
	 * 3. 초기화 시간
	 */
	// 초기화 시간
	FString strCron;
	FGsCronUtil::GetCronString(schemaDungeonGroupData->resetCycling.GetRow()->makeCron, strCron);
	_textResetTime->SetText(FText::FromString(strCron));
	_textResetTime->SetVisibility(ESlateVisibility::HitTestInvisible);
}
