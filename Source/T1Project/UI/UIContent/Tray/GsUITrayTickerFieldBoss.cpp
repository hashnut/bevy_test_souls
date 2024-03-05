// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerFieldBoss.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "GsTable.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UMG/Public/Components/Image.h"
#include "UTIL/GsTableUtil.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Paper2D/Classes/PaperSprite.h"

void UGsUITrayTickerFieldBoss::NativeOnInitialized()
{
	Super::NativeOnInitialized();
		
	_finishDelegate.BindDynamic(this, &UGsUITrayTickerFieldBoss::OnAnimationFinish);
	BindToAnimationFinished(_fieldBossTickerAnim, _finishDelegate);
}

void UGsUITrayTickerFieldBoss::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUITrayTickerFieldBoss::SetFieldBossType(EGsFieldBossTickerStateType inType, int32 creatureId)
{
	switch (inType)
	{
	case EGsFieldBossTickerStateType::Spawn:
	{
		_fieldBossWidgetSwitcher->SetActiveWidgetIndex((int32)inType);
		InvalidFieldBossImage(creatureId);	
		PlayAnimation(_fieldBossTickerAnim);
		break;
	}				
	case EGsFieldBossTickerStateType::Die:
	{
		_fieldBossWidgetSwitcher->SetActiveWidgetIndex((int32)inType);
		InvalidFieldBossImage(creatureId);
		PlayAnimation(_fieldBossTickerAnim);
		break;
	}		
	default:
		break;
	}
}

void UGsUITrayTickerFieldBoss::InvalidFieldBossImage(int32 inCreatrueId)
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
		_fieldBossImage->SetBrushFromAtlasInterface(spriteImg);
	}

	_fieldBossNameText->SetText(npcData->nameText);
}

void UGsUITrayTickerFieldBoss::OnAnimationFinish()
{
	Close(true);
}