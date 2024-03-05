// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "Classes/GsState.h"
#include "Classes/GsStateMng.h"
#include "../GsContentsMode.h"
#include "Message/GsMessageStage.h"


//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsContentsLobbyAllocator : 
	TGsSharedMapAllocator<FGsContentsMode::InLobby, FGsContentsLobbyBase>
{
public:
	FGsContentsLobbyAllocator() 
	{
	}
	virtual ~FGsContentsLobbyAllocator() 
	{
	}

public:
	virtual FGsContentsLobbyBase* Alloc(FGsContentsMode::InLobby inMode) override;

};


//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
class FGsContentsManagerLobby :
	public TGsStateMng<FGsContentsLobbyBase, FGsContentsLobbyAllocator>
{
	using Super = TGsStateMng<FGsContentsLobbyBase, FGsContentsLobbyAllocator>;

private:
	MsgStageHandleArray	_listStagesDelegate;

public:
	virtual ~FGsContentsManagerLobby() {};

public:
	virtual void RemoveAll() override;
	virtual void InitState() override;
	virtual void Update(float In_deltaTime);

public:
	// ChangeState 후, 해당 StatObject에 접근할일이 생길경우 사용
	template<typename T = FGsContentsLobbyBase>
	T* GetContentsStateObject(FGsContentsMode::InLobby InStateType)
	{
		for (const auto& iter : GetContainer())
		{
			if (InStateType == iter.Key)
			{
				FGsContentsLobbyBase* base = iter.Value.Get();
				return static_cast<T*>(base);
			}
		}

		return nullptr;
	}

public:
	FGsContentsMode::InLobby GetCurrentContentsMode() const;
};
