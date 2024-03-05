#pragma once


#include "CoreMinimal.h"
#include "../Reward/GsRewardHelper.h"
#include "../Dungeon/GsDungeonUICaptureData.h"
#include "../Dungeon/GsDungeonCommonData.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/Dungeon/GsUIPageDungeonGroupBase.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "CoopPartyDungeon/GsUIPartyDungeonPartyListViewEntry.h"
#include "../Classes/GsPoolUObject.h"
#include "GsUIPageDungeonGroupParty.generated.h"


UCLASS()
class T1PROJECT_API UGsUIPageDungeonGroupParty : public UGsUIPageDungeonGroupBase
{
	GENERATED_BODY()

protected:
	// 2022/06/01 PKT - 던전 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDungeonGroup;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLockDifficultyEasy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLockDifficultyNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLockDifficultyHard;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLockDifficultyNaghtmare;

	// 2022/05/31 PKT - 던전 타이틀
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDungeonName;
	// 2022/05/31 PKT - 던전 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDesc;
	// 2022/05/31 PKT - 보스 종족
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _raceTextBlock;
	// 2022/05/31 PKT - 보스 재질
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _materialTextBlock;
	// 2022/05/31 PKT - 보스 속성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _traitTextBlock;
	// 2022/05/31 PKT - 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTimeLimit;
	// 2022/05/31 PKT - 기본 보상 카운트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _defaultRewardCount;
	// 2022/05/31 PKT - 아이템 보상 카운트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _itemRewardCount;

	// 2022/05/31 PKT - 난이도 쉬움
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switcherButtonDifficultyEasy;
	// 2022/05/31 PKT - 난이도 보통
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switcherButtonDifficultyNormal;
	// 2022/05/31 PKT - 난이도 어려움
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switcherButtonDifficultyHard;
	// 2022/05/31 PKT - 난이도 악몽
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switcherButtonDifficultyManiac;

	// 2022/05/31 PKT - 새로 고침
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnRefresh;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swithcerMatchingBtn;
	// 2022/05/31 PKT - 자동 매칭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAutoMatching;
	// 2022/06/14 PKT - 매칭 취소
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAutoMatchingCancel;	
	
	// 2022/05/31 PKT - 파티 생성 / 탈퇴 Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherPartyButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPartyCreate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPartyQuit;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _basicRewardButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _mainRewardButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _defaultRewardListView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _mainRewardListView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listViewPartyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRedDotDefault* _rewardRedDot;
	
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherPartyList;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnToolTipPartyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipPartyListInfo;

protected:
	// 토글 그룹 Button
	FGsToggleGroup _toggleGroupButton;

	TArray<const FGsSchemaRewardItemBagData*> _basicRewardItemBagDataSet;
	TArray<const FGsSchemaRewardItemBagData*> _mainRewardItemBagDataSet;

	float _lastPartyViewScrollOffset = 0.f;

	// 2023/5/10 PKT - 가끔 가비지를 통해 포인터가 날아가는 것 같아서 'TWeakObjectPtr' -> UPROPERTY 로 변경
	TWeakObjectPtr<UGsUIIconItemInventory> _iconDungeonKey;

	UPROPERTY()
	UGsPoolUObject* _partyListViewEntryPool;

	UPROPERTY()
	UGsPoolUObject* _rewardListViewEntryPool;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// 2022/06/09 PKT - 난이도 탭
	void SetStateSideTabEnable(const class FGsDungeonGroupParty* InData);	
	// 2022/06/09 PKT - 하단 매칭 / 파티 생성 버튼 설정
	void SetStateControlButton(const class FGsDungeonGroupParty* InData);
	// 2022/06/09 PKT - 보상 횟수
	void SetStateRewardInfo(const class FGsDungeonGroupParty* InDungeonGroupParty);
	// 2022/06/01 PKT - 던전 Data를 화면에 출력
	void SetDungeonDataDisplay(const class FGsDungeonGroupParty* InDungeonGroupParty, int32 InIndex);
	// 2022/06/10 PKT - 이용 가능한 던전 레벨인지 체크
	bool CheckInDungeonPossible();
	// 2022/06/01 PKT - 파티 목록 새로 고침을 서버에게 요청
	void RequestRefreshPartyList();

public:
	virtual void Enter(const class FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData);
	virtual void Invalidate() override;
	virtual void Capture(FGsDungeonUICaptureData* OutCaptherData) const;

	virtual void InvalidatePartyInfoSet(const TArray<FGsPartyDungeonPartyStatusList>* InPartyStatusList);

protected:
	// 2022/05/31 PKT - 난이도 버튼 선택
	UFUNCTION()
	void OnSelectedDifficultyButton(int32 InIndex);
	// 2022/06/01 PKT - 새로 고침
	UFUNCTION()
	void OnClickRefreshPartyList();
	// 2022/06/02 PKT - 자동 매칭
	UFUNCTION()
	void OnClickAutoMatching();
	// 2022/06/14 PKT - 매칭 취소
	UFUNCTION()
	void OnClickMatchingCancel();
	// 2022/06/02 PKT - 파티 생성
	UFUNCTION()
	void OnClickCreateParty();
	// 2022/06/02 PKT - 파티 탈퇴
	UFUNCTION()
	void OnClickPartyLeave();
	// 2022/06/03 PKT - 파티 리스트 Scroll Offset
	UFUNCTION()
	void OnScrollPartyListView(float InOffsetInItems, float InDistanceRemaining);
	// 2022/06/07 PKT - 기본 보상 팝업 클릭
	UFUNCTION()
	void OnClickedShowBasicReward();
	// 2022/06/07 PKT - 메인 보상 팝업 클릭
	UFUNCTION()
	void OnClickedShowMainReward();
	// 2022/06/07 PKT - 파티 리스트 툴팁 클릭
	UFUNCTION()
	void OnClickedPartyListToolTip();

	void OnLongPressIconChargeItem(UGsUIIconItem& InIcon);

	void OnGoToParty(PartyId InPartyId);
	void OnIntrude(PartyId InPartyId);
};