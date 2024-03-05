// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/MonsterKnowledge/GSUIMonsterCollectionBasePage.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIMonsterCollectionCodexPage.generated.h"

class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UGsButton;
class UListView;
class UHorizontalBox;
class UGsToggleButton;
class UWidgetSwitcher;
class UTextBlock;
class UTileView;
class UGsPoolUObject;
class UGsUIRedDotDefault;
class UGsUITooltipDesc;
class UImage;

/**
 * 사운드 연출 리소스 이름 (DTGsSoundResData 로부터 조회)
 */
const FString PAGE_SOUND_CLICK = TEXT("Snd_UI_Click"); // (버튼 클릭)
const FString PAGE_SOUND_CONVERT = TEXT("Snd_UI_Confirm"); // (도감 포인트 변환)
const FString PAGE_SOUND_LEVEL_UP = TEXT("UI_MB_LevelUp"); // (탐사 레벨업)

// 오른쪽 영역 UI 전환
enum class EGsRightArea
{
	KNOWLEDGE_GAIN,
	CATEGORY_EXPLORE,
	MONSTER_MAPGROUP,
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionCodexPage : public UGSUIMonsterCollectionBasePage, 
	public IGsTutorialInteractionInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageCategoryBG;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryView;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryView;

	/** 카테고리 토글 그룹 */
	FGsToggleGroup _toggleGroupCategoryView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewMapGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _knowledgeExploreTabArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleBtnKnowledgeGain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherKnowledgeGain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleBtnExploreCategory;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherExploreCategory;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherRightArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipTotalKnowledge;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipCodexPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTotalKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCurrentTotalKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConvertibleTotalBookPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConvertKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConvertPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipExploreLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSlateTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewExploreLevel;
	
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UWidgetSwitcher* _widgetSwitcherExploreCategoryBtn;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsButton* _btnTotalExploreStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnExploreLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipMapName;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMapGroupName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMapGroupLevel;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCurrenMapGroupKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRequireKnowledgeForNextLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipMonsterList;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTileView* _tileViewBossMonster;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTileView* _tileViewEliteMonster;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTileView* _tileViewNormalMonster;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTileView* _tileViewMapGroupMonster;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUseKnowledgeGem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCloseMapGroupMonster;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotKnowledgeGainTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotCategoryExploreTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotKnowledgeConvert;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotCategoryLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUITooltipDesc* _tooltipTotalKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUITooltipDesc* _tooltipCodexPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUITooltipDesc* _tooltipExploreLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUITooltipDesc* _tooltipMapName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUITooltipDesc* _tooltipMonsterList;

private:
	UPROPERTY()
	UGsPoolUObject* _poolPortraitData;

	UPROPERTY()
	UGsPoolUObject* _poolMapGroupItem;

private:
	/** Lv.{0} 캐싱 */
	FText _levelText;

	/** 총 획득 지식 툴팁 텍스트 */
	FText _totalKnowledgeTooltipText;

	/** 도감 포인트 툴팁 텍스트 */
	FText _codexPointTooltipText;

	/** 대륙 탐사 툴팁 텍스트 */
	FText _exploreLevelTooltipText;

	/** 지역 이름 툴팁 텍스트 */
	FText _mapNameTooltipText;

	/** 몬스터 목록 툴팁 텍스트 */
	FText _monsterListTooltipText;


	/** 현재 카테고리 인덱스 */
	int32 _currentTabIndex = 0;

	/** 현재 카테고리 Id */
	CodexCategoryId _currentCategoryId = INVALID_CODEX_CATEGORY_ID;

	/** 현재 맵그룹 Id */
	CodexMapGroupId _currentMapGroupId = INVALID_CODEX_MAP_GROUP_ID;

	/** 월드맵으로 이동하여 되돌아올 때, 기억할 맵 그룹 리스트에서 선택된 항목의 Index */
	int32 _mapGroupIndex = -1;

	/** 다른 윈도우로 이동하여 되돌아올 때, 기억할 몬스터 타일뷰에서 선택해줄 항목의 Index */
	bool _isUseKnowledgeGemPopup = false;

	/** 월드맵으로 이동하여 되돌아올 때, 기억할 몬스터 타일뷰에서 선택해줄 항목의 Index */
	int32 _monsterTileIndex = -1;


public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	virtual void Enter(struct FGsMonsterKnowledgeUICaptureData* InData) override;
	virtual void Leave() override;
	virtual void Capture(struct FGsMonsterKnowledgeUICaptureData* OutData) override;

	/** 지식 획득 UI 갱신 */
	virtual void InvalidateKnowledgeGain() override;

	/** 대륙 탐사 UI 갱신 */
	virtual void InvalidateCategoryExplore() override;

	virtual void InvalidateRedDot() override;

	/** 맵 그룹 리스트 갱신 */
	virtual void InvalidateMapGroupList(EGsMapGroupRefreshType InScrollType = EGsMapGroupRefreshType::NO_SCROLL) override;

	/** 특정 맵그룹 지정시, 맵 그룹 텍스트 갱신 */
	virtual void InvalidateMapGroupText() override;

private:

	/** 오른쪽 영역 UI 전환 */
	void InvalidateRightArea(EGsRightArea InRightArea = EGsRightArea::KNOWLEDGE_GAIN);

	/** 특정 맵그룹 지정시, 몬스터 목록 UI 갱신 */
	void InvalidateMonsterMapGroup();

	/** 지식 획득 레드닷 */
	void InvalidateRedDotKnowledgeGain();

	/** 대륙탐사 탭 레드닷 */
	void InvalidateRedDotCategory();

	
	/** 몬스터 목록 타일뷰 UI 갱신 */
	void InvalidateMonsterTileView();

	/** 맵 그룹 리스트의 선택 사항을 전부 해제해 주기 */
	void ClearMapGroupListSelection();

	/** 맵 그룹 클릭 시, Toggle 을 위한 로직 */
	void ToggleMapGroupClick(UObject* InObj, bool InSelected);

	/** 카테고리 전환 시, 배경 이미지 씌우기 */
	void InvalidateCategoryBG();

protected:
	UFUNCTION()
	void OnRefreshEntryCategoryView(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectChangedCategoryView(int32 InIndex);

	UFUNCTION()
	void OnClickedMapGroupListItem(UObject* InListItem);

	UFUNCTION()
	void OnScrolledMapGroupList(float InOffsetInItems, float InDistanceRemaining);

	UFUNCTION()
	void OnClickedConvertCodexPoint();

	UFUNCTION()
	void OnClickedCategoryLevelUp();

	UFUNCTION()
	void OnClickedUseKnowledgeGem();

	UFUNCTION()
	void OnClickedKnowledgeGainTab();

	UFUNCTION()
	void OnClickedCategoryExploreTab();
	 
	UFUNCTION()
	void OnClickedMonsterTileViewItem(UObject* InItem);

	UFUNCTION()
	void OnCloseGemUsePopup();

	UFUNCTION()
	void OnClickedCloseMapGroupMonster();

	UFUNCTION()
	void OnPressedBtnTooltipTotalKnowledge();

	UFUNCTION()
	void OnPressedBtnTooltipCodexPoint();

	UFUNCTION()
	void OnPressedBtnTooltipExploreLevel();

	UFUNCTION()
	void OnPressedBtnTooltipMapName();

	UFUNCTION()
	void OnPressedBtnTooltipMonsterList();

	//------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	bool _bIsTutorialInteraction = false;

	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) override;
	virtual bool IsTutorialInteraction() const override { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() override { return OnTutorialInteraction; }	

public:
	UWidget* SetTutorial(bool bIsTutorial);
};
