// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMultiLevelRankPledgeSelect.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

#include "PaperSprite.h"

#include "DataSchema/MultiLevelRank/GsSchemaMultiLevelRankPledge.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankPledgeChange.h"
#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankQuest.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIPopupMultiLevelRankPledgeSelect::BeginDestroy()
{
	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickClose);
	}
	_btnClose = nullptr;

	if (nullptr != _btnLeftAccept)
	{
		_btnLeftAccept->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPledge);
	}
	_btnLeftAccept = nullptr;
	if (nullptr != _btnLeftChange)
	{
		_btnLeftChange->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPledge);
	}
	_btnLeftChange = nullptr;

	if (nullptr != _btnMiddleAccept)
	{
		_btnMiddleAccept->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePledge);
	}
	_btnMiddleAccept = nullptr;
	if (nullptr != _btnMiddleChange)
	{
		_btnMiddleChange->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePledge);
	}
	_btnMiddleChange = nullptr;

	if (nullptr != _btnRightAccept)
	{
		_btnRightAccept->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPledge);
	}
	_btnRightAccept = nullptr;
	if (nullptr != _btnRightChange)
	{
		_btnRightChange->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPledge);
	}
	_btnRightChange = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMultiLevelRankPledgeSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickClose);

	_btnLeftAccept->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPledge);
	_btnLeftChange->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPledge);
	_btnMiddleAccept->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePledge);
	_btnMiddleChange->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePledge);
	_btnRightAccept->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPledge);
	_btnRightChange->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPledge);

	_btnLeftPreview->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPreview);
	_btnMiddlePreview->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePreview);
	_btnRightPreview->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPreview);
}

void UGsUIPopupMultiLevelRankPledgeSelect::NativeConstruct()
{
	Super::NativeConstruct();

	const UGsTableMultiLevelRankPledge* pledgeTable = Cast<UGsTableMultiLevelRankPledge>(FGsSchemaMultiLevelRankPledge::GetStaticTable());
	if (nullptr == pledgeTable)
		return;

	TArray<const FGsSchemaMultiLevelRankPledge*> allRows;
	if (false == pledgeTable->GetAllRows(allRows))
		return;

	_groupIdMap.Empty();
	TMap<EGsMultiLevelRankPledgePosition, const FGsSchemaMultiLevelRankPledge*> pledgeDataMap;
	for (const FGsSchemaMultiLevelRankPledge* pledgeData : allRows)
	{
		if (pledgeData->groupId == 1)
		{
			_groupIdMap.Emplace(pledgeData->position, pledgeData->multiLevelRankGroupId);
			pledgeDataMap.Emplace(pledgeData->position, pledgeData);
		}
	}

	_textBlockNameLeft->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::LEFT]->pledgeName);
	_textBlockNameMiddle->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::MIDDLE]->pledgeName);
	_textBlockNameRight->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::RIGHT]->pledgeName);

	_textBlockDescLeft->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::LEFT]->pledgeDesc);
	_textBlockDescMiddle->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::MIDDLE]->pledgeDesc);
	_textBlockDescRight->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::RIGHT]->pledgeDesc);

	_textBlockDesc2Left->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::LEFT]->pledgeDesc2);
	_textBlockDesc2Middle->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::MIDDLE]->pledgeDesc2);
	_textBlockDesc2Right->SetText(pledgeDataMap[EGsMultiLevelRankPledgePosition::RIGHT]->pledgeDesc2);

	SetImage(_imageBackgroundLeft, pledgeDataMap[EGsMultiLevelRankPledgePosition::LEFT]->image);
	SetImage(_imageBackgroundMiddle, pledgeDataMap[EGsMultiLevelRankPledgePosition::MIDDLE]->image);
	SetImage(_imageBackgroundRight, pledgeDataMap[EGsMultiLevelRankPledgePosition::RIGHT]->image);

	InvalidateSlot();
}

void UGsUIPopupMultiLevelRankPledgeSelect::InvalidateSlot()
{
	int32 groupId = INDEX_NONE;
	bool isLocked = true;
	bool isPledgeSelected = false;
	if (FGsMultiLevelRankManager* manager = GSMultiLevelRank())
	{
		// 패널 잠겼는가?
		isPledgeSelected = manager->IsPledgeSelected();
		isLocked = (!manager->CanSelectPledge() && !isPledgeSelected);

		groupId = manager->GetSelectedPledgeId();
	}

	enum PledgeButtonIndex {ACCEPT = 0, CHANGE, ACTIVE, PREVIEW };
	_panelLock->SetVisibility(isLocked ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	//_switcherLeft->SetVisibility(isLocked ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	//_switcherMiddle->SetVisibility(isLocked ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	//_switcherRight->SetVisibility(isLocked ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (isLocked)
	{
		_switcherLeft->SetActiveWidgetIndex(PledgeButtonIndex::PREVIEW);
		_switcherMiddle->SetActiveWidgetIndex(PledgeButtonIndex::PREVIEW);
		_switcherRight->SetActiveWidgetIndex(PledgeButtonIndex::PREVIEW);
	}
	else
	{
		_switcherLeft->SetActiveWidgetIndex((groupId == _groupIdMap[EGsMultiLevelRankPledgePosition::LEFT]) ?
			PledgeButtonIndex::ACTIVE : (isPledgeSelected ? PledgeButtonIndex::CHANGE : PledgeButtonIndex::ACCEPT));
		_switcherMiddle->SetActiveWidgetIndex((groupId == _groupIdMap[EGsMultiLevelRankPledgePosition::MIDDLE]) ?
			PledgeButtonIndex::ACTIVE : (isPledgeSelected ? PledgeButtonIndex::CHANGE : PledgeButtonIndex::ACCEPT));
		_switcherRight->SetActiveWidgetIndex((groupId == _groupIdMap[EGsMultiLevelRankPledgePosition::RIGHT]) ?
			PledgeButtonIndex::ACTIVE : (isPledgeSelected ? PledgeButtonIndex::CHANGE : PledgeButtonIndex::ACCEPT));
	}
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickClose()
{
	Close();
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPledge()
{
	OpenPledgeChange(EGsMultiLevelRankPledgePosition::LEFT);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePledge()
{
	OpenPledgeChange(EGsMultiLevelRankPledgePosition::MIDDLE);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPledge()
{
	OpenPledgeChange(EGsMultiLevelRankPledgePosition::RIGHT);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OpenPledgeChange(EGsMultiLevelRankPledgePosition InPosition)
{
	TWeakObjectPtr<UGsUIPopupMultiLevelRankPledgeChange> popup =
		Cast<UGsUIPopupMultiLevelRankPledgeChange>(GUI()->OpenAndGetWidget(TEXT("PopupMultiLevelRankPledgeChange")));
	if (popup.IsValid())
	{
		popup->SetNextPledge(_groupIdMap[InPosition]);
	}
	Close();
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickLeftPreview()
{
	OpenPledgePreview(EGsMultiLevelRankPledgePosition::LEFT);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickMiddlePreview()
{
	OpenPledgePreview(EGsMultiLevelRankPledgePosition::MIDDLE);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OnClickRightPreview()
{
	OpenPledgePreview(EGsMultiLevelRankPledgePosition::RIGHT);
}

void UGsUIPopupMultiLevelRankPledgeSelect::OpenPledgePreview(EGsMultiLevelRankPledgePosition InPosition)
{
	TWeakObjectPtr<UGsUIPopupMultiLevelRankQuest> popup = Cast<UGsUIPopupMultiLevelRankQuest>(GUI()->OpenAndGetWidget(TEXT("PopupMultiLevelRankQuest")));
	if (popup.IsValid())
	{
		int32 pledgeGroupId = 1; // 현재 서약 그룹 ID 를 넣어야함

		const FGsMultiLevelRankSlotData* slotData = nullptr;
		if (FGsMultiLevelRankManager* rankManager = GSMultiLevelRank())
		{
			// 서약 다음 슬롯 정보를 먼저 보인다.
			MultiLevelRank rank = rankManager->GetPledgePrevSlot()->_ownerRank->_rankSchema->rank + 1;
			slotData = rankManager->GetRankSlot(rank, _groupIdMap[InPosition]);
		}

		popup->InvalidateQuest(slotData, pledgeGroupId, _groupIdMap[InPosition], false);
		OnClickClose();
	}
}

void UGsUIPopupMultiLevelRankPledgeSelect::SetImage(UImage* InImage, const FSoftObjectPath& InImagePath)
{
	TWeakObjectPtr<UImage> weakThis(InImage);
	FGsUIHelper::AsyncLoadSpriteFromPath(InImagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}


void UGsUIPopupMultiLevelRankPledgeSelect::OnInputCancel()
{
	OnClickClose();
}