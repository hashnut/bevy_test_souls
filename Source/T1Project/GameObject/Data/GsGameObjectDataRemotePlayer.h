// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectDataPlayer.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Guild/GsGuildPlayerData.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Title/GsTitlePlayerData.h"

struct FGsUserSkillListInfo;
class UGsGameObjectBase;
class FGsStatInfo;

/**
 * 
 */
class T1PROJECT_API FGsGameObjectDataRemotePlayer : 
	public FGsGameObjectDataPlayer
{

	using Super = FGsGameObjectDataPlayer;

protected:
	CreatureGenderType _creatureGenderType;

	CustomizeFaceData _faceData;
	CustomizeBodyData _bodyData;
	UserLookInfo _userLookInfo;
	
	FGsGuildPlayerData _guildData;
	UserDBId _userDBId;

	// 2022/02/03 PKT - Title(Īȣ) 
	FGsTitlePlayerData _titleData;

	InvadeBattleLevel _invadeBattleLevel = 0;

	//Aggro
	bool _isShowAggro;
	bool _isShowContribution;

	AccountGrade _accountGrade = AccountGrade::NORMAL;

	WorldId _homeWorldId = 0;
	FString _gmPrefix;

public:
	FGsGameObjectDataRemotePlayer(const FGsSpawnParam* SpawnParam);
	virtual ~FGsGameObjectDataRemotePlayer() = default;

public:
	virtual void SetUserLookInfo(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet) override;
	virtual void SetUserLookInfo(const UserLookInfo& data) override;
	virtual void SetCustomizeFaceData(const CustomizeFaceData& data) override;
	virtual void SetCustomizeBodyData(const CustomizeBodyData& data) override;
	virtual void SetCreatureGenderType(CreatureGenderType inGenderType) override;

	virtual void SetGuildEmblemId(GuildEmblemId InEmblemId) override;	
	virtual void SetClearGuildData() override;
	virtual void SetShowAggro(bool inIsAggro) override { _isShowAggro = inIsAggro; }
	virtual void SetShowContribution(bool inIsShowContribution) override { _isShowContribution = inIsShowContribution; }

	// 2022/01/28 PKT - Īȣ
	virtual void SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData) override;
	virtual void SetInvadeBattleLevel(InvadeBattleLevel InLevel) override;

public:
	virtual uint64 GetUserDBID() const override { return _userDBId; }
	virtual CreatureGenderType GetCreatureGenderType() const override		{ return _creatureGenderType; }
	virtual CreatureWeaponType GetCurrentWeaponType() const override;
	virtual ItemGrade		   GetCurrentWeaponGrade() const override;
	virtual const UserLookInfo* GetUserLookInfo() const override			{ return &_userLookInfo; }
	virtual const CustomizeFaceData* GetCustomizeFaceData() const override	{ return &_faceData; }
	virtual const CustomizeBodyData* GetCustomizeBodyData() const override	{ return &_bodyData; }
	virtual const FGsGuildPlayerData* GetGuildData() const override			{ return &_guildData; }

	virtual bool IsEnemyGuild() const override { return _guildData.IsEnemy(); }
	virtual bool IsAllyGuild() const { return _guildData.IsAlly(); }
	virtual bool IsMyGuild() const { return _guildData.IsMyGuild(); }
	virtual bool IsShowAggro() const override { return _isShowAggro; }
	virtual bool IsShowContribution() const override { return _isShowContribution; }

	virtual const FGsTitlePlayerData* GetTitleData() const override;
	virtual InvadeBattleLevel GetInvadeBattleLevel() const override;

	virtual AccountGrade GetAccountGrade() const override { return _accountGrade; }
	virtual WorldId GetHomeWorldId() const override { return _homeWorldId; }
	virtual FString GetGMPrefix() const override { return _gmPrefix; }

public:
	void SetGuildData(PD::SC::PKT_SC_NOTIFY_USER_GUILD_INFO_READ& InPacket);	
	void CheckAndSetGuildDiplomacy();
};
