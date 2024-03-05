// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUITooltipWorldMarketServerList.generated.h"

class UUniformGridPanel;
struct FGsSchemaWorldMarketServerMatchingData;

/**
 * 월드 거래소 서버 리스트 
 */
UCLASS()
class UGsUITooltipWorldMarketServerList : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUniformGridPanel* _panelRootList;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	const FGsSchemaWorldMarketServerMatchingData* _table = nullptr;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void Open() override;
	virtual void Close() override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

protected:
	const FGsSchemaWorldMarketServerMatchingData* FindMyMatchingData() const;
};
