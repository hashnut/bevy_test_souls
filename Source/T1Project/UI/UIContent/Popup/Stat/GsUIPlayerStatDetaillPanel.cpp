// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPlayerStatDetaillPanel.h"
#include "Components/SizeBox.h"
#include "Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "GsTableManager.h"

#include "Stat/GsSchemaStatFormatingData.h"
#include "GameObject/Stat/EStatImportanceType.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "UI/UIContent/Popup/Stat/GsBookmarkStatListViewEntry.h"

void UGsUIPlayerStatDetaillPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		_localPlayer = localPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
	}	

	BindMessage();

	_bookmarkStatControl = FGsPlayerStatHandler::GetInstance()->GetBookmarkStatController();
	InvalidWidget();
}

void UGsUIPlayerStatDetaillPanel::NativeDestruct()
{
	UnbindMessage();

	_statListView->ClearListItems();

	Super::NativeDestruct();
}

void UGsUIPlayerStatDetaillPanel::BindMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_uiMsgHandleArray.Empty();
		_uiMsgHandleArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_PLAYER_STAT, this, &UGsUIPlayerStatDetaillPanel::OnInvalidWidget));

		_gameObjMsgHandleArray.Empty();		
		_gameObjMsgHandleArray.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SET,
			this, &UGsUIPlayerStatDetaillPanel::OnInvalidWidget));
	}
}

void UGsUIPlayerStatDetaillPanel::UnbindMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (0 != _uiMsgHandleArray.Num())
		{
			for (auto iter : _uiMsgHandleArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiMsgHandleArray.Empty();
		}

		if (0 != _gameObjMsgHandleArray.Num())
		{
			for (auto iter : _gameObjMsgHandleArray)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjMsgHandleArray.Empty();
		}
	}
}

void UGsUIPlayerStatDetaillPanel::InitStatList()
{
	_statListView->ClearListItems();
	TArray<StatType> statBookmarkArray;
	TMap<int32, TArray<StatType>> sectorStatMap;
	_bookmarkStatControl->TryGetStat(statBookmarkArray, sectorStatMap);
	TArray<UGsBookmarkStatListViewData*> statDataList;

	if (statBookmarkArray.Num() > 0)
	{
		CreateStatItems(statBookmarkArray, statDataList);
		AddLine(statDataList);
	}

	TArray<int32> sectorArray;
	sectorStatMap.GenerateKeyArray(sectorArray);
	sectorArray.Sort();
	int32 num = sectorArray.Num();
	int32 lastIndex = num - 1;
	for (int32 i = 0; i < num; ++i)
	{
		const TArray<StatType>& statArray = sectorStatMap[sectorArray[i]];
		if (statArray.Num() > 0)
		{
			CreateStatItems(statArray, statDataList);
			if (i != lastIndex)
			{
				AddLine(statDataList);
			}
		}
	}

	_statListView->SetListItems(statDataList);
	_statListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIPlayerStatDetaillPanel::InvalidBookmarkStatCount()
{
	int32 count = _bookmarkStatControl->GetBookmarkStat().Num();
	int32 maxCount = _bookmarkStatControl->GetMaxUserStatBookmarkCount();
	FString countText = FString::Printf(TEXT("%d/%d"), count, maxCount);
	_bookmarkStatCountTextBlock->SetText(FText::FromString(countText));
}

void UGsUIPlayerStatDetaillPanel::CreateStatItems(const TArray<StatType>& inStatList, TArray<UGsBookmarkStatListViewData*>& inStatEntryList)
{
	int32 num = inStatList.Num();
	UGsBookmarkStatListViewData* temp = nullptr;
	for (int32 i = 0; i < num; ++i)
	{
		temp = NewObject<UGsBookmarkStatListViewData>();
		temp->InitBookmark(inStatList[i], false);
		inStatEntryList.Add(temp);
	}
}

void UGsUIPlayerStatDetaillPanel::AddLine(TArray<class UGsBookmarkStatListViewData*>& inStatEntryList)
{
	UGsBookmarkStatListViewData* temp = NewObject<UGsBookmarkStatListViewData>();
	temp->InitBookmark(StatType::MAX, true);
	inStatEntryList.Add(temp);
}

void UGsUIPlayerStatDetaillPanel::InvalidWidget()
{
	//get stat type
	InitStatList();
	InvalidBookmarkStatCount();
}

void UGsUIPlayerStatDetaillPanel::OnInvalidWidget(const struct IGsMessageParam* inParam)
{
	InvalidWidget();
}