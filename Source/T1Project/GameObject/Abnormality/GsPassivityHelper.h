// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Classes/Collection/GsCollectionData.h"
#include "DataSchema/Collection/GsSchemaCollectionData.h"

/*
 * 패시비티 관련 함수 모음
 */
struct T1PROJECT_API FGsPassivityHelper
{
public:
	static const IGsCollectionData* GetCollectionData(PassivityOwnerType InType);
	static const FGsSchemaCollectionData* GetCollectionTableData(PassivityOwnerType InType);

	// 패시비티에 의해 발동한 AbnormalitySet을 가져온다.
	static const struct FGsSchemaAbnormalitySet* GetInvokeAbnormality(const struct FGsSchemaPassivitySet* InPassivitySet);
	static const struct FGsSchemaAbnormalitySet* GetInvokeAbnormality(PassivityId InId);
	
	static const int32 GetArcasPoint();
	static const int32 GetArcasMaxArcasPoint();
	static FSoftObjectPath GetArcasIconPath();
	static bool GetIsCharged(int32 In_DetailID);
};
