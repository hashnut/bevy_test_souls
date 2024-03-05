#pragma once


/**
* file		GsUITitleStatItemEntry.h
* @brief	Stat Info In Title Vew List
* @author	PKT
* @date		2022/01/20
**/

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUITitleStatItemEntry.generated.h"

class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUITitleStatItemEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statValue;

public:
	void SetData(const FText& InStatNmae, const FText& InStatValue);	
};