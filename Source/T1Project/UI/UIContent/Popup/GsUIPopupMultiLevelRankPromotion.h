// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupMultiLevelRankPromotion.generated.h"


class UGsUIMultiLevelRankIcon;
class UTextBlock;
class UGsButton;
/**
 * 승급 시스템 승급 완료 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMultiLevelRankPromotion : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankIcon* _iconRank;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnClickClose();

public:
	void SetData(const FSoftObjectPath& InIconPathMain, const FSoftObjectPath& InIconPathSub, int32 InCount, const FText& InPromotionName);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
