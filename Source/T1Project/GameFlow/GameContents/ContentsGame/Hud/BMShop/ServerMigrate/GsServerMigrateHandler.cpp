#include "GsServerMigrateHandler.h"
#include "GsTable.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateConfigData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "UI/UIContent/Popup/GsUIPopupChangeNameMigration.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Net/GsNetSendServiceBMShop.h"
#include "T1Project.h"
#include "Algo/AllOf.h"
#include "Currency/GsCurrencyHelper.h"

void FGsServerMigrateHandler::Init()
{
	//ClearAll();
	//LoadMigrateConditions();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		_msgGameObjHander =
			messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsServerMigrateHandler::OnLocalPlayerSpawnComplete);

		_msgMigrationHandlerList.Emplace(messageHolder->GetMigration().AddRaw(
			MessageContentsMigration::MIGRATION_NAME_CHANGE, this, &FGsServerMigrateHandler::OnReqMigrationNameChange));
		_msgMigrationHandlerList.Emplace(messageHolder->GetMigration().AddRaw(
			MessageContentsMigration::MIGRATION_GUILD_CHANGE, this, &FGsServerMigrateHandler::OnReqMigrationGuildNameChange));
	}
}

void FGsServerMigrateHandler::Close()
{
	ClearAll();

	_migrateConditions.Reset();
	_migrateGroups.Reset();
	_migrateData.Reset();
	_serverList.Reset();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		if (_msgGameObjHander.Value.IsValid())
		{
			messageHolder->GetGameObject().Remove(_msgGameObjHander);
		}
		
		for (MsgMigrationHandle& elem : _msgMigrationHandlerList)
		{
			messageHolder->GetMigration().Remove(elem);
		}
	}
}

void FGsServerMigrateHandler::OnReconnection()
{
	ClearAll();
}

void FGsServerMigrateHandler::ClearConditionInfo()
{
	_endTIme = FDateTime(0);
	_selectedMigrateServer.Reset();

	_reqServerList = false;

	for (auto e : _serverList) e->Reset();
	for (auto e : _migrateConditions) e->Reset();
}

void FGsServerMigrateHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	ClearAll();
}

ServerMigrateType FGsServerMigrateHandler::ConvertIntToServerMigrateType(int32 InIndex) const
{
	const int32 transformIndex = InIndex + 1;	// 2023/11/3 PKT - enum값 사정에 따라 +1을 더한다.

	if (StaticCast<int32>(ServerMigrateType::NONE) >= transformIndex || StaticCast<int32>(ServerMigrateType::MAX) <= InIndex)
	{
		return ServerMigrateType::NONE;
	}

	return StaticCast<ServerMigrateType>(transformIndex);
}

void FGsServerMigrateHandler::SetConditionState(ServerMigrateType InType, ServerMigrateConditionType InConditionType, bool InFlag, int32 InParam /*= 0*/)
{
	if (InConditionType == ServerMigrateConditionType::SERVER_CHOICE)
	{
		_selectedMigrateServer.Reset();
		if (InFlag)
		{
			if (auto serverIter = _serverList.FindByPredicate([InParam](auto e) { return e->_worldId == InParam; }))
			{
				_selectedMigrateServer = *serverIter;
			}
		}
	}

	if (Migrate* migrateData = _migrateData.Find(InType))
	{
		if (auto conditionIter = _migrateConditions.FindByPredicate([InConditionType](auto e) { return e->_type == InConditionType; }))
		{
			(*conditionIter)->_isPossible = InFlag;

			if (FGsMessageHolder* msg = GMessage())
			{
				msg->GetBMShop().SendMessage(FGsBMShopInvalidateServerMigrage::MESSAGE_ID, FGsSharedParam(nullptr));
			}
		}
	}
}

FGsServerMigrateHandler::ETryReason FGsServerMigrateHandler::IsCheckedMigrateStateAll(ServerMigrateType InType) const
{
	/**
	 * 1. 이주에 대한 서버에게 정보를 받았는가?
	 * 2. 활성화 되어 있는가?
	 * 3. 길드 계정 이주 일때 길드 마스터인가?
	 * 4. 횟수를 초과 하지 않았는가?
	 * 5. 상태가 올바른가?
	 */
	//if (false == _conditionFlgaSet.Contains(InType))
	//{	// 2023/10/31 PKT - 서버에 서버 이주에 대한 정보를 요청한다.
	//	return ETryReason::InvalidWaitRequest;
	//}

	if (_reqServerList == false)
	{
		return ETryReason::InvalidWaitRequest;
	}

	if (_endTIme.GetTicks() == 0 || FGsTimeSyncUtil::GetServerNowDateTimeUTC() >= _endTIme)
	{
		return ETryReason::InvalidDate;
	}

	if (_serverList.Num() <= 0)
	{
		return ETryReason::InvalidServerList;
	}

	if (ServerMigrateType::INCLUDE_GUILD_INFO == InType)
	{
		FGsGuildManager* guildManager = GGuild();
		if (nullptr == guildManager)
		{
			GSLOG(Error, TEXT("nullptr == guildManager"));
			return ETryReason::InvalidGuildMarster;
		}

		// 2023/10/30 PKT - 길드가 있으면서 길드 마스터가 아닌 상황..
		if (guildManager->IsGuild() == false || guildManager->IsGuildMaster() == false)
		{
			return ETryReason::InvalidGuildMarster;
		}
	}
	int32 currentCount = 0;
	for (auto& p : _migrateData) currentCount += p.Value.count;

	if (const FGsSchemaServerMigrateConfigData* configData = GetConfigData())
	{
		if ((currentCount >= configData->totalCount))
		{
			return ETryReason::InvalidOverCount;
		}
	}

	if (auto iter = _migrateData.Find(InType))
	{
		bool isPossible = Algo::AllOf(iter->checkConditonList, [](auto& p) { return p.Value.IsValid() && p.Value.Pin()->_isPossible; });
		if(!isPossible) return ETryReason::InvalidCondition;
	}
	else
	{
		return ETryReason::InvalidWaitRequest;
	}

	if (_selectedMigrateServer.IsValid() == false)
	{
		return ETryReason::InvalidCondition;
	}

	return ETryReason::Valid;
}

EGsHelpContents FGsServerMigrateHandler::FindHelpContetns(ServerMigrateType InType) const
{
	if (ServerMigrateType::ACCOUNT_ONLY == InType)
	{
		return (GetConfigData()) ? GetConfigData()->accountHelpContents : EGsHelpContents::None;
	}
	else if (ServerMigrateType::INCLUDE_GUILD_INFO == InType)
	{
		return (GetConfigData()) ? GetConfigData()->guildHelpContents : EGsHelpContents::None;
	}

	return EGsHelpContents::None;
}

FText FGsServerMigrateHandler::GetTextRemainingTime() const
{
	GSLOG(Log, TEXT("GetTextRemainingTime %s"), *_endTIme.ToIso8601());

	FTimespan remainingTime = _endTIme - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	FString strRemainingTime = int64(remainingTime.GetTotalSeconds()) > 0 ? 
		FGsTimeStringHelper::GetSecondString(remainingTime.GetSeconds()) : TEXT("");

	if (int64(remainingTime.GetTotalMinutes()) > 0)
	{	
		strRemainingTime = FGsTimeStringHelper::GetMinuteString(remainingTime.GetMinutes()) + 
			(strRemainingTime.IsEmpty()  ? TEXT("") : TEXT(" ") + strRemainingTime);
	}
	if (int64(remainingTime.GetTotalHours()) > 0)
	{
		strRemainingTime = FGsTimeStringHelper::GetHourString(remainingTime.GetHours()) +
			(strRemainingTime.IsEmpty() ? TEXT("") : TEXT(" ") + strRemainingTime);
	}
	if (int64(remainingTime.GetTotalDays()) > 0)
	{
		strRemainingTime = FGsTimeStringHelper::GetDayString(remainingTime.GetDays()) +
			(strRemainingTime.IsEmpty() ? TEXT("") : TEXT(" ") + strRemainingTime);
	}
	
	return FText::FromString(strRemainingTime);
}

TPair<int32, int32> FGsServerMigrateHandler::CountInfo() const
{
	int32 currentCount = 0;
	for (auto& p : _migrateData) currentCount +=  p.Value.count;

	const static int32 INVALID_COUNT = 0;
	int32 limitCount = (GetConfigData()) ? GetConfigData()->totalCount : INVALID_COUNT;

	return TPair<int32, int32>(currentCount, limitCount);
}

CurrencyPair FGsServerMigrateHandler::GetCurrencyInfo(ServerMigrateType InType) const
{
	const CurrencyPair UnKnownCurrency = CurrencyPair(CurrencyType::GOLD, 0);
	auto GetCurrency = [UnKnownCurrency](int32 InCount, const TArray<FGsSchemaCurrencyPairData>& InList) -> CurrencyPair
	{
		if (0 > InCount || 0 >= InList.Num())
		{	// 2023/11/3 PKT - 카운트가 0보자 작거나, Data 입력이 되어 있지 않다면.. 이건 잘못된 것!!
			return UnKnownCurrency;
		}

		const int32 findIndex = FMath::Min(InCount, InList.Num() - 1);
		return CurrencyPair(InList[findIndex].currencyType, InList[findIndex].currencyAmount);
	};

	const FGsSchemaServerMigrateConfigData* configData = GetConfigData();
	if (nullptr == configData)
	{
		return UnKnownCurrency;
	}

	TMap<ServerMigrateType, TArray<FGsSchemaCurrencyPairData>> checkCurrency = {
			TPair<ServerMigrateType, TArray<FGsSchemaCurrencyPairData>>{ServerMigrateType::ACCOUNT_ONLY, configData->accountCostSet},
			TPair<ServerMigrateType, TArray<FGsSchemaCurrencyPairData>>{ServerMigrateType::INCLUDE_GUILD_INFO, configData->includeGuildCostSet},
	};

	if (auto migrateIter = _migrateData.Find(InType))
	{
		if(checkCurrency.Find(InType))
		return GetCurrency(migrateIter->count, checkCurrency[InType]);
	}

	return UnKnownCurrency;
}

void FGsServerMigrateHandler::ClearAll()
{
	ClearConditionInfo();
}

const FGsSchemaServerMigrateConfigData* FGsServerMigrateHandler::GetConfigData()
{
	static const FGsSchemaServerMigrateConfigData* outData = nullptr;

	if (nullptr == outData)
	{
		do 
		{
			if (nullptr != outData)
			{
				break;
			}

			const UGsTable* table = FGsSchemaServerMigrateConfigData::GetStaticTable();
			if (nullptr == table)
			{
				GSLOG(Error, TEXT("nullptr == table"));
				break;
			}

			TArray<const FGsSchemaServerMigrateConfigData*> OutRowMap;
			if (false == table->GetAllRows<FGsSchemaServerMigrateConfigData>(OutRowMap))
			{
				GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
				break;
			}

			for (const auto row : OutRowMap)
			{
				outData = row;
				break;
			}

		} while (0);		
	}

	if (nullptr == outData)
	{
		GSLOG(Error, TEXT("not find SchemaConfigData"));
	}

	return outData;
}

int32 FGsServerMigrateHandler::GetCheckGroups(ServerMigrateType type, OUT TArray<TWeakPtr<MigrateGroupItem>>& data)
{
	if (auto iter = _migrateData.Find(type))
	{
		data.Append(iter->groupData);
	}

	return data.Num();
}

void FGsServerMigrateHandler::LoadMigrateConditions()
{
	ClearConditionInfo();

	_migrateConditions.Reset();
	_migrateGroups.Reset();
	_migrateData.Reset();
	
	if (const UGsTable* table = FGsSchemaServerMigrateGroupData::GetStaticTable())
	{
		TArray<const FGsSchemaServerMigrateGroupData*> rows;
		table->GetAllRows<FGsSchemaServerMigrateGroupData>(rows);

		TMap<ServerMigrateType, TArray<const FGsSchemaServerMigrateGroupData*>> tempMap;
		for (auto row : rows)
		{
			tempMap.FindOrAdd(row->type).Emplace(row);
		}

		TMap<ServerMigrateConditionType, TSharedPtr<MigrateConditionItem>> tempMigrateCondition;
		for (auto& p : tempMap)
		{
			auto& migrateInfo = _migrateData.FindOrAdd(p.Key);
			migrateInfo.SetType(p.Key);

			for (const auto e : p.Value)
			{	
				auto groupData = _migrateGroups[_migrateGroups.Add(MakeShared<MigrateGroupItem>(e))];
				migrateInfo.groupData.Add(groupData);

				for (auto conditionRow : e->conditionDataSet)
				{
					if (const FGsSchemaServerMigrateConditionData* condition = conditionRow.GetRow())
					{	
						if (tempMigrateCondition.Find(condition->type) == nullptr)
						{
							tempMigrateCondition.Add(condition->type, MakeShared<MigrateConditionItem>(condition));
						}
						TSharedPtr<MigrateConditionItem> data = tempMigrateCondition[condition->type];
						groupData->_conditions.Add(data);

						migrateInfo.checkConditonList.FindOrAdd(condition->type) = data;
					}
				}
			}
		}

		tempMigrateCondition.GenerateValueArray(_migrateConditions);
		tempMigrateCondition.Reset();

		for (TPair<ServerMigrateType, Migrate>& typeData : _migrateData)
		{	
			typeData.Value.groupData.StableSort([](auto le, auto re) { 
				return le.Pin()->_data->sortNum < re.Pin()->_data->sortNum; });
		}
	}
}

int32 FGsServerMigrateHandler::GetServerList(OUT TArray<TWeakPtr<MigrateServer>>& serverList)
{
	serverList.Append(_serverList);
	return serverList.Num();
}

TWeakPtr<MigrateServer> FGsServerMigrateHandler::FindMigrateServer(WorldId worldId)
{
	if (auto iter = _serverList.FindByPredicate([worldId](auto e) {return e->_worldId == worldId; }))
	{
		return *iter;
	}
	return nullptr;
}

void FGsServerMigrateHandler::SetInitServerMigrateInfo(FDateTime endTime, int accCount, int guildCount)
{
	if (_migrateConditions.Num() <= 0)
	{
		LoadMigrateConditions();
	}

	ClearConditionInfo();

	_endTIme = endTime;

	auto& account = _migrateData.FindOrAdd(ServerMigrateType::ACCOUNT_ONLY);
	account.type = ServerMigrateType::ACCOUNT_ONLY;
	account.count = accCount;

	auto& guild = _migrateData.FindOrAdd(ServerMigrateType::INCLUDE_GUILD_INFO);
	guild.type = ServerMigrateType::INCLUDE_GUILD_INFO;
	guild.count = guildCount;

	_reqServerList = true;
}

void FGsServerMigrateHandler::AddMigrateServerList(MigrateServer data)
{
	if (auto iter = _serverList.FindByPredicate([&data](auto e) { return e.IsValid() && data._planetId == e->_planetId && data._worldId == e->_worldId; }))
	{
		(*iter)->SetData(data._congestion, data._status, data._isRecommended, data._isNewOpenServer, data._userCount);
	}
	else
	{
		TSharedPtr<MigrateServer> server = MakeShared<MigrateServer>(
			data._planetId, data._worldId, data._congestion, data._status, data._isRecommended, data._isNewOpenServer, data._userCount);
		_serverList.Add(server);
	}
}

void FGsServerMigrateHandler::SetAccountServerMigrateData(ServerMigrateConditionType miCondType, bool possible)
{
	if (auto conditionIter = _migrateConditions.FindByPredicate([miCondType](auto e) { return e->_type == miCondType; }))
	{
		(*conditionIter)->_isPossible = possible;
	}
}

void FGsServerMigrateHandler::SetGuildServerMigrateData(ServerMigrateConditionType miCondType, bool possible)
{
	if (auto conditionIter = _migrateConditions.FindByPredicate([miCondType](auto e) { return e->_type == miCondType; }))
	{
		(*conditionIter)->_isPossible = possible;
	}
}

void FGsServerMigrateHandler::SetEndServerMigrateInfo()
{
	_serverList.RemoveAll([](auto e) { return e.IsValid() == false || e->_useCheck == false; });

	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetBMShop().SendMessage(FGsBMShopInvalidateServerMigrage::MESSAGE_ID, FGsSharedParam(nullptr));
	}
}

void FGsServerMigrateHandler::ReqServerMigrateConditionState()
{
	FGsNetSendServiceBMShop::SendServerMigrateConditionList();
}

void FGsServerMigrateHandler::SendServerMigrate(ServerMigrateType type)
{
	const auto& currencyPair = GetCurrencyInfo(type);
	auto costPackage = FGsCostPackage::CreateSimple(currencyPair.mCurrencyType, currencyPair.mCurrency);

	if (!costPackage->PlayerHasEnough())
	{
		FGsCurrencyHelper::OpenLackCostPackagePopup(*costPackage);
		return;
	}

	if (auto server = _selectedMigrateServer.Pin())
	{
		if (auto iter = _migrateData.Find(type))
		{
			if (_endTIme.GetTicks() == 0 || FGsTimeSyncUtil::GetServerNowDateTimeUTC() >= _endTIme)
			{
				FText message;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_Expired"), message);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				return;
			}
			bool isPossible = Algo::AllOf(iter->checkConditonList, [](auto& p) { return p.Value.IsValid() && p.Value.Pin()->_isPossible; });
			if (!isPossible) return;
		}
		else return;

		FGsNetSendServiceBMShop::SendServerMigrate(server->_planetId, server->_worldId, type);
	}
}

bool FGsServerMigrateHandler::OnAckServerMigration(int result)
{
	if (result == PACKET_RESULT_SUCCESS)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowBMShop"));
		if (widget.IsValid()) widget->Close(true);

		FText message; //서버 이전 신청에 성공하였습니다. 게임을 종료 합니다
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_EndGame"), message);
		FGsUIHelper::PopupOK(message, []() {
				GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
			});

		return true;
	}
	else
	{
		using namespace PD;

		// 서버 이전 - 이전 하려는 서버에 인원이 가득 찼습니다.
		if (result == Result::SERVER_MIGRATE_ERROR_TARGET_SERVER_EXCEED_INCOMING_COUNT ||
			//서버 이전 - 현재 서버에 이전 가능 인원이 초과하였습니다
			result == Result::SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT ||

			/* 이전 하려는 서버에 <p>Lv. 50 이상의 캐릭터</>가있어 <pr>서버 이전이 불가능</> 합니다.
				* 이전 하려는 서버에 접속하여 확인해 주세요.
			*
			* (Lv.50 이상인 캐릭터가 <p>삭제 대기</> 상태인 경우 서버 이전 가능) */
			result == Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL ||
			/*
			* 이전하려는 서버의에 <p>기사단에 가입된 캐릭터</>가 있어 <p>서버 이전이 불가능</> 합니다.
			* 이전 하려는 서버에 접속하여 확인해 주세요.
			*/
			result == Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT ||
			// 캐릭터 삭제 실패 - 거래소 판매 및 정산 목록 확인
			result == Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION ||
			result == Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION ||
			// 조건이 맞지 않아 이전 불가능
			result == Result::SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION)
		{
			//FText message;
			//FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_Full_IncomingUser"), message);
			//FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

			ReqServerMigrateConditionState();
			return true;
		}

		/*
		// 서버 이전 - 현재 서버에 이전 가능 인원이 초과하였습니다.
		if (result == Result::SERVER_MIGRATE_ERROR_CURRENT_SERVER_EXCEED_OUTGOING_COUNT)
		{
			FText message; 
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_Full_OutgoingUser"), message);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

			ReqServerMigrateConditionState();
			return true;
		}

		
		if (result == Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL)
		{
			FText message;
			FText::FindText(TEXT("ServerMigrateText"),
				*(ResultEnumToString(Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL)), message);

			FGsUIHelper::PopupSystemMsg(message);

			ReqServerMigrateConditionState();
			return true;
		}

		if (result == Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT)
		{
				

			FText message;
			FText::FindText(TEXT("ServerMigrateText"), TEXT("DELETE_USER_ERROR_GUILD_MEMBER"), message);

			FGsUIHelper::PopupSystemMsg(message);

			ReqServerMigrateConditionState();
			return true;
		}

		// 캐릭터 삭제 실패 - 거래소 판매 및 정산 목록 확인
		if (result == Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION)
		{
			FText message;
			FText::FindText(TEXT("ServerMigrateText"),
				*(ResultEnumToString(Result::DELETE_USER_ERROR_CHECK_MARKET_TRANSACTION)), message);

			FGsUIHelper::PopupSystemMsg(message);
			return true;
		}

		if (result == Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION)
		{
			FText message;
			FText::FindText(TEXT("ServerMigrateText"),
				*(ResultEnumToString(Result::DELETE_USER_ERROR_CHECK_WDMARKET_TRANSACTION)), message);

			FGsUIHelper::PopupSystemMsg(message);
			return true;
		}
		// 조건이 맞지 않아 이전 불가능
		if (result == PD::Result::SERVER_MIGRATE_ERROR_NOT_MEET_THE_CONDITION)
		{
			ReqServerMigrateConditionState();
		}*/
	}
	return false;
}

FName NameChangeByMigrateopupKey = TEXT("PopupChangeNameMigration");

void FGsServerMigrateHandler::OnReqMigrationNameChange(const struct IGsMessageParam* InParam)
{
	static bool reqMigrationNameChange = false;
	if (const FGsMigrationNameChangeMessageParam* param = InParam->Cast<const FGsMigrationNameChangeMessageParam>())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NameChangeByMigrateopupKey);
		if (widget.IsValid())
		{
			reqMigrationNameChange = true;
			if (UGsUIPopupChangeNameMigration* popup = Cast<UGsUIPopupChangeNameMigration>(widget.Get()))
			{
				auto inUserDBId = param->_userDbId;
				popup->OnChangedName.BindLambda([inUserDBId](FText changedName) {
						if (reqMigrationNameChange)
						{
							FGsNetSendServiceBMShop::SendReqChangeMigrationNickname(inUserDBId, changedName.ToString());
							reqMigrationNameChange = false;
						}
					});

				FText title;
				FText::FindText(TEXT("UICommonText"), TEXT("NickName_Setting"), title);

				FText desc;
				FText::FindText(TEXT("LobbyText"), TEXT("Create_1stPopUp_subTitle"), desc);

				FText hint;
				FText::FindText(TEXT("LobbyText"), TEXT("Create_1stPopUp_hintText"), hint);

				UGsUIPopupChangeNameMigration::InitParam initParam =
				{
					title, desc, hint
				};

				popup->SetInit(&initParam);
			}
		}
	}
}

void FGsServerMigrateHandler::OnReqMigrationGuildNameChange(const struct IGsMessageParam* InParam)
{
	static bool reqMigrationGuildNameChange = false;
	if (const FGsMigrationNameChangeMessageParam* param = InParam->Cast<const FGsMigrationNameChangeMessageParam>())
	{
		if (GUI()->GetWidgetByKey(NameChangeByMigrateopupKey).IsValid()) return;

		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NameChangeByMigrateopupKey);
		if (widget.IsValid())
		{
			reqMigrationGuildNameChange = true;
			if (UGsUIPopupChangeNameMigration* popup = Cast<UGsUIPopupChangeNameMigration>(widget.Get()))
			{
				auto inUserDBId = param->_userDbId;
				popup->OnChangedName.BindLambda([inUserDBId](FText changedName) {

						if (reqMigrationGuildNameChange)
						{
							FGsNetSendServiceBMShop::SendReqChangeMigrationGuildName(inUserDBId, changedName.ToString());
							reqMigrationGuildNameChange = false;
						}
					});


				FText title;
				FText::FindText(TEXT("UICommonText"), TEXT("GuildName_Setting"), title);

				FText desc;
				FText::FindText(TEXT("UICommonText"), TEXT("SettingGuildName_CharCount"), desc);

				FText hint;
				FText::FindText(TEXT("UICommonText"), TEXT("SettingGuildName_Desc"), hint);

				UGsUIPopupChangeNameMigration::InitParam initParam =
				{
					title, desc, hint
				};
				popup->SetInit(&initParam);
			}
		}
	}
}
