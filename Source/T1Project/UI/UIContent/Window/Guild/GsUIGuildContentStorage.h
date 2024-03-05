// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIGuildContentStorage.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsUIRedDotBase;
class UGsUIGuildStorageBase;
class UGsUIGuildDivisionComboBox;
class UTextBlock;
class UGsButton;
/**
 * 기사단 정산 메인 페이지
 */

UCLASS()
class T1PROJECT_API UGsUIGuildContentStorage : public UGsUIGuildContentBase
{
	GENERATED_BODY()

public:
	enum EGsStorageTab
	{
		DIVISION = 0,	// 분배
		AUCTION = 1,	// 경매
		CALCULATE = 2,	// 정산 결과

		ALL,
		MAX = ALL,
	};

protected:
	// 좌측 서브 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootTab;

	// 페이지 - 메인
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildDivision* _uiPageDivision;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildAuction* _uiPageAuction;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildCalculate* _uiPageCalculate;

	/* 페이지 - 하단 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPageBottom;

	/* 페이지 - 하단 - 분배 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDivisionCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDivisionTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAuctionTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _uiTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (ToolTip = "Tooltip Life Time"))
	float _toolTipLifetime = 4.0f;

	// 분배 방식(일반/참여자/지정) / 정산 방식(일반/참여자) 콤보박스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDivisionComboBox* _comboBoxDivision;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDivisionComboBox* _comboBoxResult;

	// 분배 시작 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDivision;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDivision;

	/* 페이지 - 하단 - 경매 */
	// 경매 등록 수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAuctionCount;
	// 경매 수수료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCommission;
	// 회수 가능한 금액
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPayback;
	// 회수 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPayback;

	// 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotDivision;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotAuction;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotCalculate;

protected:
	FGsToggleGroup _toggleSubTab;

	EGsStorageTab _prevTab = EGsStorageTab::MAX;
	EGsStorageTab _lastTab = EGsStorageTab::DIVISION;

	TMap<EGsStorageTab, UGsUIGuildStorageBase*> _pageMap;
	TMap<EGsStorageTab, FGsRedDotChecker> _redDotMap;

	bool _isStackRecoverFlag = false;

public:
	virtual FString GetTabName() const override { return TEXT("GuildStorage"); }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	//virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;

	void OnNotifyJoinMember();
	void OnReceiveRemoveMember();
	// 권한 변경이 일어날 경우 호출
	virtual void OnChangeMyAuthority() override;
	// 멤버 등급 변경 노티
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;
	// 정산 컨텐츠 하위 목록 갱신
	void OnInvalidateList();

	void OnUpdatedDivisionItem();
	void OnUpdatedAuctionItem(uint64 InAuctionId);
	void OnRemovedAuctionItem(uint64 InAuctionId);

	void OnChangedDivisionListCheck();
	void OnRegisterDivision();

	void OnResetAuctionList();
	void OnUpdatedAuctionPayback();

	void OnReceiveStorageDetail();
	
	void InvalidateRedDot();

protected:
	void OnSelectTab(int32 InIndex);
	void UpdateRedDot(EGsStorageTab InTab);
	UGsUIGuildStorageBase* GetCurrentPage() const;

public:
	void OnShowDivision();
	void OnShowAuction();

	void OnHideDivision();
	void OnHideAuction();

	void InvalidateDivisionCount(int32 InCount, int32 InMaxCount);
	void InvalidateDivisionButton();

	void InvalidateAuctionCount(int32 InCount, int32 InMaxCount);
	void InvalidateAuctionCommission();
	void InvalidateAuctionPayback();

protected:
	UFUNCTION()
	void OnClickTooltipDivision();
	UFUNCTION()
	void OnClickTooltipResult();
	UFUNCTION()
	void OnClickDivision();

	void OnChangedComboBox();
	void OnClickedComboBoxDivision();
	void OnClickedComboBoxResult();

	void SetTooltipPositionDivision();
	void SetTooltipPositionResult();
	void SetTooltipPosition(bool InIsDivision);

	UFUNCTION()
	void OnClickPayback();
};