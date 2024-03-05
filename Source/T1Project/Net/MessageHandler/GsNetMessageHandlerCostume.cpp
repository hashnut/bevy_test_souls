#include "GsNetMessageHandlerCostume.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
//#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Costume/GsCostumeData.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Currency/GsCurrencyHelper.h"

#include "Item/GsItemManager.h"
#include "Item/InputBlock/GsInputBlockDefine.h"
#include "Item/InputBlock/GsInputBlockHandler.h"

#include "Summon/GsSummonDefine.h"


using namespace PD::SC;

void FGsNetMessageHandlerCostume::InitializeMessage(FGsNetManager* inManager)
{
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	InitializeMessageDelegateList(&worldProtocal);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME, this, &FGsNetMessageHandlerCostume::NotifyCostumeList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME_CREATE, this, &FGsNetMessageHandlerCostume::NotifyCreatedCostume)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME_DELETE, this, &FGsNetMessageHandlerCostume::NotifyDeletedCostume)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME_COLLECTION, this, &FGsNetMessageHandlerCostume::NotifyCostumeCollectionList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME_SUMMON_WAIT, this, &FGsNetMessageHandlerCostume::NotifyCostumeSummonWaitList)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_COSTUME_EQUIP, this, &FGsNetMessageHandlerCostume::AckCostumeEquip)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_COSTUME_COMPOSE, this, &FGsNetMessageHandlerCostume::AckCostumeCompose)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_COSTUME_SUMMON_CHANGE, this, &FGsNetMessageHandlerCostume::AckCostumeSummonWaitChange)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_COSTUME_SUMMON_CONFIRM, this, &FGsNetMessageHandlerCostume::AckCostumeSummonConfirm)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COSTUME_OBTAIN, this, &FGsNetMessageHandlerCostume::NotifyCostumeObtain)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_DELIVERY_BOX_RETRY_COSTUME_COMPOSE, this, &FGsNetMessageHandlerCostume::AckCostumeComposeDeliveryBoxRetry)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_COSTUME_CEILING, this, &FGsNetMessageHandlerCostume::AckCostumeCeiling)
	);

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_CHANGE_COSTUME, this, &FGsNetMessageHandlerCostume::AckExchangeCostume)
	);
}

void FGsNetMessageHandlerCostume::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerCostume::NotifyCostumeList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_COSTUME_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_READ*>(inPacket.Buffer))
	{
		auto gameDataManager = GGameData();
		check(gameDataManager);

		gameDataManager->Set(*pkt);
		if (pkt->Completed())
		{
			if (auto costumeManager = GSCostume())
			{
				costumeManager->UpdateCostumeDatatoItem(
					gameDataManager->GetCostumeRawData(),
					gameDataManager->GetCostumeRawDataSize(),
					gameDataManager->IsCostumeDataComplete());

				gameDataManager->ResetCostumeData();
			}
		}
	}

}

void FGsNetMessageHandlerCostume::NotifyCreatedCostume(const FGsNet::Packet& inPacket) 
{
	if (PKT_SC_COSTUME_CREATE_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_CREATE_READ*>(inPacket.Buffer))
	{
		TArray<CostumeData> createList;
		TArray<CostumeData> amountList;

		pkt->MakeCostumeDataListVector<TArray<CostumeData>, CostumeData>(createList);
		if (createList.Num() > 0) GSCostume()->UpdateCostumeItemList(createList, false, true);
	}
}

void FGsNetMessageHandlerCostume::NotifyDeletedCostume(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_COSTUME_DELETE_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_DELETE_READ*>(inPacket.Buffer))
	{
		//삭제는 없으므로 수량 감소만
		TArray<CostumeData> amountList;
		pkt->MakeCostumeDataListVector<TArray<CostumeData>, CostumeData>(amountList);
		if (amountList.Num() > 0) GSCostume()->UpdateCostumeItemList(amountList, false, true);
	}
}

void FGsNetMessageHandlerCostume::NotifyCostumeCollectionList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_COSTUME_COLLECTION_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_COLLECTION_READ*>(inPacket.Buffer))
	{
		if (pkt->Refresh()) //진입 초기에 한번
		{
			auto gameDataManager = GGameData();
			check(gameDataManager);

			gameDataManager->Set(*pkt);

			if (auto costumeManager = GSCostume())
			{
				costumeManager->UpdateCollectionCompleteList(gameDataManager->GetCostumeCollectionCompleteList());
				costumeManager->UpdateCollectionCompleteConditionList(gameDataManager->GetCostumeCollectionCompleteConditionList());

				gameDataManager->ResetCostumeCollectionData();
			}
		}
		else //변동이 있을때
		{
			if (auto costumeManager = GSCostume())
			{
				TArray<CollectionId>	collectionComplete;
				TArray<CollectionConditionId> collectionCompleteCondition;

				pkt->MakeCollectionIdListVector<TArray<CollectionId>, CollectionId>(collectionComplete);
				pkt->MakeCollectionConditionIdListVector<TArray<CollectionConditionId>, CollectionConditionId>(collectionCompleteCondition);

				costumeManager->UpdateCollectionCompleteList(collectionComplete, true);
				costumeManager->UpdateCollectionCompleteConditionList(collectionCompleteCondition, true);
			}
		}
	}
}

void FGsNetMessageHandlerCostume::NotifyCostumeSummonWaitList(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_COSTUME_SUMMON_WAIT_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_SUMMON_WAIT_READ*>(inPacket.Buffer))
	{
		if (pkt->Refresh()) //진입 초기에 한번
		{
			auto gameDataManager = GGameData();
			check(gameDataManager);

			gameDataManager->Set(*pkt);

			if (auto costumeManager = GSCostume())
			{
				costumeManager->UpdateCostumeSummonWaitingItemList(
					gameDataManager->GetCostumeSummonWaitingList(),
					gameDataManager->GetCostumeSummonWaitingsPacketTime());

				gameDataManager->ResetCostumeSummonWaitingList();
			}
		}
		else
		{
			TArray<CostumeConfirmData> arr;
			if (auto costumeManager = GSCostume())
			{
				pkt->MakeCostumeConfirmDataListVector<TArray<CostumeConfirmData>, CostumeConfirmData>(arr);
				costumeManager->UpdateCostumeSummonWaitingItemList(arr, FGsTimeSyncUtil::GetServerNowDateTimeUTC(), true);
			}
		}
	}
}

void FGsNetMessageHandlerCostume::NotifyCostumeObtain(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_COSTUME_OBTAIN_READ* pkt = reinterpret_cast<PKT_SC_COSTUME_OBTAIN_READ*>(inPacket.Buffer))
	{
		TArray<CostumeId> obtainResult;
		pkt->MakeCostumeIdObtainedVector<TArray<CostumeId>, CostumeId>(obtainResult);

		if (obtainResult.Num() == 0)
		{
			// 갯수가 0이다
			return;
		}

		CostumeObtainReason reason = pkt->ObtainReason();

		UGsSummonManager* summonManager = GSSummon();
		if (summonManager == nullptr)
		{
			return;
		}

		if (reason == CostumeObtainReason::USE_ITEM)
		{
			summonManager->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_ITEM);
		}
		else if (reason == CostumeObtainReason::COMPOSE)
		{
			summonManager->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_COMPOSE);
			summonManager->SetNotUseRetry();
		}
		else if (reason == CostumeObtainReason::CEILING)
		{
			summonManager->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_CEILING);
			summonManager->SetNotUseRetry();
		}
		else if (reason == CostumeObtainReason::EXCHANGE)
		{
			summonManager->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_EXCHANGE);
			summonManager->SetNotUseRetry();
		}
		else
		{
			return;
		}
		// 추가된 마일리지값 저장
		summonManager->SetAddedSummonMileage(pkt->ObtainSummonMileage());

		if (UGsCostumeManager* costumeManager = GSCostume())
		{
			costumeManager->UpdateObtainedCostumeList(reason, obtainResult); //받는 곳에서 실행하도록
		}
	}
}

void FGsNetMessageHandlerCostume::AckCostumeEquip(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_COSTUME_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_ACK_COSTUME_EQUIP_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();

		if (result == PACKET_RESULT_SUCCESS)
		{
			GSCostume()->CostumeEquipChange(pkt->EquipCostumeId());
		}
		else
		{
			if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerCostume::AckCostumeCompose(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_COSTUME_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_COSTUME_COMPOSE_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckCostumeCompose"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<CostumeData> createCostumeData;
			pkt->MakeCostumeDataCreatedVector<TArray<CostumeData>, CostumeData>(createCostumeData);
			GSCostume()->UpdateCostumeItemList(createCostumeData, true, true);

			TArray<CostumeIdAmountPair> costumeAmountPair;
			pkt->MakeCostumeAmountResultVector<TArray<CostumeIdAmountPair>, CostumeIdAmountPair>(costumeAmountPair);
			GSCostume()->UpdateAmountCostumeItemList(costumeAmountPair);

			TArray<ItemIdAmountPair> receiveItems;
			pkt->MakeCeilingMaterialItmsVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(receiveItems);
			GSCostume()->AckCostumeComposeRewardList(receiveItems);
		}
		else
		{
			GSCostume()->SetEffectPlaying(false);
			if (result == PD::Result::COSTUME_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerCostume::AckCostumeSummonWaitChange(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_COSTUME_SUMMON_CHANGE_READ* pkt = reinterpret_cast<PKT_SC_ACK_COSTUME_SUMMON_CHANGE_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_WAIT_CHANGE);
			GSSummon()->SetNotUseRetry();

			GSCostume()->ChangeCostumeSummonWaitingItem(pkt->CostumeConfirmData());
		}
		else
		{
			GSCostume()->SetEffectPlaying(false);
			if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerCostume::AckCostumeSummonConfirm(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_COSTUME_SUMMON_CONFIRM_READ* pkt = reinterpret_cast<PKT_SC_ACK_COSTUME_SUMMON_CONFIRM_READ*>(inPacket.Buffer))
	{
		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<CostumeData> createList;

			pkt->MakeCostumeDataCreatedVector<TArray<CostumeData>, CostumeData>(createList);
			if (createList.Num() > 0) GSCostume()->UpdateCostumeItemList(createList, true, true);

			TArray<CostumeIdAmountPair> costumeAmountPair;
			pkt->MakeCostumeAmountIncreasedVector<TArray<CostumeIdAmountPair>, CostumeIdAmountPair>(costumeAmountPair);
			GSCostume()->UpdateAmountCostumeItemList(costumeAmountPair, false);
			GSCostume()->RemoveCostumeSummonWaitingItem(pkt->CostumeConfirmId());
		}
		else
		{
			GSCostume()->SetEffectPlaying(false);

			if (result == PD::Result::COSTUME_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull_Confirm"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerCostume::AckCostumeComposeDeliveryBoxRetry(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_DELIVERY_BOX_RETRY_COSTUME_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_DELIVERY_BOX_RETRY_COSTUME_COMPOSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	auto result = pkt->Result();
	if (result == PACKET_RESULT_SUCCESS)
	{
		TArray<CostumeData> createCostumeData;
		pkt->MakeCostumeDataCreatedVector<TArray<CostumeData>, CostumeData>(createCostumeData);
		GSCostume()->UpdateCostumeItemList(createCostumeData, true, true);

		TArray<CostumeIdAmountPair> costumeAmountPair;
		pkt->MakeCostumeAmountResultVector<TArray<CostumeIdAmountPair>, CostumeIdAmountPair>(costumeAmountPair);
		GSCostume()->UpdateAmountCostumeItemList(costumeAmountPair);
	}
	else
	{
		if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
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

void FGsNetMessageHandlerCostume::AckCostumeCeiling(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_COSTUME_CEILING_READ* pkt = reinterpret_cast<PKT_SC_ACK_COSTUME_CEILING_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckFairyCeiling"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			TArray<CostumeData> createCostumeData;
			pkt->MakeCostumeDataListVector<TArray<CostumeData>, CostumeData>(createCostumeData);
			GSCostume()->UpdateCostumeItemList(createCostumeData, true, true);

			GSCostume()->OnAckCeilingResult(pkt->EnchantResult());
		}
		else
		{
			GSCostume()->SetEffectPlaying(false);
			if (result == PD::Result::COSTUME_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}

void FGsNetMessageHandlerCostume::AckExchangeCostume(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	if (PKT_SC_ACK_CHANGE_COSTUME_READ* pkt = reinterpret_cast<PKT_SC_ACK_CHANGE_COSTUME_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckExchangeCostume"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS)
		{
			// 아이템 삭제는 따로 진행됨, 코스튬 착용도 따로 진행됨 
			// 스킬 삭제는 ObjectHandler에서 따로 진행됨

			CostumeId selectedCostumeId = pkt->SelectCostumeId();
			TArray<CostumeId> obtainResult = { selectedCostumeId };

			if (UGsCostumeManager* costumeManager = GSCostume())
			{
				costumeManager->UpdateObtainedCostumeList(CostumeObtainReason::EXCHANGE, obtainResult); // 소환 연출 처리

				// 코스튬 수량 변경
				TArray<CostumeData> amountChangedCostumeList;
				pkt->MakeCostumeDataListVector<TArray<CostumeData>, CostumeData>(amountChangedCostumeList);
				if (amountChangedCostumeList.Num() > 0) 
				{
					TArray<CostumeIdAmountPair> pairList;
					Algo::Transform(amountChangedCostumeList, pairList, [](auto& data) { return CostumeIdAmountPair( data.mCostumeId, data.mAmount ); });
					costumeManager->UpdateAmountCostumeItemList(pairList);
				}

				// 코스튬 삭제
				TArray<CostumeId> deleteList;
				pkt->MakeCostumeDeleteListVector<TArray<CostumeId>, CostumeId>(deleteList); 

				for (auto& id : deleteList)
				costumeManager->RemoveCostumeItem(id);

				// 콜렉션 완료된 condition 제거
				TArray<CollectionConditionId> resetConditionList;
				pkt->MakeRemoveCollectionConditionIdListVector<TArray<CollectionConditionId>, CollectionConditionId>(resetConditionList);
				costumeManager->RemovedCollectionCompleteConditionList(resetConditionList);

				// 콜렉션 완료 reset
				TArray<CollectionId> resetCollectionList;
				pkt->MakeRemoveCollectionIdListVector<TArray<CollectionId>, CollectionId>(resetCollectionList);
				costumeManager->UpdateCollectionCompleteResetList(resetCollectionList);

				// 코스튬 교환
				costumeManager->OnAckExchangeCostume(selectedCostumeId);
			}
		}
		else
		{
			GSCostume()->SetEffectPlaying(false);
			if (result == PD::Result::COSTUME_ERROR_MAX_AMOUNT_OVER)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (result == PD::Result::ITEM_ERROR_USABLE_IN_SAFE_ZONE)
			{
				//"마을에서 사용할 수 있습니다"
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_NotInTown"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			else if (!GSCostume()->OnProcessError(static_cast<PD::Result>(result)))
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}
		}
	}
}
