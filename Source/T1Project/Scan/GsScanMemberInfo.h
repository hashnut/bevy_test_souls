#pragma once

#include "CoreMinimal.h"

#include "Scan/GsScanDefine.h"

class UGsGameObjectBase;

/*
* 스캔 멤버 정보
*/

class FGsScanMemberInfo
{
	// 멤버 변수
private:
	// 정렬된 순서
	int _orderNumber;
	// 갱신 정보()
	uint32 _dirtyFlag;
	// 딤드인가(디스폰 or 스캔 거리 밖)
	bool _isDimmed = false;

protected:
	// 타겟 오브젝트
	TWeakObjectPtr<UGsGameObjectBase> _targetObject;
	// 로컬 오브젝트
	TWeakObjectPtr<UGsGameObjectBase> _localObject;

	FString _name;

	// 생성자, 소멸자
public:
	FGsScanMemberInfo(int In_orderNumber, 
		TWeakObjectPtr<UGsGameObjectBase> In_targetObject,
		TWeakObjectPtr<UGsGameObjectBase> In_localObject):
		_orderNumber(In_orderNumber), 
		_targetObject(In_targetObject),
		_localObject(In_localObject)
	{
		_dirtyFlag = 0;
	}
	virtual ~FGsScanMemberInfo() = default;

	// 가상 함수
public:
	// 이름 초반에 얻어 오기
	virtual void UpdateName();

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 정리할때
	void Finalize();
	// 갱신 정보 추가
	void AddDirtyInfo(EGsScanMemberInfoType In_type);
	// 갱신 정보 초기화
	void ClearDirtyInfo();
	// 빌보드 스캔 넘버 처리
	void SetBillboardScanNumber();


	// 가상 함수 get
public:
	

	// get
public:	
	TWeakObjectPtr<UGsGameObjectBase> GetTargetObject()
	{
		return _targetObject;
	}
	void SetTargetObject(TWeakObjectPtr<UGsGameObjectBase> In_targetObj)
	{
		_targetObject = In_targetObj;
	}

	// game id(서버 사용 고유키)
	int64 GetGameID();
	// 이름 접근
	FString GetName();
	// 현재 hp
	int GetNowHP();
	// 최대 hp
	int GetMaxHP();
	// 거리 값
	int GetDistance();
	// 타게팅인지
	bool GetIsTargeting();
	// 공격중인지
	bool GetIsAttacker();
	// 정렬 순서
	int GetOrderNumber()
	{
		return _orderNumber;
	}
	// 하나라도 바뀌었는가
	bool GetIsChangedInfo()
	{
		return (_dirtyFlag != 0) ? true : false;
	}
	// 특정 타입이 변경되었나
	bool GetIsChangedInfo(EGsScanMemberInfoType In_type)
	{
		return (_dirtyFlag & (uint32)In_type);
	}
	// 딤드 상태 인가
	bool GetIsDimmed()
	{
		return _isDimmed;
	}
	// 딤드 상태 세팅
	void SetIsDimmed(bool In_isDimmed)
	{
		_isDimmed = In_isDimmed;
	}
};