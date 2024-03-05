//
//#pragma once
//
///**
//* file		GsDungeonBattleArena.h
//* @brief	Dungeon Party Class
//* @author	PKT
//* @date		2023/11/17
//**/
//
//#include "CoreMinimal.h"
//#include "GsDungeonData.h"
//#include "IDelegateInstance.h"
//#include "GsDungeonGroupBase.h"
//#include "../Message/GsMessageItem.h"
//#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
//#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
//
//class FGsDungeonGroupBattleArena : public FGsDungeonGroupBase
//{
//protected:
//
//public:
//	explicit FGsDungeonGroupBattleArena(const struct FGsSchemaDungeonGroupData* InSchemaData);
//
//protected:
//	virtual bool IsDungeonUnLockContentsDelegate(const class FGsDungeonData* InTarget, bool InShowTicker = false) override;
//
//public:
//	bool IsFreeSeason(const class FGsDungeonData* InTarget) const;
//	int32 SeasonNum(const class FGsDungeonData* InTarget) const;
//	const FText SeasonDateText(const class FGsDungeonData* InTarget) const;
//	const FText PlayCycleText(const class FGsDungeonData* InTarget) const;
//
//	int32 ApplyLimitGuildLevel(const class FGsDungeonData* InTarget) const;
//	int32 ApplyLimitUserLevel(const class FGsDungeonData* InTarget) const;
//
//	GuildMemberGradeType ApplyLimitGuildGrade(const class FGsDungeonData* InTarget) const;
//
//	const struct FGsSchemaGuildWarConfigData* GetConfigData(const class FGsDungeonData* InTarget) const;
//};