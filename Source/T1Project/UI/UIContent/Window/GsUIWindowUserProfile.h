// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIWindowUserProfile.generated.h"


class UTextBlock;
class UImage;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsDynamicPanelSlotHelper;
class UGsUIUserProfileStatEntry;
class UScrollBox;
class UGsUIIconItem;
class UGsUIPopupKeywordVote;
class UGsUIPopupKeywordVoteMy;
class UGsUIPopupCommentWrite;
class UGsUIPopupUserProfileSetting;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
/**
 *  유저 프로필 Window UI
 */
UCLASS()
class T1PROJECT_API UGsUIWindowUserProfile : public UGsUIWindow
{
	GENERATED_BODY()
protected:
	const uint64 MOST_KEYWORD_COUNT_MAX = 999;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _panelBackground;

	// 존재 유무 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelExistCharacter;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNoExistCharacter;

	// 캐릭터 정보 공개 상태
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPublic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPrivate;

	// 캐릭터 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCharacterName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCharacterLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherLevel; // 레벨 공개(0), 비공개(1)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageBadge;

	// 소속 기사단
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageGuildEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildGrade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGuild; // 길드표시(0), 없음표시(1), 비공개표시(2)

	// 나의 키워드
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageKeyword;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockKeywordName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockKeywordCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherKeyword; // 대표 키워드 있음(0), 대표 키워드 없음(1)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnKeywordVote;

	// 한마디 / 닉네임 변경이력 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnComment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnNickNameHistory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMenu;
	
	// 한마디
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCommentOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNoComment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxComment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCommentWrite;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfComment;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperComment;

	// 닉네임 변경 이력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryClassNickNameHistory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxNickNameHistory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyNickNameHistory;

	// 자기소개 문구
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIntroduction;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockIntroduction;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIntroduction;

	// 정보공개 옵션 On/Off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnVisible;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherVisible;

	// 장비 슬롯
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnEquipFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnEquipSecond;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootRight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootBMLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootBMRight;

	// 스탯 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelStat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry01;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry02;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry03;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry04;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry05;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry06;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserProfileStatEntry* _statEntry07;

	// 유저 컨텍스트 UI
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animMoveToFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animMoveToSecond;
	
protected:
	// 스탯
	TArray<UGsUIUserProfileStatEntry*> _statEntryList;

	// 장비
	TArray<EGsEquipSlotType> _equipSlotList;

	UPROPERTY()
	TArray<UGsDynamicIconSlotHelper*> _iconSlotHelperList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperNickNameHistory;

protected:
	TWeakObjectPtr<UGsUIPopupKeywordVote> _popupKeywordVoteUI;
	TWeakObjectPtr<UGsUIPopupKeywordVoteMy> _popupKeywordVoteMyUI;
	TWeakObjectPtr<UGsUIPopupCommentWrite> _popupCommentWriteUI;
	TWeakObjectPtr<UGsUIPopupUserProfileSetting> _popupIntroductionUI;

	FGsToggleGroup _toggleGroupMenu;

protected:
	bool _isStatOpened = false;
	bool _reqOptionSave = false;
	EGsEquipSlotType _refreshSlotType = EGsEquipSlotType::SLOT_MAX;

	TArray<TPair<FString, time_t>> _nickNameHistoryList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void CloseInternal() override;
	virtual bool SyncContentState() override;

	void FindIconSelecter(UPanelWidget* InPanel, OUT TArray<UGsDynamicIconSlotHelper*>& OutIconSlotHelperList);

protected:
	virtual void InvalidateAllInternal() override;

public:
	void InvalidateIntroduction();
	void InvalidateEquipItem();
	void InvalidateStat();
	void InvalidateStatTooltip();
	
	void InvalidateCharacterInfo();
	void InvalidateGuildInfo();
	void InvalidateKeyword();
	void InvalidateComment();
	void InvalidateNickNameHistory();

	void SetTargetUserExist(bool InIsExist);

public:
	void OpenKeywordVote();
	void CloseStatTooltip();

protected:
	void ClosePopupAll();

public:
	virtual void OnClickBack() override;
	virtual bool OnBack() override;

	UFUNCTION()
	void OnClickKeywordVote();
	UFUNCTION()
	void OnClickCommentWrite();
	UFUNCTION()
	void OnClickStat();
	UFUNCTION()
	void OnClickIntroduction();
	UFUNCTION()
	void OnClickVisible();

	void OnClickEquipItemSlot(UGsUIIconItem& InIcon);
	void OnLongPressEquipItemSlot(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnRefreshIconEquipItem(int32 InIndex, UGsUIIconBase* InIcon);

	// 0: 코멘트, 1: 닉네임 변경이력
	void OnSelectMenu(int32 InIndex);
	
	UFUNCTION()
	void OnRefreshCommentSlot(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnUserScrolled(float InScrollOffset);

	UFUNCTION()
	void OnRefreshNickNameHistory(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickUserInfo();

	void OnOptionSaveCompleted();	

protected:
	UFUNCTION()
	void OnClickPageFirst();
	UFUNCTION()
	void OnClickPageSecond();
};
