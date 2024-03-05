#pragma once
#include "CoreMinimal.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../Quest/GsQuestData.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "GsUIWindowSubStoryEntry.generated.h"

struct FGsQuestSubStoryTableCache;
class UImage;
class UWidgetSwitcher;
/*
	npc list item
*/

UCLASS()
class UGsUIWindowSubStoryEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnHudOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgInvadeLock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgInvadeProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgInvadeComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgInvadeAcceptable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherHudOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectImg;
	
	bool _bIsSelected = false;
	bool _bIsHudUIOn = false;
	int32 _npcId = 0;
	TSharedPtr<FGsQuestSubStoryTableCache> _storyData;	
	QuestContentsType _contentsType = QuestContentsType::NORMAL;

public:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickHudOnOff();

public:
	// 데이터 세팅
	void SetData(int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inStoryData);
	void SetInvadeSetting();
	void SetSelect(bool inIsSelect);

	void Refresh();

	StoryId GetStoryId() { return (nullptr == _storyData) ? 0 : _storyData->GetStoryId(); }
};
