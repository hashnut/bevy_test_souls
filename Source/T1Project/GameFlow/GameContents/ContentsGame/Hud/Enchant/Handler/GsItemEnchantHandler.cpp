// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemEnchantHandler.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemHelper.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupItemEnchantMain.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITraySafeEnchantItemResult.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantItemWait.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantItemProgress.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultBigSuccess.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultDestroy.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultFailed.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultSuccess.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultPreserved.h"
#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultDown.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupItemRefineStatList.h"
#include "Currency/GsCurrencyHelper.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Net/GsNetSendServiceItem.h"
#include "Management/GsMessageHolder.h"
#include "T1Project/GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"




FGsItemEnchantHandler::~FGsItemEnchantHandler()
{
}

void FGsItemEnchantHandler::Close()
{
	RemoveMessage();

	_isOpenSingleEnchantFlag = true;
	_isUseBGflag = true;
	_reserveItemDBId = 0;
	_copyDestroyEnchantItemTid = 0;
}

void FGsItemEnchantHandler::Init()
{
	RegisterMessage();
}

void FGsItemEnchantHandler::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();

	//////////////////////////////// ItemEnchant ///////////////////////////////////////////
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::OPEN,
		this, &FGsItemEnchantHandler::OpenItemEnchantProc));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::CLOSE,
		this, &FGsItemEnchantHandler::CloseItemEnchant));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::CLOSE_ALL_RESULT_WIDGET,
		this, &FGsItemEnchantHandler::CloseAllResultWidget));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::SHOW_ENCHANT_EFFECT,
		this, &FGsItemEnchantHandler::ShowEnchantEffect));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::SHOW_MAGICALFORGE_EFFECT,
		this, &FGsItemEnchantHandler::ShowMagicalForgeEffect));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::OPEN_EXPECT_REFINE_OPTION,
		this, &FGsItemEnchantHandler::OpenExpectRefineOptionPopup));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::INVALIDATE_EXPECT_REFINE_OPTION,
		this, &FGsItemEnchantHandler::UpdateExpectRefineOptionPopup));
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddRaw(MessageContentEnchant::CLOSE_EXPECT_REFINE_OPTION,
		this, &FGsItemEnchantHandler::CloseExpectRefineOptionPopup));

	//////////////////////////////// Item ///////////////////////////////////////////
	_itemDelegate = msg->GetItem().AddRaw(MessageItem::LOCKITEM, this, &FGsItemEnchantHandler::OnChangeIconDimmed);

	//////////////////////////////// GameObject ///////////////////////////////////////////
	_gameObjectDelegateList.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE,
		this, &FGsItemEnchantHandler::PlayerDeadState));
	_gameObjectDelegateList.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_DEAD,
		this, &FGsItemEnchantHandler::PlayerDeadState));
}

void FGsItemEnchantHandler::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentEnchant, FDelegateHandle>& elemItem : _listItemEnchantDelegate)
	{
		msg->GetItemEnchant().Remove(elemItem);
	}
	_listItemEnchantDelegate.Empty();

	for (MsgGameObjHandle& elemObject : _gameObjectDelegateList)
	{
		msg->GetGameObject().Remove(elemObject);
	}
	_gameObjectDelegateList.Empty();

	msg->GetItem().Remove(_itemDelegate);
}

void FGsItemEnchantHandler::OnReconection()
{
	CloseItemEnchant(nullptr);
}

void FGsItemEnchantHandler::CloseAllResultWidget(const struct IGsMessageParam* InMessageParam)
{
	CloseWidgetByKey(TEXT("TrayTickerSafeEnchantResult"));
	CloseWidgetByKey(TEXT("TrayEnchantItemWait"));
	CloseWidgetByKey(TEXT("TrayEnchantResultSuccess"));
	CloseWidgetByKey(TEXT("TrayEnchantResultBigSuccess"));
	CloseWidgetByKey(TEXT("TrayEnchantResultFailed"));
	CloseWidgetByKey(TEXT("TrayEnchantResultDestroy"));
	CloseWidgetByKey(TEXT("TrayEnchantResultPreserved"));
	CloseWidgetByKey(TEXT("TrayTickerRefineOptionResult"));
}

void FGsItemEnchantHandler::CloseItemEnchant(const struct IGsMessageParam* InMessageParam)
{
	CloseWidgetByKey(TEXT("PopupItemEnchant"));
	CloseWidgetByKey(TEXT("TrayTickerSafeEnchantResult"));
	CloseWidgetByKey(TEXT("TrayEnchantItemWait"));
	CloseWidgetByKey(TEXT("TrayEnchantResultSuccess"));
	CloseWidgetByKey(TEXT("TrayEnchantResultBigSuccess"));
	CloseWidgetByKey(TEXT("TrayEnchantResultFailed"));
	CloseWidgetByKey(TEXT("TrayEnchantResultDestroy"));
	CloseWidgetByKey(TEXT("TrayEnchantResultPreserved"));
	CloseWidgetByKey(TEXT("TrayTickerRefineOptionResult"));	
	CloseWidgetByKey(TEXT("PopupCollectionEnchant"));
}

void FGsItemEnchantHandler::CloseWidgetByKey(const FName& InKey)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(InKey))
		{
			uiManager->CloseByKeyName(InKey);
		}
	}
}

// 이벤트 시작
void FGsItemEnchantHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(_reserveItemDBId);
	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupInventory")))
		{
#ifdef RESTORE_INVENTORY
			// 비교창을 통해 들어오지 않은 경우에 저장
			if (false == uiManager->IsActiveWidget(TEXT("PopupItemDetailCompare")))
			{
				// 순서에 유의 인벤토리에서 열린 상세창에서만 저장함
				if (UGsInventoryManager* inventoryMgr = GSInventory())
				{
					if (inventoryMgr->CheckIsHUDState())
					{
						inventoryMgr->SetRestoreItemDBId(_reserveItemDBId);
					}
				}
			}
#endif
			uiManager->CloseByKeyName(TEXT("PopupInventory"));
		}

		if (false == uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemEnchant"));
			if (widget.IsValid())
			{
				UGsUIPopupItemEnchantMain* popup = Cast<UGsUIPopupItemEnchantMain>(widget.Get());
				if (nullptr == popup)
				{
					return;
				}

				EGsItemEnchantActionType actionType = _isOpenSingleEnchantFlag ? 
					EGsItemEnchantActionType::ENCHANT_SINGLE : EGsItemEnchantActionType::ENCHANT_MULTI;
				if (findItem.IsValid())
				{
					// 초기 오픈 접근 유형
					// 장비 선택 -> 장비만 들어간 단일강화 
					// 장비 선택2 -> 컬렉션 에서의 강화는 상황에 따라 단일/멀티 강화가 되어야한다고 함
					// 주문서 선택 -> 주문서만 들어간 단일강화
					// 재련석 선택 -> 재련석만 들어간 아이템 재련
					if (findItem.Pin()->IsEquipment())
					{
						CopyTempEnchantItemData(findItem.Pin()->GetTID());
					}
					else
					{
						// 주문서 선택 -> 주문서만 들어간 단일강화
						// 재련석 선택 -> 재련석만 들어간 아이템 재련
						if (findItem.Pin()->IsIngredient())
						{
							FGsItemIngredient* ingredientItem = findItem.Pin().Get()->GetCastItem<FGsItemIngredient>();
							if (nullptr != ingredientItem)
							{
								switch (ingredientItem->GetEffectType())
								{
								case ItemEffectType::ITEM_ENCHANT:
									actionType = EGsItemEnchantActionType::ENCHANT_SINGLE;
									break;
								case ItemEffectType::OPTION_STONE:
									actionType = EGsItemEnchantActionType::REFINE_OPTION;
									break;
								case ItemEffectType::MAGICAL_FORGE:
									actionType = EGsItemEnchantActionType::MAGICAL_FORGE;
									break;
								default:
									break;
								}
							}
						}
					}
				}
				else // 컬렉션 강화쪽에서는 TargetItem 이 없는 상태가 필요하다고 하여 분기 처리 진행
				{
					// ......
				}
				popup->SetEnchantInfo(_reserveItemDBId, actionType, findItem.IsValid() ? findItem.Pin()->IsEquipment() : false);
				popup->SetBG(_isUseBGflag);
			}
		}
	}
}

// 이벤트 종료
void FGsItemEnchantHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	CloseItemEnchant(nullptr);
}

void FGsItemEnchantHandler::PlayerDeadState(const struct IGsMessageParam* InParam)
{
	CloseItemEnchant(nullptr);
}

void FGsItemEnchantHandler::OpenItemEnchantProc(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsOpenItemEnchant* paramData = InParam->Cast<const FGsOpenItemEnchant>();
	if (nullptr == paramData)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	EGsUnlockContentsType contentsType = EGsUnlockContentsType::ITEM_ENCHANT;
	TWeakPtr<FGsItem> findItem = itemManager->FindItem(paramData->_itemDBID);
	if (findItem.IsValid())
	{
		// 장비를 선택해서 진입한 경우
		if (findItem.Pin()->IsEquipment())
		{
			// 강화로 간주
			contentsType = EGsUnlockContentsType::ITEM_ENCHANT;
		}
		// 재료를 선택해서 진입한 경우
		else if (findItem.Pin()->IsIngredient())
		{
			FGsItemIngredient* ingredientItem = findItem.Pin().Get()->GetCastItem<FGsItemIngredient>();
			if (nullptr != ingredientItem)
			{
				switch (ingredientItem->GetEffectType())
				{
				case ItemEffectType::ITEM_ENCHANT:
					contentsType = EGsUnlockContentsType::ITEM_ENCHANT;
					break;
				case ItemEffectType::OPTION_STONE:
					contentsType = EGsUnlockContentsType::ITEM_REFINE;
					break;
				case ItemEffectType::MAGICAL_FORGE:
					contentsType = EGsUnlockContentsType::ITEM_MAGICAL_FORGE;
					break;
				}
			}
		}
	}

	if (false == unlockManager->IsContentsUnlock(contentsType, true))
		return;

	// 저장
	_reserveItemDBId = paramData->_itemDBID;
	_isUseBGflag = paramData->_isUseBG;
	_isOpenSingleEnchantFlag = paramData->_isOpenSingleEnchant;

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqStartEvent(EGsEventProgressType::UI_POPUP_ITEM_ENCHANT, this);
	}
}

void FGsItemEnchantHandler::ShowEnchantEffect(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsShowEnchantEffect* paramData = InParam->Cast<const FGsShowEnchantEffect>();
	if (nullptr == paramData)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (nullptr == enchantMgr)
		return;

	if (uiManager->IsActiveWidget(TEXT("TrayEnchantItemWait")))
	{
		uiManager->CloseByKeyName(TEXT("TrayEnchantItemWait"));
	}
	if (paramData->_isResultEffect)
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(paramData->_equipItemDbid, ItemType::EQUIP);
		if (findItem.IsValid())
		{
			if(enchantMgr->IsRiskEnchantEffect(findItem, paramData->_ingredientItemDbid) || ItemEnchantResult::ENCHANT_FAILED_SHIELDED == paramData->_enchantResultType)
			{
				TWeakObjectPtr<UGsUIWidgetBase> resultWidget;
				switch (paramData->_enchantResultType)
				{
				case ItemEnchantResult::ENCHANT_SUCCEEDED:
				{
					resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultSuccess"));
					if (resultWidget.IsValid())
					{
						UGsUITrayEnchantResultSuccess* result = Cast<UGsUITrayEnchantResultSuccess>(resultWidget.Get());
						result->SetDisplayItem(paramData->_equipItemDbid, 2.f);
						if (findItem.Pin()->IsSpecialAccItem())
						{
							enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::Enchant_Process);
						}
					}
					break;
				}
				case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
				{
					resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultBigSuccess"));
					if (resultWidget.IsValid())
					{
						UGsUITrayEnchantResultBigSuccess* result = Cast<UGsUITrayEnchantResultBigSuccess>(resultWidget.Get());
						result->SetDisplayItem(paramData->_equipItemDbid, 2.f);
					}
					break;
				}
				case ItemEnchantResult::ENCHANT_FAILED_SHIELDED:
				{
					resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultFailed"));
					if (resultWidget.IsValid())
					{
						UGsUITrayEnchantResultFailed* result = Cast<UGsUITrayEnchantResultFailed>(resultWidget.Get());
						result->SetDisplayItem(paramData->_equipItemDbid, 2.f);
					}
					break;
				}
				case ItemEnchantResult::ENCHANT_PRESERVED:
				{
					resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultPreserved"));
					if (resultWidget.IsValid())
					{
						UGsUITrayEnchantResultPreserved* result = Cast<UGsUITrayEnchantResultPreserved>(resultWidget.Get());
						result->SetDisplayItem(paramData->_equipItemDbid, 2.f);
						if (findItem.Pin()->IsSpecialAccItem())
						{
							enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::Enchant_Process);
						}
					}
					break;
				}
				case ItemEnchantResult::ENCHANT_FAILED_DOWN:
				{
					resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultDown"));
					if (resultWidget.IsValid())
					{
						UGsUITrayEnchantResultDown* result = Cast<UGsUITrayEnchantResultDown>(resultWidget.Get());
						result->SetDisplayItem(paramData->_equipItemDbid, 2.f);
						if (findItem.Pin()->IsSpecialAccItem())
						{
							enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::Enchant_Process);
						}
					}
					break;
				}
				}
			}
			else
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayTickerSafeEnchantResult"));
				if (widget.IsValid())
				{
					int32 effectIndex = 0;
					switch (paramData->_enchantResultType)
					{
					case ItemEnchantResult::ENCHANT_SUCCEEDED:
						effectIndex = 0;
						break;
					case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
						effectIndex = 1;
						break;
					case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
						effectIndex = 2;
						break;
					}

					UGsUITraySafeEnchantItemResult* result = Cast<UGsUITraySafeEnchantItemResult>(widget.Get());
					result->SetEffectData(effectIndex);
					enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_Safe_Success);
				}
			}
		}
		else
		{
			// 무기 파괴 되었을때
			TWeakObjectPtr<UGsUIWidgetBase> resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultDestroy"));
			if (resultWidget.IsValid() && 0 < _copyDestroyEnchantItemTid)
			{
				UGsUITrayEnchantResultDestroy* result = Cast<UGsUITrayEnchantResultDestroy>(resultWidget.Get());
				result->SetDisplayItem(_copyDestroyEnchantItemTid, 3.f);
				_copyDestroyEnchantItemTid = 0;
			}
		}
	}
	else
	{
		//CopyTempEnchantItemData(findItem.Pin()->GetTID());

		TWeakPtr<FGsItem> findItem = GItem()->FindItem(paramData->_equipItemDbid, ItemType::EQUIP);
		if (findItem.IsValid())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantItemWait"));
			if (widget.IsValid())
			{
				UGsUITrayEnchantItemWait* trayWait = Cast<UGsUITrayEnchantItemWait>(widget.Get());
				if (nullptr == trayWait)
					return;

				ItemId equipItemTid = findItem.Pin()->GetTID();
				ItemDBId equipItemDbid = paramData->_equipItemDbid;
				ItemDBId ingredientItemDbid = paramData->_ingredientItemDbid;
				trayWait->SetData(1.0f, [this, equipItemDbid, ingredientItemDbid, equipItemTid]()
					{
						ConfirmEnchantWait(equipItemDbid, ingredientItemDbid, equipItemTid);
					});

				enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait);
			}
		}
	}
}

void FGsItemEnchantHandler::ShowMagicalForgeEffect(const struct IGsMessageParam* InParam)
{
	const FGsShowEnchantEffect* paramData = InParam->Cast<const FGsShowEnchantEffect>();
	if (nullptr == paramData)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (nullptr == enchantMgr)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("TrayEnchantItemWait")))
	{
		uiManager->CloseByKeyName(TEXT("TrayEnchantItemWait"));
	}
	if (paramData->_isResultEffect)
	{
		TWeakPtr<FGsItem> findItem = itemManager->FindItem(paramData->_equipItemDbid, ItemType::EQUIP);
		if (findItem.IsValid())
		{
			switch (paramData->_enchantResultType)
			{
			case ItemEnchantResult::ENCHANT_SUCCEEDED:
			{
				TWeakObjectPtr<UGsUIWidgetBase> resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultSuccess"));
				if (resultWidget.IsValid())
				{
					UGsUITrayEnchantResultSuccess* result = Cast<UGsUITrayEnchantResultSuccess>(resultWidget.Get());
					result->SetDisplayItem(paramData->_equipItemDbid, 2.f, false);
				}
				break;
			}
			case ItemEnchantResult::ENCHANT_PRESERVED:
			{
				TWeakObjectPtr<UGsUIWidgetBase> resultWidget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantResultPreserved"));
				if (resultWidget.IsValid())
				{
					UGsUITrayEnchantResultPreserved* result = Cast<UGsUITrayEnchantResultPreserved>(resultWidget.Get());
					result->SetDisplayItem(paramData->_equipItemDbid, 2.f, false);
				}
				break;
			}
			default:
				break;
			}
		}
	}
	else
	{
		TWeakPtr<FGsItem> findItem = itemManager->FindItem(paramData->_equipItemDbid, ItemType::EQUIP);
		if (findItem.IsValid())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayEnchantItemWait"));
			if (widget.IsValid())
			{
				UGsUITrayEnchantItemWait* trayWait = Cast<UGsUITrayEnchantItemWait>(widget.Get());
				if (nullptr == trayWait)
					return;

				ItemId equipItemTid = findItem.Pin()->GetTID();
				ItemDBId equipItemDbid = paramData->_equipItemDbid;
				ItemDBId ingredientItemDbid = paramData->_ingredientItemDbid;
				trayWait->SetData(1.0f, [this, equipItemDbid, ingredientItemDbid, equipItemTid]()
					{
						ConfirmMagicalForgeWait(equipItemDbid, ingredientItemDbid, equipItemTid);
					});

				enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait);
			}
		}
	}
}

void FGsItemEnchantHandler::ConfirmEnchantWait(ItemDBId InTargetItemDbid, ItemDBId InIngredientItemDbid, ItemId InTargetItemTid)
{
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	int32 expectLv = 0;
	if (enchantMgr->IsValidSingleEnchant(InTargetItemDbid, InIngredientItemDbid, expectLv))
	{
		_copyDestroyEnchantItemTid = InTargetItemTid;
		FGsNetSendServiceItem::SendEnchantItem(InIngredientItemDbid, 1, InTargetItemDbid);
		enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Touch);
	}
	else
	{
		CloseWidgetByKey(TEXT("TrayEnchantItemWait"));
	}
}

void FGsItemEnchantHandler::ConfirmMagicalForgeWait(ItemDBId InTargetItemDbid, ItemDBId InIngredientItemDbid, ItemId InTargetItemTid)
{
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (nullptr == enchantMgr)
		return;

	bool isSameEffect = false;
	if (FGsItemHelper::CanMagicalForge(InTargetItemDbid, InIngredientItemDbid, true, isSameEffect))
	{
		FGsNetSendServiceItem::SendMagicalForgeItem(InTargetItemDbid, InIngredientItemDbid);
		enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Touch);
	}
	else
	{
		CloseWidgetByKey(TEXT("TrayEnchantItemWait"));
	}
}

void FGsItemEnchantHandler::CopyTempEnchantItemData(const ItemId InItemTid)
{
	_copyDestroyEnchantItemTid = InItemTid;
}

void FGsItemEnchantHandler::OnChangeIconDimmed(struct FGsItemMessageParamBase& InParam)
{
	

	/*
	if (UGsUIManager* uiManager = GUI())
	{
		const FGsGameObjectMessageParamReactionLocalPlayer* paramData =
			InParam->Cast<const FGsGameObjectMessageParamReactionLocalPlayer>();
		if (nullptr == paramData)
			return;

		if (uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemEnchant"));
			if (widget.IsValid())
			{
				UGsUIPopupItemEnchantMain* popup = Cast<UGsUIPopupItemEnchantMain>(widget.Get());
				if (nullptr == popup)
				{
					return;
				}
				popup->SetAllIconDimmed(paramData->IsLock(CreatureActionType::CREATURE_ACTION_TYPE_ITEM));
			}
		}
	}
	*/
}


void FGsItemEnchantHandler::OpenExpectRefineOptionPopup(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	const FGsOpenExpectRefineOption* paramData = InParam->Cast<const FGsOpenExpectRefineOption>();
	if (nullptr == paramData)
		return;

	if (0 >= paramData->_targetItemDbid || 0 >= paramData->_ingredientItemDbid)
		return;

	if (false == uiManager->IsActiveWidget(TEXT("PopupRefineStatList")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupRefineStatList"));
		if (widget.IsValid())
		{
			UGsUIPopupItemRefineStatList* statListPopup = Cast<UGsUIPopupItemRefineStatList>(widget.Get());
			if (nullptr == statListPopup)
				return;

			TWeakPtr<FGsItem> findItem = GItem()->FindItem(paramData->_ingredientItemDbid, ItemType::INGREDIENT);
			if (false == findItem.IsValid())
				return;

			statListPopup->OpenExpectStatList(paramData->_targetItemDbid, findItem.Pin()->GetSubCategory());
		}
	}
	else
	{
		uiManager->CloseByKeyName(TEXT("PopupRefineStatList"), true);
	}
}

void FGsItemEnchantHandler::UpdateExpectRefineOptionPopup(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	const FGsOpenExpectRefineOption* paramData = InParam->Cast<const FGsOpenExpectRefineOption>();
	if (nullptr == paramData)
		return;

	if (false == uiManager->IsActiveWidget(TEXT("PopupRefineStatList")))
		return;

	if (0 >= paramData->_targetItemDbid || 0 >= paramData->_ingredientItemDbid)
	{
		uiManager->CloseByKeyName(TEXT("PopupRefineStatList"), true);
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupRefineStatList"));
	if (widget.IsValid())
	{
		UGsUIPopupItemRefineStatList* statListPopup = Cast<UGsUIPopupItemRefineStatList>(widget.Get());
		if (nullptr == statListPopup)
			return;

		UGsItemManager* itemMgr = GItem();
		TWeakPtr<FGsItem> findIngredientItem = itemMgr->FindItem(paramData->_ingredientItemDbid, ItemType::INGREDIENT);
		if (false == findIngredientItem.IsValid())
			return;

		TWeakPtr<FGsItem> findEquipItem = itemMgr->FindItem(paramData->_targetItemDbid, ItemType::EQUIP);
		if (false == findEquipItem.IsValid())
			return;

		if (findEquipItem.Pin()->GetTID() != statListPopup->GetTargetItemTid() || findIngredientItem.Pin()->GetSubCategory() != statListPopup->GetIngredientCategorySub())
		{
			statListPopup->OpenExpectStatList(paramData->_targetItemDbid, findIngredientItem.Pin()->GetSubCategory());
		}
	}
}

void FGsItemEnchantHandler::CloseExpectRefineOptionPopup(const struct IGsMessageParam* InParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("PopupRefineStatList")))
	{
		uiManager->CloseByKeyName(TEXT("PopupRefineStatList"), true);
	}
}

