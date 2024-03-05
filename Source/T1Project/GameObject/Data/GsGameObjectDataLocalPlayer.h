// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectDataPlayer.h"
#include "GameObject/Customize/Data/GsCustomizeData.h"
#include "GameObject/UseItemEffect/SpiritShot/GsSpiritShotDefine.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Title/GsTitlePlayerData.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 내 캐릭터에 해당 되는 데이터 정보
 * 내캐릭터의 경우는 생성 되기 이전에 미리 받는 데이터 구조가 존재 (FGsNetUserData)
 * UserBasic Data에 해당되는 정보들은 Creature 와 LocalPlayer 에서 Getter 구분이 필요 (ex GetUserStatus() ) 
 */
class T1PROJECT_API FGsGameObjectDataLocalPlayer final 
	: public FGsGameObjectDataPlayer
{
	using Super = FGsGameObjectDataPlayer;

protected:
	// User Basic 데이터 
	// 원본은 FGsGameDataManager 클래스에서 생성 되어 있다.
	const FGsNetUserData*	_userBasicInfo	= nullptr;
	
	// 천마석(정령탄) 상태
	EGsSpiritShotState _spiritShotState = EGsSpiritShotState::HUD_OFF;
	
	//Aggro
	bool _isShowAggro;
	bool _isShowContribution;

public:
	//FGsGameObjectDataLocalPlayer(int64 GameId, int TId);a

	// 생성과 동시에 UserData 정보를 설정해준다. 
	explicit FGsGameObjectDataLocalPlayer(const FGsSpawnParam* SpawnParam);
	virtual ~FGsGameObjectDataLocalPlayer() = default;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	virtual void SetUserLookInfo(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet) override;
	virtual void SetUserStatus(UserStatus inStatus) override;
	virtual void SetTerritoryType(TerritoryType inTerritoryType) override;
	virtual void SetLevel(uint16 InLevel) override;
	virtual void SetName(const FString& InName) override;

	virtual void SetGuildEmblemId(GuildEmblemId InEmblemId) override;
	virtual void SetClearGuildData() override;
	virtual void SetShowAggro(bool inIsAggro) override {_isShowAggro = inIsAggro;}
	virtual void SetShowContribution(bool inIsShowContribution) override { _isShowContribution = inIsShowContribution; }

	// 2022/01/28 PKT - 칭호
	virtual void SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData) override;
	// 2022/08/26 PKT - 배틀 등급
	virtual void SetInvadeBattleLevel(InvadeBattleLevel InLevel) override;
	
public:
	//-----------------------------------------------------------------------------
	// UserBasicData
	// UserBasicData Set은 FGsGameDataManager.FGsNetUserData 에서 처리 해야 할것
	//-----------------------------------------------------------------------------
	virtual uint64 GetUserDBID() const override;
	uint64 GetUserExp() const;
	const TArray<FGsNetUserData::FSkillSlot>* GetUserSkillList() const;
	const FGsNetUserData::FSkillSlot* GetUserSkillInfo(int32 SkillId) const;

	virtual uint16 GetLevel() const override;
	virtual FString GetName() const override;
	virtual const UserLookInfo* GetUserLookInfo() const override;
	virtual const CustomizeFaceData* GetCustomizeFaceData() const override;
	virtual const CustomizeBodyData* GetCustomizeBodyData() const override;
	virtual UserStatus GetUserStatus() const override;
	virtual TerritoryType GetTerritoryType() const override;
	virtual CreatureGenderType GetCreatureGenderType() const override;
	virtual CreatureWeaponType GetCurrentWeaponType() const override;
	virtual ItemGrade		   GetCurrentWeaponGrade() const override;
	virtual const FGsGuildPlayerData* GetGuildData() const override;	
	virtual bool IsMyGuild() const override;
	virtual bool IsShowAggro() const override {return _isShowAggro;}
	virtual bool IsShowContribution()  const override { return _isShowContribution; }
	virtual AccountGrade GetAccountGrade() const override;
	virtual WorldId GetHomeWorldId() const override;
	virtual FString GetGMPrefix() const override;

	//-----------------------------------------------------------------------------

public:
	EGsSpiritShotState GetSpiritShotState()
	{
		return _spiritShotState;
	}
	void SetSpiritShotState(EGsSpiritShotState In_state)
	{
		_spiritShotState = In_state;
	}

	// 2022/01/28 PKT - 칭호
public:
	virtual const FGsTitlePlayerData* GetTitleData() const override;
	virtual InvadeBattleLevel GetInvadeBattleLevel() const override;
};
