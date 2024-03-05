#include "GsCameraLookAtHandlerLocal.h"

#include "Management/GsMessageHolder.h"

#include "ActorEx/GsCharacterPlayer.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Input/GsInputEventMsgBase.h"

//#define USE_GREETING_LOOK

FGsCameraLookAtHandlerLocal::~FGsCameraLookAtHandlerLocal()
{	
}
// 초기화(처음 한번만)
void FGsCameraLookAtHandlerLocal::InitializeActor(AActor* In_char)
{
	FGsCameraLookAtHandlerBase::InitializeActor(In_char);

	FGsMessageHolder* message = GMessage();
	
#ifdef USE_GREETING_LOOK
	_delegateGreetingEnter =
		message->GetCamera().AddRaw(MessageContentCamera::GREETING_ON,
			this, &FGsCameraLookAtHandlerLocal::OnGreetingEnter);
	_delegateGreetingExit =
		message->GetCamera().AddRaw(MessageContentCamera::GREETING_OFF,
			this, &FGsCameraLookAtHandlerLocal::OnGreetingExit);
	_delegateStartLerpRotation =
		message->GetCamera().AddRaw(MessageContentCamera::START_LERP_ROT,
			this, &FGsCameraLookAtHandlerLocal::OnStartLerpRotation);
	_delegateDead =
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE,
			this, &FGsCameraLookAtHandlerLocal::OnLocalPlayerDie);
	_delegatePressSkill =
		message->GetInput().AddRaw(MessageInput::PressSkill, 
			this, &FGsCameraLookAtHandlerLocal::OnPressSkill);
#endif
	
}
// 해제
void FGsCameraLookAtHandlerLocal::Finalize()
{
	FGsCameraLookAtHandlerBase::Finalize();
	FGsMessageHolder* message = GMessage();
	
#ifdef USE_GREETING_LOOK
	message->GetCamera().Remove(_delegateGreetingEnter);
	message->GetCamera().Remove(_delegateGreetingExit);
	message->GetCamera().Remove(_delegateStartLerpRotation);
	message->GetGameObject().Remove(_delegateDead);
	message->GetInput().Remove(_delegatePressSkill);
#endif
	
}
// 상태 갱신
void FGsCameraLookAtHandlerLocal::UpdateState()
{
	// 그리팅이면
	if ( true == _isLookMode)
	{
		ChangeLookAtMode(true);
	}
	else
	{
		ChangeLookAtMode(false);
	}
}

// 아이들 상태 진입
void FGsCameraLookAtHandlerLocal::OnGreetingEnter()
{
	_isLookMode = true;
	UpdateState();
}
// 아이들 상태 나감
void FGsCameraLookAtHandlerLocal::OnGreetingExit()
{
	_isLookMode = false;
	UpdateState();
}	
// 보간 회전 시작
void FGsCameraLookAtHandlerLocal::OnStartLerpRotation()
{
	_isLerpRotation = true;
}
// 로컬 사망(죽는 연출 시작)
void FGsCameraLookAtHandlerLocal::OnLocalPlayerDie(const IGsMessageParam*)
{
	_isLookMode = false;
	UpdateState();
}
// 스킬 사용
void FGsCameraLookAtHandlerLocal::OnPressSkill(const FGsInputEventMsgBase&)
{
	_isLookMode = false;
	UpdateState();
}