// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageSystem.h"

#include "GsUIWindowGuildMain.generated.h"

class UGsUIGuildContentBase;
class UGsButton;
class UGsSwitcherButton;
class UImage;
class UGsUIRedDotBase;
struct IGsMessageParam;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIWindowGuildMain : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;
	using MsgUserInfoHandle = TPair<MessageUserInfo, FDelegateHandle>;
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

	enum EGsGuildMainTab
	{
		Main = 0,
		Member,
		Dungeon,
		Quest,
		//ItemCollection,
		Agit, // 영지
		GuildStorage, // 정산 시스템
		Operation,	
		Manage,

		All,
		Max = All,
	};

	struct FGsShotCutItemCollectionData
	{
		ItemId itemId = INVALID_ITEM_ID;
		FText itemName = FText::GetEmpty();
	};

protected:

	// 메인 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMainTab;

	// 페이지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherContent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentMain* _contentMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentMember* _contentMember;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentDungeon* _contentDungeon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentQuest* _contentQuest;	
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UGsUIGuildContentItemCollection* _contentItemCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentAgit* _contentAgit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentStorage* _contentStorage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentOperation* _contentOperation;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildContentManage* _contentManage;

	// 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotMember;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotDungeon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotAgit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotStorage;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIRedDotBase* _uiRedDotCollection;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotOperation;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotManage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	FGsToggleGroup _toggleGroupMainTab;
	EGsGuildMainTab _currentTab = EGsGuildMainTab::Main;
	EGsGuildMainTab _prevTab = EGsGuildMainTab::Max;

	TMap<EGsGuildMainTab, UGsUIGuildContentBase*> _contentMap;
	TMap<EGsGuildMainTab, FGsRedDotChecker> _redDotCheckerMap;		
	TMap<EGsGuildMainTab, EGsUnlockContentsType> _unlockTypeMap;

	TArray<MsgGuildHandle> _msgGuildHandleList;
	TArray<MsgUserInfoHandle> _msgUserInfoHandleList;
	MsgSystemHandle _msgSystemParamHandle;
	TArray<MsgHandleUnlock> _msgUnlockHandleList;

	EGsGuildMainTab _lastTab = EGsGuildMainTab::Max;

	//FGsShotCutItemCollectionData _ShotCutItemCollectionData;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool SyncContentState() override;

private:
	void BindMessages();
	void UnbindMessages();

//public:
//	// 2021/10/29 PKT - Item Collection 바로가기
//	void OnShotcutItemCollection(ItemId InItemId, const FText& InItemName);

protected:
	// All 값을 넘기면 전부 업데이트
	void UpdateRedDot(EGsGuildMainTab InType);
	void SetUICurrentContent(int32 InIndex);
	void CheckUnlock();

protected:
	void OnClickMainTab(int32 InIndex);
	void OnClickBlockedTab(int32 InIndex);

protected:
	void OnReceiveGuildInfo(const IGsMessageParam*);
	void OnChangeMyAuthority(const IGsMessageParam*);
	void OnReceiveMemberList(const IGsMessageParam*);
	void OnReceiveJoinWaitingUserList(const IGsMessageParam*);
	void OnNotifyJoinRequest(const IGsMessageParam*);
	void OnReceiveAuthorityList(const IGsMessageParam*);
	void OnReceiveChangeGuildSetting(const IGsMessageParam* InParam);
	void OnChangeGuildLevel(const IGsMessageParam*);
	void OnChangeContribution(const IGsMessageParam*);
	void OnReceiveChangeMemberGrade(const IGsMessageParam* InParam);
	void OnNotifyChangeMemberGrade(const IGsMessageParam* InParam);
	void OnReceiveChangeDailyActivity(const IGsMessageParam*);
	void OnNotifyJoinMember(const IGsMessageParam*);
	void OnReceiveRemoveMember(const IGsMessageParam*);
	void OnReceiveBuildingList(const IGsMessageParam*);
	void OnReceiveBuildingUpdate(const IGsMessageParam* InParam);
	void OnNotifyChangeNotice(const IGsMessageParam*);	
	void OnReceiveJoinApproveResult(const IGsMessageParam* InParam);
	void OnDiplomacyInvalidateList(const IGsMessageParam*);
	void OnDiplomacyResendList(const IGsMessageParam*);
	void OnDiplomacyEnemyStateNotify(const IGsMessageParam* InParam);
	void OnDiplomacyAllyStateNotify(const IGsMessageParam* InParam);
	void OnReceiveQuestInfo(const IGsMessageParam*);
	void OnUpdateQuest(const IGsMessageParam* InParam);
	void OnReceiveQuestRefresh(const IGsMessageParam*);
	void OnCampConstruction(const IGsMessageParam*);
	void OnCampDestruction(const IGsMessageParam*);	
	void OnSanctumConstructionStateChanged(const IGsMessageParam*);
	void OnSanctumDestruction(const IGsMessageParam*);
	void OnStorageInvalidateList(const IGsMessageParam*);
	void OnChangedDivisionLIstCheck(const IGsMessageParam*);
	void OnReceiveAuctionDetail(const IGsMessageParam*);
	void OnUpdatedAuctionPayback(const IGsMessageParam*);
	void OnNotifyDivisionRedDot(const IGsMessageParam*);
	void OnNotifyDivisionRegister(const IGsMessageParam*);
	void OnUpdatedDivisionItem(const IGsMessageParam* InParam);
	void OnRemovedDivisionItem(const IGsMessageParam*);
	void OnNotifyAuctionRedDot(const IGsMessageParam*);
	void OnResetAuctionList(const IGsMessageParam*);
	void OnUpdatedAuctionItem(const IGsMessageParam* InParam);
	void OnRemovedAuctionItem(const IGsMessageParam* InParam);
	void OnAgitStateChanged(const IGsMessageParam*);

	void OnChangeUserCurrency(uint64 InType);

	void OnResetTimeContents(const IGsMessageParam* InParam);

	//// 2021/10/05 PKT - Item Collection 속성 변경
	//void OnItemCollectionUpdateList(const IGsMessageParam* InParam);
	//// 2021/10/05 PKT - Item Collection 완료 상태에 대한 스탯 상태 변경
	//void OnItemCollectionChangedStatStaus(const IGsMessageParam*);
	//// 2021/10/05 PKT - Item Collection 리스트를 비운다.
	//void OnItemCollectionCleanUp(const IGsMessageParam*);
	//// 2021/10/05 PKT - Item Collection 리스트 전체 갱신
	//void OnItemCollectionAll(const IGsMessageParam*);

	void OnStorageChanged(const IGsMessageParam* InParam);
	void OnStorageClose(const IGsMessageParam* InParam);

	void OnContentsUnlock(const IGsMessageParam* InParam);

protected:
	// 하위 페이지에서 메인탭 레드닷 갱신 요청 할 경우
	void OnUpdateMainTabRedDot(int32 InMainTabIndex);

protected:
	UGsUIGuildContentBase* GetContent(EGsGuildMainTab InType) const;
	UGsUIGuildContentBase* GetCurrentContent() const;
	EGsGuildMainTab GetCurrentTab() const { return _currentTab; }
	FGsRedDotChecker* GetRedDotChecker(EGsGuildMainTab InType);

public:
	FString GetCurrentTabString() const;
};