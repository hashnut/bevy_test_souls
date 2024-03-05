// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIWindow.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIWindowRanking.generated.h"

class UGsButton;
class UWidget;
class UUserWidget;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UWidgetSwitcher;
class UGsUIRankingPageGuild;
class UGsUIRankingPagePersonal;
class UGsUIRankingPageGuildDungeon;
class UTextBlock;
class UListView;
class UPanelWidget;
struct FGsSchemaWorldData;
/**
 * 랭킹시스템 UI
 */
UCLASS()
class T1PROJECT_API UGsUIWindowRanking : public UGsUIWindow
{
	GENERATED_BODY()
protected:
	enum EGsRankingPage
	{
		PERSONAL = 0,
		GUILD,
		GUILD_DUNGEON_CLEAR,
		SERVER_LIST,
	};

protected:
	/************************************************************************/
	/* Buttons                                                              */
	/************************************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRewardInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnServer;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockServer;

	/************************************************************************/
	/* Tab                                                                  */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _rootPanelMainTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _rootPanelSubTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _rootPanelGroupTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGroupTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelServerList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _rootPanelSubTabSide;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfMainTab;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSubTab;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfGroupTab;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfServerList;
	
protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMainTabBtn;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSubTabBtn;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperGroupTabBtn;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperServerList;
	
protected:
	FGsToggleGroup _toggleGroupMainTab;
	FGsToggleGroup _toggleGroupSubTab;
	FGsToggleGroup _toggleGroupGroupTab;
	
	/************************************************************************/
	/* Pages                                                                */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRankingPageGuild* _pageGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRankingPagePersonal* _pagePersonal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRankingPageGuildDungeon* _pageGuildDungeon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuide;

protected:
	TSet<WorldId> _currentWorldList;
	TArray<const FGsSchemaWorldData*> _worldList;
	const FGsSchemaWorldData* _selectedServer = nullptr;

	/************************************************************************/
	/* overrides                                                            */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

	virtual bool SyncContentState() override;

	/************************************************************************/
	/* Main, Sub, Group Tab                                                 */
	/************************************************************************/
private:
	void InvalidateMainTab();
	void InvalidateSubTab();
	void InvalidateSubTabSide();
	void InvalidateServerList();
	
private:
	void OnSelectChangedMainTab(int32 InIndex);
	void OnSelectChangedSubTab(int32 InIndex);
	void OnSelectChangedGroupTab(int32 InIndex);

private:
	// 만들어진 탭의 텍스트를 설정하고 토글 그룹에 추가한다.
	UFUNCTION()
	void MainTabListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void SubTabListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void GroupTabListRefresh(int32 InIndex, UWidget* InEntry);

	/************************************************************************/
	/* Ranking List															*/
	/************************************************************************/
private:
	void SetRankingPage(int32 InMainTabIndex, int32 InSubTabIndex, int32 InGroupTabIndex = 0);
	void SetRankingCheckCycle(const FString& InCheckCycle, const FString& InRewardCycle);

public:
	void OnUpdateRankingPage();

	/************************************************************************/
	/* Ranking Reward Info Button                                           */
	/************************************************************************/
private:
	UFUNCTION()
	void OnClickRewardInfo(); // 보상 정보 팝업 오픈

	/************************************************************************/
	/* Server Select                                                        */
	/************************************************************************/
	UFUNCTION()
	void ServerListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnClickServer();
	void OnSelectedServer(const FGsSchemaWorldData* InWorldData);


};
