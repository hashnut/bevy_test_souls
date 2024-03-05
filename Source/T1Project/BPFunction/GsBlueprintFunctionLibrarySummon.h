#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsBlueprintFunctionLibrarySummon.generated.h"

class AGsActorSummonFairy;
UCLASS()
class T1PROJECT_API UGsBlueprintFunctionLibrarySummon : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 선택된 뽑기용 페어리
	UFUNCTION(BlueprintPure, Category = "Summon")
	static AGsActorSummonFairy* GetSelectSummonFairy();

	// sequence fairy by index
	UFUNCTION(BlueprintPure, Category = "Summon")
	static AGsActorSummonFairy* GetSequenceSummonFairyByIndex(int In_index);
	// sequence fairy high get
	UFUNCTION(BlueprintPure, Category = "Summon")
	static AGsActorSummonFairy* GetHighGetSummonFairy();
	// 선택된 뽑기용 페어리 등급
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetSelectSummonFairyGradeInt();
	// 선택된 뽑기용 코스튬 등급
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetSelectSummonCostumeGradeInt();
	// 선택된 뽑기용 코스튬 id
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetSelectSummonCostumeId();
	// sequence costume high get
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetHighGetSummonCostumeId();
	// 뽑기 연출 기본 코스튬 id
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetBaseSummonCostumeId();

	// sequence costume id by index
	UFUNCTION(BlueprintPure, Category = "Summon")
	static int GetSequenceSummonCostumeIdByIndex(int In_index);
	UFUNCTION(BlueprintPure, Category = "Summon")
	static bool GetSequenceSummonCostumeHighValByIndex(int In_index);

	UFUNCTION(BlueprintPure, Category = "Summon")
	static bool GetSequenceSummonFairyHighValByIndex(int In_index);

	UFUNCTION(BlueprintPure, Category = "Summon")
	static bool GetSummonFairysHighCardExist();
	UFUNCTION(BlueprintPure, Category = "Summon")
	static bool GetSummonCostumesHighCardExist();
};