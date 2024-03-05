// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuickSlotHandler.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/GsMessageHolder.h"

#include "Net/GsNetSendServiceItem.h"
#include "T1Project/GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"


const float REQUEST_AUTO_DELAY_TIME = 0.2f;
const float RECEIVE_AUTO_PACKET_CHECK_TIME = 0.2f;

//----------------------------------------------------------------------------------------------------------------------
bool FGsQuickSlotHandler::FGsAutoDelayData::Update(float InDeltaTime)
{
	_remainSec -= InDeltaTime;
	if (0 >= _remainSec)
	{
		_remainSec = 0.f;

		if (UGsQuickSlotManager* slotMgr = GSQuickSlot())
		{
			if (const QuickSlotInfo* info = slotMgr->GetQuickSlotInfoBySlotID(_slotId))
			{
				// 아이템 정보가 같고 오토 상태가 다를때만 자동 사용 요청
				if (info->mItemDBId == _itemDBId &&
					info->mAutoUse != _bUseAuto)
				{
					FGsNetSendServiceItem::SendAutoUseStateQuickSlot(_slotId, _itemDBId, _bUseAuto);
				}
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------------------------------
FGsQuickSlotHandler::FGsQuickSlotHandler()
{
}

FGsQuickSlotHandler::~FGsQuickSlotHandler()
{
}

void FGsQuickSlotHandler::Init()
{
	RegisterMessage();
	_slotMode = QuickSlotMode::Normal;
}

void FGsQuickSlotHandler::Close()
{
	RemoveMsg();

	_reqAutoMap.Empty();
	_rcvAutoCheckMap.Empty();
}

void FGsQuickSlotHandler::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	//////////////////////////////// QuickSlot ///////////////////////////////////////////
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_REGISTER,
		this, &FGsQuickSlotHandler::RegisterQuickSlot));
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_SWAP,
		this, &FGsQuickSlotHandler::SwapQuickSlot));
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_REMOVE,
		this, &FGsQuickSlotHandler::RemoveQuickSlotIcon));
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_USE,
		this, &FGsQuickSlotHandler::QuickSlotUseItem));
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_AUTOSTATE,
		this, &FGsQuickSlotHandler::OnChangeQuickSlotAutoState));
	_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::QUICKSLOT_AUTOSTATE_RECEIVE,
		this, &FGsQuickSlotHandler::OnReceiveAutoState));

	//////////////////////////////// Item ///////////////////////////////////////////
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::ADDITEMLIST,
		this, &FGsQuickSlotHandler::AddItemListMessage));
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::UPDATEITEM,
		this, &FGsQuickSlotHandler::UpdateItemMessage));
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::REMOVEITEM,
		this, &FGsQuickSlotHandler::RemoveItemMessage));
}

void FGsQuickSlotHandler::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	for (TPair<MessageContentItem, FDelegateHandle>& elemQuickSlot : _listQuickSlotDelegate)
	{
		msg->GetItemContents().Remove(elemQuickSlot);
	}
	_listQuickSlotDelegate.Empty();

	for (TPair<MessageItem, FDelegateHandle>& elemItem : _listItemDelegate)
	{
		msg->GetItem().Remove(elemItem);
	}
	_listItemDelegate.Empty();
}

void FGsQuickSlotHandler::OnChagneQuickSlotState(const struct IGsMessageParam* InParamData)
{
	/*
	const FGsOnSelectInvenItemDbid* paramData = InParamData->Cast<const FGsOnSelectInvenItemDbid>();
	if (nullptr == paramData)
		return;

	GSQuickSlot()->SetInvenSelectionItem(paramData->_selectionDbid);
	_slotMode = 0 < paramData->_selectionDbid ? QuickSlotMode::Edit : QuickSlotMode::Normal;

	//FGsOnChangeQuickSlotMode param(_slotMode);
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_INVALIDATE_STATE, &param);
	*/
}

void FGsQuickSlotHandler::RegisterQuickSlot(const struct IGsMessageParam* InParamData)
{
	const FGsRegisterQuickSlot* param = InParamData->Cast<const FGsRegisterQuickSlot>();
	if (nullptr == param)
		return;

	RegisterQuickSlot(param->_invenItemDBID, param->_quickSlotIndex);
}

void FGsQuickSlotHandler::SwapQuickSlot(const struct IGsMessageParam* InParamData)
{
	const FGsSwapQuickSlot* param = InParamData->Cast<const FGsSwapQuickSlot>();
	if (nullptr == param)
		return;

	SwapQuickSlot(param->_quickSlotIndex1, param->_quickSlotIndex2);
}

void FGsQuickSlotHandler::RemoveQuickSlotIcon(const struct IGsMessageParam* InParamData)
{
	const FGsOnClickRemoveQuickSlotIcon* param = InParamData->Cast<const FGsOnClickRemoveQuickSlotIcon>();
	if (nullptr == param)
		return;

	OnClickRemoveSlotItem(param->_quickSlotIndex);
}

void FGsQuickSlotHandler::QuickSlotUseItem(const struct IGsMessageParam* InParamData)
{
	const FGsOnClickUseQuickSlotItem* param = InParamData->Cast<const FGsOnClickUseQuickSlotItem>();
	if (nullptr == param)
		return;

	OnClickUseItem(param->_quickSlotIndex);
}

void FGsQuickSlotHandler::OnChangeQuickSlotAutoState(const struct IGsMessageParam* InParamData)
{
	const FGsOnChangeQuickSlotAutoState* param = InParamData->Cast<const FGsOnChangeQuickSlotAutoState>();
	if (nullptr == param)
		return;

	OnChangeAutoState(param->_quickSlotIndex, param->_itemDBID, param->_autoState);
}

void FGsQuickSlotHandler::OnReceiveAutoState(const IGsMessageParam* InParamData)
{
	if (InParamData)
	{
		if (const FGsOnReceiveQuickSlotAutoState* param = InParamData->Cast<const FGsOnReceiveQuickSlotAutoState>())
		{
			// 패킷을 받았으므로 체크 데이터 있으면 제거
			for (int i = 0; i < param->_slotList.Num(); ++i)
			{
				_rcvAutoCheckMap.Remove(param->_slotList[i]);
			}
		}
	}
}

// 현재 퀵슬롯에 등록된 아이템 추가 처리
// 장비류는 갱신하지 않는다(장비류는 장비 옵션이 다르기 때문에)
void FGsQuickSlotHandler::AddItemListMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	if(0 >= paramData->_itemDbidList.Num())
		return;

	UGsQuickSlotManager* quickSlotMgr = GSQuickSlot();
	UGsItemManager* itemMgr = GItem();
	TMap<QuickSlotId, TPair<uint64, uint32>> groupDataMap;
	for (const ItemDBId iter : paramData->_itemDbidList)
	{
		TWeakPtr<FGsItem> itemData = itemMgr->FindItem(iter);
		if (false == itemData.IsValid())
			continue;

		if (false == itemData.Pin()->IsEquipment() && true == quickSlotMgr->IsExistSlotItemByItemTID(itemData.Pin()->GetTID()))
		{
			// 퀵슬롯에 등록은 되어있지만, 실제로 보유하지 않은 슬롯의 정보를 가져와야 함
			TArray<uint8> slotIDList;
			quickSlotMgr->FindAllNotOwnedSlotIdByItemTID(itemData.Pin()->GetTID(), slotIDList);

			// 기존에 등록은 되어있었지만, 실제 보유하지 않았던 아이템을 획득한 것이므로 
			// 서버에 새로 등록 패킷을 날림
			bool isExistNewItem = 0 < slotIDList.Num();
			if (true == isExistNewItem)
			{
				for (uint8 i = 0; i < slotIDList.Num(); ++i)
				{
					uint8 slotID = slotIDList[i];
					const QuickSlotInfo* slotInfo = quickSlotMgr->GetQuickSlotInfoBySlotID(slotID);
					if (nullptr == slotInfo)
						continue;

					groupDataMap.Emplace(slotID, TPair<uint64, uint32>(iter, itemData.Pin()->GetTID()));
#if WITH_EDITOR
					GSLOG(Warning, TEXT("[FGsQuickSlotHandler::AddItemMessage] - SlotID : %d"), slotID);
					GSLOG(Warning, TEXT("[FGsQuickSlotHandler::AddItemMessage] - iter - itemDBID : %llu"), iter);
					GSLOG(Warning, TEXT("[FGsQuickSlotHandler::AddItemMessage] - tid : %d"), itemData.Pin()->GetTID());
#endif
				}
			}
			else // 갱신처리만 필요
			{
				UpdateItemMessage(iter);
			}
		}
	}

	if (0 < groupDataMap.Num())
	{
		FGsNetSendServiceItem::SendQuickSlotRegistItemGroup(groupDataMap);
	}
}

// 현재 퀵슬롯에 등록된 아이템 Update 처리
void FGsQuickSlotHandler::UpdateItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (nullptr == paramData)
		return;

	UGsQuickSlotManager* quickSlotMgr = GSQuickSlot();
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(paramData->_itemDBID);
	if (true == itemData.IsValid() && true == quickSlotMgr->IsExistSlotItemByItemTID(itemData.Pin()->GetTID()))
	{
		TArray<QuickSlotId> slotIDList;
		quickSlotMgr->FindAllSlotIdByItemTID(itemData.Pin()->GetTID(), slotIDList);

		// 기존에 등록 및 보유하고 있는 아이템이므로 갱신처리
		quickSlotMgr->InvalidateSlotList(slotIDList);
	}
}

void FGsQuickSlotHandler::UpdateItemMessage(const ItemDBId InItemDbid)
{
	UGsQuickSlotManager* quickSlotMgr = GSQuickSlot();
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(InItemDbid);
	if (true == itemData.IsValid() && true == quickSlotMgr->IsExistSlotItemByItemTID(itemData.Pin()->GetTID()))
	{
		TArray<QuickSlotId> slotIDList;
		quickSlotMgr->FindAllSlotIdByItemTID(itemData.Pin()->GetTID(), slotIDList);

		// 기존에 등록 및 보유하고 있는 아이템이므로 갱신처리
		quickSlotMgr->InvalidateSlotList(slotIDList);
	}
}

void FGsQuickSlotHandler::RemoveItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (nullptr == paramData)
		return;

	// quickslot Manager 에서 해당 InItemDBID 가지고 있는 슬롯은 
	// 모두 딤드 처리 필요
	UGsQuickSlotManager* quickSlotMgr = GSQuickSlot();
	quickSlotMgr->OnRemoveInvenItem(paramData->_itemDBID);

	TArray<QuickSlotId> slotIDList;
	quickSlotMgr->ResetQuickSlotOwnedItemInfo(paramData->_itemDBID, paramData->_itemTID, slotIDList);
	if (0 < slotIDList.Num())
	{
		// 오토 체크용 데이터들 삭제
		for (const QuickSlotId& slotId : slotIDList)
		{
			RemoveAutoCheckData(slotId);
		}

		quickSlotMgr->InvalidateSlotList(slotIDList);
	}
}

// 인벤아이템을 퀵슬롯에 등록
void FGsQuickSlotHandler::RegisterQuickSlot(const uint64 InInvenItemDBID, const QuickSlotId InQuickSlotID)
{
	if (0 >= InInvenItemDBID || 0 >= InQuickSlotID)
		return;

	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	if (netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUICK_SLOT_SET)))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	// firstSlotID 에 _selectedInvenItemDBID 아이템 등록 패킷 전송
	const QuickSlotInfo* slotInfoA = GSQuickSlot()->GetQuickSlotInfoBySlotID(InQuickSlotID);
	if (nullptr == slotInfoA)
		return;

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(InInvenItemDBID);
	if (false == itemData.IsValid())
		return;

	//FGsResetInvenSelectedIcon invenParam;
	//GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::ResetSelectedIcon, invenParam);

	// 오토 체크용 데이터들 삭제
	RemoveAutoCheckData(InQuickSlotID);

	FGsNetSendServiceItem::SendItemRegistOrRemoveQuickSlot(InQuickSlotID, InInvenItemDBID, itemData.Pin()->GetTID());
}

void FGsQuickSlotHandler::SwapQuickSlot(const QuickSlotId InQuickSlotID1, const QuickSlotId InQuickSlotID2)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	if (0 < InQuickSlotID1 && 0 < InQuickSlotID2)
	{		// firstSlotID 와 secondSlotID 서로 Swap 패킷 전송
		const QuickSlotInfo* slotInfoA = slotMgr->GetQuickSlotInfoBySlotID(InQuickSlotID1);
		if (nullptr == slotInfoA)
			return;

		const QuickSlotInfo* slotInfoB = slotMgr->GetQuickSlotInfoBySlotID(InQuickSlotID2);
		if (nullptr == slotInfoB)
			return;

		bool isValidA = 0 < slotInfoA->mTemplateId;
		bool isValidB = 0 < slotInfoB->mTemplateId;

		// 두 슬롯 다 비어있다면 리셋 처리
		if (false == isValidA && false == isValidB)
			return;

		if (netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUICK_SLOT_SET)))
		{
			// TEXT: 잠시 후 다시 시도해주세요.
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			return;
		}

		// 오토 체크용 데이터들 삭제
		RemoveAutoCheckData(InQuickSlotID1);
		RemoveAutoCheckData(InQuickSlotID2);

		TPair<uint64, uint32> slotAData;
		TPair<uint64, uint32> slotBData;
		// A 슬롯 B 슬로 서로 교체함
		//slotAData.Key = _selectedSecondQuickSlotID;
		slotAData.Key = slotInfoB->mItemDBId;
		slotAData.Value = slotInfoB->mTemplateId;

		slotBData.Key = slotInfoA->mItemDBId;
		slotBData.Value = slotInfoA->mTemplateId;

#if WITH_EDITOR
		GSLOG(Warning, TEXT("slotAData slotID : %d"), InQuickSlotID1);
		GSLOG(Warning, TEXT("slotAData.Key : %llu"), slotAData.Key);
		GSLOG(Warning, TEXT("slotAData.Value : %d"), slotAData.Value);

		GSLOG(Warning, TEXT("slotBData slotID : %d"), InQuickSlotID2);
		GSLOG(Warning, TEXT("slotBData.Key : %llu"), slotBData.Key);
		GSLOG(Warning, TEXT("slotBData.Value : %d"), slotBData.Value);
#endif

		TMap<QuickSlotId, TPair<uint64, uint32>> swapDataMap;
		swapDataMap.Emplace(InQuickSlotID1, slotAData);
		swapDataMap.Emplace(InQuickSlotID2, slotBData);

		FGsNetSendServiceItem::SendQuickSlotRegistItemGroup(swapDataMap);
	}
}

void FGsQuickSlotHandler::RemoveAutoCheckData(QuickSlotId InQuickSlotId)
{
	// 슬롯 제거나 스왑 상황에 기존 체크용 데이터 삭제 위함
	_reqAutoMap.Remove(InQuickSlotId);
	_rcvAutoCheckMap.Remove(InQuickSlotId);
}

void FGsQuickSlotHandler::Enter()
{
}

void FGsQuickSlotHandler::OnReconection()
{
	_reqAutoMap.Empty();
	_rcvAutoCheckMap.Empty();

	GSQuickSlot()->InvalidateAll();
}

void FGsQuickSlotHandler::OnClickRemoveSlotItem(const QuickSlotId InSlotID)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	if (netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUICK_SLOT_SET)))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	const QuickSlotInfo* slotInfo = slotMgr->GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == slotInfo)
		return;

	// 오토 체크용 데이터들 삭제
	RemoveAutoCheckData(InSlotID);

	//GSLOG(Warning, TEXT("[Call FGsQuickSlotHandler::OnClickRemoveSlotItem]"));

	// 해제요청이라 dbid 및 tid 강제로 0으로 보냄
	FGsNetSendServiceItem::SendItemRegistOrRemoveQuickSlot(InSlotID, 0, 0);
	slotMgr->ResetSelectionInfo(InSlotID);
}

void FGsQuickSlotHandler::OnClickUseItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* slotInfo = GSQuickSlot()->GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr != slotInfo && 0 < slotInfo->mItemDBId)
	{
		UGsItemManager* itemMgr = GItem();
		TWeakPtr<FGsItem> itemData = itemMgr->FindItem(slotInfo->mItemDBId);
		if (true == itemData.IsValid() || false == itemData.Pin()->IsIngredient())
		{
			// 소환 상품이면 retry 정보 여기서 세팅한다
			switch (itemData.Pin()->GetSubCategory())
			{
			case ItemCategorySub::SUMMON_FAIRY:
			case ItemCategorySub::SUMMON_COSTUME:
			{
				// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
				// 1, 2 연속해서 누르면
				// 1번(페어리) 성공, 2번(코스튬) 실패 
				// 하지만 다시뽑기 데이터는 2번이 저장되어서
				// 두번째는 코스튬뽑게됨
				// 유효하지 않으면 저장 안하게 처리
				//	https://jira.com2us.com/jira/browse/C2URWQ-5204				
				if(FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler())
				{
					summonHandler->SetRetryDataByItemInfo(itemData.Pin().Get());
				}				
				break;
			}
			case ItemCategorySub::REWARD_BOX:
			{
				if (false == itemMgr->TryUseItem(itemData.Pin().Get(), 0, true))
					return;

				bool isPickOneType = FGsItemHelper::IsPickOneTypeRewardItem(itemData.Pin().Get());
				if (isPickOneType)
				{
					// 선택형 보상 상자는 아이템을 사용하지 않고 아이템 선택 팝업을 출력한다.
						// 수량이 하나뿐이면 단일 선택 팝업 출력
					if (itemData.Pin()->GetAmount() == 1)
					{
						int32 rewardId = itemData.Pin()->GetPickOneTypeRewardID();
						struct FGsOpenPickOneBox paramData(itemData.Pin()->GetDBID(), rewardId, false);
						GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);
					}
					// 수량이 여러개일땐 일괄 선택 팝업 출력
					else if (itemData.Pin()->GetAmount() > 1)
					{
						int32 rewardId = itemData.Pin()->GetPickOneTypeRewardID();
						struct FGsOpenPickOneBox paramData(itemData.Pin()->GetDBID(), rewardId, true);
						GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);
					}
					else
					{
						// 아이템 수량 없음
					}
					return;
				}
				else
				{
					// 수량이 하나뿐이면 바로 사용한다.
					if (itemData.Pin()->GetAmount() == 1)
					{
						// 별도 처리 없음.
					}
					// 수량이 여러개일땐 계산기 팝업 출력
					else if (itemData.Pin()->GetAmount() > 1)
					{
						TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
						if (widget.IsValid())
						{
							if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(widget.Get()))
							{
								uint64 itemDBID = itemData.Pin()->GetDBID();
								FText findText;
								FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_UseAll"), findText);
								popup->SetDimmed(true);
								popup->SetTitleText(findText);
								popup->SetDefaultCount(itemData.Pin()->GetAmount());
								popup->SetMinimumCount(1);
								popup->SetMaximumCount(itemData.Pin()->GetAmount());
								popup->InvalidateNumText();
								popup->SetButtonCallBack([&, this, popup, itemDBID](bool state)
									{
										if (state)
										{
											if (0 >= itemDBID || 0 >= popup->GetCurrentNum())
												return;

											GItem()->TryUseItem(itemDBID, popup->GetCurrentNum());
										}
									});
							}
						}
						return;
					}
				}
				break;
			}
			}
			itemMgr->TryUseItem(itemData.Pin().Get());
		}
	}
}

void FGsQuickSlotHandler::OnChangeAutoState(const QuickSlotId InSlotID, const uint64 InItemDBID, const bool InFlag)
{
	if (INVALID_QUICK_SLOT_ID >= InSlotID)
	{
		return;
	}

	// REQUEST_AUTO_DELAY_TIME 초 뒤에 오토 상태가 데이터 다르면 서버에 오토 요청(SendAutoUseStateQuickSlot)
	if (FGsAutoDelayData* data = _reqAutoMap.Find(InSlotID))
	{
		data->_itemDBId = InItemDBID;
		data->_bUseAuto = InFlag;
		data->_remainSec = REQUEST_AUTO_DELAY_TIME;
	}
	else
	{
		FGsAutoDelayData delayData;
		delayData._slotId = InSlotID;
		delayData._itemDBId = InItemDBID;
		delayData._bUseAuto = InFlag;
		delayData._remainSec = REQUEST_AUTO_DELAY_TIME;

		_reqAutoMap.Emplace(InSlotID, delayData);
	}
}

void FGsQuickSlotHandler::Update(float InDeltaTime)
{
	// U1: https://jira.com2us.com/jira/browse/C2URWQ-6177
	// - 같은 슬롯을 연속으로 보내지 못하게 REQUEST_AUTO_DELAY_TIME 초 체크로직 추가 
	// - 서버에서 같은 패킷이 연속으로 들어올 경우 어뷰징 방지를 위해 리턴시키기 때문에, 패킷 응답을 못받는 상황 발생할 수 있음.
	//   따라서 RECEIVE_AUTO_PACKET_CHECK_TIME초를 기다리며 응답받으면 제거, 받지 않으면 강제 갱신

	// 일정 시간 
	if (0 < _rcvAutoCheckMap.Num())
	{
		TArray<QuickSlotId> removeList;

		for (TPair<QuickSlotId, float>& iter : _rcvAutoCheckMap)
		{
			iter.Value -= InDeltaTime;

			// 패킷 못받은채로 시간 경과
			if (0 >= iter.Value)
			{
				iter.Value = 0.f;
				removeList.Emplace(iter.Key);
			}
		}

		if (0 < removeList.Num())
		{
			// 갱신요청
			if (UGsQuickSlotManager* slotMgr = GSQuickSlot())
			{
				slotMgr->InvalidateAutoState(removeList);
			}

			// 제거
			for (QuickSlotId& removeId : removeList)
			{
				_rcvAutoCheckMap.Remove(removeId);
			}
		}
	}

	// 시간 체크하여 서버에 요청 보내기
	if (0 < _reqAutoMap.Num())
	{
		TArray<QuickSlotId> removeList;

		for (TPair<QuickSlotId, FGsAutoDelayData>& iter : _reqAutoMap)
		{
			if (iter.Value.Update(InDeltaTime))
			{
				removeList.Emplace(iter.Key);
			}
		}

		for (QuickSlotId& removeId : removeList)
		{
			// 응답 체크할 목록에 등록
			if (_rcvAutoCheckMap.Contains(removeId))
			{
				_rcvAutoCheckMap[removeId] = RECEIVE_AUTO_PACKET_CHECK_TIME;
			}
			else
			{
				_rcvAutoCheckMap.Emplace(removeId, RECEIVE_AUTO_PACKET_CHECK_TIME);
			}

			// 요청 목록에서는 제거
			_reqAutoMap.Remove(removeId);
		}
	}
}
