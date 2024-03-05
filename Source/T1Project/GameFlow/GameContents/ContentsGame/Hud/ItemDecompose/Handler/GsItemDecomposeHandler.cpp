// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemDecomposeHandler.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Net/GsNetSendServiceItem.h"

#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "T1Project/UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIContent/Popup/GsUIPopupCommonItemAction.h"
#include "UI/UIContent/Popup/ItemDecompose/GsUIPopupItemDecompose.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "T1Project.h"


FGsItemDecomposeHandler::~FGsItemDecomposeHandler()
{
}

void FGsItemDecomposeHandler::Close()
{
	RemoveMessage();
}

void FGsItemDecomposeHandler::Init()
{
	RegisterMessage();
}

void FGsItemDecomposeHandler::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		_listItemDeComposeDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DECOMPOSE_SINGLE,
			this, &FGsItemDecomposeHandler::DecomposeSingleProc));
		_listItemDeComposeDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DECOMPOSE_MULTI,
			this, &FGsItemDecomposeHandler::DecomposeMultiProc));
		_listItemDeComposeDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DECOMPOSE_RESULT,
			this, &FGsItemDecomposeHandler::DecomposeMultiResult));
	}
}

void FGsItemDecomposeHandler::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		for (TPair<MessageContentItem, FDelegateHandle>& elemItem : _listItemDeComposeDelegate)
		{
			msg->GetItemContents().Remove(elemItem);
		}
		_listItemDeComposeDelegate.Empty();
	}
}

void FGsItemDecomposeHandler::DecomposeSingleProc(const struct IGsMessageParam* InParamData)
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::DECOMPOSE, true))
		return;

	// 싱글 분해
	const FGsItemDecomposeSingle* param = InParamData->Cast<const FGsItemDecomposeSingle>();
	if (nullptr == param)
		return;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(param->_itemDBID);
	if (findItem.IsValid())
	{
		if (UGsUIManager* uiMgr = GUI())
		{
			UGsUIPopupCommonItemAction* popup = Cast<UGsUIPopupCommonItemAction>(uiMgr->OpenAndGetWidget(TEXT("PopupCommonItemAction")));
			if (popup)
			{
				FText result;
				FText textDesc;
				FText::FindText(TEXT("DecomposeText"), TEXT("Notice_Confirm"), textDesc);
				result = FText::Format(textDesc, 1);

				ItemDBId dbid = param->_itemDBID;
				popup->SetDescVisibility(true);
				popup->SetButtonStyle(UGsUIPopupCommonItemAction::EButtonStyle::Decidable);
				popup->SetItem(findItem.Pin().Get(), 1, true);
				popup->SetCurrencyVisibility(false);
				popup->SetDescription(result);
				popup->SetCallBack([&,this, dbid](bool bYes)
					{
						if (bYes)
						{
							do 
							{
								// 분해 아이템들 중, 하나라도 유효하지 않는 아이템이면 아무것도 하지 않고
								// 창을 닫아버림 - 재오픈 유도 함으로써 자동 갱신처리 로직 태움
								UGsItemManager* itemMgr = GItem();
								if (nullptr == itemMgr)
									break;

								TWeakPtr<FGsItem> findItem = itemMgr->FindItem(dbid);
								if (false == findItem.IsValid())
								{
									// 분해할 수 없습니다
									FText findText;
									FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Cannot"), findText);
									FGsUIHelper::TrayMessageTicker(findText);
									break;
								}

								if (findItem.Pin()->GetLock())
								{
									// 티커 출력 : 잠긴 아이템은 분해할 수 없습니다.
									FText findText;
									FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Locked"), findText);
									FGsUIHelper::TrayMessageTicker(findText);
									break;
								}

								ItemDBIdAmountPair pair(dbid, 1);
								FGsNetSendServiceItem::SendItemDecompose(TArray<ItemDBIdAmountPair> { pair });

							} while (0);

							ClosePopup();
						}
					}
				);
			}
		}
	}
}

void FGsItemDecomposeHandler::DecomposeMultiProc(const struct IGsMessageParam* InParamData)
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::DECOMPOSE, true))
		return;

	// 멀티 분해창 출력
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	UGsUIPopupItemDecompose* popup = Cast<UGsUIPopupItemDecompose>(uiMgr->OpenAndGetWidget(TEXT("PopupItemDecompose")));
	if (popup)
	{
		TMap<ItemType, TArray<ItemDBId>> resultItemMap;
		TArray<ItemDBId> dbidList;
		TArray<TWeakPtr<FGsItem>> findEquipItemList = itemMgr->FindItem(ItemType::EQUIP);
		for (const TWeakPtr<FGsItem> iter : findEquipItemList)
		{
			if (iter.IsValid())
			{
				if (false == iter.Pin()->IsEquipment())
					continue;

				if(false == iter.Pin()->IsDecomposable())
					continue;

				if (iter.Pin()->IsEquipped())
					continue;

				if (iter.Pin()->GetLock())
					continue;

				if (iter.Pin()->IsCashItem())
					continue;

				if (iter.Pin()->IsPresetEquipped())
					continue;

				dbidList.Emplace(iter.Pin()->GetDBID());
			}
		}
		SortItemList(dbidList);
		resultItemMap.Emplace(ItemType::EQUIP, dbidList);

		dbidList.Empty();
		TArray<TWeakPtr<FGsItem>> findSkillbookItemList = itemMgr->FindItem(ItemType::CONSUME);
		for (const TWeakPtr<FGsItem> iter : findSkillbookItemList)
		{
			if (iter.IsValid())
			{
				if (false == iter.Pin()->IsConsumable())
					continue;

				if (false == iter.Pin()->IsDecomposable())
					continue;

				if (iter.Pin()->GetLock())
					continue;

				if (iter.Pin()->IsCashItem())
					continue;

				dbidList.Emplace(iter.Pin()->GetDBID());
			}
		}
		SortItemList(dbidList);
		resultItemMap.Emplace(ItemType::CONSUME, dbidList);

		dbidList.Empty();
		TArray<TWeakPtr<FGsItem>> findMonsterKnowledgeSlate = itemMgr->FindItem(ItemType::INGREDIENT);
		for (const TWeakPtr<FGsItem> iter : findMonsterKnowledgeSlate)
		{
			if (iter.IsValid())
			{
				if (false == iter.Pin()->IsIngredient())
					continue;

				if (false == iter.Pin()->IsDecomposable())
					continue;

				if (iter.Pin()->GetLock())
					continue;

				if (iter.Pin()->IsCashItem())
					continue;

				dbidList.Emplace(iter.Pin()->GetDBID());
			}
		}
		SortItemList(dbidList);
		resultItemMap.Emplace(ItemType::INGREDIENT, dbidList);

		popup->SetMultiDecomposeItemList(resultItemMap);
	}
}

void FGsItemDecomposeHandler::ClosePopup()
{

}

void FGsItemDecomposeHandler::DecomposeMultiResult(const struct IGsMessageParam* InParamData)
{
	const FGsItemMultiDecomposeResult* param = InParamData->Cast<const FGsItemMultiDecomposeResult>();
	if (nullptr == param)
		return;

	// 자동 분해가 수동 분해 과정에 영향을 주면 안된다.
	if (param->_isAutoDecompose)
		return;

	// 멀티 분해창 출력
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	if (false == uiMgr->IsActiveWidget(TEXT("PopupItemDecompose")))
		return;

	UGsUIPopupItemDecompose* popup = Cast<UGsUIPopupItemDecompose>(uiMgr->OpenAndGetWidget(TEXT("PopupItemDecompose")));
	if (popup)
	{
		popup->ResetData();
	}
}

bool FGsItemDecomposeHandler::IsValidDecompose(const FGsItem* InItem)
{
	if (false == FGsItem::IsValidItem(InItem))
		return false;

	// 유료장비 불가능
	if (true == InItem->IsCashItem())
		return false;

	if (true == InItem->GetLock())
		return false;

	if (true == InItem->IsEquipped())
		return false;

	// 프리셋 장착중인 장비 제외
	if (true == InItem->IsPresetEquipped())
		return false;

	if (0 >= InItem->GetAmount())
		return false;

	return true;
}

void FGsItemDecomposeHandler::SortItemList(OUT TArray<uint64>& outItemDBIdList)
{
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	TArray<FGsItem*> itemList;
	itemList.Empty();
	for (int32 i = 0; i < outItemDBIdList.Num(); ++i)
	{
		TWeakPtr<FGsItem> itemWeakPtr = itemMgr->FindItem(outItemDBIdList[i]);
		if (false == itemWeakPtr.IsValid())
			continue;

		itemList.Emplace(itemWeakPtr.Pin().Get());
	}

	if (itemList.Num() > 1)
	{
		itemList.Sort([](FGsItem& lhs, FGsItem& rhs)
			{
				// 아이템 등급
				if ((uint8)lhs.GetGradeType() != (uint8)rhs.GetGradeType())
				{
					return (uint8)lhs.GetGradeType() < (uint8)rhs.GetGradeType();
				}

				// 아이템 타입
				if ((uint8)lhs.GetType() != (uint8)rhs.GetType())
				{
					return (uint8)lhs.GetType() < (uint8)rhs.GetType();
				}

				// 아이템 메인 카테고리
				if ((uint8)lhs.GetMainCategory() != (uint8)rhs.GetMainCategory())
				{
					return (uint8)lhs.GetMainCategory() < (uint8)rhs.GetMainCategory();
				}

				// 아이템 서브 카테고리
				if ((uint8)lhs.GetSubCategory() != (uint8)rhs.GetSubCategory())
				{
					return (uint8)lhs.GetSubCategory() < (uint8)rhs.GetSubCategory();
				}

				// 아이템 그룹 ID
				if (lhs.GetItemGroupId() != rhs.GetItemGroupId())
				{
					return lhs.GetItemGroupId() < rhs.GetItemGroupId();
				}

				// 아이템 TID
				if (lhs.GetTID() != rhs.GetTID())
				{
					return lhs.GetTID() < rhs.GetTID();
				}
				
				// 아이템 강화도
				if (lhs.GetLevel() != rhs.GetLevel())
				{
					return lhs.GetLevel() > rhs.GetLevel();
				}

				// 아이템 보유 개수
				if (lhs.GetAmount() != rhs.GetAmount())
				{
					return lhs.GetAmount() > rhs.GetAmount();
				}

				// 아이템 DBID. 고유 정렬
				return lhs.GetDBID() < rhs.GetDBID();
			}
		);
	}

	outItemDBIdList.Empty();
	for (int32 i = 0; i < itemList.Num(); ++i)
	{
		outItemDBIdList.Emplace(itemList[i]->GetDBID());
	}
}
