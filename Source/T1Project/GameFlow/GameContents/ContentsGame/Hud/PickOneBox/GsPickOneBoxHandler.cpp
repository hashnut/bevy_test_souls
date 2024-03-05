// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameContents/ContentsGame/Hud/PickOneBox/GsPickOneBoxHandler.h"
#include "Management/GsMessageHolder.h"
#include "Net/GsNetSendServiceItem.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "UI/UIContent/Popup/SelectItemBox/GsUIPopupSelectItemBox.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Item/GsItemManager.h"


void FGsPickOneBoxHandler::Init()
{
	InitMsg();
}

void FGsPickOneBoxHandler::Close()
{
	RemoveMsg();
}

void FGsPickOneBoxHandler::OnReconnection()
{
}

void FGsPickOneBoxHandler::InitMsg()
{
	FGsMessageHolder* msg = GMessage();
	_itemActionMsgHandler.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::PICKONEBOX_OPEN,
		this, &FGsPickOneBoxHandler::OpenPopup));
	_itemActionMsgHandler.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::PICKONEBOX_SELECT,
		this, &FGsPickOneBoxHandler::ConfirmPickItem));

}

void FGsPickOneBoxHandler::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentItem, FDelegateHandle>& elemItem : _itemActionMsgHandler)
	{
		msg->GetItemContents().Remove(elemItem);
	}
	_itemActionMsgHandler.Empty();
}

void FGsPickOneBoxHandler::OpenPopup(const struct IGsMessageParam* InMessageParam)
{
	const FGsOpenPickOneBox* param = InMessageParam->Cast<const FGsOpenPickOneBox>();
	if (nullptr == param)
		return;

	if (0 >= param->_itemDBID || 0 >= param->_rewardID)
		return;

	// consumeDetail 에 EffectID 로 RewardData 가져옴
	const UGsTableRewardData* rewardTableData = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == rewardTableData)
	{
		return;
	}
	const FGsSchemaRewardData* rewardRow = nullptr;
	if (false == rewardTableData->FindRowById(param->_rewardID, rewardRow))
	{
		return;
	}

	if (RewardType::PICK_ONE != rewardRow->rewardType)
	{
		return;
	}

	if (0 >= rewardRow->rewardBoxList.Num())
	{
		return;
	}

	// 선택 상자인 경우, rewardBox 는 한개만 입력하기로함
	const FGsSchemaRewardBox* box = &rewardRow->rewardBoxList[0];
	if (nullptr == box)
		return;

	const FGsSchemaRewardBoxData* boxData = box->rewardBoxDataRow.GetRow();
	if (nullptr == boxData)
		return;

	if (0 >= boxData->itemBagList.Num())
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupPickOneItem")))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupPickOneItem"));
	if (widget.IsValid())
	{
		UGsUIPopupSelectItemBox* popup = Cast<UGsUIPopupSelectItemBox>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}

		popup->SetItemList(param->_itemDBID, boxData, param->_isSeveralUse);
	}
}

void FGsPickOneBoxHandler::ConfirmPickItem(const struct IGsMessageParam* InMessageParam)
{
	do 
	{
		const FGsConfirmPickOne* param = InMessageParam->Cast<const FGsConfirmPickOne>();
		if (nullptr == param)
			break;

		if (0 >= param->_itemDBID || 0 > param->_index)
			break;

		UGsItemManager* itemManager = GItem();
		if (nullptr == itemManager)
			break;

		if (false == itemManager->TryUseItem(param->_itemDBID, param->_amount, true))
			break;

		if (0 < param->_amount)
		{
			FGsNetSendServiceItem::SendSeveralPickOneItem(param->_itemDBID, param->_amount, param->_index);
		}
		else
		{
			FGsNetSendServiceItem::SendPickOneItem(param->_itemDBID, param->_index);
		}

	} while (0);
	
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("PopupPickOneItem"));
}