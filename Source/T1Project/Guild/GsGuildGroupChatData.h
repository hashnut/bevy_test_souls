#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

class FGsGuildManager;
class UGsUIPopupChatGroup;

struct FGsGuildChatList
{
	GuildChatDBId _guildChatDBId = UINT64_MAX;
	time_t _messageTime;
	FString _userName;
	FString _message;
	Rank _rank{ INVALID_RANK };

	WorldId		 _worldId{ INVALID_WORLD_ID };
	AccountGrade _accountGrade{ AccountGrade::NORMAL };
	GuildDBId	 _guildId{ INVALID_GUILD_DB_ID };
	GuildEmblemId _emblemId{ INVALID_GUILD_EMBLEM_ID };
};

struct FGsGuildGroupChatMemberGuild
{
	enum class GuildMemberState : uint8 { None = 0, WaitMyResponse, InvitationGuest, Joined, ForcedOut, Max };

	GuildDBId _guildDBId{ INVALID_GUILD_DB_ID };
	FString _guildName;
	FString _masterName;
	GuildEmblemId _emblemId = 0;
	PlanetWorldId _homePlanetWorldId = 0;

	bool _isHomeWorldGuild{ true };
	bool _isMaster{ false };
	bool _isNew{ false };
	GuildMemberState _state{ GuildMemberState::None };

	FGsGuildGroupChatMemberGuild() = default;
	FGsGuildGroupChatMemberGuild(GuildDBId guildDBId, FString guildName, FString masterName, GuildEmblemId emblemId, 
		PlanetWorldId homePlanetWorldId = 0, bool isHomeWorldGuild = true, bool isMaster = false, bool isNew = false, GuildMemberState state = GuildMemberState::None)
		: _guildDBId(guildDBId)
		, _guildName(guildName)
		, _masterName(masterName)
		, _emblemId(emblemId)
		, _homePlanetWorldId(homePlanetWorldId)
		, _isHomeWorldGuild(isHomeWorldGuild)
		, _isMaster(isMaster)
		, _isNew(isNew)
		, _state(state)
	{
	}

	FGsGuildGroupChatMemberGuild(const FGsGuildGroupChatMemberGuild& other)
		: _guildDBId(other._guildDBId)
		, _guildName(other._guildName)
		, _masterName(other._masterName)
		, _emblemId(other._emblemId)
		, _homePlanetWorldId(other._homePlanetWorldId)
		, _isHomeWorldGuild(other._isHomeWorldGuild)
		, _isMaster(other._isMaster)
		, _isNew(other._isNew)
		, _state(other._state)
	{	
	}

	FGsGuildGroupChatMemberGuild(const FGsGuildGroupChatMemberGuild&& other)  noexcept
		: _guildDBId(other._guildDBId)
		, _guildName(other._guildName)
		, _masterName(other._masterName)
		, _emblemId(other._emblemId)
		, _homePlanetWorldId(other._homePlanetWorldId)
		, _isHomeWorldGuild(other._isHomeWorldGuild)
		, _isMaster(other._isMaster)
		, _isNew(other._isNew)
		, _state(other._state)
	{
	}

	FGsGuildGroupChatMemberGuild& operator=(const FGsGuildGroupChatMemberGuild& other)
	{
		// Guard self assignment
		if (this == &other)
			return *this;

		_guildDBId = other._guildDBId;
		_guildName = other._guildName;
		_masterName = other._masterName;
		_emblemId = other._emblemId;
		_homePlanetWorldId = other._homePlanetWorldId;
		_isHomeWorldGuild = other._isHomeWorldGuild;
		_isMaster = other._isMaster;
		_isNew = other._isNew;
		_state = other._state;

		return *this;
	}

	FGsGuildGroupChatMemberGuild& operator=(FGsGuildGroupChatMemberGuild&& other) noexcept
	{
		if (this == &other)
			return *this;

		_guildDBId = other._guildDBId;
		_guildName = other._guildName;
		_masterName = other._masterName;
		_emblemId = other._emblemId;
		_homePlanetWorldId = other._homePlanetWorldId;
		_isHomeWorldGuild = other._isHomeWorldGuild;
		_isMaster = other._isMaster;
		_isNew = other._isNew;
		_state = other._state;

		return *this;
	}
};

struct FGsGuildGroupChatInviteHost
{
	using GuildMemberState = FGsGuildGroupChatMemberGuild::GuildMemberState;

	GuildGroupChatRoomId _roomId;
	FString _roomName;

	FGsGuildGroupChatMemberGuild _guildData;

	FGsGuildGroupChatInviteHost() = default;
	FGsGuildGroupChatInviteHost(const FGsGuildGroupChatInviteHost& other)
		: _roomId(other._roomId)
		, _roomName(other._roomName)
	{
		_guildData = other._guildData;
	}

	FGsGuildGroupChatInviteHost(FGsGuildGroupChatInviteHost&& other) noexcept
		: _roomId(other._roomId)
		, _roomName(other._roomName)
	{
		_guildData = other._guildData;
	}

	FGsGuildGroupChatInviteHost(GuildGroupChatRoomId roomId, FString roomName, FGsGuildGroupChatMemberGuild& guildData) 
		: _roomId(roomId), _roomName(roomName)
	{
		_guildData = guildData;
	}

	FGsGuildGroupChatInviteHost(GuildGroupChatRoomId roomId, FString roomName, FGsGuildGroupChatMemberGuild&& guildData)
		: _roomId(roomId), _roomName(roomName)
	{
		_guildData = guildData;
	}

	FGsGuildGroupChatInviteHost& operator=(const FGsGuildGroupChatInviteHost& other)
	{
		// Guard self assignment
		if (this == &other)
			return *this;
		
		_roomId = other._roomId;
		_roomName = other._roomName;
		_guildData = other._guildData;

		return *this;
	}

	FGsGuildGroupChatInviteHost& operator=(FGsGuildGroupChatInviteHost&& other) noexcept
	{
		if (this == &other)
			return *this;

		_roomId = other._roomId;
		_roomName = other._roomName;
		_guildData = other._guildData;

		return *this;
	}

	const FGsGuildGroupChatMemberGuild& GetGuildData() const { return _guildData; }
	GuildGroupChatRoomId GetRoomId() { return _roomId; }
};

struct FGsGroupChatRoomInfo
{
	GuildGroupChatRoomId _roomId{ INVALID_GUILD_GROUP_CHAT_ROOM_ID };
	FString _chatRoomName;
	int32	_guildCount{ 0 };
	GuildMemberGradeType _authorizedGrade{ GuildMemberGradeType::KNIGHT };
	GuildDBId _masterGuildDBId{ INVALID_GUILD_DB_ID };
};

class FGsGuildGroupChatRoom
{
public:
	enum class GroupRoomState : uint8 { None = 0, Playing, InvitationHost, Max};

protected:

	GuildGroupChatRoomId _roomId{ INVALID_GUILD_GROUP_CHAT_ROOM_ID };
	FGsGroupChatRoomInfo _info;
	GroupRoomState _roomState{ GroupRoomState::None }; //참여중, 초대신청 받은

	TWeakPtr<FGsGuildGroupChatMemberGuild> _masterGuild{ nullptr };
	bool _isNewRoom{ false };

	TArray<TSharedPtr<FGsGuildGroupChatMemberGuild>> _memberGuilds;
	TArray<TSharedPtr<FGsGuildGroupChatMemberGuild>> _inviteGuilds; //내가 응답을 기다리는 invitation

public:
	TArray<GuildGroupChatRoomId> _toMyInvitations; //나에게 온 invitation
	GuildDBId _myGuildDBId{ INVALID_GUILD_DB_ID };

	GuildMemberGradeType _changingAuthGrade{ GuildMemberGradeType::KNIGHT };

public:
	FGsGuildGroupChatRoom() = default;

	FGsGuildGroupChatRoom(GuildGroupChatRoomId roomId, GuildDBId myGuildDBId) : _roomId(roomId), _myGuildDBId(myGuildDBId) {}
	FGsGuildGroupChatRoom(GuildGroupChatRoomId roomId, FGsGroupChatRoomInfo& info, GuildDBId myGuildDBId)
		: _roomId(roomId)
		, _info(info)
		, _roomState(GroupRoomState::Playing)
		, _myGuildDBId(myGuildDBId)
		, _changingAuthGrade(info._authorizedGrade)
	{}

	const bool IsValidGroup() { return _roomId != INVALID_GUILD_GROUP_CHAT_ROOM_ID; }
	const bool IsWaitingRoom() { return _roomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID && _roomState == GroupRoomState::InvitationHost; }
	const bool IsInvitationGuests() { return _inviteGuilds.Num() > 0; }
	const bool IsInvitations() { return _toMyInvitations.Num() > 0; }
	const bool IsNewMember() { return _memberGuilds.FilterByPredicate([](auto e) { return e->_isNew; }).Num() > 0; }

	const GuildGroupChatRoomId GetRoomId() { return _roomId; }
	const FString GetGroupRoomName() const { return  _info._chatRoomName ; }
	void SetGroupRoomName(FString roomName) { _info._chatRoomName = roomName; }

	void SetChangingAuthGrade(GuildMemberGradeType grade) { _changingAuthGrade = grade;  }
	const GuildMemberGradeType GetChangingAuthGrade() const { return _changingAuthGrade; }

	void SetAuthorizedGrade(GuildMemberGradeType grade) {
		_changingAuthGrade = grade;
		_info._authorizedGrade = grade;
	}
	const GuildMemberGradeType GetAuthorizedGrade() const { return _info._authorizedGrade; }
	const GuildMemberGradeType GetChatEnableGrade() const { return  _info._authorizedGrade; }

	void SetData(FGsGroupChatRoomInfo& info);
	void ResetData();
	void ResetInvitations();

	void AddMemberGuild(FGsGuildGroupChatMemberGuild& member);
	void AddMemberGuild(FGsGuildGroupChatMemberGuild&& member);
	void RemoveMemberGuild(GuildDBId guildDBId);
	void AddInviteGuestGuild(FGsGuildGroupChatMemberGuild& guest);
	void AddInviteGuestGuild(FGsGuildGroupChatMemberGuild&& guest);
	void RemoveInviteGuestGuild(GuildDBId guildDBId);

	int GetMemberCount() { return _memberGuilds.Num(); }
	bool IsMasterGuild(GuildDBId guildDbId) { return _info._masterGuildDBId == guildDbId; }
	void SetMasterGuild(GuildDBId guildDbId);

	int GetMemberList(TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>>& memberList);

	TWeakPtr<FGsGuildGroupChatMemberGuild> GetMember(GuildDBId guildDbId);
	int GetInviteGuestList(TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>>& inviteList);
	bool IsRoomMasterGuild() { return IsValidGroup() && IsMasterGuild(_myGuildDBId); }
	bool IsRoomMasterAuthorized();
	bool IsRoomLeaveAuthorized();

	friend class FGsGuildGroupChatData;
};

class T1PROJECT_API FGsGuildGroupChatData
{
private:
	int32 _maxChatGroupRoomCount{ 5 };
	int32 _maxChatGroupGuildCount{ 20 };
	int32 _maxChatGroupInvitationCount{ 50 };
	int32 _invitationCount{ 0 };

	TArray<TSharedPtr<FGsGuildGroupChatRoom>> _groupChatRooms;
	TArray<TSharedPtr<FGsGuildGroupChatInviteHost>> _groupChatInvitations;

	GuildDBId _myGuildDbId{ INVALID_GUILD_DB_ID };
	WorldId _myWorldId{ INVALID_WORLD_ID };

	bool _refresh{ false };
public:
	FGsGuildGroupChatData() = default;

protected:
	bool GetInitRefresh() { return _refresh; }
	void SetInitRefresh(bool set) { _refresh = set; }
	void Reset();
	void InitGroupChatRoom(GuildDBId myGuildDbId);
	void InitInvitations();

	bool AddGroupChatRoom(FGsGroupChatRoomInfo& info, bool refresh = false);
	bool RemoveChatRoom(GuildGroupChatRoomId roomId);

	void AddGroupChatMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild&& member);
	void AddGroupChatMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& member);
	void RemoveGroupChatMemberGuild(GuildGroupChatRoomId roomId, GuildDBId guildId);

	// 내가 받은 초대장
	void AddGroupChatInvitation(GuildGroupChatRoomId inviteRoomId, FGsGuildGroupChatInviteHost& host);
	void AddGroupChatInvitation(GuildGroupChatRoomId inviteRoomId, FGsGuildGroupChatInviteHost&& host);
	void RemoveGroupChatInvitation(GuildGroupChatRoomId roomId);

	// 내가 보낸 초대장을 받은 길드
	void AddGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& guest);
	void AddGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild&& guest);
	void RemoveGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, GuildDBId guildId);

	void ChangeGroupChatRoomName(GuildGroupChatRoomId roomId, FString roomName);
	void ChangeGroupChatRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guldId);
	void ChangeGroupChatRoomAuthChanged(GuildGroupChatRoomId roomId, GuildMemberGradeType grade);

	int32 GetAllGroupRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms) const;

public:
	bool IsInvitationsToChatRoom() const { return _invitationCount > 0;  }
	
	int32 GetMaxGroupRoomCount() const { return _maxChatGroupRoomCount; }
	int32 GetMaxGuildCountPerRoom() const { return _maxChatGroupGuildCount; }

	int32 GetGroupChatRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms) const;
	TWeakPtr<FGsGuildGroupChatRoom> GetGroupChatRoom(GuildGroupChatRoomId roomId) const;

	int32 GetInvitationsToGroupChat(OUT TArray<TWeakPtr<FGsGuildGroupChatInviteHost>>& invitations) const;
	TWeakPtr<FGsGuildGroupChatInviteHost> GetInvitationToGroupChat(GuildGroupChatRoomId roomId);

	bool IsMemberInGroup(GuildGroupChatRoomId roomId, FString guildName, WorldId worldId = 0) const;
	bool IsInviteGuest(GuildGroupChatRoomId roomId, FString guildName, WorldId worldId = 0) const;

	friend class FGsGuildManager;
	friend class UGsUIPopupChatGroup;
};

using GroupRoomState = FGsGuildGroupChatRoom::GroupRoomState;