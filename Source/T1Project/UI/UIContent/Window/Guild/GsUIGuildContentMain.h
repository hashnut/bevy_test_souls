// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildContentMain.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsButton;
class UGsUIRedDotBase;
class UImage;
class UListView;
class UScrollBox;
class UGsSwitcherButton;
class FGsGuildDataDetail;
class UGsDynamicPanelSlotHelper;
class UGsUIGuildDiplomacyPage;
class UGsUIProgressBar;
struct FGsGuildMsgParamDiplomacyNotify;

/**
 * 길드 메인 페이지 메인 탭
 * - 주의: 다른 길드 정보 상세보기 시에도 동일UI 활용함에 유의
 */
UCLASS()
class T1PROJECT_API UGsUIGuildContentMain : public UGsUIGuildContentBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCreateDate;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCaptain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMember;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _barContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCampLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipCamp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDonationRank;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDungeonClearRank;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAttendance;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildAssetTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildAsset;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconGuildAsset;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _snsLinkIconListView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollNotice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textBlockNotice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNoticeEmpty;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotNotice;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDonation;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAttendance;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotAttendance;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStorage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotStorage;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildDiplomacyPage* _uiDiplomacy;

protected:
	bool _bIsMyGuild = false;
	FGsRedDotChecker _redDotNotice;
	FGsRedDotChecker _redDotAttendance;
	FGsRedDotChecker _redDotStorage;

public:
	virtual FString GetTabName() const override { return TEXT("Main"); }

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void InvalidateAllInternal() override;

public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;

	virtual void OnResetTimeContents(ResetTimeTicket InType) override;
	virtual void OnChangeMyAuthority() override;
	virtual void OnReceiveChangeGuildSetting(const struct FGsGuildMsgParamSetting* InParam) override;	
	
	virtual void OnChangeCurrency(CurrencyType InType) override;
	virtual void OnChangeGuildLevel() override;
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) override;
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;
	virtual void OnNotifyChangeNotice() override;	

public:
	void OnReceiveGuildInfo();

	void OnChangeContribution();
	// 일일 활동(출석/기부)
	void OnReceiveChangeDailyActivity();
	// 기사단 창고 갱신
	void OnUpdateStorage();
	// 외교 리스트 요청
	void OnDiplomacyResendList();
	// 외교 리스트 갱신
	void OnDiplomacyInvalidateList();
	// 외교 적대길드 상태 바뀜
	void OnDiplomacyEnemyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam);
	// 외교 동맹/동맹현황 정보 바뀜
	void OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam);

protected:
	UFUNCTION()
	void OnClickDonation();
	UFUNCTION()
	void OnClickAttendance();
	UFUNCTION()
	void OnClickStorage();
	UFUNCTION()
	void OnClickTooltipCamp();
	UFUNCTION()
	void OnClickSnsIcon(const int32 InIndex);

	void OnChangeDiplomacyTab();

protected:
	void SetUIGuildFrame(bool bIsMyGuild);

	void SetUIGuildLevel(GuildLevel InLevel, int32 InCampLevel);
	void SetUIGuildMember(int32 InCurrMember, int32 InMaxMember);
	void SetUIGuildEmblem(GuildEmblemId InEmblemId);
	void SetUIGuildAttendance(const FGsGuildDataDetail* InData);
	void SetUIGuildAsset(Currency InAmount);
	void SetUIGuildLimitLevel(Level InLevel);
	void SetUIMasterName(const FString& InUserName);
	void SetUIGuildNotice(const FString& InNotice);
	void SetUIConribution(GuildLevel InLevel, GuildContribution InContribution);
	void SetUIRank(int32 InDonationRank, int32 InDungeonClearRank);
	void SetSnsLinkIcons(const TArray<FString>& InSnsLinks);

	bool CheckIsJoinedDay();
};
