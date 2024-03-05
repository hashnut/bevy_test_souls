// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUIPopupPKBookTryToActionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupPKBookTryToRevenge.generated.h"

class UImage;
class UGsButton;
class UTextBlock;
class UGsUICurrencySlot;

/**
 * 상태이상 상세 정보창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupPKBookTryToRevenge : public UGsUIPopupPKBookTryToActionBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;

protected:
	virtual void TryToAction(PKRecordId InRecordId) override;

public:
	void SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData);

};
