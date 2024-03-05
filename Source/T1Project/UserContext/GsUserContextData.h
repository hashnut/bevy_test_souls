#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


struct FGsUserContextInfo
{
protected:
	UserDBId _userDBId = INVALID_USER_DB_ID;
	FString _userName;
	Level _userLevel = INVALID_LEVEL;
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	FString _guildName;
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	bool _isVisible = false;
	AccountGrade _accountGrade = AccountGrade::NORMAL;
	bool _isSameWorldUser = true;
	WorldId _homeWorldId = INVALID_WORLD_ID;

public:
	FGsUserContextInfo() = default;
	FGsUserContextInfo(FGsUserContextInfo& InContextInfo)
	{
		_userDBId = InContextInfo.GetUserDBId();
		_userName = InContextInfo.GetUserName();
		_userLevel = InContextInfo.GetUserLevel();
		_guildDBId = InContextInfo.GetGuildDBId();
		_guildName = InContextInfo.GetGuildName();
		_guildEmblemId = InContextInfo.GetGuildEmblemId();
		_isVisible = InContextInfo.IsVisible();
		_accountGrade = InContextInfo.GetAccountGrade();
		_isSameWorldUser = InContextInfo.IsSameWorldUser();
		_homeWorldId = InContextInfo.GetHomeWorldId();
	}

	FGsUserContextInfo& operator=(const FGsUserContextInfo& InContextInfo)
	{
		_userDBId = InContextInfo.GetUserDBId();
		_userName = InContextInfo.GetUserName();
		_userLevel = InContextInfo.GetUserLevel();
		_guildDBId = InContextInfo.GetGuildDBId();
		_guildName = InContextInfo.GetGuildName();
		_guildEmblemId = InContextInfo.GetGuildEmblemId();
		_isVisible = InContextInfo.IsVisible();
		_accountGrade = InContextInfo.GetAccountGrade();
		_isSameWorldUser = InContextInfo.IsSameWorldUser();
		_homeWorldId = InContextInfo.GetHomeWorldId();

		return *this;
	}

	explicit FGsUserContextInfo(PD::GC::PKT_GC_ACK_USER_CONTEXT_UI_READ& InPacket, WorldId InWorldId)
		: _userDBId(InPacket.UserDBId()), _userName(TEXT("")), _userLevel(InPacket.UserLevel()), _guildDBId(InPacket.GuildDBId()),
		_guildName(InPacket.GuildName()), _guildEmblemId(InPacket.GuildEmblemId()), _isVisible(InPacket.IsVisible()), _accountGrade(InPacket.AccountGrade()),
		_isSameWorldUser(true), _homeWorldId(InWorldId)
	{}

	explicit FGsUserContextInfo(PD::SC::PKT_SC_ACK_USER_CONTEXT_UI_OTHER_SERVER_READ& InPacket)
		: _userDBId(InPacket.UserDBId()), _userName(TEXT("")), _userLevel(InPacket.UserLevel()), _guildDBId(InPacket.GuildDBId()),
		_guildName(InPacket.GuildName()), _guildEmblemId(InPacket.GuildEmblemId()), _isVisible(InPacket.IsVisible()), _accountGrade(InPacket.AccountGrade()),
		_isSameWorldUser(false), _homeWorldId(InPacket.UserWorldId())
	{}

public:
	void SetUserDBId(UserDBId InUserDBId) { _userDBId = InUserDBId; }
	void SetUserName(const FString& InUserName) { _userName = InUserName; }
	void SetUserLevel(Level InUserLevel) { _userLevel = InUserLevel; }
	void SetGuildDBId(GuildDBId InGuildDBId) { _guildDBId = InGuildDBId; }
	void SetGuildName(const FString& InGuildName) { _guildName = InGuildName; }
	void SetGuildEmblemId(GuildEmblemId InGuildEmblemId) { _guildEmblemId = InGuildEmblemId; }
	void SetIsVisible(bool InIsVisible) { _isVisible = InIsVisible; }
	void SetAccountGrade(AccountGrade InAccountGrade) { _accountGrade = InAccountGrade; }
	void SetIsSameWorldUser(bool InSameWorldUser) { _isSameWorldUser = InSameWorldUser; }
	void SetHomeWorldId(WorldId InHomeWorldId) { _homeWorldId = InHomeWorldId; }

	UserDBId GetUserDBId() const { return _userDBId; }
	const FString& GetUserName() const { return _userName; }
	Level GetUserLevel() const { return _userLevel; }
	GuildDBId GetGuildDBId() const { return _guildDBId; }
	const FString& GetGuildName() const { return _guildName; }
	GuildEmblemId GetGuildEmblemId() const { return _guildEmblemId; }
	bool IsVisible() const { return _isVisible; }
	AccountGrade GetAccountGrade() const { return _accountGrade; }
	bool IsSameWorldUser() const { return _isSameWorldUser; }
	WorldId GetHomeWorldId() const { return _homeWorldId; }

};