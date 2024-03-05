#pragma once
#include "CoreMinimal.h"
#include "Message/GsMessageGameObject.h"
#include "AI/Data/GsAIDefine.h"
//------------------------------------
// ai 컨텐츠 조건: 제자리 돌아가기
//------------------------------------
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsAIManager;

struct IGsMessageParam;

class FGsAIContentsConditionAutoReturn
{
	// 멤버 변수
private:
	// 제자리 돌아가기 모드인가(옵션창에서 수정)(이동)
	bool _isAutoReturnMode = false;
	// 돌아갈 위치(옵션 on, 조이스틱 놓는 시점에 저장)(이동)
	FVector _autoStartPos = FVector::ZeroVector;
	// 돌아와야 하는 거리(이동)
	float _returnDist = 800.0f;
	// 돌아와야 하는 거리 제곱값(이동)
	float _squaredReturnDist = 0.0f;
	// 돌아왔다 인정 거리(이동)
	float _returnAcceptanceDist = 20.0f;
	// idle 대기시간(복귀전 대기시간)(이동)
	float _waitReturnTime = 2.0f;
	// 돌아오기 타입
	EGsAIAutoReturnStartPosType _returnStartPosType = EGsAIAutoReturnStartPosType::Dist50Meter;
	
	// 다음 스킬 범위
	float _nextSkillRange = 0.0f;

	// 참조 데이터
protected:
	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer; 
	FGsAIManager* _aiManager;

	// 메시지 바인딩 해제용
private:
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;

	// 생성자, 소멸자
public:
	~FGsAIContentsConditionAutoReturn();

	// 로직 함수
public:
	// 초기화
	void Initialize(FGsAIManager* In_mng);
	// 해제
	void Finalize();
	// 돌아갈 위치 저장(이동)
	void SaveAutoStartPos();

	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	void RemoveCharacter();

	// 바인드 함수
public:
	// 위치 저장 message(이동)
	void OnSaveAutoStartPos(const IGsMessageParam* In_contentsType);
	// 제자리 돌아오기 모드 변경
	void OnReturnStartPosTypeChanged(const IGsMessageParam* In_param);

	// 체크함수
public:
	// 돌아가야할 거리보다 멀리 갔는지(이동)
	bool IsTooFarFromAutoStartPos();
	// 돌아가야할 거리 근처인지(이동)
	//bool IsNearFromAutoStartPos();

	// get, set
public:
	// 돌아갈 위치 가져오기(이동)
	const FVector GetAutoStartPos() { return _autoStartPos; }
	void SetAutoStartPos(FVector& In_val)
	{
		_autoStartPos = In_val;
	}
	// 컴백모드인가(이동)
	bool GetIsAutoReturnMode() { return _isAutoReturnMode; }	
	// idle 대기 시간 가져오기(이동)
	float GetWaitReturnTime() { return _waitReturnTime; }
	// 제자리 돌아왔다 인정거리(이동)
	float GetReturnAcceptanceDist() { return _returnAcceptanceDist; }
	// 돌아가야하는 거리(이동)
	float GetReturnDist() { return _returnDist; }
	// 타겟팅 할 obj 거리 체크하는 함수 가져오기(이동)
	TFunction<bool(UGsGameObjectBase * obj)> GetReturnDistCheckFunc();
	// 제자리 모드 인지 저장(이동)
	void SetIsAutoReturnMode(bool In_val);
	// 돌아갈 거리 세팅
	void SetReturnDist(float In_returnDist);
	// 돌아오기 타입 세팅
	void SetAutoReturnStartPosType(EGsAIAutoReturnStartPosType In_type);
	// 돌아가기 타입 가져오기
	EGsAIAutoReturnStartPosType GetAutoReturnStartPosType()
	{
		return _returnStartPosType;
	}

	bool GetIsMapUseAutoReturn();


	void SetNextSkillRange(float In_val)
	{
		_nextSkillRange = In_val;
	}
};