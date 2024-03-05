// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Main/GsUIWindowQuestMainEntry.h"

#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "../GsUIQuestSlotHelper.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsSchemaEnums.h"

#include "Engine/AssetManager.h"

#include "TextBlock.h"
#include "TreeView.h"
#include "WidgetSwitcher.h"
#include "Text.h"
#include "Image.h"
#include "SlateBrush.h"
#include "PaperSprite.h"
#include "CanvasPanel.h"
#include "Color.h"

#include "Option/GsServerOption.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "GsUIWindowQuestMainItem.h"



void UGsUIWindowQuestMainEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_item = Cast<UGsUIWindowQuestMainItem>(ListItemObject);

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGsUIWindowQuestMainEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	SetIsSelected(bIsSelected);

	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGsUIWindowQuestMainEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIWindowQuestMainEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	if (true == _bIsChapterSlot)
		return;

	if (nullptr != _selectImg)
	{
		_selectImg->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

bool UGsUIWindowQuestMainEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIWindowQuestMainEntry::SetRedDot(bool bInIsOn)
{
	_redDot->SetRedDot(bInIsOn, EGsIconRedDotType::CHECK);
}

TSharedPtr<FGsQuestChapterTableCache> UGsUIWindowQuestMainEntry::GetchapterTableCacheInIndex(int32 inChapterIndex)
{
	FGsQuestMainTableCacheSet* mainTableCacheSet = GSQuest()->GetQuestMainTableCache();
	if (nullptr != mainTableCacheSet)
	{
		const TArray<TSharedPtr<FGsQuestChapterTableCache>>& chapterDataList = mainTableCacheSet->GetQuestChapterTableCacheList();

		for (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache : chapterDataList)
		{
			if (nullptr == chapterTableCache)
				continue;

			if (false == chapterTableCache->IsExposeQuest())
				continue;

			if (chapterTableCache->GetChapterIndex() == inChapterIndex)
			{
				return chapterTableCache;
			}
		}
	}

	return nullptr;
}

const FGsSchemaQuestMainChapter* UGsUIWindowQuestMainEntry::GetSchemaQuestChapterInIndex(int32 inChapterIndex)
{
	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetchapterTableCacheInIndex(inChapterIndex);
	if (chapterTableCache.IsValid())
	{
		return chapterTableCache->GetSchemaQuestChapter();
	}

	return nullptr;
}

void UGsUIWindowQuestMainEntry::SetDataChapter(int32 inChapterIndex)
{
	_isChapter = true;
	/*
	* 0 : 챕터
	* 1 : 스토리
	*/
	_switcherSlot->SetActiveWidgetIndex(0);
	_switcherChapterSlot->SetActiveWidgetIndex(static_cast<int32>(false));

	const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return;

	/*
	* 2 : 챕터 완료상태
	* 1 : 챕터 진행상태
	* 0 : 챕터 잠금상태
	*/	
	const FGsSchemaQuestMainChapter* schemaQuestChapter = GetSchemaQuestChapterInIndex(inChapterIndex);
	if (nullptr != schemaQuestChapter)
	{		
		bool isLock = GSQuest()->GetIsChapterLockedInIndex(inChapterIndex);
		if (true == isLock)
		{
			// 슬롯 결정
			_switcherSelectSlot->SetActiveWidgetIndex(0);
			_switcherUnSelectSlot->SetActiveWidgetIndex(0);
		}
		else
		{
			// 현재 진행 중인 스토리
			int myStoryIndex = questMainData->GetStoryIndex();
			bool isMyChapter = GSQuest()->GetIsMyChapterInIndex(inChapterIndex);
			if (isMyChapter)
			{
				TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetchapterTableCacheInIndex(inChapterIndex);
				if (chapterTableCache.IsValid())
				{
					// 챕터안에 스토리 갯수 (5개가 있다)
					int count = chapterTableCache->GetStoryTableCacheList().Num();
					if (0 < count)
					{
						int storyIndex = count - 1; // (0, 1, 2, 3, 4) // 그러므로 마지막 슬롯은 4
						TSharedPtr<FGsQuestStoryTableCache> storyTableCache = chapterTableCache->GetStoryTableCache(storyIndex);
						if (storyTableCache.IsValid())
						{
							if (const FGsSchemaQuestMainStory* schemaStory = storyTableCache->GetSchemaQuestStory())
							{
								if (isMyChapter)
								{
									// 내가 진행 중인 스토리가 전체 스토리 보다 작으면
									// 4미만
									if (myStoryIndex < storyIndex)
									{
										_switcherSelectSlot->SetActiveWidgetIndex(1);
										_switcherUnSelectSlot->SetActiveWidgetIndex(1);
									}
									// 내가 진행 중인 스토리가 4보다 크면 
									else if (myStoryIndex > storyIndex)
									{
										_switcherSelectSlot->SetActiveWidgetIndex(2);
										_switcherUnSelectSlot->SetActiveWidgetIndex(2);
									}
									else
									{
										if (GSQuest()->IsStoryRewarded(QuestType::MAIN, schemaStory->id))
										{
											// 완료 스토리
											_switcherSelectSlot->SetActiveWidgetIndex(2);
											_switcherUnSelectSlot->SetActiveWidgetIndex(2);
										}
										else
										{
											QuestId outQuestId = 0;
											bool isCompleted = GSQuest()->IsQuestStateCompleted(QuestType::MAIN, schemaStory->id, outQuestId);
											if (isCompleted)
											{
												// 완료 스토리
												_switcherSelectSlot->SetActiveWidgetIndex(2);
												_switcherUnSelectSlot->SetActiveWidgetIndex(2);
											}
											else
											{
												_switcherSelectSlot->SetActiveWidgetIndex(1);
												_switcherUnSelectSlot->SetActiveWidgetIndex(1);
											}
										}
									}
								}
							}
						}
					}
				}				
			}
			else
			{
				_switcherSelectSlot->SetActiveWidgetIndex(2);
				_switcherUnSelectSlot->SetActiveWidgetIndex(2);
			}
		}				

		_numText = FText::AsNumber(inChapterIndex + 1);
		_nameText = schemaQuestChapter->chapterTitleText;
	}

	_selectImg->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowQuestMainEntry::SetDataStory(int32 inChapterIndex, StoryId inStoryId)
{
	_isChapter = false;
	/*
	* 0 : 챕터
	* 1 : 스토리
	*/
	_switcherSlot->SetActiveWidgetIndex(1);

	/*
	* 0 : 스토리 완료상태
	* 1 : 스토리 진행상태
	* 2 : 스토리 잠금상태
	*/
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	bool isLock = questManager->GetIsChapterLockedInIndex(inChapterIndex);
	if (true == isLock)
		return;

	FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return;

	StoryId myStoryId = questMainData->GetCurrentStoryId();
	bool isMyChapter = questManager->GetIsMyChapterInIndex(inChapterIndex);

	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetchapterTableCacheInIndex(inChapterIndex);
	if (chapterTableCache.IsValid())
	{
		TSharedPtr<FGsQuestStoryTableCache> storyTableCache = chapterTableCache->GetStoryTableCacheInId(inStoryId);
		if (storyTableCache.IsValid())
		{
			const FGsSchemaQuestMainStory* schemaStory = storyTableCache->GetSchemaQuestStory();
			if (nullptr != schemaStory)
			{
				if (isMyChapter)
				{
					if (myStoryId < inStoryId)
					{
						// 잠긴 스토리
						_switcherStorySlot->SetActiveWidgetIndex(2);
					}
					else if (myStoryId > inStoryId)
					{
						// 완료 스토리
						_switcherStorySlot->SetActiveWidgetIndex(0);
					}
					else
					{
						if (GSQuest()->IsStoryRewarded(QuestType::MAIN, schemaStory->id))
						{
							// 완료 스토리
							_switcherStorySlot->SetActiveWidgetIndex(0);
						}
						else
						{
							QuestId outQuestId = 0;
							bool isCompleted = GSQuest()->IsQuestStateCompleted(QuestType::MAIN, schemaStory->id, outQuestId);
							if (isCompleted)
							{
								// 완료 스토리
								_switcherStorySlot->SetActiveWidgetIndex(0);
							}
							else
							{
								_switcherStorySlot->SetActiveWidgetIndex(1);
							}
						}
					}
				}
				else
				{
					_switcherStorySlot->SetActiveWidgetIndex(0);
				}

				FText findText;
				FText::FindText(TEXT("QuestUIText"), TEXT("WindowStoryTitle"), findText);

				_nameText = FText::Format(findText, _item->GetChapterIndex() + 1, _item->GetStoryIndex() + 1, schemaStory->storyTitleText);
			}
		}
	}

	_selectImg->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIWindowQuestMainEntry::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;

	_switcherChapterSlot->SetActiveWidgetIndex(static_cast<int32>(_bIsSelected));
	if (_isChapter)
	{		
		_selectImg->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_selectImg->SetVisibility((_bIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}