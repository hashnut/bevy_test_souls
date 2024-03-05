// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyListEntryBase.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Guild/GsGuildHelper.h"

void UGsUIGuildDiplomacyListEntryBase::SetBaseData(const FGsGuildDiplomacyDataBase* InData)
{
	_guildDBId = InData->_guildDBId;

	if (const FGsSchemaGuildEmblem* tableEmblem = FGsGuildHelper::GetGuildEmblem(InData->_emblemId))
	{
		_imgEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(tableEmblem->iconPathSmall));
	}
	
	FText textGuildName;
	FGsGuildHelper::GetFormattedGuildName(InData->_guildName, InData->GetWorldId(), false, textGuildName);
	_tbGuildName->SetText(textGuildName);

	_tbMasterName->SetText(FText::FromString(InData->_masterName));
}
