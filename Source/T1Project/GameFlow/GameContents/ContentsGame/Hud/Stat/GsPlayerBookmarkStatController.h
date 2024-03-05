// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

class T1PROJECT_API FGsPlayerBookmarkStatController
{
public:
	TArray<StatType> _statArray;
	TArray<const struct FGsSchemaStatFormatingData*> _statFormatDataArray;
	int32 _maxStatBookmarkCount = 20;

public:
	FGsPlayerBookmarkStatController();
	virtual ~FGsPlayerBookmarkStatController();

public:
	void Init();
	void Close();
	void SetBookmarkStat(struct PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ* inPacket);
	void AddBookmarkStat(struct PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_ADD_READ* inPacket);
	void RemoveBookmarkStat(struct PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_DELETE_READ* inPacket);

public:	
	bool AddStatBookmark(StatType inStatType);
	void DeleteStatBookmark(StatType inStatType);
	const TArray<StatType>& GetBookmarkStat() {return _statArray;}
	bool IsBookmarkStat(StatType inStat);	

public:
	//get 
	bool TryGetStat(OUT TArray<StatType>& outFavoritStatArray, OUT TMap<int32, TArray<StatType>>& outSectionStatMap);	
	CreatureWeaponType GetPlayerWeapon();
	int32 GetMaxUserStatBookmarkCount() {return _maxStatBookmarkCount;}

};
