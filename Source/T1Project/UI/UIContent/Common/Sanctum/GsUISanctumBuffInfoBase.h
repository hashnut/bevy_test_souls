// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISanctumBuffInfoBase.generated.h"

class UPanelWidget;
class UTextBlock;
class UGsButton;
class UGsUITooltipDesc;
class UGsDynamicPanelSlotHelper;
class UGsUIGuildIcon;

/**
 * 성소 버프 효과 정보 표시
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUISanctumBuffInfoBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 버프 효과 리스트용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSanctumName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTipSanctumBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipSanctumBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSanctumBuffList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTipNexusBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipNexusBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNexusBuffList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyNexusBuff;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSanctumBuff;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperNexusBuff;

protected:
	TArray<TPair<FText, FText>> _sanctumBuffTextList;
	TArray<TPair<FText, FText>> _nexusBuffTextList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	// bIsShowGuildInfo: 상단 기사단 정보를 보여줄 것인가? 지역맵에서는 보여주고, 기사단 페이지에서는 안보여준다.
	virtual void SetData(SanctumAreaId InSanctumId);

public:
	UFUNCTION()
	void OnPressTipSanctumBuff();
	UFUNCTION()
	void OnPressTipNexusBuff();

	UFUNCTION()
	void OnRefreshSanctumBuff(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshNexusBuff(int32 InIndex, UWidget* InEntry);

protected:
	void SetNexusBuff(PassivityId InPassivityId);
	void GetEffectTextList(PassivityId InPassivityId, OUT TArray<TPair<FText, FText>>& OutList);	
};
