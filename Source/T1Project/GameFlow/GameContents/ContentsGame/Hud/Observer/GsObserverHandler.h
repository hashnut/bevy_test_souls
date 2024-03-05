#pragma once

#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"

#include "GameFlow/GameContents/GsContentsMode.h"

/*
* 관전자 핸들러
*/
class UGsGameObjectBase;
struct IGsMessageParam;

class FGsObserverHandler
{
private:
	// 메시지 델리게이트(관전)
	TArray<TPair<MessageContentObserver, FDelegateHandle>> _msgContensObserverDelegates;
	// 메시지 델리게이트(컨텐츠 state 변경)
	TArray<TPair<MessageContents, FDelegateHandle>>				_msgContentexDelegates;
	// 길드 던전 델리게이트
	MsgGuildDungeonHandleArray _msgGuildDungeonDelegates;

	MsgSystemHandleArray _msgSystemNoParamDelegates;
	// 로직 함수
public:
	// 초기화(최초 한번)
	void Init();
	// 정리(마지막 한번)
	void Close();
	// 메시지 등록
	void AddMessages();
	// 메시지 해제
	void RemoveMessages();

	// 옵저버 모드 일때만 쓰는 메시지 등록
	void AddMessgesObserverMode();
	// 옵저버 모드 일때만 쓰는 메시지 해제
	void RemoveMessgesObserverMode();

	// 관전 시작
	void StartObserve();
	// 관전 종료
	void EndObserve();
	// 타겟 변경
	void ChangeTarget(UGsGameObjectBase* In_target, bool In_isStart = true);
	
	// 스프링암을 이동시킨다
	void MoveSpringArmToTarget(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target);
	// 카메라 모드 타겟을 변경한다
	void ChangeCameraModeTarget(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target, bool In_isStart);
	// 미니맵의 로컬(중심 표시) 변경
	void ChangeMinimapLocal(UGsGameObjectBase* In_local, UGsGameObjectBase* In_newTarget, 
		UGsGameObjectBase* In_oldTarget,
		bool In_isStart);
	// hud 타겟 정보에 관전 타겟정보 넣기
	void ChangeHUDTargetInfo(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target);
	// clear hud target ui
	void ClearHUDTargetInfo(UGsGameObjectBase* In_local);

	// 조이스틱 보이게
	void ShowJoystick();
	// 조이스틱 안보이게
	void HideJoystick();

	// 관전 타겟에서 삭제(디스폰, 사망)
	void RemoveObserveTarget(UGsGameObjectBase* In_target);
	// 관전 타겟에 추가(스폰)
	void AddObserveTarget(UGsGameObjectBase* In_target);
	// observer ui visible 세팅
	void SetObserverUIVisibility(bool In_visible);

	// ui에서 호출
public:
	// 타겟 변경 클릭(좌, 우)
	void OnClickTargetChangeSide(bool In_isRight);
	// 타겟 변경 클릭(아이템)
	void OnClickTargetChangeItem(UGsGameObjectBase* In_target);

	// 메시지
public:
	// 관전 시작 메시지
	void OnStartObserve(const IGsMessageParam* In_param);
	// bm shop open
	void OnStateBMShop();
	// hud 복구(bm shop 닫힘)
	void OnStateHud();
	// guild dungeon clear message
	void OnGuildDungeonClear(const IGsMessageParam* In_param);

	void OnContentsEnterStateEvent(FGsContentsMode::InGame InStateType);
	// 재접속후 ui 끄기 처리
	void OnGameServerReconnectionSuccess();
};