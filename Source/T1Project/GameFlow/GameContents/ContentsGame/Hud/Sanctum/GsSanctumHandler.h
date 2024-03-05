// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsMessageParam;
class UGsGameObjectBase;

/**
 * 성소, 성소점령전 관련 이벤트를 핸들, 로직 처리
 */
class T1PROJECT_API FGsSanctumHandler final
{
public:
	using MsgHandleSanctum = TPair<MessageContentSanctum, FDelegateHandle>;

private:
	TArray<MsgHandleSanctum> _msgHandleSanctumList;
	bool _bReserveInateraction = false;

public:
	FGsSanctumHandler() = default;
	~FGsSanctumHandler() {}

public:
	static bool ShowSanctumMessageTicker(int32 InResult);

public:
	void Init();
	void Close();
	void OnReconnection();

private:
	void BindMessages();
	void UnbindMessages();

private:
	void OnBattleFirstNotify(const IGsMessageParam*);
	void OnBattleFinalNotify(const IGsMessageParam*);
	void OnBattleStart(const IGsMessageParam*);
	void OnBattleBeforeEndNotify(const IGsMessageParam* InParam);
	void OnBattleEnd(const IGsMessageParam*);
	void OnBattleOccupySuccess(const IGsMessageParam* InParam);

	// 건설 인터렉션 시도(권한 체크, Range체크 등 수행 후 확인 팝업 출력)
	void OnTryStartConstruct(const IGsMessageParam* InParam);
	// 건설 시작 시 바로 인터렉션하는 기능 취소
	void OnCancelReservedInteraction(const IGsMessageParam*);
	// 건설 시작 시 바로 인터렉션 하는 기능 수행 시도
	void OnTrayStartReservedInteraction(const IGsMessageParam* InParam);
	// 성소 상태 갱신(기사단 가입, 탈퇴등에 따른 HP바와 인터렉션 버튼 갱신 위함)
	void OnInvalidateSanctumState(const IGsMessageParam* InParam);
		
private:
	void SendStartConstruction(SanctumAreaId InSanctumId, int64 InGameId);
	
	bool CheckAuthority(bool bShowMsg);
	bool CheckDistance(UGsGameObjectBase* InLocalPlayerObj, UGsGameObjectBase* InSanctumObj, bool bShowMsg);
};
