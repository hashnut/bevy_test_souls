// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GsUIGuildOperationCamp.generated.h"

class UTextBlock;
class UImage;
class UGsButton;
class UPanelWidget;
class UGsUITooltipDesc;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaGuildBuilding;

/**
 * 길드 수호탑탭 페이지 내용
 */
UCLASS()
class T1PROJECT_API UGsUIGuildOperationCamp : public UGsUIGuildOperationBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	// 버프 효과 리스트용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	// 0: 캠프없음 1: 캠프있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCamp;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbCampName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipCamp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgCamp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemoveCamp;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTipConstructionBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipConstructionBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelConstructionBuffList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTipAreaBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipAreaBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAreaBuffList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMapName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnWarp;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperConstructionBuff;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperAreaBuff;

private:
	int32 _mapId = 0;
	CampId _campId = INVALID_CAMP_ID;
	float _remainCheckTime = 0.f;
	time_t _endTimeSec = 0;

	TArray<TPair<FText, FText>> _constructionBuffTextList;
	TArray<TPair<FText, FText>> _areaBuffTextList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// IGsInvalidateUIInterface override;
protected:
	virtual void InvalidateAllInternal() override;

	// UGsUIGuildOperationBase override
public:
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;

	virtual void OnChangeGuildLevel() override;
	virtual void OnChangeMyAuthority() override;

public:
	void OnCampDestruction();
	
	void SetCampImage(class UTexture2D* InTexture);

protected:
	void UpdateRemainTime();
	void UpdateRemoveCampButton();

protected:
	UFUNCTION()
	void OnClickTooltipCamp();
	UFUNCTION()
	void OnClickRemoveCamp();
	UFUNCTION()
	void OnClickTipConstructionBuff();
	UFUNCTION()
	void OnClickAreaBuff();
	UFUNCTION()
	void OnClickWarp();

	UFUNCTION()
	void OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshAreaBuff(int32 InIndex, UWidget* InEntry);
};
