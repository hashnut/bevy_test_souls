//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsPlayerDetailInfoUIMsgReceiver.h"
//
//#include "Management/GsMessageHolder.h"
//
//#include "T1Project/GameObject/Stat/GsStatInfo.h"
//#include "T1Project/GameObject/Stat/GsStatHelper.h"
//
//
//FGsPlayerDetailInfoUIMsgReceiver::~FGsPlayerDetailInfoUIMsgReceiver()
//{
//	_playerDetailUI.Reset();
//	RemoveMessage();
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::RemoveMessage()
//{
//	FGsMessageHolder* msg = GMessage();
//
//	for (TPair<MessageContentPlayerInfo, FDelegateHandle>& elemItem : _listEventDelegate)
//	{
//		msg->GetPlayerDetailInfo().Remove(elemItem);
//	}
//	_listEventDelegate.Empty();
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::RegisterMessage()
//{
//	FGsMessageHolder* msg = GMessage();
//
//	_listEventDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::INVALIDATE_EQUIP_ITEM,
//		this, &FGsPlayerDetailInfoUIMsgReceiver::UpdateEquippedItem));
//	_listEventDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::INVALIDATE_EQUIP_ITEM_LIST,
//		this, &FGsPlayerDetailInfoUIMsgReceiver::UpdateEquippedItemList));
//	_listEventDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::INVALIDATE_WEAPON_TYPE,
//		this, &FGsPlayerDetailInfoUIMsgReceiver::UpdateClassImage));
//	_listEventDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::INVALIDATE_STAT_LIST,
//		this, &FGsPlayerDetailInfoUIMsgReceiver::UpdatePlayerStatList));
//
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::UpdateEquippedItem(FGsPlayerDetailInfoMessageParamBase* InData)
//{
//	if (_playerDetailUI.IsValid())
//	{
//		FGsUpdateEquippedInfo* paramData = static_cast<FGsUpdateEquippedInfo*>(InData);
//		if (nullptr == paramData)
//			return;
//
//		_playerDetailUI->UpdateEquippedItem(paramData->_equippedItem);
//	}
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::UpdateEquippedItemList(FGsPlayerDetailInfoMessageParamBase* InData)
//{
//	if (_playerDetailUI.IsValid())
//	{
//		FGsUpdateEquippedInfoList* paramData = static_cast<FGsUpdateEquippedInfoList*>(InData);
//		if (nullptr == paramData)
//			return;
//
//		_playerDetailUI->UpdateEquippedItemList(paramData->_equippedItemList);
//	}
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::UpdateClassImage(FGsPlayerDetailInfoMessageParamBase* InData)
//{
//	if (_playerDetailUI.IsValid())
//	{
//		FGsUpdateClassInfo* paramData = static_cast<FGsUpdateClassInfo*>(InData);
//		if (nullptr == paramData)
//			return;
//
//		_playerDetailUI->UpdatePlayerClassImage(paramData->_eCreatureWeaponType);
//	}
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::UpdatePlayerStatList(FGsPlayerDetailInfoMessageParamBase* InData)
//{
//	if (_playerDetailUI.IsValid())
//	{
//		FGsUpdatePlayerStatInfo* paramData = static_cast<FGsUpdatePlayerStatInfo*>(InData);
//		if (nullptr == paramData)
//			return;
//
//		FGsStatHelper::SortUiOrder(paramData->_statInfoList);
//		TArray<StatType> statTypeList;
//		TArray<int32> statValueList;
//		for (int32 i = paramData->_statInfoList.Num() -1 ; i >= 0; --i)
//		{
//			if(false == paramData->_statInfoList.IsValidIndex(i))
//				continue;
//
//			if(nullptr == paramData->_statInfoList[i])
//				continue;
//
//			const FGsStatInfo* statInfo = paramData->_statInfoList[i];
//			statTypeList.Emplace(statInfo->GetStatType());
//			statValueList.Emplace(statInfo->GetStatValue());
//		}
//
//		_playerDetailUI->UpdatePlayerStatList(statTypeList , statValueList);
//	}
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::Send_UpdateEquippedItemList() const
//{
//	GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::UPDATE_EQUIPED_ITEM, nullptr);
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::Send_UpdateClassImage() const
//{
//	GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::UPDATE_WEAPON_TYPE, nullptr);
//}
//
//void FGsPlayerDetailInfoUIMsgReceiver::Send_UpdatePlayerStatList() const
//{
//	GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::UPDATE_STAT, nullptr);
//}