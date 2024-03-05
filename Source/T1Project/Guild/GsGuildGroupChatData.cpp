#include "GsGuildGroupChatData.h"
#include "DataSchema/Chat/GsSchemaChatConfig.h"
#include "Management//ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Guild/GsGuildHelper.h"

void FGsGuildGroupChatRoom::SetData(FGsGroupChatRoomInfo& info)
{
	_changingAuthGrade = info._authorizedGrade;
	info._masterGuildDBId = info._masterGuildDBId != INVALID_GUILD_DB_ID ? info._masterGuildDBId : _info._masterGuildDBId;

	_info = CopyTemp(info);
	_roomState = GroupRoomState::Playing;
}

void FGsGuildGroupChatRoom::ResetData()
{
	_roomId = INVALID_GUILD_GROUP_CHAT_ROOM_ID;
	_info._roomId = INVALID_GUILD_GROUP_CHAT_ROOM_ID;
	_info._guildCount = 0;
	_roomState = GroupRoomState::None;
	_info._authorizedGrade = GuildMemberGradeType::KNIGHT;
	_changingAuthGrade = GuildMemberGradeType::KNIGHT;
	_info._chatRoomName.Empty();
	_myGuildDBId = INVALID_GUILD_DB_ID;
	_masterGuild = nullptr;
	_memberGuilds.Reset();
	_inviteGuilds.Reset();
	_toMyInvitations.Reset();
}

void FGsGuildGroupChatRoom::ResetInvitations()
{
	_toMyInvitations.Reset();
}

void FGsGuildGroupChatRoom::AddMemberGuild(FGsGuildGroupChatMemberGuild& member)
{
	if (IsValidGroup() == false) return;

	auto guildDBId = member._guildDBId;
	int32 removedCount = _inviteGuilds.RemoveAll([guildDBId](auto e) { return e->_guildDBId == guildDBId; });

	member._isNew = removedCount > 0;
	member._isMaster = _info._masterGuildDBId != INVALID_GUILD_DB_ID && _info._masterGuildDBId == guildDBId;
	member._state = FGsGuildGroupChatMemberGuild::GuildMemberState::Joined;

	if (auto iter = _memberGuilds.FindByPredicate([guildDBId](auto e) { return e->_guildDBId == guildDBId; }))
	{
		*iter->Get() = member;
	}
	else
	{
		auto data = MakeShared<FGsGuildGroupChatMemberGuild>();
		data.Get() = member;

		_memberGuilds.Emplace(data);

		if (member._isMaster) _masterGuild = data;
	}
}

void FGsGuildGroupChatRoom::AddMemberGuild(FGsGuildGroupChatMemberGuild&& member)
{
	if (IsValidGroup() == false) return;

	auto guildDBId = member._guildDBId;
	int32 removedCount = _inviteGuilds.RemoveAll([guildDBId](auto e) { return e->_guildDBId == guildDBId; });

	member._isNew = removedCount > 0;
	member._isMaster = _info._masterGuildDBId != INVALID_GUILD_DB_ID && _info._masterGuildDBId == guildDBId;
	member._state = FGsGuildGroupChatMemberGuild::GuildMemberState::Joined;

	if (auto iter = _memberGuilds.FindByPredicate([guildDBId](auto e) { return e->_guildDBId == guildDBId; }))
	{
		*iter->Get() = member;
	}
	else
	{
		auto data = MakeShared<FGsGuildGroupChatMemberGuild>();
		data.Get() = member;

		_memberGuilds.Emplace(data);

		if (member._isMaster) _masterGuild = data;
	}
}

void FGsGuildGroupChatRoom::RemoveMemberGuild(GuildDBId guildDBId)
{
	if (IsValidGroup() == false) return;

	_inviteGuilds.RemoveAll([guildDBId](auto e) { return e->_guildDBId == guildDBId; });
	_memberGuilds.RemoveAll([guildDBId](auto e) { return e->_guildDBId == guildDBId; });
}

void FGsGuildGroupChatRoom::AddInviteGuestGuild(FGsGuildGroupChatMemberGuild& guest)
{
	if (IsValidGroup() == false) return;

	auto guildDBId = guest._guildDBId;
	
	guest._isMaster = false;
	guest._state = FGsGuildGroupChatMemberGuild::GuildMemberState::InvitationGuest;
	if (auto iter = _inviteGuilds.FindByPredicate([guildDBId](auto e) { return e->_guildDBId == guildDBId; }))
	{
		*iter->Get() = guest;
	}
	else
	{
		auto data = MakeShared<FGsGuildGroupChatMemberGuild>();
		data.Get() = guest;

		_inviteGuilds.Emplace(data);
	}
}

void FGsGuildGroupChatRoom::AddInviteGuestGuild(FGsGuildGroupChatMemberGuild&& guest)
{
	if (IsValidGroup() == false) return;

	auto guildDBId = guest._guildDBId;

	guest._isMaster = false;
	guest._state = FGsGuildGroupChatMemberGuild::GuildMemberState::InvitationGuest;
	if (auto iter = _inviteGuilds.FindByPredicate([guildDBId](auto e) { return e->_guildDBId == guildDBId; }))
	{
		*iter->Get() = guest;
	}
	else
	{
		auto data = MakeShared<FGsGuildGroupChatMemberGuild>();
		data.Get() = guest;

		_inviteGuilds.Emplace(data);
	}
}

void FGsGuildGroupChatRoom::RemoveInviteGuestGuild(GuildDBId guildDBId)
{
	if (IsValidGroup() == false) return;

	_inviteGuilds.RemoveAll([guildDBId](auto e) { return e->_guildDBId == guildDBId; });
}

void FGsGuildGroupChatRoom::SetMasterGuild(GuildDBId guildDbId)
{
	_info._masterGuildDBId = guildDbId;

	for (auto member : _memberGuilds)
	{
		member->_isMaster = guildDbId == member->_guildDBId;
		if (member->_isMaster)
		{
			_masterGuild = member;
		}
	}
}

int FGsGuildGroupChatRoom::GetMemberList(TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>>& memberList)
{
	memberList.Append(_memberGuilds);
	return memberList.Num();
}

TWeakPtr<FGsGuildGroupChatMemberGuild> FGsGuildGroupChatRoom::GetMember(GuildDBId guildDbId)
{
	if (auto iter = _memberGuilds.FindByPredicate([guildDbId](auto e) {return e && e->_guildDBId == guildDbId; }))
	{
		return *iter;
	}
	return nullptr;
}

int FGsGuildGroupChatRoom::GetInviteGuestList(TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>>& inviteList)
{
	inviteList.Append(_inviteGuilds);
	return inviteList.Num();
}

bool FGsGuildGroupChatRoom::IsRoomMasterAuthorized()
{
	bool isDiplomat = FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority());
	return (!IsValidGroup() || IsRoomMasterGuild()) && isDiplomat;
}

bool FGsGuildGroupChatRoom::IsRoomLeaveAuthorized()
{
	bool isDiplomat = FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority());
	return IsRoomMasterGuild() ? isDiplomat && GetMemberCount() <= 1 : isDiplomat;
}

/// <summary>
/// FGsGuildGroupChatData
/// </summary>
void FGsGuildGroupChatData::Reset()
{
	_groupChatInvitations.Reset();
	_groupChatRooms.Reset();
	_invitationCount = 0;
}

void FGsGuildGroupChatData::InitGroupChatRoom(GuildDBId myGuildDbId)
{
	_myWorldId = GGameData()->GetUserData()->mHomeWorldId;

	_myGuildDbId = myGuildDbId;
	_groupChatRooms.Reset();
	if (const auto table = Cast<UGsTableChatConfig>(FGsSchemaChatConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaChatConfig*> chatConfigs;
		table->GetAllRows(chatConfigs);

		if (chatConfigs.IsValidIndex(0))
		{
			const auto chatConfig = chatConfigs[0];

			_maxChatGroupRoomCount = chatConfig->groupChatRoomMaxCount;
			_maxChatGroupGuildCount = chatConfig->groupChaRoomMemberMaxCount;
		}
	}

	for (int index = 0; index < _maxChatGroupRoomCount; index++)
	{
		_groupChatRooms.Add(MakeShared<FGsGuildGroupChatRoom>());
	}

	_groupChatRooms[0]->_roomState = GroupRoomState::InvitationHost;
}

void FGsGuildGroupChatData::InitInvitations()
{
	_groupChatInvitations.Reset();
	_invitationCount = 0;
	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return e->IsWaitingRoom(); }))
	{
		(*room)->ResetInvitations();
	}
}

bool FGsGuildGroupChatData::AddGroupChatRoom(FGsGroupChatRoomInfo& info, bool refresh /*= false*/)
{
	bool isNewAdd = false;
	if (info._roomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID) return false;

	if (auto iterRoom = _groupChatRooms.FindByPredicate([&info](auto e) { return e->GetRoomId() == info._roomId; }))
	{
		(*iterRoom)->SetData(info);
	}
	else
	{
		isNewAdd = true;
		auto index = _groupChatRooms.Emplace(MakeShared<FGsGuildGroupChatRoom>(info._roomId, _myGuildDbId));
		_groupChatRooms[index]->SetData(info);
		_groupChatRooms[index]->_isNewRoom = refresh;
	}

	_groupChatRooms.StableSort([](auto pe, auto fe) {
		if (pe->IsValidGroup() != fe->IsValidGroup())
		{
			return pe->IsValidGroup();
		}

		if (pe->IsWaitingRoom() != fe->IsWaitingRoom())
		{
			return pe->IsWaitingRoom();
		}
		return false;

		});

	while (_groupChatRooms.Num() > _maxChatGroupRoomCount)
	{
		_groupChatRooms.RemoveAt(_maxChatGroupRoomCount - 1);
	}

	_invitationCount = 0;
	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return e->IsWaitingRoom(); }))
	{
		_invitationCount = (*room)->_toMyInvitations.Num();
	}
	return isNewAdd;
}


bool FGsGuildGroupChatData::RemoveChatRoom(GuildGroupChatRoomId roomId)
{
	if (roomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID) return false;

	_groupChatRooms.RemoveAll([roomId](auto e) { return e->GetRoomId() == roomId; });

	while (_groupChatRooms.Num() < _maxChatGroupRoomCount)
	{
		_groupChatRooms.Add(MakeShared<FGsGuildGroupChatRoom>());
	}

	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return !e->IsValidGroup(); }))
	{
		if ((*room)->IsWaitingRoom() == false)
		{
			(*room)->_roomState = GroupRoomState::InvitationHost;
			return false;
		}
	}
	return true;
}

void FGsGuildGroupChatData::AddGroupChatMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& member)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		member._isHomeWorldGuild = _myWorldId == member._homePlanetWorldId.st.worldId;
		(*iter)->AddMemberGuild(member);
	}
}

void FGsGuildGroupChatData::AddGroupChatMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild&& member)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		member._isHomeWorldGuild = _myWorldId == member._homePlanetWorldId.st.worldId;
		(*iter)->AddMemberGuild(member);
	}
}

void FGsGuildGroupChatData::RemoveGroupChatMemberGuild(GuildGroupChatRoomId roomId, GuildDBId guildId)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		(*iter)->RemoveMemberGuild(guildId);
	}
}
void FGsGuildGroupChatData::AddGroupChatInvitation(GuildGroupChatRoomId inviteRoomId, FGsGuildGroupChatInviteHost& host)
{
	if (inviteRoomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID) return;

	_groupChatInvitations.RemoveAll([inviteRoomId](auto e) { return e->GetRoomId() == inviteRoomId; });

	host._guildData._isHomeWorldGuild = _myWorldId == host._guildData._homePlanetWorldId.st.worldId;
	host._guildData._state = FGsGuildGroupChatMemberGuild::GuildMemberState::WaitMyResponse;

	auto hostData = MakeShared<FGsGuildGroupChatInviteHost>();
	hostData.Get() = host;

	_groupChatInvitations.Add(hostData);

	_invitationCount = 0;
	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return e->IsWaitingRoom(); }))
	{
		(*room)->_toMyInvitations.Emplace(inviteRoomId);
		_invitationCount = (*room)->_toMyInvitations.Num();
	}
}

void FGsGuildGroupChatData::AddGroupChatInvitation(GuildGroupChatRoomId inviteRoomId, FGsGuildGroupChatInviteHost&& host)
{
	if (inviteRoomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID) return;

	_groupChatInvitations.RemoveAll([inviteRoomId](auto e) { return e->GetRoomId() == inviteRoomId; });

	host._guildData._isHomeWorldGuild = _myWorldId == host._guildData._homePlanetWorldId.st.worldId;
	host._guildData._state = FGsGuildGroupChatMemberGuild::GuildMemberState::WaitMyResponse;

	auto hostData = MakeShared<FGsGuildGroupChatInviteHost>();
	hostData.Get() = host;

	_groupChatInvitations.Add(hostData);

	_invitationCount = 0;
	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return e->IsWaitingRoom(); }))
	{
		(*room)->_toMyInvitations.Emplace(inviteRoomId);
		_invitationCount = (*room)->_toMyInvitations.Num();
	}
}

void FGsGuildGroupChatData::RemoveGroupChatInvitation(GuildGroupChatRoomId roomId)
{
	_groupChatInvitations.RemoveAll([roomId](auto e) { return e->GetRoomId() == roomId; });

	_invitationCount = 0;
	if (auto room = _groupChatRooms.FindByPredicate([](auto e) { return e->IsWaitingRoom(); }))
	{
		(*room)->_toMyInvitations.Remove(roomId);
		_invitationCount = (*room)->_toMyInvitations.Num();
	}
}

void FGsGuildGroupChatData::AddGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& guest)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		guest._isHomeWorldGuild = _myWorldId == guest._homePlanetWorldId.st.worldId;
		(*iter)->AddInviteGuestGuild(guest);
	}
}

void FGsGuildGroupChatData::AddGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild&& guest)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		guest._isHomeWorldGuild = _myWorldId == guest._homePlanetWorldId.st.worldId;
		(*iter)->AddInviteGuestGuild(guest);
	}
}

void FGsGuildGroupChatData::ChangeGroupChatRoomName(GuildGroupChatRoomId roomId, FString name)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		(*iter)->SetGroupRoomName(name);
	}
}

void FGsGuildGroupChatData::ChangeGroupChatRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guldId)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		(*iter)->SetMasterGuild(guldId);
	}
}

void FGsGuildGroupChatData::ChangeGroupChatRoomAuthChanged(GuildGroupChatRoomId roomId, GuildMemberGradeType grade)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		(*iter)->SetAuthorizedGrade(grade);
	}
}

void FGsGuildGroupChatData::RemoveGroupChatInvitationGuestGuild(GuildGroupChatRoomId roomId, GuildDBId guildId)
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
	{
		(*iter)->RemoveInviteGuestGuild(guildId);
	}
}

int32 FGsGuildGroupChatData::GetAllGroupRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms) const
{
	rooms.Append(_groupChatRooms);
	return rooms.Num();
}

int32 FGsGuildGroupChatData::GetGroupChatRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms) const
{
	auto validChatgroupList = _groupChatRooms.FilterByPredicate([](auto e) { return e->IsValidGroup(); });

	rooms.Append(validChatgroupList);
	return rooms.Num();
}

TWeakPtr<FGsGuildGroupChatRoom> FGsGuildGroupChatData::GetGroupChatRoom(GuildGroupChatRoomId roomId) const
{
	if (auto iter = _groupChatRooms.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
		return *iter;
	return nullptr;
}

int32 FGsGuildGroupChatData::GetInvitationsToGroupChat(OUT TArray<TWeakPtr<FGsGuildGroupChatInviteHost>>& invitations) const
{
	invitations.Append(_groupChatInvitations);
	return invitations.Num();
}

TWeakPtr<FGsGuildGroupChatInviteHost> FGsGuildGroupChatData::GetInvitationToGroupChat(GuildGroupChatRoomId roomId)
{
	if (auto iter = _groupChatInvitations.FindByPredicate([roomId](auto e) { return e->GetRoomId() == roomId; }))
		return *iter;
	return nullptr;
}

bool FGsGuildGroupChatData::IsMemberInGroup(GuildGroupChatRoomId roomId, FString guildName, WorldId worldId /*= 0*/) const
{
	if (auto room = GetGroupChatRoom(roomId).Pin())
	{
		if (worldId == 0) worldId = _myWorldId;

		TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>> memberList;
		if (room->GetMemberList(memberList) > 0)
		{
			return memberList.FindByPredicate([guildName, worldId](auto e) {
				return e.Pin()->_homePlanetWorldId == worldId && e.Pin()->_guildName == guildName; }) != nullptr;
		}
	}

	return false;
}

bool FGsGuildGroupChatData::IsInviteGuest(GuildGroupChatRoomId roomId, FString guildName, WorldId worldId /*= 0*/) const
{
	if (auto room = GetGroupChatRoom(roomId).Pin())
	{
		if (worldId == 0) worldId = _myWorldId;

		TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>> inviteGuests;
		if (room->GetInviteGuestList(inviteGuests) > 0)
		{
			return inviteGuests.FindByPredicate([guildName, worldId](auto e) {
				return e.Pin()->_homePlanetWorldId == worldId && e.Pin()->_guildName == guildName; }) != nullptr;
		}
	}

	return false;
}
