#include "GsUIWindowSubNPCListItem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "UMG/Public/Components/Image.h"
#include "Option/GsServerOption.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

void UGsUIWindowSubNPCListItem::NativeOnInitialized()
{
	_btnItem->OnClicked.AddDynamic(this, &UGsUIWindowSubNPCListItem::OnClickItem);

	Super::NativeOnInitialized();
}

void UGsUIWindowSubNPCListItem::OnClickItem()
{
	/*_bIsSelected = !_bIsSelected;

	_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);*/

	if (_delegateClick.IsBound())
	{
		_delegateClick.ExecuteIfBound(this);
	}
}

void UGsUIWindowSubNPCListItem::OnClickHudUIOnOffBtn()
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	_bIsHudUIOn = !_bIsHudUIOn;
	_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	serverOption->SetQuestRepeatUIVisibility(_bIsHudUIOn, _storyData->GetStoryId());
}

// 데이터 세팅
void UGsUIWindowSubNPCListItem::SetData(int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inStoryData)
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
}

void UGsUIWindowSubNPCListItem::SetInvadeSetting()
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

void UGsUIWindowSubNPCListItem::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;

	_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}