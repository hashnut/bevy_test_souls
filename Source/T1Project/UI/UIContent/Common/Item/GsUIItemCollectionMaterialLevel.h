// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Item/Collection/EItemCollectionEnumData.h"
#include "GsUIItemCollectionMaterialLevel.generated.h"

/**
 * ItemCollection 에 등록 가능한 아이템 일 경우 레벨을 표기
 */
UCLASS()
class T1PROJECT_API UGsUIItemCollectionMaterialLevel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UImage* _imgDelimiter;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UTextBlock* _textLevel;

public:
	void SetData(ECollectionMaterialActionType InType, Level InLevel, bool InDrawDelimiter = true);
};
