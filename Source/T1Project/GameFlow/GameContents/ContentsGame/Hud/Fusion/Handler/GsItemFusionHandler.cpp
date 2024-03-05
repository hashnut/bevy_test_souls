
#include "GsItemFusionHandler.h"
#include "Unlock/GsUnlockDefine.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "../Item/GsItemStat.h"
#include "../Item/GsItemManager.h"
#include "../Item/GsItemEquipment.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageItem.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsUIMessageParam.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGame/GsInventoryManager.h"
#include "../Management/ScopeGame/GsEventProgressManager.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupCommonItemAction.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "DataSchema/Item/GsSchemaItemGroup.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "DataSchema/Item/GsSchemaItemEquipFuse.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Net/GsNetSendServiceItem.h"
#include "../T1Project.h"
#include "../Currency/GsCurrencyHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"




FGsItemFusionHandler::~FGsItemFusionHandler()
{
	UnBindStaticMessage();
}

void FGsItemFusionHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	IGsEventProgressInterface::StartEvent(In_startType, In_preType);

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupInventory")))
		{
			// 비교창을 통해 들어오지 않은 경우에 저장
			// 순서에 유의 인벤토리에서 열린 상세창에서만 저장함
			if (UGsInventoryManager* inventoryMgr = GSInventory())
			{
				if (inventoryMgr->CheckIsHUDState())
				{
					inventoryMgr->SetRestoreItemDBId(_srcItemDBId);
				}
			}

			uiManager->CloseByKeyName(TEXT("PopupInventory"));
		}
	}	

	// 2023/9/27 PKT - Dynamic Message Bind
	BindDynamicMessage();

	// 2023/10/11 PKT - message holder 초기화
	_messageHolder = false;

	// 2023/9/27 PKT - Popup Open
	TWeakObjectPtr<UGsUIPopupItemFusion> popup = GetPopup(true);
	if (popup.IsValid())
	{
		popup->SetData(this);
		// 2023/10/4 PKT - Set Data 이후 곧바로 화면 갱신을 위해 호출
		popup->Invalidate();
	}
}

void FGsItemFusionHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	IGsEventProgressInterface::FinishEvent(In_finishType, In_nextType);
	
	Close();
}

void FGsItemFusionHandler::ClearData()
{
	_srcItemDBId = INVALID_ITEM_DB_ID;
	_destItemDBId = INVALID_ITEM_DB_ID;
	_refainItemDBId = INVALID_ITEM_DB_ID;
}

TWeakObjectPtr<UGsUIPopupItemFusion> FGsItemFusionHandler::GetPopup(bool InIsOpen)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget;
	if (InIsOpen)
	{
		widget = GUI()->OpenAndGetWidget(TEXT("PopupItemFusion"));
	}
	else
	{
		widget = GUI()->GetWidgetByKey(TEXT("PopupItemFusion"));
	}

	return widget.IsValid() ? Cast<UGsUIPopupItemFusion>(widget.Get()) : nullptr;
}

void FGsItemFusionHandler::InvalidateUI()
{
	if (_messageHolder)
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupItemFusion> popup = GetPopup();
	if (popup.IsValid())
	{
		popup->Invalidate();
	}
}

void FGsItemFusionHandler::Init()
{
	ClearData();	
	// 2023/9/25 PKT - DynamicMessage는 UI Open 시에 Bind
	UnBindDynamicMessage();

	BindStaticMessage();
}

void FGsItemFusionHandler::Close() 
{
	ClearData();

	_messageHolder = false;

	UnBindDynamicMessage();

	TWeakObjectPtr<UGsUIPopupItemFusion> popup = GetPopup();
	if (popup.IsValid())
	{
		popup->Close();

		if (UGsInventoryManager* inventoryMgr = GSInventory())
		{
			// 내부에서 CheckIsHUDState 검사 함
			inventoryMgr->RestoreInventoryAndDetailPopup();
		}
	}

	if (FGsEventProgressManager* eventMgr = GSEventProgress())
	{	// 이벤트 진행 순서에 전송
		eventMgr->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_ITEMFUSION);
	}
}

void FGsItemFusionHandler::OnReconection() 
{
	ClearData();

	UnBindDynamicMessage();
}

void FGsItemFusionHandler::BindStaticMessage()
{
	UnBindStaticMessage();

	if (FGsMessageHolder* msgHolder = GMessage())
	{
		_uiStaticDelegate = msgHolder->GetItemFusion().AddRaw(MessageContentsFusion::UI_OPEN, this, &FGsItemFusionHandler::OnOpenItemFusionProc);
	}
}

void FGsItemFusionHandler::UnBindStaticMessage()
{
	if (FGsMessageHolder* msgHolder = GMessage())
	{
		if (_uiStaticDelegate.Value.IsValid())
		{
			msgHolder->GetItemFusion().Remove(_uiStaticDelegate);
		}		
	}
}

void FGsItemFusionHandler::BindDynamicMessage()
{
	// 2023/9/25 PKT - 중복 호출 등 사고를 미연에 방지 하고자.. Unbind 먼저 호출.
	UnBindDynamicMessage();
	if (FGsMessageHolder* msgHolder = GMessage())
	{
		_listItemDynamicDelegates.Emplace(msgHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST,	this,	&FGsItemFusionHandler::OnAddItemList));
		_listItemDynamicDelegates.Emplace(msgHolder->GetItem().AddRaw(MessageItem::UPDATEITEM,	this,	&FGsItemFusionHandler::OnUpdateItem));
		_listItemDynamicDelegates.Emplace(msgHolder->GetItem().AddRaw(MessageItem::REMOVEITEM,	this,	&FGsItemFusionHandler::OnRemoveItem));
		_listItemDynamicDelegates.Emplace(msgHolder->GetItem().AddRaw(MessageItem::REMOVEITEMS,	this,	&FGsItemFusionHandler::OnRemoveItemList));
		_listItemDynamicDelegates.Emplace(msgHolder->GetItem().AddRaw(MessageItem::LOCKITEM,	this,	&FGsItemFusionHandler::OnLocalPlayerUseItemLock));

		_listGameObjectDynamicDelegates.Emplace(msgHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE,	this, &FGsItemFusionHandler::OnLocalPlayerStateChange));
		_listGameObjectDynamicDelegates.Emplace(msgHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_DEAD,this, &FGsItemFusionHandler::OnLocalPlayerStateChange));

		_uiDynamicDelegates.Emplace(msgHolder->GetItemFusion().AddRaw(MessageContentsFusion::RECV_FUSTION_RESULT, this, &FGsItemFusionHandler::OnTryItemFusionResult));
	}	
}

void FGsItemFusionHandler::UnBindDynamicMessage()
{
	if (FGsMessageHolder* msgHolder = GMessage())
	{
		for (auto& message : _uiDynamicDelegates)
		{
			msgHolder->GetItemFusion().Remove(message);
		}
		_uiDynamicDelegates.Empty();

		for (auto& message : _listGameObjectDynamicDelegates)
		{
			msgHolder->GetGameObject().Remove(message);
		}
		_listGameObjectDynamicDelegates.Empty();

		for (auto& message : _listItemDynamicDelegates)
		{
			msgHolder->GetItem().Remove(message);
		}
		_listItemDynamicDelegates.Empty();
	}
}

const FGsSchemaItemEquipFuse* FGsItemFusionHandler::GetSchemaItemEquipFuse(const ItemId InItemId) const
{
	const FGsSchemaItemEquipFuse* outData = nullptr;

	do 
	{
		const FGsSchemaItemEquipDetail* schemaItemEquipData = UGsItemManager::GetEquipItemTableDataByTID(InItemId);
		if (nullptr == schemaItemEquipData)
		{
			break;
		}

		const FGsSchemaItemEquipFuse* schemaItemEquipFuse = schemaItemEquipData->itemFusionData.GetRow();
		if (nullptr == schemaItemEquipFuse)
		{
			break;
		}

		outData = schemaItemEquipFuse;

		break;
	} while (0);

	return outData;
}

const FGsSchemaItemEquipFuse* FGsItemFusionHandler::GetSchemaItemEquipFuse(const TWeakPtr<FGsItem> InItem) const
{
	return InItem.IsValid() ? GetSchemaItemEquipFuse(InItem.Pin().Get()->GetTID()) : nullptr;
}

const FGsSchemaItemEquipFuse* FGsItemFusionHandler::GetSchemaItemEquipFuse(const ItemDBId InItemDBId) const
{
	return GetSchemaItemEquipFuse(GItem()->FindItem(_srcItemDBId, ItemType::EQUIP));
}

TPair<FGsItemFusionHandler::ECheckValid, int32> FGsItemFusionHandler::IsTargetItemSuitable(const TWeakPtr<FGsItem> InItem) const
{
	ECheckValid outReason = ECheckValid::Valid;
	int32 outParam = 0;

#define SET_REASON_PARAM_BREAK(Reason, Param)	\
	outReason = Reason;							\
	outParam = Param;							\
	break

	do 
	{
		if (false == InItem.IsValid())
		{
			SET_REASON_PARAM_BREAK(ECheckValid::NonFusionInvalid, 0);
		}

		if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_FUSITION, false))
		{
			SET_REASON_PARAM_BREAK(ECheckValid::ContentsUnLockInvalid, 0);
		}

		const FGsItem* targetData = InItem.Pin().Get();

		// 2023/9/25 PKT - Inventory에 실제 존재 하는 아이템 인가?
		if (false == GItem()->FindItem(targetData->GetDBID(), ItemType::EQUIP).IsValid())
		{
			SET_REASON_PARAM_BREAK(ECheckValid::NonFusionInvalid, 0);
		}

		if (targetData->IsDepotItem())
		{	// 2023/9/27 PKT - 창고 아이템 안됨.
			SET_REASON_PARAM_BREAK(ECheckValid::NonFusionInvalid, 0);
		}

		// 2023/9/27 PKT - 합성 Data가 없으면 합성 할 수 있는 아이템이 아니다.
		const FGsSchemaItemEquipFuse* schemaItemEquipFuse = GetSchemaItemEquipFuse(targetData->GetTID());
		if (nullptr == schemaItemEquipFuse)
		{
			SET_REASON_PARAM_BREAK(ECheckValid::NonFusionInvalid, 0);
		}

		// 2023/9/27 PKT - Item Data
		const FGsSchemaItemCommon* schemaItemData = targetData->GetTableData();
		if (nullptr == schemaItemData)
		{
			SET_REASON_PARAM_BREAK(ECheckValid::NonFusionInvalid, 0);
		}

		if (schemaItemEquipFuse->fuseMinEnchantLevel > targetData->GetLevel())
		{	// 2023/9/27 PKT - 해당 아이템의 최소 합성 레벨 비교
			SET_REASON_PARAM_BREAK(ECheckValid::LevelInvalid, schemaItemEquipFuse->fuseMinEnchantLevel);
		}

		//if (targetData->IsEquipped())
		//{	// 2023/9/27 PKT - 착용한 아이템은 안됨
		//	SET_REASON_PARAM_BREAK(ECheckValid::EquipInvalid, 0);
		//}

		//if (INVALID_PRESET_VALUE != targetData->GetEquipPresetValue())
		//{	// 2023/9/27 PKT - 프리셋 안됨
		//	SET_REASON_PARAM_BREAK(ECheckValid::PresetInvalid, 0);
		//}

		if (targetData->GetLock())
		{	// 2023/9/27 PKT - 잠긴 것 안됨
			SET_REASON_PARAM_BREAK(ECheckValid::LockInvalid, 0);
		}

	} while (0);

#ifdef SET_REASON_PARAM_BREAK
	#undef SET_REASON_PARAM_BREAK
#endif

	return TPair<ECheckValid, int32>(outReason, outParam);
}

FGsItemFusionHandler::ECheckTry FGsItemFusionHandler::IsCheckTryFusion() const
{
	ECheckTry outReason = ECheckTry::Valid;

#define SET_REASON_BREAK(Reason)		\
	outReason = Reason;					\
	break

	do 
	{
		if (INVALID_ITEM_DB_ID == _srcItemDBId || INVALID_ITEM_DB_ID == _destItemDBId)
		{
			SET_REASON_BREAK(ECheckTry::InvalidRegist);
		}

		// 2023/10/11 PKT - Dest 아이템을 선택 하였을 경우 Src와 Dest 둘 중 하나에만 재련이 있을때에만 기본 선택 한다.
		bool isAttachSrcRefin = (0 < GetItemRefineStatSet(_srcItemDBId).Num());
		bool isAttachDestRefin = (0 < GetItemRefineStatSet(_destItemDBId).Num());
		if (isAttachDestRefin || isAttachDestRefin)
		{
			if (INVALID_ITEM_DB_ID == _refainItemDBId)
			{
				SET_REASON_BREAK(ECheckTry::InvalidRefine);
			}
		}

		if (GetCost().mCurrency > FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD))
		{
			SET_REASON_BREAK(ECheckTry::invalidCurrency);
		}

		SET_REASON_BREAK(ECheckTry::Valid);
		
	} while (0);
	

	return outReason;
}

const TArray<class FGsItemStat*> FGsItemFusionHandler::GetItemRefineStatSet(ItemDBId InItemDBId) const
{
	TArray<class FGsItemStat*> outList;
	outList.Empty(0);

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(InItemDBId, ItemType::EQUIP);
	if (false == itemData.IsValid())
	{
		return outList;
	}

	if (itemData.Pin().Get() && itemData.Pin().Get()->IsEquipment())
	{
		FGsItemEquipment* itemEquipmentData = StaticCast<FGsItemEquipment*>(itemData.Pin().Get());
		if (itemEquipmentData)
		{
			itemEquipmentData->GetAllRefineOptionStatList(outList);
		}
	}

	return outList;
}

CurrencyPair FGsItemFusionHandler::GetCost() const
{
	// 2023/10/4 PKT - 비용은 골드로 고정
	CurrencyPair outCurrencyInfo = CurrencyPair(CurrencyType::GOLD, 0);
	
	do 
	{
		if (INVALID_ITEM_DB_ID >= _srcItemDBId)
		{
			break;
		}

		const FGsSchemaItemEquipFuse* schemaItemEquipFuse = GetSchemaItemEquipFuse(_srcItemDBId);
		if (nullptr == schemaItemEquipFuse)
		{
			break;
		}

		outCurrencyInfo = CurrencyPair(CurrencyType::GOLD, schemaItemEquipFuse->fuseCost);
		
		break;
	} while (0);

	return outCurrencyInfo;
}

bool FGsItemFusionHandler::IsSuitableForSrcItem(const TWeakPtr<FGsItem> InItem, bool InIsPrintMessage /* = false */) const
{
	bool outResult = false;

	TPair<ECheckValid,int32> reason = IsTargetItemSuitable(InItem);

	if (ECheckValid::Valid != reason.Key && InIsPrintMessage)
	{
		FText textMsg;
		switch (reason.Key)
		{
			case ECheckValid::NonFusionInvalid:
			{	// 2023/9/27 PKT - 합성이 불가능한 아이템 입니다.
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_Cannot"), textMsg);
			}	break;
			case ECheckValid::LevelInvalid:
			{	// 2023/9/27 PKT - {0}강화 단계 부터 합성이 가능합니다.
				FText format;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_NotEnough"), format);
				textMsg = FText::FromString(FString::Format(*format.ToString(), {reason.Value}));
			}	break;
			case ECheckValid::EquipInvalid:
			{	// 2023/9/27 PKT - 장착 상태에선 합성이 불가능 합니다.
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_Equiped"), textMsg);
			}	break;
			case ECheckValid::PresetInvalid:
			{	// 2023/9/27 PKT - 프리셋에 등록되어 있어 합성이 불가능 합니다.
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_Preset"), textMsg);
			}	break;
			case ECheckValid::LockInvalid:
			{	// 2023/9/27 PKT - 잠금 상태에선 합성이 불가능 합니다.
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_Locked"), textMsg);
			}	break;
			case ECheckValid::ContentsUnLockInvalid:
			{
				GSUnlock()->GetUnLockMessage(EGsUnlockContentsType::ITEM_FUSITION, textMsg);
			}	break;
		}

		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}

	return (ECheckValid::Valid == reason.Key);
}

bool FGsItemFusionHandler::IsSuitableForSrcItem(const ItemDBId InItemDBId, bool InIsPrintMessage /* = false */) const
{
	return (nullptr != GItem() ) ? IsSuitableForSrcItem(GItem()->FindItem(InItemDBId), InIsPrintMessage) : false;
}

bool FGsItemFusionHandler::IsSuitableForDestItem(const TWeakPtr<FGsItem> InItem) const
{
	bool outResult = false;

	do
	{
		/**
		 * 재료 아이템의 공통 규칙
		 */
		if (INVALID_ITEM_DB_ID >= _srcItemDBId)
		{	// 2023/9/25 PKT - 현재 Src Item에 아무것도 넣지 않았다면 어떤 Dest Item도 유효할 수 없다.
			break;
		}

		TPair<ECheckValid, int32> reason = IsTargetItemSuitable(InItem);
		if (ECheckValid::Valid != IsTargetItemSuitable(InItem).Key)
		{	// 2023/9/25 PKT - 합성의 공통 규칙 적용
			break;
		}

		const FGsItem* destItemData = InItem.Pin().Get();

		// 2023/9/25 PKT - Dest 같은 경우 Src의 상태에 따라 가능 여부가 갈림.
		TWeakPtr<FGsItem> srcFindItemData = GItem()->FindItem(_srcItemDBId, ItemType::EQUIP);
		if (false == srcFindItemData.IsValid())
		{
			break;
		}

		const FGsItem* srcItemData = srcFindItemData.Pin().Get();

		if (srcItemData->GetDBID() == destItemData->GetDBID())
		{	// 2023/10/5 PKT - 같은 Item
			break;
		}

		if (srcItemData->GetSubCategory() != destItemData->GetSubCategory())
		{	// 2023/10/4 PKT - 같은 서브 카테고리를 갖고 있어야 합성 가능.
			break;
		}

		/**
		 * 재료 아이템의 서브 카테고리 별 규칙( 참 / 그외 아이템 )
		 */
		if (ItemCategorySub::CHARM == srcItemData->GetSubCategory())
		{
			const FGsSchemaItemEquipDetail* schemaItemEquipData = UGsItemManager::GetEquipItemTableDataByTID(srcItemData->GetTID());
			if (nullptr == schemaItemEquipData)
			{
				break;
			}

			const FGsSchemaItemEquipDetail* schemaDestItemEquipData = UGsItemManager::GetEquipItemTableDataByTID(destItemData->GetTID());
			if (nullptr == schemaDestItemEquipData || schemaItemEquipData->equipGroupId != schemaDestItemEquipData->equipGroupId)
			{	// 2023/10/4 PKT - '참'은 같은 장착 ID를 소유한 것 끼리 가능 함.
				break;
			}

			// 2023/10/4 PKT - 참은 같은 ItemTId끼리는 합성 불가.
			if (srcItemData->GetTID() == destItemData->GetTID())
			{
				break;
			}

			// 2023/10/4 PKT - 그룹 아이디도 검사.(귀속 / 비귀속)
			const TSet<ItemId> groupItemSet = UGsItemManager::GetAllItemIdsTheSameGroup(srcItemData->GetItemGroupId());
			if (nullptr != groupItemSet.Find(destItemData->GetTID()))
			{
				break;
			}
		}
		else
		{
			// 2023/10/4 PKT - 장비류 아이템은 ItemTId끼리는 합성.
			if (srcItemData->GetTID() != destItemData->GetTID())
			{
				break;
			}

			// 2023/10/5 PKT - 장비류 아이템은 Enchant Level도 같아야 한다.
			if (srcItemData->GetLevel() != destItemData->GetLevel())
			{
				break;
			}
		}

		// 2023/9/25 PKT - Enable
		outResult = true;

		break;
	} while (0);

	return outResult;
}

const FGsItem FGsItemFusionHandler::PreviewFusionResult() const
{
	FGsItem outItem;
	int32 outLevel = 0;
	do 
	{
		TWeakPtr<FGsItem> srcItemData = GItem()->FindItem(_srcItemDBId, ItemType::EQUIP);
		TWeakPtr<FGsItem> destItemData = GItem()->FindItem(_destItemDBId, ItemType::EQUIP);
		if (false == srcItemData.IsValid() || false == destItemData.IsValid())
		{
			break;
		}

		int32 highEnchantLevel = FMath::Max(srcItemData.Pin().Get()->GetLevel(), destItemData.Pin().Get()->GetLevel());

		if (ItemCategorySub::CHARM == srcItemData.Pin().Get()->GetSubCategory())
		{
			const FGsSchemaItemExpiredTime* schemaSrcItemTimeData = srcItemData.Pin().Get()->GetExpriedTimeData();
			const FGsSchemaItemExpiredTime* schemaDestItemTimeData = destItemData.Pin().Get()->GetExpriedTimeData();

			/**
			* 2024/2/6 PKT - 참 합성에 대한 기획 수정
			* https://jira.com2us.com/jira/browse/CHR-26013
			*/
			if (nullptr == schemaSrcItemTimeData || nullptr == schemaDestItemTimeData)
			{	// 2024/2/6 PKT - Time Data가 하나라도 없는 경우
				if (nullptr != schemaSrcItemTimeData)
				{
					outItem.UpdateTableData(destItemData.Pin().Get()->GetTID());
				}
				else if (nullptr != schemaDestItemTimeData)
				{
					outItem.UpdateTableData(srcItemData.Pin().Get()->GetTID());
				}
				else
				{
					// 2024/2/6 PKT - 둘다 ExpiredTime 데이터가 없다면 등급 순.. 
					if (srcItemData.Pin().Get()->GetGradeType() > destItemData.Pin().Get()->GetGradeType())
					{
						outItem.UpdateTableData(srcItemData.Pin().Get()->GetTID());
					}
					else if (srcItemData.Pin().Get()->GetGradeType() < destItemData.Pin().Get()->GetGradeType())
					{
						outItem.UpdateTableData(destItemData.Pin().Get()->GetTID());
					}
				}

				outItem.SetLevel(highEnchantLevel);

				break;
			}

			FDateTime srcDateTime;
			FDateTime destDateTime;
			if (false == FDateTime::Parse(schemaSrcItemTimeData->endTime, srcDateTime)
				|| false == FDateTime::Parse(schemaDestItemTimeData->endTime, destDateTime))
			{	// 2023/10/5 PKT - '참'은 무조건 기간이 정해져 있어야 한다.
				break;
			}

			if (srcDateTime.GetTicks() > destDateTime.GetTicks())
			{	// 2023/10/5 PKT - Time값이 더 크다면 더 늦게 종료 되는것.
				outItem.UpdateTableData(srcItemData.Pin().Get()->GetTID());
			}
			else if (srcDateTime.GetTicks() < destDateTime.GetTicks())
			{
				outItem.UpdateTableData(destItemData.Pin().Get()->GetTID());
			}
			else
			{
				// 2023/10/5 PKT - Time이 같다면 등급으로..
				if (srcItemData.Pin().Get()->GetGradeType() > destItemData.Pin().Get()->GetGradeType())
				{
					outItem.UpdateTableData(srcItemData.Pin().Get()->GetTID());
				}
				else if (srcItemData.Pin().Get()->GetGradeType() < destItemData.Pin().Get()->GetGradeType())
				{
					outItem.UpdateTableData(destItemData.Pin().Get()->GetTID());
				}
			}
			
			outItem.SetLevel(highEnchantLevel);
		}
		else
		{
			const FGsSchemaItemEquipFuse* schemaItemEquipFuse = GetSchemaItemEquipFuse(srcItemData);
			if (schemaItemEquipFuse)
			{
				const FGsSchemaItemCommon* schemaItemData = schemaItemEquipFuse->fuseResultItemId.GetRow();
				if (schemaItemData)
				{
					outItem.UpdateTableData(schemaItemData->id);
					outItem.SetLevel(highEnchantLevel);
				}
			}
		}

	} while (0);
	
	return outItem;
}

bool FGsItemFusionHandler::IsSuitableForDestItem(const ItemDBId InItemDBId) const
{
	return (nullptr != GItem()) ? IsSuitableForDestItem(GItem()->FindItem(InItemDBId)) : false;
}

void FGsItemFusionHandler::OnLocalPlayerStateChange(const IGsMessageParam* InParam)
{
	if (GSGameObject()->IsLocalPlayerDead())
	{	// 2023/9/25 PKT - Local Player 가 사망했다면 즉시 하던 일을 멈춘다.
		Close();
	}
}

void FGsItemFusionHandler::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* param = StaticCast<const FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}
}

void FGsItemFusionHandler::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	//인벤토리 갱신 / 합성창 갱신
	if (nullptr == Param || INVALID_ITEM_DB_ID == Param->_itemDBID)
	{
		return;
	}

	// 2023/10/11 PKT - 변경된 아이템이 Src에 등록되어 있는 아이템이라면..
	if (_srcItemDBId == Param->_itemDBID)
	{	// 2023/10/11 PKT - 강화가능 상태가 인지를 체크
		if (false == IsSuitableForSrcItem(_srcItemDBId))
		{	// 2023/10/11 PKT - Src가 가능 상태가 아니라면 모두 초기화
			ClearData();
			// 2023/10/11 PKT - 갱신..
			InvalidateUI();
		}
		else if (false == IsSuitableForDestItem(_destItemDBId))
		{	
			// 2023/10/11 PKT - Src 변경에 따라 Dest가 계속해서 재료 아이템으로 사용 될 수 없다면 Dest 아이템 초기화
			_destItemDBId = INVALID_ITEM_DB_ID;
			
			_refainItemDBId = INVALID_ITEM_DB_ID;

			InvalidateUI();
		}
	}
	else if (_destItemDBId == Param->_itemDBID)
	{	// 2023/10/11 PKT - Dest 아이템이 변경되어 합성 아이템 재료로 사용 될 수 있는가?
		if (false == IsSuitableForDestItem(_destItemDBId))
		{
			// 2023/10/11 PKT - 사용 될 수 없다면 초기화
			_destItemDBId = INVALID_ITEM_DB_ID;

			_refainItemDBId = INVALID_ITEM_DB_ID;			

			InvalidateUI();
		}
	}
}

void FGsItemFusionHandler::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	if (nullptr == Param || INVALID_ITEM_DB_ID == Param->_itemDBID)
	{
		return;
	}

	if (_srcItemDBId == Param->_itemDBID)
	{	// 2023/10/11 PKT - 삭제된 아이템이 Src 아이템이라면 전체 초기화
		ClearData();

		InvalidateUI();
	}
	else if (_destItemDBId == Param->_itemDBID)
	{	// 2023/10/11 PKT - 삭제된 아이템이 Dest 아이템이라면 초기화 진행
		_destItemDBId = INVALID_ITEM_DB_ID;

		_refainItemDBId = INVALID_ITEM_DB_ID;

		InvalidateUI();
	}
}

void FGsItemFusionHandler::OnRemoveItemList(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItemList* paramData = static_cast<const FGsRemoveItemList*>(&InParam);
	if (nullptr == paramData)
	{
		return;
	}

	bool IsInvalidate = false;

	for (ItemDBId itemDBId : paramData->_itemDBIdList)
	{
		if (_srcItemDBId == itemDBId)
		{	// 2023/10/11 PKT - 삭제된 아이템이 Src 아이템이라면 전체 초기화
			ClearData();
			// 2023/10/11 PKT - 갱신 플래그
			IsInvalidate = true;
			break;
		}
		else if (_destItemDBId == itemDBId)
		{
			_destItemDBId = INVALID_ITEM_DB_ID;

			_refainItemDBId = INVALID_ITEM_DB_ID;
			// 2023/10/11 PKT - 갱신 플래그( 원본도 초기화 되어야 할 수도 있으므로 반복문을 다 돈다. )
			IsInvalidate = true;
		}
	}

	if (IsInvalidate)
	{
		InvalidateUI();
	}
}

void FGsItemFusionHandler::OnLocalPlayerUseItemLock(FGsItemMessageParamBase& InParam)
{
	// 2023/10/11 PKT - 이 부분 어떻게 처리 해야 할지..난해 하다. 기존 처럼 처리 하면 버그 예상 됨!
	
}

void FGsItemFusionHandler::OnOpenItemFusionProc(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		GSLOG(Error, TEXT("nullptr == InParam"));
		return;
	}

	const FGsPrimitiveUInt64* message = StaticCast<const FGsPrimitiveUInt64*>(InParam);

	if (IsSuitableForSrcItem(message->_data, true))
	{
		ClearData();

		// 2023/9/27 PKT - Src Item 설정
		_srcItemDBId = message->_data;

		if (FGsEventProgressManager* eventMgr = GSEventProgress())
		{
			eventMgr->OnReqStartEvent(EGsEventProgressType::UI_POPUP_ITEMFUSION, this);
		}
	}
}

void FGsItemFusionHandler::OnTryItemFusionResult(const IGsMessageParam* InParam)
{
	const FGsPrimitivePairUInt64* param = StaticCast<const FGsPrimitivePairUInt64*>(InParam);
	if (nullptr == param)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS == StaticCast<int32>(param->_first))
	{	// 2023/10/11 PKT - 성공
		TWeakObjectPtr<UGsUIPopupItemFusion> popup = GetPopup();
		if (popup.IsValid())
		{
			// 2023/10/5 PKT - 연출 시작
			popup->ActionFusionEffect(param->_second);
		}
	}
	else
	{
		// 2023/10/11 PKT - 상황에 맞는 에러 메세지를 뿌린다.

		_messageHolder = false;

		InvalidateUI();
	}
	
}

void FGsItemFusionHandler::TryFusion()
{
	// 2023/9/25 PKT - 합성 시도
	ECheckTry isValid = IsCheckTryFusion();
	if (ECheckTry::Valid == isValid)
	{
		if (UGsUIManager* uiMgr = GUI())
		{
			if (UGsUIPopupCommonItemAction* popup = Cast<UGsUIPopupCommonItemAction>(uiMgr->OpenAndGetWidget(TEXT("PopupCommonItemAction"))))
			{
				FText textDesc;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_CheckPopup"), textDesc);

				FGsItem PreviewFusionItem = PreviewFusionResult();

				popup->SetNameVisibility(true);
				popup->SetDescVisibility(true);
				popup->SetButtonStyle(UGsUIPopupCommonItemAction::EButtonStyle::Decidable);
				popup->SetItemEx(&PreviewFusionItem, 1, true, true);
				popup->SetIconVisibility(true);	// 2024/01/03 CSH - Icon 노출로 변경
				popup->SetCurrencyVisibility(false);
				popup->SetDescription(textDesc);
				popup->SetCallBack([&](bool bYes)
					{
						if (bYes)
						{
							// 2023/10/11 PKT - 여기서 message를 hold한다. 결과를 받아서 막으면 늦는다. 아이템 삭제 패킷이 먼저 오므로..
							this->_messageHolder = true;

							// 2023/10/11 PKT - Send
							FGsNetSendServiceItem::SendTryItemFusion(this->_srcItemDBId, this->_destItemDBId, this->_refainItemDBId);
						}
					}
				);
			}
		}
	}
	else if (ECheckTry::invalidCurrency == isValid)
	{
		Currency lackAmount = FMath::Max(GetCost().mCurrency - FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD), StaticCast<Currency>(0));
		// 2023/10/12 PKT - 재화 유도창 띄움.
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, lackAmount);
	}
	else
	{
		GSLOG(Warning, TEXT("IsCheckTryFusion() : %d"), int32(isValid));
	}
}

void FGsItemFusionHandler::PushMaterialItem(ItemDBId InItemDBId)
{
	if (InItemDBId == _srcItemDBId || InItemDBId == _destItemDBId)
	{	// 2023/10/11 PKT - 이미 등록한 아이템을 클릭했다면 아무런 행동도 취하지 않는다.
		return;
	}

	// 2023/10/5 PKT - 선택된 제련 정보에 대한 처리 고려해야 함.
	if (INVALID_ITEM_DB_ID == _srcItemDBId)
	{
		if (true == IsSuitableForSrcItem(InItemDBId))
		{
			ClearData();

			_srcItemDBId = InItemDBId;

			InvalidateUI();
		}
	}
	else
	{
		if (true == IsSuitableForDestItem(InItemDBId))
		{
			_destItemDBId = InItemDBId;

			_refainItemDBId = INVALID_ITEM_DB_ID;

			// 2023/10/11 PKT - Dest 아이템을 선택 하였을 경우 Src와 Dest 둘 중 하나에만 재련이 있을때에만 기본 선택 한다.
			bool isAttachSrcRefin = (0 < GetItemRefineStatSet(_srcItemDBId).Num());
			bool isAttachDestRefin = (0 < GetItemRefineStatSet(_destItemDBId).Num());

			if (isAttachSrcRefin != isAttachDestRefin)
			{	// 2023/10/11 PKT - 둘 중 하나에만 재련 옵션이 붙어 있는 경우에만 기본 설정.
				if (isAttachSrcRefin)
				{
					_refainItemDBId = _srcItemDBId;
				}
				else
				{
					_refainItemDBId = _destItemDBId;
				}
			}

			InvalidateUI();
		}
	}
}

void FGsItemFusionHandler::PopMaterialItem(ItemDBId InItemDBId)
{
	// 원본/재료 아이템 중 하나를 선택 한 경우.( 취소 )
	if (InItemDBId == _srcItemDBId)
	{
		_srcItemDBId = INVALID_ITEM_DB_ID;
	}

	// 2023/10/4 PKT - 원본 아이템을 취소 해도 재료는 초기화, 재료만 초기화 해도 초기화 어찌되었든 초기화 되어야 함.
	_destItemDBId = INVALID_ITEM_DB_ID;

	// 2023/10/11 PKT - 무언가를 취소 했다면 재련 정보는 항상 초기화
	_refainItemDBId = INVALID_ITEM_DB_ID;

	InvalidateUI();
}

void FGsItemFusionHandler::SelectedRefain(ItemDBId InItemDBId)
{
	_refainItemDBId = InItemDBId;

	InvalidateUI();
}

void FGsItemFusionHandler::CleanView()
{
	ClearData();

	_messageHolder = false;

	InvalidateUI();
}

void FGsItemFusionHandler::CloseUI()
{
	Close();
}