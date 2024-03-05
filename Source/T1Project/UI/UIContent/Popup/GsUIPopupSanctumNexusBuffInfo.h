// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupSanctumNexusBuffInfo.generated.h"

class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUISanctumNexusInfoByLevel;
struct FGsSchemaSanctumNexusData;

/**
 * 성소 수호탑 버프 상세정보 툴팁
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSanctumNexusBuffInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 추후에 뭔가 집어넣을 것 같아서 탭 상태 유지함
	enum EGsSanctumTopTabType : uint8
	{
		NEXUS = 0,
	};

protected:
	// 버프 효과 리스트용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _levelEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootTopTab;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootLevelTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISanctumNexusInfoByLevel* _nexusInfoUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootEmpty;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEmpty;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperLevel; // 레벨에 따라 동적 생성

protected:
	FGsToggleGroup _toggleGroupTop;
	FGsToggleGroup _toggleGroupLevel;

	uint16 _nexusLevel = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

public:
	void SetData(uint16 InNexusLevel, int32 InTabIndex = 0);

protected:
	void InvalidateLevel(uint16 InCampLevel);
	
protected:
	void OnSelectTopTab(int32 InIndex);
	void OnSelectLevelTab(int32 InIndex);

	UFUNCTION()
	void OnRefreshLevel(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();

protected:
	const FGsSchemaSanctumNexusData* GetNexusData() const;
};
