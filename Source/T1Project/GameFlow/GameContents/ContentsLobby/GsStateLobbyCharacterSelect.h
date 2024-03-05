// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Engine/EngineTypes.h"
#include "Message/GsMessageSystem.h"
#include "Management/ScopeLobby/GsLobbyDataType.h"
#include "Message/MessageParam/GsMessageParam.h"

class FGsCustomizeHandler;
class AGsCharacterPlayer;
class ACharacter;

/**
 * 
 */
class T1PROJECT_API FGsStateLobbyCharacterSelect : public FGsContentsLobbyBase
{
private:
	TWeakObjectPtr<class UGsUIWindowCharacterSelect>	_windowUI;
	FGsCustomizeHandler* _customizeHandler;	
	MsgUIHandle _msgUiHandle;

public:
	FGsStateLobbyCharacterSelect();
	virtual ~FGsStateLobbyCharacterSelect();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Close() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

public:
	class AGsCharacterPlayer* LoadChangeCharacter(const LobbyDataType::FGsLobbyUserListData& data);
	// 캐릭터 입장
	void OnClickSelectUser(const UserDBId inUserDBId);
	// 캐릭터 삭제
	void OnClickDeleteUser(const UserDBId inUserDBId);
	// 캐릭터 삭제 철회
	void OnClickDeleteCancel(const UserDBId inUserDBId);
	// 캐릭터 선택
	void OnClickSlot(const UserDBId inUserDBId);
	//// 무비 중지OR끝
	//void OnStopMovie(int32 inIndex);
	void OnStartGame(const UserDBId inUserDBId);
	// 게임 입장 시 연출 카메라이동
	void OnStartCam();
	// 외형 변경 사용
	void OnClickCustomizingTicketUse(const UserDBId inUserDBId);

private:
	LobbyDataType::FGsLobbyUserListData* GetUserData(const UserDBId inUserDBId);
	bool CheckDeleteLevel(LobbyDataType::FGsLobbyUserListData* inUserData);
	bool CheckDeleteGuild(LobbyDataType::FGsLobbyUserListData* inUserData);

	void OnCharacterEmpty(const IGsMessageParam*);
};
