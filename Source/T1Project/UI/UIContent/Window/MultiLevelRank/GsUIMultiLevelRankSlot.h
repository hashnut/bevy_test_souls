// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMultiLevelRankSlot.generated.h"


class UGsButton;
class UImage;
class UWidgetSwitcher;
class UGsUIMultiLevelRankIcon;
class UTextBlock;
class UPanelWidget;
class UProgressBar;
class USizeBox;
class UGsUIMultiLevelRankGradeLoopEffect;
struct FGsMultiLevelRankSlotData;
struct FGsMultiLevelRankData;
/**
 * 승급 시스템
 * Rank Slot / ProgressBar
 */
UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankSlot : public UUserWidget, 
	public IGsEntryInterface, 
	public IGsTutorialInteractionInterface
{
	GENERATED_BODY()
	
protected:
	DECLARE_DELEGATE_OneParam(FOnClickRankSlot, class UUserWidget*);

	enum class FunctionButtonState
	{
		QuestAccept,
		QuestInProgress,
		RankUp,
		Completed,
		Pledge,
		PledgeCompleted,
		LevelLimit,
		StepLimit,
	};

	enum SectionPointIndex
	{
		Front = 0
		, Back
		, Max
	};

	struct SectionPoint
	{
		uint64 _beginePoint = INDEX_NONE;
		uint64 _endPoint = INDEX_NONE;
		float _maxRateValue = 0.f;
	};

	enum SlotStateIndex
	{
		None = 0, // 임무 수락, 승급하기
		Pledge,
		Complete,	// 서약 완료, 임무 완료
		QuestInProgress,
		LevelLimit,
		StepLimit,
	};

protected:
	// 슬롯 선택 여부
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageHighlight;

	// 슬롯 배경 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageField;

	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	FSoftObjectPath _pledgeImagePath;

	// 아이콘 및 퀘스트 아이콘 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankIcon* _iconRank;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelQuest;

	// 슬롯 명칭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	// 보상 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankGradeLoopEffect* _effectRewardGrade;

	// 하단 버튼 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSlotState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLevelLimit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStepLimit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFunction;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevelLimit;

	// 경험치 상태 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarExp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageExpPointOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageExpPointOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageExpPointOnPledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageExpPointOffPledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizeBoxExp;


	/************************************************************************/
	/* 멤버 변수                                                            */
	/************************************************************************/
protected:
	const FGsMultiLevelRankSlotData* _slotData = nullptr;
	const FGsMultiLevelRankData* _rankData = nullptr;

	bool _isNormalSlot = false;
	bool _isSelected = false;

	FunctionButtonState _functionButtonState;

	SectionPoint _point[SectionPointIndex::Max];

	/************************************************************************/
	/* Event Delegate                                                       */
	/************************************************************************/
public:
	FOnClickRankSlot _onClickRankSlot;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

	/************************************************************************/
	/* Event Func                                                           */
	/************************************************************************/
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickFunction();

	void OnClickedQuestAccept();
	void OnClickedPromotion();
	void OnClickedPledgeSelect();
	void OnClickedLevelLimit();
	void OnClickedStepLimit();

	/************************************************************************/
	/* Init                                                                 */
	/************************************************************************/
public:	
	void OnInitialized(const FGsMultiLevelRankData* InRank);
	void InitializePledgeSlot(const FGsMultiLevelRankData* InRank);
	void InitializeSlot(const FGsMultiLevelRankSlotData* InSlot);
	void InitializeProgressBar();

	/************************************************************************/
	/* Invaildate                                                           */
	/************************************************************************/
	void InvalidateSlotAll();
	void InvalidateProgressBar();
	void InvalidateReward();
	void InvalidateButton();
	void InvalidateQuest();

	/************************************************************************/
	/* Get / Set	                                                        */
	/************************************************************************/
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() { return _isSelected; }

	const FGsMultiLevelRankSlotData* GetData() const { return _slotData; };

	bool IsNormalSlot();

protected:
	void AsyncLoadSprite(UImage* InImage, const FSoftObjectPath& InImagePath);
	void SetButtonText(FTextKey InTextKey);

	/************************************************************************/
	/* Tutorial		                                                        */
	/************************************************************************/
protected:
	bool _bIsTutorialInteraction = false;
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) override { _bIsTutorialInteraction = bIsTutorial; }
	virtual bool IsTutorialInteraction() const override { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() override { return OnTutorialInteraction; }
};
