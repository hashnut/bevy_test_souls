#pragma once

#include "AIModule/Classes/Navigation/PathFollowingComponent.h"

DECLARE_DELEGATE(FGsAutoMoveCompletedDelegate);
DECLARE_DELEGATE_OneParam(FGsAutoMoveCompletedOneDelegate, const FPathFollowingResult& /*result*/);

// 자동 이동 구조체
struct T1PROJECT_API FGsAutoMoveInfo
{
	FAIMoveRequest					_moveRequest;
	// 이동 성공 처리
	FGsAutoMoveCompletedDelegate	_funcSuccessDelegate;
	// 이동 실패 처리
	FGsAutoMoveCompletedOneDelegate	_funcFailDelegate;

	void Clear() 
	{
		//GSLOG(Warning, TEXT("[AUTO] FGsAutoMoveInfo::Clear _moveRequest"));
		_moveRequest = FAIMoveRequest();
		
		// 크래쉬 에러 나서(FGsMovementStateAuto::CallbackFinish 에서) nullptr 대입 대신 unbind 처리
		_funcSuccessDelegate.Unbind();
		_funcFailDelegate.Unbind();

	}

	bool IsValid() const
	{
		return _moveRequest.IsValid();
	}


};

