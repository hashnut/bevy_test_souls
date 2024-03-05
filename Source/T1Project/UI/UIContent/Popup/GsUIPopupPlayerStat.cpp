// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupPlayerStat.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Stat/GsStatInfo.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Popup/Stat/GsUIPlayerEquipInfo.h"
#include "UI/UIContent/Popup/Stat/GsUIPopupPlayerStatDetail.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Item/GsItemManager.h"

#include "Message/GsMessageGameObject.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsPlayerDetailInfoMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIPopupPlayerStat::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupPlayerStat::OnClickClose);
	}
	if (_btnStatDistribution)
	{
		_btnStatDistribution->OnClicked.RemoveDynamic(this, &UGsUIPopupPlayerStat::OnClickStatDistribution);
	}
	if (_btnStatDetailInfo)
	{
		_btnStatDetailInfo->OnClicked.RemoveDynamic(this, &UGsUIPopupPlayerStat::OnClickStatDetailInfo);
	}
	if (_slotHelperBookmark)
	{
		_slotHelperBookmark->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupPlayerStat::OnRefreshBookmark);
		_slotHelperBookmark = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupPlayerStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupPlayerStat::OnClickClose);
	_btnStatDistribution->OnClicked.AddDynamic(this, &UGsUIPopupPlayerStat::OnClickStatDistribution);
	_btnStatDetailInfo->OnClicked.AddDynamic(this, &UGsUIPopupPlayerStat::OnClickStatDetailInfo);

	if (nullptr == _slotHelperBookmark)
	{
		_slotHelperBookmark = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperBookmark->Initialize(_subClassOfBookmarkEntry, _panelBookmarkEntry);
		_slotHelperBookmark->OnRefreshEntry.AddDynamic(this, &UGsUIPopupPlayerStat::OnRefreshBookmark);
	}
}

void UGsUIPopupPlayerStat::NativeConstruct()
{
	Super::NativeConstruct();

	_statTextBlockMap.Empty();
	_statTextBlockMap.Emplace(StatType::STR, _textBlockStr);
	_statTextBlockMap.Emplace(StatType::DEX, _textBlockDex);
	_statTextBlockMap.Emplace(StatType::INT, _textBlockInt);
	_statTextBlockMap.Emplace(StatType::CON, _textBlockCon);
	_statTextBlockMap.Emplace(StatType::AGI, _textBlockAgi);
	_statTextBlockMap.Emplace(StatType::WIS, _textBlockWis);

	// 메시지 설정
	RegisterMessage();

	// 북마크 정보 요청
	FGsNetSendServiceWorld::SendReqUserStatBookmarkList();

	// 분배 스탯 정보 초기화
	if (FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance())
	{
		if (FGsPlayerStatDistributionController* distributionController = statHandler->GetStatDistributionController())
		{
			distributionController->UpdateStatPoint();
		}
	}

	// UI 갱신
	_invalidateFlags = EGsPlayerStatInvalidateFlags::ALL;
	InvalidateAllInternal();

	_scrollBoxBookmark->ScrollToStart();
}

void UGsUIPopupPlayerStat::NativeDestruct()
{
	_statTextBlockMap.Empty();

	RemoveMessage();

	// not in close
	// 1. popup show
	// 2. costum window show
	// 3. popup out of viewport
	GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_EQUIP_VIEW_END, nullptr);

	Super::NativeDestruct();
}

void UGsUIPopupPlayerStat::Close(bool bImmediate /*= false*/, bool bRecoverPrevStack /*= true*/)
{
	GUI()->CloseByKeyName(TEXT("PopupPlayerStatDetail"));


	Super::Close();
}

void UGsUIPopupPlayerStat::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();

	_listItemDelegate.Empty();
	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEM,
		this, &UGsUIPopupPlayerStat::OnUpdateItem));

	_listUIDelegate.Empty();
	_listUIDelegate.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_PLAYER_STAT,
		this, &UGsUIPopupPlayerStat::OnInvalidatePlayerStat));
	_listUIDelegate.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION,
		this, &UGsUIPopupPlayerStat::OnInvalidatePlayerStatDistribution));

	_listRedDotDelegate.Empty();
	_listRedDotDelegate.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::STAT,
		this, &UGsUIPopupPlayerStat::InvalidateStatDistributionRedDot));

	_listPlayerDetailInfoDelegate.Empty();
	_listPlayerDetailInfoDelegate.Emplace(msg->GetPlayerDetailInfo().AddUObject(MessageContentPlayerInfo::EQUIP,
		this, &UGsUIPopupPlayerStat::OnInvalidateEquip));
	_listPlayerDetailInfoDelegate.Emplace(msg->GetPlayerDetailInfo().AddUObject(MessageContentPlayerInfo::EQUIP_ALL,
		this, &UGsUIPopupPlayerStat::OnInvalidateEquipAll));

	_listGameObjectDelegate.Empty();
	_listGameObjectDelegate.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::PLAYER_STAT_UPDATE,
		this, &UGsUIPopupPlayerStat::OnPlayerStatUpdate));
}

void UGsUIPopupPlayerStat::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();

	for (TPair<MessageItem, FDelegateHandle>& elemItem : _listItemDelegate)
	{
		msg->GetItem().Remove(elemItem);
	}
	_listItemDelegate.Empty();

	for (MsgUIHandle& elemItem : _listUIDelegate)
	{
		msg->GetUI().Remove(elemItem);
	}
	_listUIDelegate.Empty();

	for (MsgHUDRedDotHandle& elemItem : _listRedDotDelegate)
	{
		msg->GetHudRedDot().Remove(elemItem);
	}
	_listRedDotDelegate.Empty();

	for (TPair<MessageContentPlayerInfo, FDelegateHandle>& elemItem : _listPlayerDetailInfoDelegate)
	{
		msg->GetPlayerDetailInfo().Remove(elemItem);
	}

	for (MsgGameObjHandle& elemItem : _listGameObjectDelegate)
	{
		msg->GetGameObject().Remove(elemItem);
	}
	_listGameObjectDelegate.Empty();
}

void UGsUIPopupPlayerStat::InvalidateAllInternal()
{
	if (EnumHasAnyFlags(_invalidateFlags, EGsPlayerStatInvalidateFlags::STAT_DEFAULT))
	{
		InvalidateStatDefault();
	}
	if (EnumHasAnyFlags(_invalidateFlags, EGsPlayerStatInvalidateFlags::STAT_BOOKMARK))
	{
		InvalidateStatFavorite();
	}
	if (EnumHasAnyFlags(_invalidateFlags, EGsPlayerStatInvalidateFlags::EQUIP_SLOT))
	{
		InvalidateEquipSlotAll();
	}
	if (EnumHasAnyFlags(_invalidateFlags, EGsPlayerStatInvalidateFlags::RED_DOT))
	{
		InvalidateStatDistributionRedDot();
	}

	_invalidateFlags = EGsPlayerStatInvalidateFlags::NONE;
}

void UGsUIPopupPlayerStat::InvalidateStatDefault()
{
	SetMainStatValueText(StatType::STR);
	SetMainStatValueText(StatType::DEX);
	SetMainStatValueText(StatType::INT);
	SetMainStatValueText(StatType::CON);
	SetMainStatValueText(StatType::AGI);
	SetMainStatValueText(StatType::WIS);
}

void UGsUIPopupPlayerStat::InvalidateStatFavorite()
{
	if (FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance())
	{
		const TArray<StatType>& statTypeList = statHandler->GetBookmarkStatController()->GetBookmarkStat();
		if (statTypeList.Num() > 0)
		{
			_switcherBookmark->SetActiveWidgetIndex(0);
			_slotHelperBookmark->RefreshAll(statTypeList.Num());
		}
		else
		{
			_switcherBookmark->SetActiveWidgetIndex(1);
		}
	}
}

void UGsUIPopupPlayerStat::InvalidateEquipSlot(EGsEquipSlotType InSlotType)
{
	_equipInfo->InvalidateSlot(InSlotType);
}

void UGsUIPopupPlayerStat::InvalidateEquipSlotAll()
{
	_equipInfo->InvalidateSlotAll();
}

void UGsUIPopupPlayerStat::InvalidateStatDistributionRedDot(bool InIsActive)
{
	if (_statDistributionRedDot)
	{
		FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance();
		if (nullptr == statHandler)
			return;

		FGsPlayerStatDistributionController* distributionController = statHandler->GetStatDistributionController();
		if (nullptr == distributionController)
			return;

		int32 bonusStatPoint = distributionController->GetCurrentRemainBonusStatPoint(); // 분배중인 스탯
		//int32 bonusStatPoint = GGameData()->GetUserData()->GetStatPoint(); // 분배하지 않은 스탯
		bool isActive = bonusStatPoint > 0;
		_statDistributionRedDot->SetRedDot(isActive, EGsIconRedDotType::NUMBER, bonusStatPoint);
	}
}

void UGsUIPopupPlayerStat::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (nullptr == paramData)
		return;

	if (0 == paramData->_itemDBID)
		return;

	EGsEquipSlotType slotType = GItem()->GetEquippedSlotTypeByDBID(paramData->_itemDBID);
	if (EGsEquipSlotType::SLOT_MAX == slotType)
		return;

	InvalidateEquipSlot(slotType);
}

void UGsUIPopupPlayerStat::OnInvalidatePlayerStat(const IGsMessageParam* InParam)
{
	_invalidateFlags |= EGsPlayerStatInvalidateFlags::STAT_DEFAULT;
	_invalidateFlags |= EGsPlayerStatInvalidateFlags::STAT_BOOKMARK;
	InvalidateAll();
}

void UGsUIPopupPlayerStat::OnInvalidatePlayerStatDistribution(const IGsMessageParam* InParam)
{
	_invalidateFlags |= EGsPlayerStatInvalidateFlags::RED_DOT;
	InvalidateAll();
}

void UGsUIPopupPlayerStat::OnInvalidateEquip(FGsPlayerDetailInfoMessageParamBase* InParam)
{
	if (const FGsInvalidateEquipSlotInfo* param = StaticCast<const FGsInvalidateEquipSlotInfo*>(InParam))
	{
		InvalidateEquipSlot(param->_slotType);
	}
}

void UGsUIPopupPlayerStat::OnInvalidateEquipAll(FGsPlayerDetailInfoMessageParamBase* InParam)
{
	InvalidateEquipSlotAll();
}

void UGsUIPopupPlayerStat::OnPlayerStatUpdate(const IGsMessageParam* InParam)
{
	do 
	{
		FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance();
		if (nullptr == statHandler)
			break;

		const TArray<StatType>& statTypeList = statHandler->GetBookmarkStatController()->GetBookmarkStat();
		if (0 >= statTypeList.Num())
			break;

		const FGsGameObjectMessageParamPlayerStat* param = InParam->Cast<const FGsGameObjectMessageParamPlayerStat>();
		if (nullptr == param)
			break;

		for (const TPair<StatType, int32>& stat : param->StatMap)
		{
			int32 index = statTypeList.Find(stat.Key);
			if (INDEX_NONE != index)
			{
				_switcherBookmark->SetActiveWidgetIndex(0);
				_slotHelperBookmark->RefreshByIndex(index);
			}
		}
	} while (0);

	_invalidateFlags |= EGsPlayerStatInvalidateFlags::STAT_DEFAULT;
	InvalidateAll();
}

void UGsUIPopupPlayerStat::OnClickClose()
{
	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);


	Close();
}

void UGsUIPopupPlayerStat::OnClickStatDistribution()
{
	UGsUIPopupPlayerStatDetail* popupStatDetail = Cast<UGsUIPopupPlayerStatDetail>(GUI()->OpenAndGetWidget(TEXT("PopupPlayerStatDetail")));
	if (nullptr == popupStatDetail)
		return;

	popupStatDetail->SetTab(EGsPlayerStatDetailType::Distribution);
}

void UGsUIPopupPlayerStat::OnClickStatDetailInfo()
{
	UGsUIPopupPlayerStatDetail* popupStatDetail = Cast<UGsUIPopupPlayerStatDetail>(GUI()->OpenAndGetWidget(TEXT("PopupPlayerStatDetail")));
	if (nullptr == popupStatDetail)
		return;

	popupStatDetail->SetTab(EGsPlayerStatDetailType::Detail);
}

void UGsUIPopupPlayerStat::OnRefreshBookmark(int32 InIndex, UWidget* InEntry)
{
	UGsUISimpleTwoTextEntry* entry = Cast< UGsUISimpleTwoTextEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance())
	{
		const TArray<StatType>& statTypeList = statHandler->GetBookmarkStatController()->GetBookmarkStat();

		if (false == statTypeList.IsValidIndex(InIndex))
			return;

		FText statNameText;
		if (false == FGsStatHelper::GetDisPlayStatName(statTypeList[InIndex], statNameText))
			return;
		
		FText statValueText;
		if (false == FGsPlayerStatHandler::TryGetPlayerStatText(statTypeList[InIndex], statValueText))
			return;
		
		entry->SetTextFront(statNameText);
		entry->SetTextBack(statValueText);
	}
}

void UGsUIPopupPlayerStat::SetMainStatValueText(StatType InStatType)
{
	if (_statTextBlockMap.Contains(InStatType))
	{
		FText statValueText;
		if (FGsPlayerStatHandler::TryGetPlayerStatText(InStatType, statValueText))
		{
			_statTextBlockMap[InStatType]->SetText(statValueText);
		}
	}
}

void UGsUIPopupPlayerStat::OnInputCancel()
{
	OnClickClose();
}