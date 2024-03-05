#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"
#include "GsUIQuestBoardListViewEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsQuestBoardListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	TWeakPtr<FGsQuestBoardData> _questboardData;
	TWeakObjectPtr<class UGsUIWindowQuestBoard> _parent;

public:
	void Initialize(TSharedPtr<FGsQuestBoardData>& inPair, class UGsUIWindowQuestBoard* inParent);
};

UCLASS()
class T1PROJECT_API UGsUIQuestBoardListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _questIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questTitleTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _selectButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _acceptWidget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _selectImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _questStateWidgetSwitcher;

private:
	TWeakPtr<FGsQuestBoardData> _questBoardData;
	TWeakObjectPtr<class UGsUIWindowQuestBoard> _parent;	
	TWeakPtr<struct FGsQuestRepeatStoryTableCache> _storyTable;

private:
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;
	MsgGameObjHandleArray _msgGameObjectHandleList;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	UFUNCTION()
		void OnClickQuestBoard();
	void OnSelect(const struct IGsMessageParam* inParam);
	void OnInvalidate(const struct IGsMessageParam* inParam);
	void InvalidTitle();
	void InvalidSelect();
	void InvalidState();
};

