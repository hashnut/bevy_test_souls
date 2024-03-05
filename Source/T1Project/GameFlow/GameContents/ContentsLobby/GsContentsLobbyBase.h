// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GsContentsMode.h"
#include "../../../Classes/GsState.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

//------------------------------------------------------------------------------
// 로비 컨텐츠를 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class T1PROJECT_API FGsContentsLobbyBase : public TGsState<FGsContentsMode::InLobby>
{
public:
	FGsContentsLobbyBase() : TGsState<FGsContentsMode::InLobby>(FGsContentsMode::InLobby::MAX) 
	{
	}
	explicit FGsContentsLobbyBase(FGsContentsMode::InLobby inMode) : TGsState<FGsContentsMode::InLobby>(inMode) 
	{
	}
	virtual ~FGsContentsLobbyBase() 
	{
	}

private:
	bool _isBlock = false;

public:
	virtual void Enter() override { OnResetUI(GUI()->OnUIManagerResetUI); };
	virtual void Exit() override { GUI()->OnUIManagerResetUI.Clear(); };
	virtual void Update(float In_deltaTime) override {};

public:
	virtual void OnReconectionStart() {};
	virtual void OnReconectionEnd() {};

	// 조건 체크를 위한 타이머 시작
	virtual void StartTimer() {};
	// 타이머 끝이 불리면 다시 타이틀로 이동
	virtual void EndTimer() {};
	// 스테이트 나갈 때 타이머 삭제
	virtual void StopTimer() {};

protected:
	virtual void InitMessageHandler() {};
	virtual void OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler) {}

public:
	// 터치 불가능 상태여부 체크
	bool IsBlocking() { return _isBlock; }

	// 터치 불가능 상태
	void Blocking()
	{
		_isBlock = true;
		// UI 터치 막기
		FGsUIHelper::ShowBlockUI();
	}

	// 터치 가능 상태
	void UnBlocking()
	{
		_isBlock = false;
		// UI 터치 풀기
		FGsUIHelper::HideBlockUI();
	}
};
