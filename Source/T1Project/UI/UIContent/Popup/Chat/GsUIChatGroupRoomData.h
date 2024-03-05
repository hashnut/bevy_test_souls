// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guild/GsGuildGroupChatData.h"
#include "GsUIChatGroupRoomData.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatGroupRoomData : public UObject
{
	GENERATED_BODY()

private:
	TWeakPtr<FGsGuildGroupChatRoom> _data;

public:
	void SetItemData(TWeakPtr<FGsGuildGroupChatRoom> data) { _data = data; }
	TWeakPtr<FGsGuildGroupChatRoom> GetItemData() { return _data; }

	bool IsValidGroup() { return _data.IsValid() ? _data.Pin()->IsValidGroup() : false; }
	bool IsWaitingRoom() { return _data.IsValid() ? _data.Pin()->IsWaitingRoom() : false; }

	uint64 GetGroupRoomId() { return IsValidGroup() ? _data.Pin()->GetRoomId() : 0; }
	int GetMemberCount() { return  _data.IsValid() ? _data.Pin()->GetMemberCount() : 0; }
	FText GetGroupRoomName() { return _data.IsValid() ? FText::FromString(_data.Pin()->GetGroupRoomName()) : FText::GetEmpty(); }

	bool IsNewGroupMember() { return IsValidGroup() ? _data.Pin()->IsNewMember() : false; }
	bool IsInvitations() { return IsWaitingRoom() && _data.Pin()->IsInvitations(); }
	bool IsRoomMasterAuthorized()
	{
		return _data.IsValid() && _data.Pin()->IsRoomMasterAuthorized();
	}
};
