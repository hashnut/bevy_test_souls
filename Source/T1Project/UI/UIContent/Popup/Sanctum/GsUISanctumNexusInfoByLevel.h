// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISanctumNexusInfoByLevel.generated.h"

class UTextBlock;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaCampData;
struct FGsSchemaSanctumNexusData;

/**
 * 성소 수호탑 버프 상세 정보
 */
UCLASS()
class T1PROJECT_API UGsUISanctumNexusInfoByLevel : public UUserWidget,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 버프 효과 리스트용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelConstructionBuffList;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperConstructionBuff;

private:
	TArray<TPair<FText, FText>> _constructionBuffTextList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const FGsSchemaSanctumNexusData* InData, int32 InLevel);
	
protected:
	UFUNCTION()
	void OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry);
};
