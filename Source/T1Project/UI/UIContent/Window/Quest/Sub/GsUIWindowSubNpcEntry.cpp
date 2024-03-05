#include "GsUIWindowSubNpcEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "UMG/Public/Components/Image.h"
#include "Option/GsServerOption.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "UTIL/GsTableUtil.h"
#include "Engine/AssetManager.h"
#include "PaperSprite.h"

void UGsUIWindowSubNpcEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIWindowSubNpcEntry::OnClickItem()
{
	/*_bIsSelected = !_bIsSelected;

	_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);*/

	/*if (_delegateClick.IsBound())
	{
		_delegateClick.ExecuteIfBound(this);
	}*/
}

// 데이터 세팅
void UGsUIWindowSubNpcEntry::SetData(int32 inNpcId)
{
	_npcId = inNpcId;

	int32 key = GLevel()->GetCurrentLevelId();

	const FGsSchemaNpcData* npcData =
		UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inNpcId);

	if (nullptr == npcData)
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TSharedPtr<FGsQuestSubChapterTableCache> subChapterTableCache = questManager->GetSubChapterTableCache(inNpcId);
	if (!subChapterTableCache.IsValid())
		return;

	const FGsSchemaQuestSubChapter* schemaQuestSubChapter = subChapterTableCache->GetSchemaQuestChapter();
	if (nullptr == schemaQuestSubChapter)
		return;

	FSoftObjectPath npcFaceIconPath;
	if (const FGsSchemaQuestImg* chemaQuestImg = schemaQuestSubChapter->npcIcon.GetRow())
		npcFaceIconPath = chemaQuestImg->resPath;

	SetImg(_imgSelectFace, npcFaceIconPath);
	SetImg(_imgUnSelectFace, npcFaceIconPath);

	_nameText = npcData->nameText;

	QuestWindowUISubStateType type = questManager->GetSubNpcStateType(inNpcId);
	_switcherSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
	_switcherUnSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
}

void UGsUIWindowSubNpcEntry::SetImg(UImage* inImg, const FSoftObjectPath& iconPath)
{
	if (nullptr == inImg)
		return;

	UPaperSprite* sprite = Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(iconPath));
	if (nullptr == sprite)
		return;

	FSlateAtlasData atlasData = sprite->GetSlateAtlasData();
	FVector2D size = atlasData.GetSourceDimensions();

	inImg->SetBrushFromAtlasInterface(sprite);
	inImg->SetBrushSize(size);
}

void UGsUIWindowSubNpcEntry::Refresh()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestWindowUISubStateType type = questManager->GetSubNpcStateType(_npcId);
	_switcherSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
	_switcherUnSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
}

void UGsUIWindowSubNpcEntry::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;
	_switcherSelect->SetActiveWidgetIndex(static_cast<int>(inIsSelect));

	/*_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);*/
}