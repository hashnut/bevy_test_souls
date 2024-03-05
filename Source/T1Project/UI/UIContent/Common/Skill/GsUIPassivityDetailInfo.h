// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPassivityDetailInfo.generated.h"

class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaPassivitySet;
struct FGsSchemaEffectTextPassivity;
struct FGsPassivityData;
class UGsUISkillDetailInfoEntry;
class UGsUISkillDetailInfoRichEntry;

/**
 * 패시비티 상세정보를 보여주는 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPassivityDetailInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoDesc;
	// 정보 리스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoRichEntry* _infoSupply;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	const FGsSchemaPassivitySet* _passivityData = nullptr;
	const FGsSchemaEffectTextPassivity* _effectTextData = nullptr;
	bool _bIsCollection = false;
	FString _strCollectionEffect;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:	
	void SetPassivityData(PassivityId InPassivityId);
	void SetPassivityData(const FGsSchemaPassivitySet* InPassivity);
	void SetPassivityData(const FGsSchemaPassivitySet* InPassivity, const FText& InSkillSupply);
	void SetPassivityData(const FGsPassivityData* InData);

private:
	void SetDescInfo(const FText& InText);
	void SetSkillSupplyInfo(const FText& InText);

private:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
};
