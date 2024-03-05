// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectDataCreature.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Title/GsTitlePlayerData.h"

/**
 * 이 클래스에서 공통으로 접근 할수있는 인터페이스 함수를 정의하고
 * 상속 받은 각클래스에서 오버라이드 하여 구현
 */
class T1PROJECT_API FGsGameObjectDataPlayer :
	public FGsGameObjectDataCreature
{
	using Super = FGsGameObjectDataCreature;

private:
	/** 천마석을 사용할 것인지에 대한 상태 */
	bool _isActiveSpiritShot = false;

	/** 천마석 누적 합산 레벨 (달성 레벨에 따라 이펙트 종류와 스케일을 분기해야 한다) */
	Level _spiritShotSumLevel;

public:
	FGsGameObjectDataPlayer(const FGsSpawnParam* SpawnParam);
	virtual ~FGsGameObjectDataPlayer() = default;

public:
	virtual void SetCustomizeFaceData(const CustomizeFaceData& data) {}
	virtual void SetCustomizeBodyData(const CustomizeBodyData& data) {}

	virtual void SetUserLookInfo(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet) {}
	virtual void SetUserLookInfo(const UserLookInfo& data) {}
	virtual void SetCreatureGenderType(CreatureGenderType inGenderType) {}

	virtual void SetGuildEmblemId(GuildEmblemId InEmblemId) {}
	virtual void SetClearGuildData() {}
	virtual void SetShowAggro(bool inAggro){}
	virtual void SetShowContribution(bool inAggro) {}

	// 2022/01/28 PKT - 칭호
	virtual void SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData) {}
	virtual void SetInvadeBattleLevel(InvadeBattleLevel InLevel) {}

public:
	virtual uint64 GetUserDBID() const = 0;
	virtual CreatureGenderType GetCreatureGenderType() const = 0;
	virtual CreatureWeaponType GetCurrentWeaponType() const = 0;
	virtual ItemGrade		   GetCurrentWeaponGrade() const = 0;
	virtual const UserLookInfo* GetUserLookInfo() const = 0;
	virtual const CustomizeFaceData* GetCustomizeFaceData() const = 0;
	virtual const CustomizeBodyData* GetCustomizeBodyData() const = 0;
	virtual const struct FGsGuildPlayerData* GetGuildData() const = 0;
	 
	virtual bool IsEnemyGuild() const { return false; }
	virtual bool IsAllyGuild() const { return false; }
	virtual bool IsMyGuild() const { return false; }
	virtual bool IsShowAggro() const { return false;}
	virtual bool IsShowContribution() const { return false; }

	virtual const FGsTitlePlayerData* GetTitleData() const = 0;
	virtual InvadeBattleLevel GetInvadeBattleLevel() const = 0;

	virtual AccountGrade GetAccountGrade() const { return AccountGrade::NORMAL; }
	virtual FString GetGMPrefix() const { return TEXT(""); }

public:
	bool GetIsActiveSpiritShot() const { return _isActiveSpiritShot; }
	void SetIsActiveSpiritShot(bool In_val)  { _isActiveSpiritShot = In_val; }

	Level GetSpiritShotSumLevel() const { return _spiritShotSumLevel; }
	void SetSpiritShotSumLevel(Level InLevel) { _spiritShotSumLevel = InLevel; }
};
