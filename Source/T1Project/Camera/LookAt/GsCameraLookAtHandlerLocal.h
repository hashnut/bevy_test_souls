#pragma once

#include "GsCameraLookAtHandlerBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
//--------------------------------------
// 카메라 바라보기 관리(게임내 내캐릭)
//--------------------------------------

class AGsCharacterPlayer;
class USkeletalMeshComponent;
class FGsInputEventMsgBase;
struct IGsMessageParam;
class FGsCameraLookAtHandlerLocal : public FGsCameraLookAtHandlerBase
{
private:
	TPair<MessageContentCamera, FDelegateHandle> _delegateGreetingEnter;
	TPair<MessageContentCamera, FDelegateHandle> _delegateGreetingExit;
	TPair<MessageContentCamera, FDelegateHandle> _delegateStartLerpRotation;
	MsgGameObjHandle _delegateDead;
	TPair<MessageInput, FDelegateHandle> _delegatePressSkill;
	// look mode 인지
	bool _isLookMode = false;

public:
	virtual ~FGsCameraLookAtHandlerLocal();

	// 가상함수
public:
	// 초기화(처음 한번만)
	virtual void InitializeActor(class AActor* In_char) override;
	// 해제
	virtual void Finalize() override;
public:

	// 상태 갱신
	void UpdateState();

	// 이벤트
public:
	// 그리팅 상태 진입
	void OnGreetingEnter();
	// 그리팅 상태 나감
	void OnGreetingExit();
	// 보간 회전 시작
	void OnStartLerpRotation();
	// 로컬 사망(죽는 연출 시작)
	void OnLocalPlayerDie(const IGsMessageParam*);
	// 스킬 사용
	void OnPressSkill(const FGsInputEventMsgBase&);
};