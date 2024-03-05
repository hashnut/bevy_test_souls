#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "GsUIQuestBoardDetailPanel.generated.h"

UCLASS()
class T1PROJECT_API UGsUIQuestBoardDetailPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _questIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questTitleTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questDescriptionTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _questTargetTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIRewardInfoBase* _rewardInfo;

public:
	QuestStoryIdPair _selectedQuest;	
	TWeakPtr<struct FGsQuestRepeatStoryTableCache> _storyTable;
	MsgGameObjHandleArray _msgGameObjectHandleList;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;
	int32 _allQuestObjectCount;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Invalidate(const QuestStoryIdPair& inQuest);

private:
	bool IsNeedInvalid(const QuestStoryIdPair& inQuest);
	void InvalidQuestInfo();		
	void InvalidQuestObject();
	void InvalidReward(const QuestStoryIdPair& inQuest);
	void OnInvalidObject(const struct IGsMessageParam* inParam);
	void OnSelect(const struct IGsMessageParam* inParam);
};

