#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Message/GsMessageUI.h"
#include "UTIL/GsTimeUtil.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageGameObject.h"
#include "../OfflinePlay/GsOfflinePlayData.h"
#include "../EventProgress/GsEventProgressInterface.h"



struct FGsSchemaOfflinePlay;
struct IGsMessageParam;
/**
* file		GsOfflinePlayManager.h
* @brief	오프라인 플레이에 대한 Manager Class - 서버 Base로 동작
* @author	PKT
* @date		2021/06/25
* */
class T1PROJECT_API FGsOfflinePlayManager final : public IGsManager, public IGsEventProgressInterface
{
	/**
	 * Hud Mode 초기값
	 */
	const static int32 INVAILD_HUD_VALUE = -1;

	/************************************************************************/
	/* 멤버 변수                                                            */
	/************************************************************************/
protected:	
	time_t _freeRemainingSecounds = 0;			// 2021/06/28 PKT - 무료 남은 시간
	time_t _paidRemainingSecounds = 0;			// 2021/06/28 PKT - 유료 남은 시간
	bool _isBuyTime = false;					// 2021/06/28 PKT - 추가 시간을 유료로 샀는가?

	bool _IsOnFlag = false;

	FGsOfflinePlayResultData _resultData;

	TArray<TPair<MessageContentUnlock, FDelegateHandle>> _unlockContentsDelegateList;
	MsgGameObjHandle _gameobjectDelegate;	

	/************************************************************************/
	/* IGsManager 함수 정의                                                 */
	/************************************************************************/
protected:
	virtual void Initialize() override;
	virtual void Finalize() override;

	void BindMessage();
	void UnBindMessage();

	/************************************************************************/
	/* IGsEventProgressInterface                                            */
	/************************************************************************/
public:
	 // 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	/************************************************************************/
	/* 서버 -> 클라                                                          */
	/************************************************************************/
	void NotifyRemainingSeconds(time_t InFresRemainingSeconds, time_t InPaidRemainingSeconds, bool InIsBuyTime = false);
	void NotifyCancel();	// 2021/06/29 PKT - 서버에서 어떠한 이유로 명시적 오프라인 플레이를 종료
	void AckReadyState(int32 InResult, int32 InDelaySeconds);	// 2021/06/29 PKT - 오프라인 플레이 준비 상태
	void AckPlayStart(int32 InResult);							// 2021/06/29 PKT - 오프라인 플레이 시작( App 종료 )
	// 2023/6/27 PKT - OfflinePlay Debug Only 
	void AckLocalPlayerSpectatorMode(bool InEnable);				// 2021/10/21 PKT - 오프라인 플레이 관전자 모드 시작.( 개발용 )

	void NotifyOfflineResult(const FGsOfflinePlayResultData& InData);

	/************************************************************************/
	/* 클라 -> 서버                                                          */
	/************************************************************************/
	void Request_ReadyState();


	/************************************************************************/
	/* popup                                                                */
	/************************************************************************/
	enum class FailedType {	IsNotFailed, IsCommon, IsNotEnoughTime, IsImpossibleArea, IsNotAutoPlay	};
	void UnavailablePlay(FailedType InType = FailedType::IsCommon);

	/************************************************************************/
	/* Get...                                                               */
	/************************************************************************/		
	time_t FreeRemainingSecounds() const;
	time_t PaidRemainingSecounds() const;
	bool IsBuyTime() const;

	bool InOnFlag() const;
	void FlagReset();

protected:
	bool IsNotPlayResult(int32 InResult);

	void OnOfflinePlayContetnsUnlock(const struct IGsMessageParam*);
	// 2023/5/29 PKT - 이거 주석 활성화 하면 침공전 인터렉션 중 서버에서 팅김( 버그 있음. )
	//void OnInvalidateAllContetnsUnlock(const struct IGsMessageParam*);
	void OnAIModeActive(const struct IGsMessageParam*);
};

#define GSOfflinePlay() UGsScopeHolder::GetGlobalManagerFType<FGsOfflinePlayManager>(UGsScopeGlobal::EManagerType::OfflinePlay)
