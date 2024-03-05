// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"
#include "Misc/DateTime.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UTIL/GsTimeUtil.h"

#include "GsUIPopupRevive.generated.h"

class UTextBlock;

/**
 * 부활 팝업
 */

const FName POPUP_REVIVE_NAME = TEXT("PopupRevive");

//오프라인 플레이시 부활 팝업 창이 오프라인 플레이 트레이 위에 생성되는 이슈가 있었다.
//오프라인 플레이 트레이가 아이템 디테일 팝업창을 위에 올리는 것을 허용하면서 발생한 이슈였다.
//EventProgress로 오더를 설정하여 수정하려고 하였으나 자동 부활시 맵을 다시 로드하면 
//EventProgress가 데이터 초기화하여 정상적으로 EventProgress를 활용하여 관리할 수 없는 현상이 발생
//따라서 부활 창은 EventProgress를 안쓰고 Window로 변경하여 활용한다. 
//https://jira.com2us.com/jira/browse/C2URWQ-1949
UCLASS()
class T1PROJECT_API UGsUIPopupRevive : public UGsUIWindow
{
	GENERATED_BODY()

		//resurrection ifno
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _attackerNameText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _guildNameText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _imgGuildEmblem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UHorizontalBox* _guildSection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _paneltyText;	
	// 길드 던전 강퇴 남은 시간 텍스트
	// https://jira.com2us.com/jira/browse/CHR-24638
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _guildDungeonKickRemainTimeText;

	//buttons
public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnReviveTown;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnReviveField;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnReviveInDungeon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnReviveLastLocation;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnObserving;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnInvasionOut;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUICurrencyButton* _btnReviveImmediatly;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnOk;

	//immedidate resurrection
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _immediateResurrectLeftTimeText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _immediateResurrectionLeftTimeWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _immediateResurrectionWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _penaltyPanel;

private:
	const struct FGsNetUserData* _userData;
	TArray<ResurrectType> _resurrectTypeList;

	//penalty item icon
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsHorizontalBoxIconSelector* _itemIconSelector1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsHorizontalBoxIconSelector* _itemIconSelector2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsHorizontalBoxIconSelector* _itemIconSelector3;

	//text
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (MultiLine = true))
	FText _contentText;
	FText _timeFormat;
	FText _attackNameText;
	FText _lossExpText;		
	FText _hourMinutSecondText;
	FText _minutSecondText;
	FText _secondText;
	FText _resurrectNowText;
	FText _dungeonObservatingNoUserText;

	// 길드 던전 사망 강퇴 대기 텍스트 포맷
	// https://jira.com2us.com/jira/browse/CHR-24638
	FText _guildDungeonDeathKickWaitTextFormat;

	//immediate resurreciton 
private:
	bool _isAutoRevive;
	bool _isActiveImmediateResurrectButton;
	bool _isShowImmediateResurrectionTime;
	FGsDateTimer _immediateResurrectionTimer;

private:
	FGsDateTimer _waitTimer;

	//resurrection panelty item
private:
	TArray<TWeakObjectPtr<class UGsHorizontalBoxIconSelector>> _itemIconSelectorList;
	UPROPERTY()
		TArray<class UGsUIIconItemInventory*> _itemIconList;
	UPROPERTY()
		TArray<class UGsDynamicIconSlotHelper*> _itemIconHelperList;

	UPROPERTY()
		TArray<class UWidget*> _buttonArray;

	// 기사단 던전 사망 강퇴 시간 표시
private:
	// 사망후 강퇴 대기인가
	// https://jira.com2us.com/jira/browse/CHR-24638
	bool _isWaitDeathKick = false;
	// 사망후 강퇴 대기 시작시간
	// https://jira.com2us.com/jira/browse/CHR-24638
	int64 _waitDeathKickStartTime = 0;
	// 사망후 강퇴 대기 시간(tick)
	float _waitDeathKickTimeTick = 0.0f;
	// 사망후 강퇴 대기 시간(초)
	int _waitDeathKickTimeSec = 0;
	// 현재 남은 시간(초)
	int _currentRemainDeathKickTimeSec = 0;


private:

	MsgGameObjHandle _invalidObservingEvent;
	

public:
	void ShowRevivePopup(bool inIsAutoRevive = false);
	void ClearLockAll();
	/// <summary>
	/// 외부 입력 부활 타입
	/// </summary>
	void ShowPopupByExternalResurrectTypes(TArray<ResurrectType> In_arrResurrectType);
	// 마지막 위치 버튼만
	void ShowPopupLastPosition();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& inMyGeometry, float InDeltaTime);
	virtual bool OnBack() override;

	// Hide 메시지 시 Tray처럼 동작
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY; }

	//button 
private:
	UFUNCTION()
		void OnClickReviveTown();
	UFUNCTION()
		void OnClickReviveField();
	UFUNCTION()
	void OnClickInDungeon();
	UFUNCTION()
		void OnClickReviveImmediatly();
	UFUNCTION()
		void OnClickOk();
	UFUNCTION()
		void OnClickReviveLastPosition();
	UFUNCTION()
		void OnClickObserving();	
	UFUNCTION()
		void OnClickInvasionOut();

	//공통 정보
private:	
	void InvalidAttackerName();
	void InvalidInternal();	

	//버튼
private:
	void InvalidButtons();
	void HideAllButtons();	
	void InvalidButtonInternal(ResurrectType inType, bool inShow = true);
	void ShowOkButton(bool inShow);
	void ShowImmediateReviveButton(bool inShow);
	void ShowLastPositionReviveButton(bool inShow);
	void ShowObservingButton(bool inShow);
	void ShowInvasionButton(bool inShow);
	void InvalidImmediateReviveButton();
	void InitImmediateResurrectionButton();	

private:
	void InvalidImmediateResurrectionTime();
	void DisableImmediateResurrectionButton();

	//패널티
private:
	void InvalidPenalty();
	void InvalidPenaltyInternal(ResurrectType inType);
	void InvalidExpPenalty();
	void InvalidItemPenalty();	

	void ShowPenaltyPanel(bool inShow = true);	
	void HideAllPenalty();			
	
	void OnInvalidRevive(const IGsMessageParam* inParam = nullptr);


	//패널티 아이템 아이콘
private:
	void InitIconList();
	void ClearIconList();	

public:
	// 길드 던전 사망시 퇴장 시간 표시 보여줄지
	// https://jira.com2us.com/jira/browse/CHR-24638
	void ShowGuildDungeonKickRemainTimeText(bool In_show);
	// 남은시간 표시
	void SetDeathKickWaitRemainTime(int In_remainSec);
};
