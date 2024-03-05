// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIQuestSlotHelper.h"
#include "UI/Quest/GsSchemaQuestSlotColor.h"
#include "UI/Quest/GsSchemaQuestSlotData.h"
#include "UI/Quest/GsSchemaQuestSlotImg.h"
#include "GsSchemaEnums.h"
#include "T1Project.h"
#include "PaperSprite.h"
#include "Color.h"

const FGsSchemaQuestSlotColor* GetSchemaQuestSlotColor(const FGsSchemaQuestSlotData* inSchema)
{
	const FGsSchemaQuestSlotColor* shemaQuestSlotColor =
		inSchema->typeInfo.GetRowByType<FGsSchemaQuestSlotColor>();

	return shemaQuestSlotColor;
}

const FGsSchemaQuestSlotImg * GetSchemaQuestSlotImg(const FGsSchemaQuestSlotData* inSchema)
{
	const FGsSchemaQuestSlotImg* shemaQuestSlotImg = 
		inSchema->typeInfo.GetRowByType<FGsSchemaQuestSlotImg>();

	return shemaQuestSlotImg;
}

const FLinearColor& FGsUIQuestSlotHelper::GetColor(EGsQuestSlotType InType)
{
	const UGsTableQuestSlotData* table = Cast<UGsTableQuestSlotData>(FGsSchemaQuestSlotData::GetStaticTable());
	if (table)
	{
		const FGsSchemaQuestSlotData* row = nullptr;
		if (table->FindRowById(InType, row))
		{
			const FGsSchemaQuestSlotColor* schemaQuestSlotColor = GetSchemaQuestSlotColor(row);
			if (nullptr != schemaQuestSlotColor)
			{
				return schemaQuestSlotColor->color;
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[QuestSlotHelper] Fail to find color. QuestSlotType: %d"), static_cast<uint8>(InType));
#endif

	return FLinearColor::White;
}

UPaperSprite* FGsUIQuestSlotHelper::GetImg(EGsQuestSlotType InType)
{
	const UGsTableQuestSlotData* table = Cast<UGsTableQuestSlotData>(FGsSchemaQuestSlotData::GetStaticTable());
	if (table)
	{
		const FGsSchemaQuestSlotData* row = nullptr;
		if (table->FindRowById(InType, row))
		{
			const FGsSchemaQuestSlotImg* schemaQuestSlotImg = GetSchemaQuestSlotImg(row);
			if (nullptr != schemaQuestSlotImg)
			{
				if (UPaperSprite* loadObject = Cast<UPaperSprite>(schemaQuestSlotImg->imgPath.TryLoad()))
				{
					return loadObject;
				}
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[QuestSlotHelper] Fail to find Img. QuestSlotType: %d"), static_cast<uint8>(InType));
#endif

	return nullptr;
}