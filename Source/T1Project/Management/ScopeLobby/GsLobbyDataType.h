// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

namespace LobbyDataType {

	//--------------------------------------------------------------------------------------
	// 서버 목록 데이터
	//-------------------------------------------------------------------------------------
	
	struct FGsServerListData
	{
	public:
		WorldId _id;
		FString _serverName;	// 서버명
	public:
		FGsServerListData(const uint16& id, const FString& serverName) : _id(id), _serverName(serverName) {}
	};

	//--------------------------------------------------------------------------------------
	// 유저 목록 데이터
	//--------------------------------------------------------------------------------------
	struct FGsLobbyUserListData
	{
	public:
		UserDBId			_id;
		CreatureId			_creatureId;
		CreatureGenderType	_genderType;
		CreatureWeaponType	_classType;
		Level				_lv;
		FString				_userName;	// 캐릭명
		time_t				_lastLoginTime = 0;
		MapId				_mapId;
			
		UserLookInfo		_lookInfo;
		CustomizeFaceData	_faceData;
		CustomizeBodyData	_bodyData;

		int32				_weaponId;
		int32				_weaponLevel;

		time_t				_expiringTime;
		time_t				_createdTime;

		FString				_guildName;

		bool				_isMigrationNameChanged{ true };
		bool				_isMigrationGuildNameChanged{ true };

	public:
		FGsLobbyUserListData(const uint64 userDBId, const CreatureId creatureId, CreatureGenderType genderType, CreatureWeaponType classType, uint16 level,
			const FString& userName, const time_t& lastTime, MapId mapId, 
			const UserLookInfo& InLookInfo, const CustomizeFaceData& InFaceData, const CustomizeBodyData& InBodyData
			,const int32 inWeaponId, const int32 inWeaponLevel, const time_t& expiringTime, const time_t& createdTime, const FString& guildName
			,bool isAccountMigration = true, bool isGuildMigration = true)
			:
			_id(userDBId), _creatureId(creatureId), _genderType(genderType), _classType(classType), _lv(level),
			_userName(userName), _lastLoginTime(lastTime), _mapId(mapId),
			_lookInfo(InLookInfo), _faceData(InFaceData), _bodyData(InBodyData)
			, _weaponId(inWeaponId), _weaponLevel(inWeaponLevel), _expiringTime(expiringTime), _createdTime(createdTime), _guildName(guildName)
			, _isMigrationNameChanged(isAccountMigration), _isMigrationGuildNameChanged(isGuildMigration)
			{}
	};
}