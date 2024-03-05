// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project/GameObject/Define/GsGameObjectDefine.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsItemTableStruct.generated.h"

/**
 * Item 관련 DataCenter 에서 변수로 사용될 단순 구조체 모음
 * 별도의 테이블로 만들지 않아야됨
 */
UCLASS()
class T1PROJECT_API UGsItemTableStruct : public UObject
{
	GENERATED_BODY()
	
};

//////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FGsItemRestoreStatData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	DamageStatType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 value;
};
//////////////////////////////////////////////////////////////////////////////////////