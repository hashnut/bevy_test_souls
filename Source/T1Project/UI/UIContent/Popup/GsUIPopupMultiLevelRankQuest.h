// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageContentHud.h"

#include "MultiLevelRank/GsMultiLevelRankDefine.h"

#include "GsUIPopupMultiLevelRankQuest.generated.h"


class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UTextBlock;
class UGsButton;
class UImage;
class UWidgetSwitcher;
struct FGsQuestData;
struct FGsMultiLevelRankSlotData;
/**
 * 승급 시스템 퀘스트 목록 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMultiLevelRankQuest : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	/**
	 * Quest Refresh에 필요한 Data 구조체
	 * Quest자체가 Quest가 복수의 Objective를 관리 하고 있는 구조이기 때문에 Index형식으로 접근하기 어렵다.
	 * 여기에서는 배열 형태로 풀어서 Refresh 용도로 사용하기 위해 구조체 선언.
	 */
	struct FGsRefreshQuestBuffer
	{
		TSharedPtr<FGsQuestData> questData = nullptr;
		int32 beginePoint = 0;	// Entry Refresh 할때 InIndex에 순차적으로 접근 하기 위해 BegineIndex 선언
		const FSoftObjectPath* iconPath = nullptr;
	};

protected:
	// 좌측 퀘스트 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfQuestEntry;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperQuest;

	// 우측 보상 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherReward;

	// 우측 보상 영역 - 코스튬, 페어리
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRewardDesc;

	// 우측 보상 영역 - 스탯
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollRewardStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfRewardStat;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperRewardStat;

	// 우측 보상 영역 - 아이템
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollRewardItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfRewardItem;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperRewardItem;

	// 하단 임무 상태
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockState;

	// 기타
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRight;

protected:
	MsgHudHandleArray _hudHandlerList;

	TArray<FGsRefreshQuestBuffer> _refreshQuestBuffer;
	TArray<TPair<StatType, int32>> _refreshStatBuffer;
	TArray<FGsRefreshRewardBuffer> _refreshItemBuffer;

	const FGsMultiLevelRankSlotData* _focusRankSlotData;
	const FGsMultiLevelRankSlotData* _leftRankSlotData;
	const FGsMultiLevelRankSlotData* _rightRankSlotData;

	int32 _pledgeGroupId = 0;
	int32 _rankGroupId = 0;

	bool _isFromSlot = false;
	bool _isInitialize = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnClickLeft();
	UFUNCTION()
	void OnClickRight();

	UFUNCTION()
	void OnRefreshQuestEntryItem(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshRewardEntryStat(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshRewardEntryItem(int32 InIndex, UWidget* InEntry);

public:
	void InvalidateQuest(const FGsMultiLevelRankSlotData* InFocusData, int32 InPledgeGroupId, int32 InRankGroupId, bool InIsFromSlot = true);

protected:
	void InvalidateQuestInternal();
	void InvalidateRewardInternal();
	void InvalidateButtonInternal();

protected:
	void AsyncLoadSprite(UImage* InImage, const FSoftObjectPath& InImagePath);
	
	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
