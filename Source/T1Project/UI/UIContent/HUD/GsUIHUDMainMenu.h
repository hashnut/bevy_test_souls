// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIControlLib/Control/GsStaticPanelSlotHelper.h"
#include "Unlock/GsUnlockDefine.h"
#include "RedDot/GsRedDotCheck.h"
#include "RedDot/GsRedDotCheckFunc.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageInput.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIHUDMainMenu.generated.h"

class UPanelWidget;
class UGsButton;
class UGsUIMainMenuBtnBase;
class UGsUIMainMenuBtnNormal;
class UGsUIMainMenuBtnSimple;
class UGsUIMainMenuBtnInventory;
class UGsUIMainMenuBtnBMShop;
class UGsUIMainMenuBtnHamberger;
struct IGsMessageParam;

/**
 * HUD 우상단 메인 메뉴 및 좌우 메뉴
 */
UCLASS()
class T1PROJECT_API UGsUIHUDMainMenu : public UGsUIHUD, public IGsEventProgressInterface
{
	GENERATED_BODY()

public:
	using MsgHandleInventory = TPair<MessageContentInven, FDelegateHandle>;
	using MsgHandleContents = TPair<MessageContents, FDelegateHandle>;	
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;
	using MsgHandleBMShop = TPair<MessageContentStoreBM, FDelegateHandle>;

public:
	enum EGsMenuType
	{
		//-------------------------------------------------
		// 퀵메뉴 안에 들어가지 않는 것들
		EVENT = 0,
		BMSHOP,		
		SKILL,
		INVENTORY,		
		//-------------------------------------------------
		QUICK_MENU,
		//-------------------------------------------------
		// 이하 퀵메뉴 안에 들어가는 것들(레드닷이 퀵메뉴에 영향을 주는 것들)	
		START_QUICK_MENU_SUB,
		COSTUME = START_QUICK_MENU_SUB,
		FAIRY,
		ITEM_COLLECTION,
		QUEST,
		MULTI_LEVEL_RANK,
		MARKET,
		CRAFT,
		GUILD,
		RANKING,
		BUDDY,
		MAIL,
		OPTION,
		OFFLINEPLAY,
		ACHIEVEMENT,
		MONSTER_COLLECTION,
		TITLE,
		GAME_STATISTICS,
		DUNGEON,
		USER_PROFILE,
		BOSS,
		GOTO_LOBBY,
		PK_BOOK,
		INVADE,
		SEAL,
		BATTLEPASS,
		SIEGE_WARFARE,
		SPIRIT_SHOT,
		MAX
	};	

protected:
	// 상단에 항상 표시되는 메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTopMenu;
	
	// 햄버거 메뉴 안에 들어가는 메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubMenu;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubMenuBg;

	// Top메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnInventory* _btnInventory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnBMShop* _btnBMShop;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnHamberger* _btnQuickMenu;
	// 별도처리 항목
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnExit;

	// 햄버거 메뉴 1
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnFairy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnItemCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnMultiLevelRank;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnMarket;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnCraft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnDungeon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnAchievement;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnMonsterKnowledgeCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnGameStatistics;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnBoss;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnPKBook;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnRanking;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnInvade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnSeal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnSpiritShot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnStarLegacy;

	// 햄버거 메뉴 2
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnCommunity;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnMail;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnFriend;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnUserProfile;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnBattlePass;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnSiegeWarfare;

	// 햄버거 메뉴 3
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnSimple* _btnScreenshot;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnNormal* _btnOffLinePlay;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnSimple* _btnOption;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMainMenuBtnSimple* _btnGoToLobby;

protected:
	MsgHUDRedDotHandleArray _messageDelegateList;
	TArray<MsgHandleInventory> _inventoryDelegateList;
	TArray<MsgHandleContents> _contentsDelegateList;
	MsgGameObjHandle _objectDelegate;
	MsgGameObjHandle _objectReviveDelegate;	
	MsgHandleBMShop _bmShopDelegate;
	TArray<MsgHandleUnlock> _unlockDelegateList;
	MsgInputHandle _inputParamDelegate;
	MsgHUDMainMenuActHandleArray _mainMenuActDelegateList;

protected:
	TMap<EGsMenuType, TWeakObjectPtr<UGsUIMainMenuBtnBase>> _menuMap;
	bool _isOpen = false; // True : 열린 상태, False : 닫힌 상태 or 닫는 중
	EGsUIHUDMode _prevMode = EGsUIHUDMode::NORMAL;

	TArray<EGsMenuType> _disabledMenu;

protected:
	FGsRedDotCheckFunc _reinforceSkill;
	FGsRedDotCheckQuickMenu _redDotCheckQuickMenu;
	FGsRedDotCheckFunc _redDotCheckCostume;
	FGsRedDotCheckFunc _redDotCheckFairy;
	FGsRedDotCheckFunc _redDotCheckBuddy;
	FGsRedDotCheckFunc _redDotCheckMail;
	FGsRedDotCheckFunc _redDotCheckEvent;
	FGsRedDotCheckFunc _redDotCheckAchievement;
	FGsRedDotCheckFunc _redDotCheckCraft;
	FGsRedDotCheckFunc _redDotCheckStatistics;
	FGsRedDotCheckFunc _redDotCheckOfflinePlay;
	FGsRedDotCheckFunc _redDotCheckRanking;
	FGsRedDotCheckFunc _reddotCheckBoss;	
	FGsRedDotCheckFunc _redDotCheckBattlePass;

	//------------------------------------------------------------------------------------------------------------------
	// 상속 함수
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	// UGsUIHUD override
protected:
	virtual void InvalidateAllInternal() override;
public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

	// IGsEventProgressInterface override
public:
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	//------------------------------------------------------------------------------------------------------------------
	// 초기 세팅 함수
private:
	void InitializeMenuFunc();
	void InitializeMenuWidget();
	void InitializeMenuUnlockType();
	void InitializeMenuRedDot();

private:
	void BindMessages();
	void UnbindMessages();

	//------------------------------------------------------------------------------------------------------------------	
	// 메시지 핸들 함수
private:
	// MHudRedDot
	void OnInventoryRedDot(bool bInActive);
	void OnBuddyRedDot(bool bInActive);
	void OnGuildRedDot(bool bInActive);
	void OnMailRedDot(bool bInActive);
	void OnItemCollectionRedDot(bool bInActive);
	void OnFairyRedDot(bool bInActive);
	void OnCostumeRedDot(bool bInActive);
	void OnSkillRedDot(bool bInActive);
	void OnMarketRedDot(bool bInActive);
	void OnCraftRedDot(bool bInActive);
	void OnEventRedDot(bool bInActive);
	void OnBMShopRedDot(bool bInActive);
	void OnMultiLevelRankRedDot(bool bInActive);
	void OnQuestRedDot(bool bInActive);
	void OnAchievementRedDot(bool bInActive);
	void OnMonsterCollectionRedDot(bool bInActive);
	void OnTitleRedDot(bool bInActive);
	void OnUserProfileRedDot(bool bInActive);
	void OnDungeon(bool bInActive);
	void OnPKBook(bool bInActive);
	void OnBattlePass(bool bInActive);

	// MContents
	void OnCloseQuickMenu();	

	// MInventory
	void UpdateInvenWeightCallback(const struct IGsMessageParam* InParam);
	void UpdateWeightPenaltyTypeCallback(const struct IGsMessageParam* InParam);

	// MGameObject
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);

	// MBMShop
	void UpdateBMShopEffect(struct FGsSharedParam& InMessageParam);

	// MUnlock
	void InvalidateMenu(const IGsMessageParam* InParam);
	void OnContentsUnlock(const IGsMessageParam* InParam);

	// MWindowInput
	void OnWindowKeyMapping(const IGsMessageParam* InParam);	

	// invasion window 가능하면 띄운다 (map호출)
	void OnCheckAndActiveInvasion();

	void CheckMainMenuAct();
	void OnMainMenuForcedDisable(int menuType);
	void OnMainMenuReturnToNormalRule(int menuType);
	//------------------------------------------------------------------------------------------------------------------
	// 버튼 바인딩
protected:
	void OnClickInvenOpen();	
	void OnClickOption();
	void OnClickQuest();
	void OnClickQuickMenu();
	void OnClickMail();
	void OnClickItemCollection();
	void OnClickFriend();
	void OnClickFairy();
	void OnClickGuild();
	void OnClickCostume();
	void OnClickDungeon();
	void OnClickSkill();
	void OnMultiLevelRank();
	void OnClickMarket();
	void OnClickCraft();
	void OnClickOfflinePlay();
	void OnClickEvent();
	void OnClickBMShop();
	void OnClickRanking();
	void OnClickAchievement();
	void OnClickMonsterKnowledgeCollection();
	void OnClickTitle();
	void OnClickGameStatistics();
	void OnClickUserProfile();
	void OnClickBoss();
	void OnClickGoToLobby();
	void OnClickPKBook();
	void OnClickInvade();
	void OnClickSeal();
	void OnClickScreenshot();
	void OnClickBattlePass();
	void OnClickSiegeWarfare();
	void OnClickSpiritShot();
	void OnClickStarLegacy();

	UFUNCTION()
	void OnClickExit();

	//------------------------------------------------------------------------------------------------------------------
	// 기타 로직
private:
	// 모두 갱신(체크)
	void UpdateRedDotAll(bool bUpdate);
	// bUpdate: T: 퀵메뉴 하위항목 전부 다시검사, F: 퀵메뉴 하위의 항목을 검사하지 않고, 캐싱된 값으로 업데이트
	void UpdateRedDot(EGsMenuType InType, bool bUpdate);
	// bUpdate: T: 퀵메뉴 하위항목 전부 다시검사, F: 퀵메뉴 하위의 항목을 검사하지 않고, 캐싱된 값으로 업데이트
	void UpdateRedDotQuickMenu(bool bUpdate);
private:
	void ShowSubMenu(bool bInOpen, bool bInPlayAnimation = true);
	void SetSubMenuVisibility(ESlateVisibility InVisibility);
	bool CanClickMenu(EGsMenuType InMenuType);
	void CloseInven();
	void ShowUpdateMessage();
	bool IsEnableExit() const;

	void OnInputCloseQuickMenu();

protected:
	// BP에 Open/Close 애니메이션 요청	
	// true: MenuOpen
	// false: MenuClose
	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpenAnimation(bool bInOpen);
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnExitCloseAnimation();

protected:
	UGsUIMainMenuBtnBase* GetMenu(EGsMenuType InType) const;
	UGsUIMainMenuBtnBase* GetMenu(int32 InTypeNum) const;

	// Back 시 햄버거 메뉴 닫기
public:
	bool OnBack();
};
