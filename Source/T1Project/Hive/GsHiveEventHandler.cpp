#include "GsHiveEventHandler.h"
#include "DataSchema/Hive/GsSchemaHiveEventData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

void FGsHiveEventHandler::Initialize(UGsHiveManager* InHiveManager)
{
	InitializeData();
	BindMessage();

	_hiveMgr = InHiveManager;
}

void FGsHiveEventHandler::Finalize()
{
	FinalizeData();
	UnbindMessage();

	_hiveMgr = nullptr;
}

void FGsHiveEventHandler::InitializeData()
{
	// 테이블로부터 이벤트 정보 긁어오기
	const UGsTableHiveEventData* table = Cast<UGsTableHiveEventData>(FGsSchemaHiveEventData::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return;
	}

	TArray<const FGsSchemaHiveEventData*> hiveEvents;
	table->GetAllRows<FGsSchemaHiveEventData>(hiveEvents);

	// 카테고리별로 각각의 Map 에 캐싱을 진행한다
	for (const FGsSchemaHiveEventData* data : hiveEvents)
	{
		switch (data->hiveActionType)
		{
		case EGsHiveActionType::PURCHASE:
		{
			// 만약 id 가 0 이라면, 재화 타입에 대해 '매 구매시' 체크하는 이벤트를 캐싱
			if (INVALID_BMPRODUCT_ID == data->id)
			{
				_currencyPurchaseEventMap.Add(data->currencyType, data->eventName);
			}
			// id 가 Valid 하다면, 특정 상품에 대해 이벤트 전송 처리를 할 것이다.
			else
			{
				_purchaseEventMap.Add(data->id, data->eventName);
			}


			break;
		}
		case EGsHiveActionType::ATTENDANCE:
		{
			// 기존 출석부에 대한 정보가 없으면 새로 만들어 주기
			if (!_attendanceEventMap.Contains(data->id))
			{
				_attendanceEventMap.Add(data->id);
			}
			_attendanceEventMap[data->id].Add(data->ordinalCount, data->eventName);

			break;
		}
		case EGsHiveActionType::SPECIAL:
		{
			_specialEventMap.Add(data->id, data->eventName);
			break;
		}
		default:
			break;
		}
	}

	GSLOG(Log, TEXT("_purchaseEventMap data"));
	for (auto& purchasePair : _purchaseEventMap)
	{
		GSLOG(Log, TEXT("Purchase id : %d, eventName : %s"), purchasePair.Key, *purchasePair.Value);
	}

	GSLOG(Log, TEXT("_currencyPurchaseEventMap data"));
	for (auto& currencyPurchasePair : _currencyPurchaseEventMap)
	{
		GSLOG(Log, TEXT("Currency type : %d, eventName : %s"), static_cast<int>(currencyPurchasePair.Key), *currencyPurchasePair.Value);
	}


	GSLOG(Log, TEXT("_attendanceEventMap data"));
	for (auto& attendancePair : _attendanceEventMap)
	{
		GSLOG(Log, TEXT("Attendance id : %d"), attendancePair.Key);
	}


	GSLOG(Log, TEXT("_specialEventMap data"));
	for (auto& specialPair : _specialEventMap)
	{
		GSLOG(Log, TEXT("Special id : %d, eventName : %s"), specialPair.Key, *specialPair.Value);
	}

	_isDataLazyLoaded = true;
}

void FGsHiveEventHandler::FinalizeData()
{
	_purchaseEventMap.Empty();
	_attendanceEventMap.Empty();
	_specialEventMap.Empty();
}

void FGsHiveEventHandler::BindMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	_hiveEventMsg.Emplace(
		messageHolder->GetHiveEvent().AddRaw(MessageContentHiveEvent::SEND_ANALYTICS_EVENT, this, &FGsHiveEventHandler::OnHiveAnalyticsEvent));

	_hiveEventMsg.Emplace(
		messageHolder->GetHiveEvent().AddRaw(MessageContentHiveEvent::SEND_PURCHASE_EVENT, this, &FGsHiveEventHandler::OnHiveAnalyticsPurchaseEvent));

	_isMessageBinded = true;
}

void FGsHiveEventHandler::UnbindMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	if (_hiveEventMsg.Num() != 0)
	{
		for (auto iter : _hiveEventMsg)
		{
			messageHolder->GetHiveEvent().Remove(iter);
		}

		_hiveEventMsg.Empty();
	}
}

void FGsHiveEventHandler::OnHiveAnalyticsEvent(const IGsMessageParam* InParam)
{
	CheckDataMessageLoaded();

	const FGsHiveEventMsg* param = InParam->Cast<const FGsHiveEventMsg>();
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	// 하이브 이벤트 핸들링할 때, 로그를 찍어주기
	GSLOG(Log, TEXT("[Hive user Event Handling] ActionType : %d, id : %d, ordinalCount : %d"),
		static_cast<int>(param->actionType), param->id, param->ordinalCount);

	// 각 ActionType 별로 구현이 조금 달라진다 (처리해야 되는 특이 케이스 존재)
	switch (param->actionType)
	{
	// 구매 관련은 OnHiveAnalyticsPurchaseEvent 에 따로 분리
	//case EGsHiveActionType::PURCHASE:
	case EGsHiveActionType::ATTENDANCE:
	{
		// 출석 시, 출석부 번호와 횟수는 당연히 0 이 아닐 것이다
		if (INVALID_ATTENDANCE_ID != param->id && INVALID_ATTENDANCE_COUNT != param->ordinalCount)
		{
			// '특정 출석부' 확인
			if (_attendanceEventMap.Contains(param->id))
			{
				// '특정 번째' 확인

				// (1) '특정 출석부' 있고, '특정 번째' 도 있으면 : 정상 처리
				if (_attendanceEventMap[param->id].Contains(param->ordinalCount))
				{
					GSLOG(Log, TEXT("EventName : %s"), *_attendanceEventMap[param->id][param->ordinalCount]);

					if (_hiveMgr)
					{
						_hiveMgr->AnalyticsSendEvent(_attendanceEventMap[param->id][param->ordinalCount]);
					}
				}
				// (2) '특정 출석부' 는 있지만, '특정 번째' 없으면 : '특정 출석부'에 대해 매번 처리
				if (_attendanceEventMap[param->id].Contains(INVALID_ATTENDANCE_COUNT))
				{
					GSLOG(Log, TEXT("EventName : %s"), *_attendanceEventMap[param->id][INVALID_ATTENDANCE_COUNT]);

					if (_hiveMgr)
					{
						_hiveMgr->AnalyticsSendEvent(_attendanceEventMap[param->id][INVALID_ATTENDANCE_COUNT]);
					}
				}
			}

			// '특정 출석부' 와 관계 없이 처리 (모든 출석부)
			if (_attendanceEventMap.Contains(INVALID_ATTENDANCE_ID))
			{
				// (3) '특정 출석부' 없고, '특정 번째' 있으면 : 모든 출석부에 대해 '특정 번째' 처리
				if (_attendanceEventMap[INVALID_ATTENDANCE_ID].Contains(param->ordinalCount))
				{
					GSLOG(Log, TEXT("EventName : %s"), *_attendanceEventMap[INVALID_ATTENDANCE_ID][param->ordinalCount]);

					if (_hiveMgr)
					{
						_hiveMgr->AnalyticsSendEvent(_attendanceEventMap[INVALID_ATTENDANCE_ID][param->ordinalCount]);
					}
				}
				
				// (4) '특정 출석부' 없고, '특정 번째' 없으면 : 모든 출석부에 대해 출석할 때마다 처리
				if (_attendanceEventMap[INVALID_ATTENDANCE_ID].Contains(INVALID_ATTENDANCE_COUNT))
				{
					GSLOG(Log, TEXT("EventName : %s"), *_attendanceEventMap[INVALID_ATTENDANCE_ID][INVALID_ATTENDANCE_COUNT]);

					if (_hiveMgr)
					{
						_hiveMgr->AnalyticsSendEvent(_attendanceEventMap[INVALID_ATTENDANCE_ID][INVALID_ATTENDANCE_COUNT]);
					}
				}
			}
		}
		else
		{
			GSLOG(Error, TEXT("INVALID_ATTENDANCE_ID != param->id && INVALID_ATTENDANCE_COUNT != param->ordinalCount"));
			return;
		}
		break;
	}
	case EGsHiveActionType::SPECIAL:
	{
		// 단발성 이벤트에 대한 처리
		if (_specialEventMap.Contains(param->id))
		{
			if (_hiveMgr)
			{
				_hiveMgr->AnalyticsSendEvent(_specialEventMap[param->id]);
			}
		}
		break;
	}
	default:
		break;
	}
}

void FGsHiveEventHandler::OnHiveAnalyticsPurchaseEvent(const IGsMessageParam* InParam)
{
	CheckDataMessageLoaded();

	const FGsHiveEventPurchaseMsg* purchaseParam = InParam->Cast<const FGsHiveEventPurchaseMsg>();
	if (nullptr == purchaseParam)
	{
		GSLOG(Error, TEXT("nullptr == purchaseParam"));
		return;
	}

	// 구매 리스트에 쌓인 상품에 대해 이벤트 메시지 전송
	for (const TPair<uint32, CurrencyType>& idCurrencyPair : purchaseParam->productIdCurrencyList)
	{
		// 하이브 이벤트 핸들링할 때, 로그를 찍어주기
		GSLOG(Log, TEXT("[Hive Purchase Event Handling] ActionType : %d, ProductId : %d, CurrencyType : %d"),
			static_cast<int>(purchaseParam->actionType), idCurrencyPair.Key, static_cast<int>(idCurrencyPair.Value));

		// 특정 상품에 대한 처리
		if (INVALID_BMPRODUCT_ID != idCurrencyPair.Key)
		{
			if (_purchaseEventMap.Contains(idCurrencyPair.Key))
			{
				GSLOG(Log, TEXT("Purchase id : %d, EventName : %s"), idCurrencyPair.Key, *_purchaseEventMap[idCurrencyPair.Key]);

				if (_hiveMgr)
				{
					_hiveMgr->AnalyticsSendEvent(_purchaseEventMap[idCurrencyPair.Key]);
				}
			}
		}

		// 특이 케이스 1 : 데이터 테이블의 id 가 0 이면 상품 종류와 관계 없이 '구매했다' 는 정보 전송
		// 0 일 경우는, 각 재화 타입에 맞게 체크를 하면 될 것이다.
		// 일괄 구매의 경우, 아이템 종류마다 이벤트가 전송된다.
		CheckDefaultPurchaseEvent(idCurrencyPair.Value);
	}
}

void FGsHiveEventHandler::CheckDefaultPurchaseEvent(const CurrencyType& InType)
{
	if (_currencyPurchaseEventMap.Contains(InType))
	{
		GSLOG(Log, TEXT("EventName : %s"), *_currencyPurchaseEventMap[InType]);

		if (_hiveMgr)
		{
			_hiveMgr->AnalyticsSendEvent(_currencyPurchaseEventMap[InType]);
		}
	}
}

void FGsHiveEventHandler::CheckDataMessageLoaded()
{
	// 만약 데이터가 로드가 되지 않았다면 Lazy 하게 Load 한다
	if (false == _isDataLazyLoaded)
	{
		InitializeData();
	}

	// 메시지가 바인드 되지 않았다면 Lazy 하게 Bind 한다
	if (false == _isMessageBinded)
	{
		BindMessage();
	}
}
