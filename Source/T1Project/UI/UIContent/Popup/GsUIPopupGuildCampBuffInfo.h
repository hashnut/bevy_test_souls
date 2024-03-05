// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupGuildCampBuffInfo.generated.h"

class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUIGuildCampBuffInfoEntry;
struct FGsSchemaCampData;

/**
 * 길드 캠프 버프 상세정보 툴팁
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildCampBuffInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	enum EGsGuildCampType : int32
	{
		CAMP_PVE = 0,
		CAMP_PVB = 1,
		CAMP_PVP = 2,
		
		MAX_CAMP_TYPE
	};

protected:
	// 버프 효과 리스트용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _levelEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootCampTypeTab;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootLevelTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildCampBuffInfoEntry* _uiCampInfo;
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
	//0: 사냥, 1: 필드보스, 2: PVP
	FGsToggleGroup _toggleGroupCampType;
	FGsToggleGroup _toggleGroupLevel;

private:
	TMap<int32, const FGsSchemaCampData*> _campTypeMap; // key: EGsGuildCampType
	TMap<int32, int32> _constructableLevelMap; // key: EGsGuildCampType
	uint16 _campLevel = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

public:
	void SetData(uint16 InCampLevel, int32 InTabIndex = 0);

protected:
	void InvalidateLevel(uint16 InCampLevel);
	void SetEmptyText(EGsGuildCampType InType);
	
protected:
	void OnSelectCampTypeTab(int32 InIndex);
	void OnSelectLevelTab(int32 InIndex);

	UFUNCTION()
	void OnRefreshLevel(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();

protected:
	const FGsSchemaCampData* GetSelectedCamp() const;
	int32 GetSelectedConstructableLevel() const;
};
