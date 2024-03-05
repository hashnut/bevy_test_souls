// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUserInfo.h"
#include "GsUIWindowAgitDungeon.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsUIAgitDungeonPageBase;
class UGsUIAgitDungeonPageRaid;
class UGsUITabButtonCommon;
struct IGsMessageParam;

/**
 * 기사단 영지 던전 윈도우(NPC와 대화하여 열리는 창)
 */
UCLASS()
class UGsUIWindowAgitDungeon : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;
	using MsgHandleUserInfo = TPair<MessageUserInfo, FDelegateHandle>;

public:
	enum class EGsAgitMainTab
	{
		Raid = 0,

		MaxType,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMainTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITabButtonCommon* _btnTabRaid;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIAgitDungeonPageRaid* _pageRaid;

protected:
	FGsToggleGroup _toggleGroupMainTab;
	TMap<EGsAgitMainTab, TWeakObjectPtr<UGsUIAgitDungeonPageBase>> _pageMap;

	TArray<MsgHandleGuild> _msgHandleGuildList;
	MsgSystemHandle _msgHandleSystemParam;
	MsgHandleUserInfo _msgHandleUserInfo;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool SyncContentState() override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	// MGuild
	void OnNotifyChangeMemberGrade(const IGsMessageParam* InParam);
	void OnRcvRaidInfo(const IGsMessageParam*);
	void OnRcvRaidDetailInfo(const IGsMessageParam* InParam);
	void OnRcvRaidBookmarkList(const IGsMessageParam*);
	void OnRcvRaidBookmarkChanged(const IGsMessageParam*);
	void OnRcvRaidOpenChanged(const IGsMessageParam* InParam);

	// MSystemParam
	void OnResetTimeContents(const IGsMessageParam* InParam);

	// MUserBasicInfo
	void OnChangeUserCurrency(uint64 InType);

private:
	void OnSelectMainTab(int32 InIndex);
	TWeakObjectPtr<UGsUIAgitDungeonPageBase> GetPage(EGsAgitMainTab InTabType);
	EGsAgitMainTab GetCurrentTab() const;
};
