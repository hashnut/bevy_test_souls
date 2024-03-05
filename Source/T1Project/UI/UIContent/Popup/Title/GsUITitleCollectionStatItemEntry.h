#pragma once


/**
* file		GsUITitleCollectionStatItemEntry.h
* @brief	Stat Info In Title Collection List
* @author	PKT
* @date		2022/01/20
**/

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUITitleCollectionStatItemEntry.generated.h"

class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUITitleCollectionStatItemEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _textStatName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _textStatValue;

public:
	void SetData(const FText& InStatNmae, const FText& InStatValue);
};