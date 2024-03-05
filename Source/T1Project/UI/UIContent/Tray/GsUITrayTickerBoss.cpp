// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerBoss.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "GsTable.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UMG/Public/Components/Image.h"
#include "UTIL/GsTableUtil.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Paper2D/Classes/PaperSprite.h"

void UGsUITrayTickerBoss::NativeOnInitialized()
{
	Super::NativeOnInitialized();
		
	_finishDelegate.BindDynamic(this, &UGsUITrayTickerBoss::OnAnimationFinish);
	BindToAnimationFinished(_bossTickerAnim, _finishDelegate);
}

void UGsUITrayTickerBoss::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUITrayTickerBoss::SetData(EGsBossTickerStateType inType, int32 creatureId)
{
	switch (inType)
	{
	case EGsBossTickerStateType::Spawn:
	{
		_bossWidgetSwitcher->SetActiveWidgetIndex((int32)inType);
		InvalidBossImage(creatureId);
		PlayAnimation(_bossTickerAnim);
		break;
	}				
	case EGsBossTickerStateType::Die:
	{
		_bossWidgetSwitcher->SetActiveWidgetIndex((int32)inType);
		InvalidBossImage(creatureId);
		PlayAnimation(_bossTickerAnim);
		break;
	}		
	default:
		break;
	}
}

void UGsUITrayTickerBoss::InvalidBossImage(int32 inCreatrueId)
{
	//_creatureId	
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inCreatrueId);
	if (nullptr == npcData)
	{
		return;
	}

	const FGsSchemaShapeData* shape = npcData->shapeId.GetRow();
	if (shape)
	{
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(shape->imageRegionMap);
		_bossImage->SetBrushFromAtlasInterface(spriteImg);
	}

	_bossNameText->SetText(npcData->nameText);
}

void UGsUITrayTickerBoss::OnAnimationFinish()
{
	Close(true);
}