// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"

#include "Mail/GsMailDefine.h"
#include "Mail/GsMailInfo.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

struct IGsMessageParam;
class FGsMailInfo;
struct FGsSchemaMailStringData;
/**
 * 메일 매니져
 */
class T1PROJECT_API FGsMailManager final : public IGsManager
{
private:
	int				_requestCount = 0;

	EGMailReqType	_reqType = EGMailReqType::None;
	FTimerHandle	_timerHandle;

	TArray<MailDBId> _requestList;
	TMap<MailDBId, TSharedPtr<FGsMailInfo>> _mailMap;			// 개인 우편
	TMap<MailDBId, TSharedPtr<FGsMailInfo>> _guildMailMap;		// 길드 우편
	TMap<MailDBId, TSharedPtr<FGsMailInfo>> _accountMailMap;	// 계정 우편	

	MsgGameObjHandle	_gameObjectDelegate;
	MsgGameObjHandle	_gameObjecReviveDelegate;
	TPair<MessageContentMail, FDelegateHandle>	_mailtDelegate;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	

protected:
	void Clear();
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	void MailNotifyUnread(const IGsMessageParam* inParam);
	void OnTimer();

public:
	void InitializeMailData();	

public:
	// 메일리스트를 서버에서 받다.
	void Set(struct PD::SC::PKT_SC_ACK_MAIL_LIST_READ& Packet);
	// 우편 수령(삭제) or 모두 수령(삭제) 요청 응답을 받는다.
	void Set(struct PD::SC::PKT_SC_ACK_MAIL_RECV_READ& Packet);
	// 읽음 상태 전송 결과를 받는다.
	void Set(struct PD::SC::PKT_SC_ACK_MAIL_READ_READ& Packet);

	// 메일 삭제
	void DeleteMail(MailBoxType inBoxType, MailDBId inMailDBId);

private:
	// 메일 추가
	void AddMail(TSharedPtr<FGsMailInfo> inDataMailData);

	// 모두 받기 시 보상리스트 팝업
	void OpenItemReceive();
	
public:
	// 우편 리스트 요청
	void SendReqMailList();
	// 우편 신규 시스템 메일 우편함에 지급 요청
	void SendReqNewSystmeMailRecv();
	// 모두 받기 요청
	void SendMailReceiveAll(MailBoxType inBoxType);
	// 모두 삭제 요청
	void SendMailDeleteAll(MailBoxType inBoxType);

	// 받기 요청
	void SendMailReceive(MailBoxType inBoxType, MailDBId inMailDBId);
	// 삭제 요청
	void SendMailDelete(MailBoxType inBoxType, MailDBId inMailDBId);
	// 읽음 상태 전송
	void SendReqMailRead(MailBoxType inBoxType, MailDBId inMailDBId);

private:
	// 타입별 메일데이터 리스트
	TMap<MailDBId, TSharedPtr<FGsMailInfo>>* GetMailBoxTypeData(MailBoxType inBoxType);

	// 타입별 아이템포함 여부분류별 메일데이터 리스트
	void GetMailDBIdList(MailBoxType inBoxType, bool isRecv, OUT TArray<MailDBId>& outMailDBIdList);

public:
	// 우편타입 별 우편리스트
	void GetMailList(MailBoxType inBoxType, OUT TArray<TSharedPtr<FGsMailInfo>>& outList);
	// 우편ID로 우편데이터 찾기
	bool GetMailData(MailDBId inMailDBId, OUT TSharedPtr<FGsMailInfo>& outData);
	// 우편타입 별 우편리스트 갯수
	int GetMailBoxTypeDataListCount(MailBoxType inBoxType);
	// 모두받기 요청 리스트
	TArray<MailDBId>& GetRequestList() { return _requestList; }
	// 모두받기로 받은 첨부보상아이템 리스트
	void AllItemList(OUT TMap<uint32, int>& outCurrencyList, OUT TMap<uint32, int>& outItemList);
	// 전체 메일갯수
	void AllItemList(OUT TArray<TSharedPtr<FGsMailAttachment>>& outItemList);
	int GetMailCount();

private:
	// 서버로 요청한 타입 (받기, 모두받기, 삭제, 모두삭제)
	void SetReqType(EGMailReqType inValue) { _reqType = inValue; }
	// 모두받기, 모두삭제 요청한 우편 갯수
	void SetRequestCount(int inCount) { _requestCount = inCount; }
	// 우편은 1개씩 결과(삭제,받기)가 오기때문에 다받고 결과를 출력하기 위해 개별 차감한다.
	void RemoveRequestCount() { _requestCount = (0 == _requestCount)? 0 : --_requestCount; }
	// 요청한 우편에 대한 결과를 다 받았는지 체크
	bool IsRequestOver() { return (0 == _requestCount) ? true : false; }
	// 모두받기, 모두삭제 중 실패가 오면 나머지 요청은 중지된다.
	void ResetRequestCount() { _requestCount = 0; }

	// 서버에서 내려준 에러타입 별 티커
	void MailErrorMsg(PD::Result inResult);
	// 서버요청에 대한 결과 티커
	void MailReqMsg(EGMailReqType inReqType);

public:
	bool IsRedDot();
	void IsRedDot(MailBoxType inBoxType, OUT bool& outIsRedDot);

	// 임시 테스트
	void GetMailString(FString inString, OUT TArray<TSharedPtr<FGsMailStringData>>& outMailStringDataList);
};

#define GSMail() UGsScopeHolder::GetGameManagerFType<FGsMailManager>(UGsScopeGame::EManagerType::Mail)
