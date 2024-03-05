//
//#pragma once
//
///**
//* file		GsDungeonGuildWar.h
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
//#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
//
//class FGsDungeonGuildWar : public FGsDungeonGroupBase
//{
//protected:	
//
//public:
//	explicit FGsDungeonGuildWar(const struct FGsSchemaDungeonGroupData* InSchemaData);
//
//protected:
//	virtual bool IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker = false) override;
//
//public:
//	const struct FGsSchemaGuildWarConfigData* GetConfigData(const FGsDungeonData* InTarget);
//};