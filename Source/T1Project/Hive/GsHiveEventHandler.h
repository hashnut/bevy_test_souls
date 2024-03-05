#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "GsHiveEventMsg.h"

class UGsHiveManager;

/*
* 하이브 이벤트 핸들러
* 하이브 - 유저 추적용 이벤트 전송하기 관련 로직을 구현합니다
* 참고 문서 : https://jira.com2us.com/wiki/pages/viewpage.action?pageId=348957189
* 
* 다만 GsHiveEventHandler 의 수명은 GsHiveManager 에서 관리할 것입니다
*/

class FGsHiveEventHandler
{
private:
	/* 자주 사용되므로 캐싱. 어차피 이 클래스의 수명관리는 HiveManager 가 한다 */
	UGsHiveManager* _hiveMgr;

	/* HiveEvent 메시지 관리용 */
	MsgContentHiveEventHandleArray	_hiveEventMsg;

	/*
	* ActionType 별로 컨테이너를 별도로 관리 한다 
	*/

	/* id -> eventName */
	TMap<int64, FString> _purchaseEventMap;

	/* CurrencyType -> eventName */
	TMap<CurrencyType, FString> _currencyPurchaseEventMap;

	/* id -> ordinalCount -> eventName */
	TMap<int64, TMap<int32, FString>> _attendanceEventMap;

	/* id -> eventName */
	TMap<int64, FString> _specialEventMap;

	/**
	 * HiveManager 는 GlobalManager 인데, InitializeData 를 하는 시점에, 테이블이 없을 수 있다.
	 * 따라서 lazy loading 을 한다.
	 */
	bool _isDataLazyLoaded = false;
	bool _isMessageBinded = false;


	/************************************************************************/
	/* Initializer & Finalizer                                               */
	/************************************************************************/
public:
	/** Initializer & Finalizer 함수들을 HiveManager 에서 불러주고 있어 public 처리 */
	void Initialize(UGsHiveManager* InHiveManager = nullptr);
	void Finalize();

	void InitializeData();
	void FinalizeData();

	void BindMessage();
	void UnbindMessage();

	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/
private:
	void OnHiveAnalyticsEvent(const IGsMessageParam* InParam);
	void OnHiveAnalyticsPurchaseEvent(const IGsMessageParam* InParam);

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private:
	/** 재화 타입에 따라, 매 구매시 전송할 이벤트가 있는지를 체크한다 */
	void CheckDefaultPurchaseEvent(const CurrencyType& InType);
	void CheckDataMessageLoaded();
};
