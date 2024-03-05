// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildApproveJoin.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildApproveJoinEntry.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Guild/GsGuildData.h"


void UGsUIPopupGuildApproveJoin::BeginDestroy()
{
	if (_panelSlotHelper)
	{
		_panelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupGuildApproveJoin::OnRefreshEntry);
	}
	_panelSlotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupGuildApproveJoin::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelSlotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildApproveJoin::OnRefreshEntry);
	
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupGuildApproveJoin::OnClickClose);
}

void UGsUIPopupGuildApproveJoin::NativeConstruct()
{
	Super::NativeConstruct();
	
	RegisterMessages();

	InvalidateList();	
}

void UGsUIPopupGuildApproveJoin::NativeDestruct()
{
	DeregisterMessages();

	//_dataList 를 편집하거나 Empty 시키면 안됨
	_dataList = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupGuildApproveJoin::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildApproveJoin::RegisterMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT,
		this, &UGsUIPopupGuildApproveJoin::OnReceiveJoinApproveResult));
}

void UGsUIPopupGuildApproveJoin::DeregisterMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	for (MsgGuildHandle& handle : _msgGuildHandleList)
	{
		msgGuild.Remove(handle);
	}
	_msgGuildHandleList.Empty();
}

void UGsUIPopupGuildApproveJoin::InvalidateList()
{
	TArray<FGsGuildJoinWaitingUser> tempList = GGuild()->GetJoinWaitingUserList();
	_dataList = &tempList;
	_panelSlotHelper->RefreshAll(_dataList->Num());

	// 리스트가 비었을 때만 보여준다
	_panelEmptyList->SetVisibility((0 < _dataList->Num()) ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIPopupGuildApproveJoin::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList->IsValidIndex(InIndex))
	{
		return; 
	}
	
	if (UGsUIGuildApproveJoinEntry* entry = Cast<UGsUIGuildApproveJoinEntry>(InEntry))
	{
		entry->SetData(_dataList->GetData()[InIndex]);
	}
}

void UGsUIPopupGuildApproveJoin::OnClickClose()
{
	OnInputCancel();
}

void UGsUIPopupGuildApproveJoin::OnReceiveJoinApproveResult(const IGsMessageParam*)
{
	// (0:거절/1:승인인데, 리스트 전체갱신할거라 인자 필요없음)
	InvalidateList();
}
