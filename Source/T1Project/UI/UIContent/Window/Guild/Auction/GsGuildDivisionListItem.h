// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsGuildDivisionListItem.generated.h"


/**
 * 기사단 분배 목록 리스트뷰의 데이터
 */
UCLASS()
class T1PROJECT_API UGsGuildDivisionListItem : public UObject
{
	GENERATED_BODY()

public:
	ItemDBId _divisionItemDBId = INVALID_ITEM_DB_ID;

public:
	void Clear();
	void SetData(ItemDBId InItemDBId);
};
