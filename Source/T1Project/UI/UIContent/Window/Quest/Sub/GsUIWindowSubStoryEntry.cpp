#include "GsUIWindowSubStoryEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "UMG/Public/Components/Image.h"
#include "Option/GsServerOption.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

void UGsUIWindowSubStoryEntry::NativeOnInitialized()
{
	_btnHudOnOff->OnClicked.AddDynamic(this, &UGsUIWindowSubStoryEntry::OnClickHudOnOff);

	Super::NativeOnInitialized();
}

void UGsUIWindowSubStoryEntry::OnClickHudOnOff()
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	// 0 허드 봄
	// 1 허브 안봄	
	_bIsHudUIOn = !_bIsHudUIOn;
	_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	serverOption->SetQuestSubUIVisibility(_bIsHudUIOn, _storyData->GetStoryId());
}

// 데이터 세팅
void UGsUIWindowSubStoryEntry::SetData(int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inStoryData)
{
	_contentsType = inContentsType;
	_storyData = inStoryData;
	_npcId = inNpcId;

	if (!_storyData.IsValid())
		return;

	const FGsSchemaQuestSubStory* schemaQuestSubStory = _storyData->GetSchemaQuestStory();
	if (nullptr == schemaQuestSubStory)
		return;

	_nameText = schemaQuestSubStory->storyTitleText;

	// lock : 0
	// 진행중 : 1
	// 완료 : 2
	SetInvadeSetting();
	SetSelect(false);

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestWindowUISubStateType type = questManager->GetSubStoryStateType(_npcId, _storyData->GetStoryId());
	_switcherSlot->SetActiveWidgetIndex(static_cast<int>(type));

	if (FGsServerOption* serverOption = GServerOption())
	{
		// 안보임 상태이면
		if (true == serverOption->IsHideQuestSubUI(_storyData->GetStoryId()))
		{
			// 오른쪽 아이콘 _ 안보임 상태일때
			_bIsHudUIOn = false;
		}
		else
		{
			// 오른쪽 아이콘 _ 보임 상태일때
			_bIsHudUIOn = true;
		}

		_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	}
}

void UGsUIWindowSubStoryEntry::Refresh()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (!_storyData.IsValid())
		return;

	QuestWindowUISubStateType type = questManager->GetSubStoryStateType(_npcId, _storyData->GetStoryId());
	_switcherSlot->SetActiveWidgetIndex(static_cast<int>(type));
}

void UGsUIWindowSubStoryEntry::SetInvadeSetting()
{
	_imgInvadeLock->SetVisibility((_contentsType == QuestContentsType::INVADE) ? 
					ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	_imgInvadeProgress->SetVisibility((_contentsType == QuestContentsType::INVADE) ? 
					ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	_imgInvadeComplete->SetVisibility((_contentsType == QuestContentsType::INVADE) ?
					ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	_imgInvadeAcceptable->SetVisibility((_contentsType == QuestContentsType::INVADE) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIWindowSubStoryEntry::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;

	_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}