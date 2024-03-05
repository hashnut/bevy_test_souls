// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EffectText/GsEffectTextDefine.h"

class IGsCollectionData;
class UGsTablePassivitySet;
struct FGsSchemaPassivitySet;

/*
 * 이펙트 설명글 표시 위함
 */
struct T1PROJECT_API FGsEffectStringHelper
{
public:
	struct FGsEffectAddData
	{
		TMap<StatType, int32> _statMap;
	};	

public:
	// 콜렉션 타입 아니면 false 리턴됨	
	static bool GetCollectionEffectString(PassivityOwnerType InType, OUT FString& OutString);
	static bool GetCollectionEffectStringList(PassivityOwnerType InType, OUT TArray<TPair<FText, FText>>& OutList);

	static bool GetCollectionEffectString(const IGsCollectionData* InData, OUT FString& OutString);
	static bool GetCollectionEffectStringList(const IGsCollectionData* InData, OUT TArray<TPair<FText, FText>>& OutList);

	// 한 개만 가져올 때
	static bool GetPassivityEffectString(PassivityId InId, OUT FString& OutString, int32 InOption = 0);
	static bool GetPassivityEffectString(const FGsSchemaPassivitySet* InData, OUT FString& OutString, int32 InOption = 0);		
	static bool GetPassivityEffectString(const FGsSchemaPassivitySet* InData, OUT TArray<TPair<FText, FText>>& OutList, int32 InOption = 0);

public:
	// 주의: EffectTextHelper 사용안하는거
	static bool AddPassivityEffect(const FGsSchemaPassivitySet* InData, OUT FGsEffectAddData& OutAddData);

	static bool AddCollectionEffect(const IGsCollectionData* InData, OUT FGsEffectTextCollection& OutAddData);

private:
	//static bool AddCollectionEffectInter(const IGsCollectionData* InData, OUT FGsEffectAddData& OutAddData);
	
	// 정보 출력
	static void SetEffectString(const FGsEffectTextCollection& InData, OUT FString& OutString);
	static void SetEffectString(const FGsEffectTextCollection& InData, OUT TArray<TPair<FText, FText>>& OutList);
};
