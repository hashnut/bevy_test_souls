#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIMonsterCollectionSlateStatEntry.generated.h"

class UGsUIVFX;
class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateStatEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textIncreaseValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTotalValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxEffect;
protected:
	virtual void NativeConstruct() override;

public:
	void SetData(const FText& InName, const FText& InIncreaseValue, const FText& InTotalValue);
	void PlayEffect();
};
