#pragma once

#include "CoreMinimal.h"

#include "Scan/GsScanDefine.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
/*
* 스캔 핸들러:
* 스캔 관련 처리
*/

class UGsGameObjectBase;

struct IGsMessageParam;

// 스캔 정렬 정보
struct FGsScanSortInfo
{
	// 타겟 오브젝트
	UGsGameObjectBase* _targetObj;
	// 정렬 대상 타입(우선순위 2)
	int _sortTargetType;
	// 거리 제곱값(우선순위 3)
	float _distSquared = 0.0f;
	// 정렬 대상 타입 하부(우선순위 4)
	int _sortTargetTypeSub = 0;
};

class FGsScanHandler
{
private:
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;
	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;

	int64 _oldTargetGameId = 0;

	// 메시지 수신
public:
	// 로컬 타겟 클리어 됨
	void OnLocalTargetClear(const IGsMessageParam* In_param);
	// 로컬 타겟 변경 됨
	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	// 로컬 얻어 맞음
	void OnLocalHit(const IGsMessageParam* In_param);
	// 로컬 로드 완료
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	// 워프전 처리
	void OnLoadTopoStart(const IGsMessageParam* inParam);

	void OnAbnormalityAllUpdate(const IGsMessageParam* In_param);

	// 로직 함수
public:
	// 초기화
	void Init();
	// 종료
	void Close();

	// 스캔 시작
	void StartScan(bool In_isClick = true, bool In_useLocalPos= true, FVector In_scanCenterPos = FVector::ZeroVector);

	// 스캔 인포 삭제
	void RemoveScanMemberInfo(int64 In_gameId);

	// 스캔 리스트 삭제
	void ClearScanList();


	void ClearOldTarget();
};