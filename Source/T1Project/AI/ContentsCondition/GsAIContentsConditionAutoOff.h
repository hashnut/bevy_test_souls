#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageInput.h"

#include "Input/GsInputEventMsgBase.h"

//----------------------------------
// 수동 조작 일때(auto 꺼져있을때)
//----------------------------------

struct FGsGameObjectMessageParamBase;
class UGsGameObjectLocalPlayer;
class FGsAIManager;
struct IGsMessageParam;
class FGsInputEventMsgBase;

class FGsAIContentsConditionAutoOff
{
	// 반격
private:

	// 참조 데이터
private:
	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer;
	FGsAIManager* _aiManager;

	// 메시지 바인딩 해제용
private:

	// 소멸자
public:
	~FGsAIContentsConditionAutoOff();

	// 로직 함수
public:
	// 초기화
	void Initialize(FGsAIManager* In_mng);
	// 해제
	void Finalize();


	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	void RemoveCharacter();

	// 메시지 바인딩
public:

	// 체크 함수
public:

	// get, set	
public:

};