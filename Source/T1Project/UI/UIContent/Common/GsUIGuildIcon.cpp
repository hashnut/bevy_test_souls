// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildIcon.h"
#include "Components/Image.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "PaperSprite.h"
#include "Classes/Engine/Texture2D.h"


void UGsUIGuildIcon::SetEmblemId(GuildEmblemId InEmblemId, bool bInBigSize)
{
	const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InEmblemId);
	if (emblemData)
	{
		_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (bInBigSize)
		{
			_imgIcon->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(emblemData->iconPathBig));
		}
		else
		{
			_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall));
		}
	}
	else
	{
		_imgIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void UGsUIGuildIcon::SetInvasionEnemyIcon()
{
	_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(GData()->GetGlobalData()->_invasionEnemyIcon));
}