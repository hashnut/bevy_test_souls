//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Message/GsMessageContents.h"
//#include "UI/UIContent/Helper/GsRedDotChecker.h"
//#include "UI/UIControlLib/Control/GsToggleGroup.h"
//#include "UI/UILib/Base/GsUIPopup.h"
//#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
//#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "GsUITrayGuildStorage.generated.h"
//
//
//class UGsButton;
//class UPanelWidget;
//class UGsToggleButton;
//class UImage;
//class UWidgetSwitcher;
//class UGsUIGuildStoragePageBase;
//class UGsUIGuildStoragePageDistribution;
//class UGsUIGuildStoragePageAcquireDetail;
//class UGsUIGuildStoragePageDistributionDetail;
//class UGsUIInventoryCommon;
//class FGsInventoryLogicGuildStorage;
//class FGsItem;
//struct IGsMessageParam;
///**
// * 기사단 창고 메인 UI
// */
//UCLASS()
//class T1PROJECT_API UGsUITrayGuildStorage : public UGsUIPopup
//{
//	GENERATED_BODY()
//
//public:
//	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;
//
//	enum EGsGuildStorageMainTab
//	{
//		Distribution = 0,
//		AcquireDetail,
//		DistributionDetail,
//
//		All,
//		Max = All
//	};
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnBack;
//
//	// 상단 탭 : 아이템 분배, 획득 내역, 분배 내역
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UPanelWidget* _panelMainTab;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsToggleButton* _btnDistribution;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsToggleButton* _btnAcquireDetail;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsToggleButton* _btnDistributionDetail;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UImage* _uiRedDotAcquired;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UImage* _uiRedDotDistributed;
//
//	// 하부 페이지 : 아이템 분배, 획득 내역, 분배 내역
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherPage;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildStoragePageDistribution* _pageDistribution;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildStoragePageAcquireDetail* _pageAcquireDetail;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildStoragePageDistributionDetail* _pageDistributionDetail;
//	
//	// 창고 인벤토리
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIInventoryCommon* _inventoryUI;
//	FGsInventoryLogicGuildStorage* _inventoryLogic = nullptr;
//
//
//protected:
//	TArray<MsgGuildHandle> _msgGuildHandleList;
//
//protected:
//	FGsToggleGroup _toggleGroup;
//	TArray<UGsUIGuildStoragePageBase*> _pageList;
//	TArray<FGsRedDotChecker> _redDotCheckerList;
//	EGsGuildStorageMainTab _tabPrev = EGsGuildStorageMainTab::Max;
//
//	/************************************************************************/
//	/* overrides                                                            */
//	/************************************************************************/
//public: 
//	virtual void BeginDestroy() override;
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//	virtual void NativeTick(const FGeometry& InMyGeometry, float InDeltaTime) override;
//
//	virtual void InvalidateAllInternal() override;
//
//	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
//	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
//	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
//
//	virtual void OnClickBack() override;
//	virtual void OnInputCancel() override;
//
//	/************************************************************************/
//	/* Message                                                              */
//	/************************************************************************/
//private:
//	void RegisterMessages();
//	void DeregisterMessages();
//
//	/************************************************************************/
//	/* Setter & Getter                                                      */
//	/************************************************************************/
//protected:
//	void SetUICurrentPage(int32 InIndex);
//
//	UGsUIGuildStoragePageBase* GetCurrentPage() const;
//	EGsGuildStorageMainTab GetCurrentTab() const;
//
//public:
//	bool IsItemSelected();
//	FGsItem* GetSelectedItem();
//	FString GetSelectedUserName();
//	UserDBId GetSelectedUserDBId();
//
//	/************************************************************************/
//	/* RedDot																*/
//	/************************************************************************/
//public:
//	void UpdateRedDot(EGsGuildStorageMainTab InType);
//
//	/************************************************************************/
//	/* Events																*/
//	/************************************************************************/
//protected:
//	void OnClickMainTab(int32 InIndex);
//	void OnClickInventoryItem(ItemDBId InItemDBId);
//	
//protected:
//	void OnNotifyChangeMemberGrade(const IGsMessageParam* InParam);
//	void OnNotifyJoinMember(const IGsMessageParam* InParam);
//	void OnReceiveRemoveMember(const IGsMessageParam* InParam);
//	void OnChangeMyAuthority(const IGsMessageParam* InParam);
//	void OnNotifyAcquireDetail(const IGsMessageParam* InParam);
//	void OnNotifyDistributeDetail(const IGsMessageParam* InParam);
//	void OnExtendSlot(const IGsMessageParam* InParam);
//	void OnInvalidatePage(const IGsMessageParam* InParam);
//	void OnInvalidateInventory(const IGsMessageParam* InParam);
//	void OnInvalidateDistribution(const IGsMessageParam* InParam);
//	void OnInvalidateAllData(const IGsMessageParam* InParam);
//
//public:
//	void InvalidateInventory();
//};