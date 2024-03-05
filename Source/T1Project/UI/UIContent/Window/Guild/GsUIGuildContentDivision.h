//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//
//#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
//
//#include "UI/UIControlLib/Control/GsToggleGroup.h"
//
//#include "Guild/GsGuildAuctionData.h"
//
//#include "GsUIGuildContentDivision.generated.h"
//
//
//class UGsSwitcherButton;
//class UWidgetSwitcher;
//class UListView;
//class UTextBlock;
//class UGsButton;
//class UPanelWidget;
//class UUserWidget;
//class UGsUITooltipDesc;
//class UGsUIGuildDivisionComboBox;
///**
// * 기사단 분배 페이지
// */
//
//UCLASS()
//class T1PROJECT_API UGsUIGuildContentDivision : public UGsUIGuildContentBase
//{
//	GENERATED_BODY()
//
//protected:
//	// 전체 선택 버튼
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnCheckAll;
//
//	// 분배 목록 <-> 빈 목록
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherList;
//
//	// 분배 목록
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UListView* _listView;
//
//	// 분배 등록 수
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockRegister;
//
//	/* 분배/정산 방식 툴팁 */
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnDivisionTooltip;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnAuctionTooltip;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUITooltipDesc* _uiTooltip;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (ToolTip = "Tooltip Life Time"))
//	float _toolTipLifetime = 4.0f;
//
//	/* 분배 방식(일반/참여자/지정) / 정산 방식(일반/참여자) 콤보박스 */
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildDivisionComboBox* _comboBoxDivision;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildDivisionComboBox* _comboBoxResult;
//
//	// 분배 시작 버튼
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnStart;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherStart;
//
//protected:
//	FGsToggleGroup _toggleGroupDivision;
//	FGsToggleGroup _toggleGroupResult;
//
//	GuildAuctionType _currentDivisionType = GuildAuctionType::NORMAL;
//	GuildAuctionResultType _currentResultType = GuildAuctionResultType::NORMAL;
//
//public:
//	virtual FString GetTabName() const override { return TEXT("Division"); }
//
//protected:
//	virtual void NativeOnInitialized() override;
//
//public:
//	// 메인 탭 변경 후 필요한 패킷을 요청
//	virtual void SendRequestPacket() override;
//	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
//	virtual void Hide() override;
//
//	// 권한 변경이 일어날 경우 호출
//	virtual void OnChangeMyAuthority() override;
//	// 멤버 등급 변경 노티
//	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;
//
//	// 멤버 가입 노티
//	void OnNotifyJoinMember();
//	void OnReceiveDivisionList();
//	void OnChangedDivisionListCheck();
//	void OnUpdatedDivisionData();
//	void OnReceiveDivisionDetail();
//	void OnRegisterDivision();
//	void InvalidateItem(uint64 InItemDBId);
//
//	void OnChangedComboBox();
//	void OnClickedComboBoxDivision();
//	void OnClickedComboBoxResult();
//
//protected:
//	void InvalidateCheckAll();
//	void InvalidateList();
//	void InvalidateListEntry();
//	void InvalidateStart();
//
//protected:
//	UFUNCTION()
//	void OnClickCheckAll();
//
//	UFUNCTION()
//	void OnClickTooltipDivision();
//	UFUNCTION()
//	void OnClickTooltipResult();
//	
//	void SetTooltipPositionDivision();
//	void SetTooltipPositionResult();
//	void SetTooltipPosition(bool InIsDivision);
//
//	UFUNCTION()
//	void OnClickStart();
//
//};