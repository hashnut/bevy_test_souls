#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsGameOptions.generated.h"

UCLASS(Config=GameOptions)
class T1PROJECT_API UGsGameOptions : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Config)
	ItemGrade SystemMessageMax;

	UPROPERTY(Config)
	TArray<ItemGrade> SystmeMessageOpt;

protected:
	static TWeakObjectPtr<UGsGameOptions> GameOptions;

	UFUNCTION(BlueprintCallable, Category=Settings)
	static void CreateGameOptions();

public:
	UFUNCTION(BlueprintCallable, Category=Settings)
	static UGsGameOptions* GetGameUserOptions();

	UFUNCTION(BlueprintCallable, Category = Settings)
	static void Reset();

	UFUNCTION(BlueprintCallable)
	void SaveOptionConfig();

public:
	UFUNCTION(BlueprintCallable)
	bool GetSystemMessageGradeOption(int32 numVal);

	UFUNCTION(BlueprintCallable)
	void SetSystemMessageGradeOption(int32 numVal, bool use);
};

