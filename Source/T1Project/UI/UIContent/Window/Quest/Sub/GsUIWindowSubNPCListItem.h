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
#include "GsUIWindowSubNPCListItem.generated.h"

struct FGsQuestSubStoryTableCache;
class UImage;
class UWidgetSwitcher;
/*
	npc list item
*/

UCLASS()
class UGsUIWindowSubNPCListItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, class UGsUIWindowSubNPCListItem*);

	// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UWidgetSwitcher* _switcherContentsType;

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
	FOnClickSlot  _delegateClick;

	// 가상 함수
public:
	virtual void NativeOnInitialized() override;

	// 로직 함수
protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickItem();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickHudUIOnOffBtn();

public:
	// 데이터 세팅
	void SetData(int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inStoryData);
	void SetInvadeSetting();
	void SetSelect(bool inIsSelect);

	StoryId GetStoryId() { return (nullptr == _storyData) ? 0 : _storyData->GetStoryId(); }
};
