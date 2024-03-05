// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayTickerBMPurchase.generated.h"


class UTextBlock;
class UWidgetAnimation;
class FGsBMShopProductData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerBMPurchase : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _productNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _descText;
	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* _anim;

private:
	FWidgetAnimationDynamicEvent _finishDelegate;

	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	// System 메시지는 위에 출력될 수 있도록 기본 Depth를 높게 설정
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_TRAY; }

public:
	void SetData(TPair<FGsBMShopProductData*, ItemAmount> InPairData, bool InIsBulkSale, bool InIsCash);

public:
	UFUNCTION()
	void OnAnimationFinish();
};
