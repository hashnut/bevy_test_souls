#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "DataSchema/QuestBoard/GsSchemaRefreshCurrencyInfoList.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsUIWindowQuestBoard.generated.h"

enum class EGsQuestBoardTab
{
	Village,
	Personal,
	Guild,
	None
};

class URichTextBlock;

UCLASS()
class T1PROJECT_API UGsUIWindowQuestBoard : public UGsUIWindow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _questBoardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _tabPanelWidget;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIQuestBoardDetailPanel* _detailPanel;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questCountTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questBoardTitleCountTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questBoardCountTextBlock;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUICurrencyRefreshCountButton* _refreshButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _acceptButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _giveUpButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _completeButton;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _backButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _buttonSwitcher;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _sortPanelWidget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _questAcceptedSortButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _questNotAcceptedSortButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _questCompletedSortButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _questBoardSortButton;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _lockPanelWidget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class URichTextBlock* _lockRichText;

private:
	FGsToggleGroup _toggleGroupMainTab;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;
	TArray<TPair<MessageContentHud, FDelegateHandle>> _contentsDelegateArray;
	MsgGameObjHandleArray _msgGameObjectHandleList;
	class FGsGameStateQuestBoard* _questBoard;

	//refresh
private:
	const FGsSchemaRefreshCurrencyInfoList* _refreshCost;
	int32 _refreshCount;

	//select
private:
	QuestBoardType _questBoardTab = QuestBoardType::VILLAGE;
	QuestStoryIdPair _selectQuest;

private:
	FText _refreshText;
	FText _questAcceptText;
	FText _maxQuestReachText;
	FText _townQuestTitleText;
	FText _invasionQuestTitleText;
	FText _noticeDailyQuestAllClearText;
	FText _noticeNoProgressableQuestsText;
	FText _renewerDenyText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
		void OnRefresh();
	UFUNCTION()
		void OnAcceptQuest();
	UFUNCTION()
		void OnGiveUpQuest();
	UFUNCTION()
		void OnCompleteQuest();
	UFUNCTION()
		void OnClickSort();
	UFUNCTION()
		void OnSortByAccept();
	UFUNCTION()
		void OnSortByUnaccept();
	UFUNCTION()
		void OnSortByComplete();

private:
	void OnInitialize(const struct IGsMessageParam* inParam);
	void OnInvaliate(const struct IGsMessageParam* inParam);
	void OnSelect(const struct IGsMessageParam* inParam);
	void OnInvalidQuestIndex(const struct IGsMessageParam* inParam);
	void OnSort(const struct IGsMessageParam* inParam);
	void SelectFirstQuestBoard();

private:
	bool OnCheckTable(int32 inIndex);
	void OnClickTab(int32 InIndex);
	void OnInvalidButton(const struct IGsMessageParam* inParam);
	void OpenPopupQuestReward();
	void InvalidateAll();
	bool InvalidQuestBoardList();
	void InvalidateInfo();
	void InvalidButton();
	void InvalidRefresh();
	void InvalidQuestCount();
	void InvalidQuestCountTitle();
	void HideSort();
	void ClearData();
	bool IsContentsUnlock(QuestBoardType inBoardType);
	bool IsMainQuestUnlock(QuestBoardType inBoardType);
	bool IsInvalidBoardType(QuestBoardType inBoardType);
	void InvalidLockPanel();
};

