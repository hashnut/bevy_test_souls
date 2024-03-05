// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUITrayEnchantFairyBase.h"
#include "GsUITrayEnchantFairyResult.generated.h"

struct FGsFairyData;
class UGsUIFairyItem;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayEnchantFairyResult : public UGsUITrayEnchantFairyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyItem* _ItemFairyPortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _detail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _title;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _desc;

	float _lockMaxTime = 0.f;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnclickConfirm();

public:
	class EnchantResultEffectInitData : public EffectInitData
	{
	public:
		TWeakPtr<FGsFairyData> _data;
		FText _title;
		FText _desc;
		bool _useDetail{ true };
		EnchantResultEffectInitData(TWeakPtr<FGsFairyData> data, FText title, FText desc, bool useDetail= true) 
			: _data(data), _title(title), _desc(desc), _useDetail(useDetail) {}
	};

	void InitializeData(EffectInitData* initParam) override;
};
