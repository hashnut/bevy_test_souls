#include "GsNetMessageHandlerFairy.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Message/MessageParam/GsFairyMessageParam.h"

#include "Item/GsItemManager.h"
#include "Item/InputBlock/GsInputBlockDefine.h"
#include "Item/InputBlock/GsInputBlockHandler.h"

#include "Fairy/GsFairyData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Currency/GsCurrencyHelper.h"

#include "GameMode/GsGameModeWorld.h"

#include "Summon/GsSummonDefine.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include <vector>

using namespace PD::SC;

void FGsNetMessageHandlerFairy::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&worldProtocal);

	//gameobject handler가 처리하게 함
	//AddMessageDelegate(worldProtocal.AddRaw(
	//	SCPacketId::SC_SPAWN_ME, this, &FGsNetMessageHandlerFairy::SpawnMeAck)
	//);

	//AddMessageDelegate(worldProtocal.AddRaw(
	//	SCPacketId::SC_SPAWN_USER, this, &FGsNetMessageHandlerFairy::SpawnUserAck)
	//);

	//AddMessageDelegate(worldProtocal.AddRaw(
	//	SCPacketId::SC_USER_LOOK_INFO_CHANGED, this, &FGsNetMessageHandlerFairy::UserLookInfoChangeAck)
	//);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY, this, &FGsNetMessageHandlerFairy::NotifyFairyList)
	);
		
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY_CREATE, this, &FGsNetMessageHandlerFairy::NotifyCreatedFairy)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY_DELETE, this, &FGsNetMessageHandlerFairy::NotifyDeletedFairy)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_EQUIP, this, &FGsNetMessageHandlerFairy::AckFairyEquip)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY_COLLECTION, this, &FGsNetMessageHandlerFairy::NotifyFairyCollectionList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_ENCHANT, this, &FGsNetMessageHandlerFairy::AckFairyEnchant)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_COMPOSE, this, &FGsNetMessageHandlerFairy::AckFairyCompose)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY_SUMMON_WAIT, this, &FGsNetMessageHandlerFairy::NotifyFairySummonWaitList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_SUMMON_CHANGE, this, &FGsNetMessageHandlerFairy::AckFairySummonWaitChange)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_SUMMON_CONFIRM, this, &FGsNetMessageHandlerFairy::AckFairySummonConfirm)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_FAIRY_CEILING, this, &FGsNetMessageHandlerFairy::AckFairyCeiling)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_FAIRY_OBTAIN, this, &FGsNetMessageHandlerFairy::NotifyFairyObtain)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_MOUNT_VEHICLE, this, &FGsNetMessageHandlerFairy::NotifyMountVehicle)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_UNMOUNT_VEHICLE, this, &FGsNetMessageHandlerFairy::NotifyUnmountVehicle)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_DELIVERY_BOX_RETRY_FAIRY_COMPOSE, this, &FGsNetMessageHandlerFairy::AckFairyComposeDeliveryBoxRetry)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_MOUNT_VEHICLE_FAILURE, this, &FGsNetMessageHandlerFairy::AckMountVehicleFailure)
	);
}

void FGsNetMessageHandlerFairy::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerFairy::NotifyFairyList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_FAIRY_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_READ*>(inPacket.Buffer))
	{
		auto gameDataManager = GGameData();
		check(gameDataManager);

		gameDataManager->Set(*pkt);
		if (pkt->Completed())
		{ 
			if (auto fairyManager = GSFairy())
			{
				fairyManager->UpdateFairyDatatoItem(
					gameDataManager->GetFairyRawData(),
					gameDataManager->GetFairyRawDataSize(),
					gameDataManager->IsFairyDataComplete() );

				gameDataManager->ResetFairyData();
			}
		}
	}
}

void FGsNetMessageHandlerFairy::NotifyCreatedFairy(const FGsNet::Packet& inPacket) //update list
{
	if (PKT_SC_FAIRY_CREATE_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_CREATE_READ*>(inPacket.Buffer))
	{
		TArray<FairyData> createList;
		TArray<FairyIdAmountPair> amountList;

		pkt->MakeFairyDataCreatedVector<TArray<FairyData>, FairyData>(createList);
		if(createList.Num() > 0) GSFairy()->UpdateFairyItemList(createList, true, true);

		pkt->MakeFairyAmountIncreasedVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(amountList);
		if (amountList.Num() > 0) GSFairy()->UpdateAmountFairyItemList(amountList);
	}
}

void FGsNetMessageHandlerFairy::NotifyDeletedFairy(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_FAIRY_DELETE_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_DELETE_READ*>(inPacket.Buffer))
	{
		//삭제는 없으므로 수량 감소만
		TArray<FairyIdAmountPair> amountList;
		pkt->MakeFairyAmountDecreasedVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(amountList);
		if (amountList.Num() > 0) GSFairy()->UpdateAmountFairyItemList(amountList);
	}
}

void FGsNetMessageHandlerFairy::NotifyFairyCollectionList(const FGsNet::Packet& inPacket)//fairy collection info 
{
	if (PKT_SC_FAIRY_COLLECTION_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_COLLECTION_READ*>(inPacket.Buffer))
	{
		if (pkt->Refresh()) //진입 초기에 한번
		{
			auto gameDataManager = GGameData();
			check(gameDataManager);

			gameDataManager->Set(*pkt);

			if (auto fairyManager = GSFairy())
			{
				fairyManager->UpdateFairyCollectionCompleteList(gameDataManager->GetFairyCollectionCompleteList());
				fairyManager->UpdateFairyCollectionCompleteConditionList(gameDataManager->GetFairyCollectionCompleteConditionList());

				gameDataManager->ResetFairyCollectionData();
			}
		}
		else //변동이 있을때
		{
			if (auto fairyManager = GSFairy())
			{
				TArray<CollectionId>	collectionComplete;
				TArray<CollectionConditionId> collectionCompleteCondition;

				pkt->MakeCollectionIdListVector<TArray<CollectionId>, CollectionId>(collectionComplete);
				pkt->MakeCollectionConditionIdListVector<TArray<CollectionConditionId>, CollectionConditionId>(collectionCompleteCondition);

				fairyManager->UpdateFairyCollectionCompleteList(collectionComplete, true);
				fairyManager->UpdateFairyCollectionCompleteConditionList(collectionCompleteCondition, true);
			}
		}
	}
}

void FGsNetMessageHandlerFairy::NotifyFairySummonWaitList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_FAIRY_SUMMON_WAIT_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_SUMMON_WAIT_READ*>(inPacket.Buffer))
	{
		if (pkt->Refresh()) //진입 초기에 한번
		{
			auto gameDataManager = GGameData();
			check(gameDataManager);

			gameDataManager->Set(*pkt);

			if (auto fairyManager = GSFairy())
			{
				fairyManager->UpdateFairySummonWaitingItemList(
					gameDataManager->GetFairySummonWaitingList(),
					gameDataManager->GetFairySummonWaitingsPacketTime());

				gameDataManager->ResetFairySummonWaitingList();
			}
		}
		else
		{	
			TArray<FairyConfirmData> arr;
			if (auto fairyManager = GSFairy())
			{
				pkt->MakeFairyConfirmDataListVector<TArray<FairyConfirmData>, FairyConfirmData>(arr);
				fairyManager->UpdateFairySummonWaitingItemList(arr, FGsTimeSyncUtil::GetServerNowDateTimeUTC(), true);
			}
		}
	}
}
void FGsNetMessageHandlerFairy::NotifyFairyObtain(const FGsNet::Packet& inPacket) //fairy 획득
{

	if (PKT_SC_FAIRY_OBTAIN_READ* pkt = reinterpret_cast<PKT_SC_FAIRY_OBTAIN_READ*>(inPacket.Buffer))
	{
		TArray<FairyId> fairyObtainResult;
		pkt->MakeFairyIdObtainedVector<TArray<FairyId>, FairyId>(fairyObtainResult);

		if (fairyObtainResult.Num() == 0)
		{
			// 갯수가 0이다
			return;
		}

		FairyObtainReason reason = pkt->ObtainReason();

		if (reason == FairyObtainReason::USE_ITEM)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_ITEM);
		}
		else if(reason == FairyObtainReason::COMPOSE)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_COMPOSE);
			GSSummon()->SetNotUseRetry();
		}
		else if (reason == FairyObtainReason::CEILING)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_CEILING);
			GSSummon()->SetNotUseRetry();
		}
		else
		{
			return;
		}
		// 추가된 마일리지값 저장
		GSSummon()->SetAddedSummonMileage(pkt->ObtainSummonMileage());

		GSFairy()->OnUpdateObtainedFairyList(pkt->ObtainReason(),fairyObtainResult); //받는 곳에서 실행하도록
	}
}
void FGsNetMessageHandlerFairy::NotifyMountVehicle(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_MOUNT_VEHICLE_READ* pkt = reinterpret_cast<PKT_SC_MOUNT_VEHICLE_READ*>(inPacket.Buffer))
	{
		// 발동 애니메이션은 응답받을때 한번만 처리
		if (UGsGameObjectBase* findObject = GSGameObject()->FindObject(EGsGameObjectType::Player, pkt->GameId()))
		{
			UGsGameObjectPlayer* castObject = findObject->CastGameObject<UGsGameObjectPlayer>();
			castObject->OnVehicleStartAnimation();
		}
	}
}
void FGsNetMessageHandlerFairy::NotifyUnmountVehicle(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_UNMOUNT_VEHICLE_READ* pkt = reinterpret_cast<PKT_SC_UNMOUNT_VEHICLE_READ*>(inPacket.Buffer))
	{

	}
}

void FGsNetMessageHandlerFairy::AckFairyEquip(const FGsNet::Packet& inPacket) //InGame equip 변경
{
	if (PKT_SC_ACK_FAIRY_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_EQUIP_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();

		if (result == PACKET_RESULT_SUCCESS)
		{
			GSFairy()->FairyEquipChange(pkt->EquipFairyId()); //내 페어리에 대한 처리 UserLookInfoChangeAck 로 처리되는 것으로		
		}
		else
		{
			if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairyEnchant(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_FAIRY_ENCHANT_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_ENCHANT_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckFairyEnchant"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			auto enchantResult = pkt->EnchantResult();

			GSFairy()->UpdateFairyItem(FairyData{pkt->FairyId(), pkt->Level(), MAX_FAIRY_LEVEL + 1, MAX_ITEM_AMOUNT + 1, 1 << MAX_PRESET_ID }, true);

			//collectionConditionIdList //나중에... 아직 패킷에 포함되어 있지 않음
			GSFairy()->OnAckEnchantResult(pkt->FairyId(), enchantResult);
		}
		else
		{
			GSFairy()->OnAckEnchantResult(INVALID_FAIRY_ID, FairyEnchantResult::NONE);

			if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairyCompose(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_FAIRY_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_COMPOSE_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckFairyCompose"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<FairyData> createFairyData;
			pkt->MakeFairyDataCreatedVector<TArray<FairyData>, FairyData>(createFairyData);
			GSFairy()->UpdateFairyItemList(createFairyData, true, true);

			TArray<FairyIdAmountPair> fairyAmoutPair;
			pkt->MakeFairyAmountResultVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(fairyAmoutPair);
			GSFairy()->UpdateAmountFairyItemList(fairyAmoutPair);

			TArray<ItemIdAmountPair> receiveItems;

			pkt->MakeCeilingMaterialItmsVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(receiveItems);
			GSFairy()->AckFairyComposeRewardList(receiveItems);
		}
		else
		{
			GSFairy()->SetEffectPlaying(false);

			if (result == PD::Result::FAIRY_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairySummonWaitChange(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_FAIRY_SUMMON_CHANGE_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_SUMMON_CHANGE_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			// 여기는 대기 변경쪽만 오므로 여기에서 세팅 하겠습니다.
			// 어디서부턴 소환인지 컨텐츠 타입 저장
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_WAIT_CHANGE);
			// 합성 페어리 소환은 재시도 없음
			GSSummon()->SetNotUseRetry();

			GSFairy()->ChangeFairySummonWaitingItem(pkt->FairyConfirmData());
		}
		else
		{
			GSFairy()->SetEffectPlaying(false);

			if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairySummonConfirm(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_FAIRY_SUMMON_CONFIRM_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_SUMMON_CONFIRM_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<FairyData> createList;
			TArray<FairyIdAmountPair> amountList;

			pkt->MakeFairyDataCreatedVector<TArray<FairyData>, FairyData>(createList);
			if (createList.Num() > 0) GSFairy()->UpdateFairyItemList(createList, true, true);

			pkt->MakeFairyAmountIncreasedVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(amountList);
			TArray<FairyData> addList;
			for (auto pair : amountList)
			{
				addList.Add(FairyData{ pair.mFairyId, INVALID_LEVEL, INVALID_LEVEL, pair.mAmount, 1 << MAX_PRESET_ID });
			}
			GSFairy()->UpdateFairyItemList(addList, true, true);
			GSFairy()->RemoveFairySummonWaitingItem(pkt->FairyConfirmId());
		}
		else
		{
			GSFairy()->SetEffectPlaying(false);

			if (result == PD::Result::FAIRY_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull_Confirm"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairyComposeDeliveryBoxRetry(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_DELIVERY_BOX_RETRY_FAIRY_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_DELIVERY_BOX_RETRY_FAIRY_COMPOSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	auto result = pkt->Result();
	if (result == PACKET_RESULT_SUCCESS)
	{
		TArray<FairyData> createFairyData;
		pkt->MakeFairyDataCreatedVector<TArray<FairyData>, FairyData>(createFairyData);
		if (createFairyData.Num() > 0) GSFairy()->UpdateFairyItemList(createFairyData, true, true);

		TArray<FairyIdAmountPair> fairyAmoutPair;
		pkt->MakeFairyAmountResultVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(fairyAmoutPair);
		if (fairyAmoutPair.Num() > 0) GSFairy()->UpdateAmountFairyItemList(fairyAmoutPair);
	}
	else
	{
		if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowRecompose"));
		if (widget.IsValid())
		{
			widget->Close();
		}
	}
}

void FGsNetMessageHandlerFairy::AckFairyCeiling(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_FAIRY_CEILING_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAIRY_CEILING_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckFairyCeiling"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<FairyData> createFairyData;
			pkt->MakeFairyDataCreatedVector<TArray<FairyData>, FairyData>(createFairyData);
			GSFairy()->UpdateFairyItemList(createFairyData, true, true);

			TArray<FairyIdAmountPair> fairyAmoutPair;
			pkt->MakeFairyAmountIncreasedVector<TArray<FairyIdAmountPair>, FairyIdAmountPair>(fairyAmoutPair);
			GSFairy()->UpdateAmountFairyItemList(fairyAmoutPair);

			GSFairy()->OnAckCeilingResult(pkt->EnchantResult());
		}
		else
		{
			GSFairy()->SetEffectPlaying(false);

			if (result == PD::Result::FAIRY_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSFairy()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerFairy::AckMountVehicleFailure(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_MOUNT_VEHICLE_FAILURE_READ* pkt = reinterpret_cast<PKT_SC_ACK_MOUNT_VEHICLE_FAILURE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* target = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				FText result;
				FString errMessage = PD::ResultEnumToString(static_cast<PD::Result>(pkt->Result()));
				FText::FindText(TEXT("NetText"), FTextKey(errMessage), result);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, result);
				
				// 실패시 타입별로 다시 검사하고 이상 없으면 재요청 처리
				UGsGameObjectLocalPlayer* localPlayer = target->CastGameObject<UGsGameObjectLocalPlayer>();
				localPlayer->UpdateAllFairyVehicleCondition();
				localPlayer->OnVehicleSendTimer(1.f);
			}
		}
	}
}

//void FGsNetMessageHandlerFairy::SpawnMeAck(const FGsNet::Packet& inPacket)
//{
//	if (PKT_SC_SPAWN_ME_READ* pkt = reinterpret_cast<PKT_SC_SPAWN_ME_READ*>(inPacket.Buffer))
//	{
//		//GameObjectHandler에서 처리
//	}
//}
//
//void FGsNetMessageHandlerFairy::SpawnUserAck(const FGsNet::Packet& inPacket)
//{
//	if (PKT_SC_SPAWN_USER_READ* pkt = reinterpret_cast<PKT_SC_SPAWN_USER_READ*>(inPacket.Buffer))
//	{
//		//GameObjectHandler에서 처리		
//	}
//}
//
//void FGsNetMessageHandlerFairy::UserLookInfoChangeAck(const FGsNet::Packet& inPacket) 
//{
//	if (PKT_SC_USER_LOOK_INFO_CHANGED_READ* pkt =
//		reinterpret_cast<PKT_SC_USER_LOOK_INFO_CHANGED_READ*>(inPacket.Buffer))
//	{	
//		//GameObjectHandler에서 처리
//
//		if (GGameData()->GetUserData()->mGameId == pkt->GameId()) 
//		{
//			GSFairy()->FairyEquipChange(pkt->LookInfo().mFairy);
//		}
//	}
//}
