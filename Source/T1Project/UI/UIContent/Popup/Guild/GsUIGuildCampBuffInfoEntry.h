// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildCampBuffInfoEntry.generated.h"

class UTextBlock;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaCampData;

/**
 * 길드 캠프 버프 상세정보 툴팁 리스트 엔트리
 */
UCLASS()
class T1PROJECT_API UGsUIGuildCampBuffInfoEntry : public UUserWidget,
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
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAreaBuffList;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperConstructionBuff;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperAreaBuff;

private:
	TArray<TPair<FText, FText>> _constructionBuffTextList;
	TArray<TPair<FText, FText>> _areaBuffTextList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const FGsSchemaCampData* InData, int32 InLevel);
	
protected:
	UFUNCTION()
	void OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshAreaBuff(int32 InIndex, UWidget* InEntry);
};
