// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsSchemaEnums.h"

class UPaperSprite;
struct FLinearColor;
class UPaperSprite;
/**
 * 
 */
class T1PROJECT_API FGsUIQuestSlotHelper
{
public:
	static const FLinearColor& GetColor(EGsQuestSlotType InType);
	static UPaperSprite* GetImg(EGsQuestSlotType InType);
};
