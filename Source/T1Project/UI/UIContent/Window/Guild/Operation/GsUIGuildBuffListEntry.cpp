// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildBuffListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsGrayscaleImage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "Guild/GsGuildBuildingData.h"
#include "Guild/GsGuildHelper.h"
#include "PaperSprite.h"

void UGsUIGuildBuffListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildBuffListEntry::OnClickSlot);
}

void UGsUIGuildBuffListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildBuffListEntry::SetData(const FGsSchemaGuildBuilding* InData)
{
	// 아이콘과 이름
	_imgIcon->SetPaperSprite(FGsUIHelper::LoadSpriteFromPath(InData->buffIconPath));
	_textBlockName->SetText(InData->nameText);

	int32 currBuildingLevel = 0;
	bool bIsBuffActive = false; // 활성중인 버프인가

	FGsGuildManager* guildMgr = GGuild();
	const FGsGuildBuildingData* buildingData = guildMgr->GetBuildingData(InData->id);
	if (buildingData)
	{
		currBuildingLevel = buildingData->GetCurrBuildingLevel();
		bIsBuffActive = buildingData->IsBuffActive();
	}

	_imgIcon->SetGrayscale((0 == currBuildingLevel) ? true : false);

	// 건물 레벨 표시
	FString strLevel = FString::Format(TEXT("Lv.{0}"), { currBuildingLevel });
	_textBlockLevel->SetText(FText::FromString(strLevel));

	// 효과 텍스트 표시
	FText textEffect;
	FGsGuildHelper::GetBuildingEffectString(InData, currBuildingLevel, textEffect);
	_textBlockEffect->SetText(textEffect);

	// 활성중인가
	_panelActive->SetVisibility(bIsBuffActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_panelDimmed->SetVisibility(bIsBuffActive ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIGuildBuffListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
