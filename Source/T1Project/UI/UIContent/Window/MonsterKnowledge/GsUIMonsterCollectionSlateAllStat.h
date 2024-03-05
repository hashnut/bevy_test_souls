#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIMonsterCollectionSlateAllStat.generated.h"

class UGsUIVFX;
class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateAllStat : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textExpect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTotal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxEffect;
protected:
	virtual void NativeConstruct() override;

public:
	void SetData(const FText& InName, const FText& InExpect, const FText& InTotal);
	void PlayEffect();
};
