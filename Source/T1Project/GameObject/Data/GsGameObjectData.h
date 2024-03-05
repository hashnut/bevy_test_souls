// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"

/**
 * 서버에서 내려온 각종 동적 정보 및 맵전환 후에도 삭제되지 않는 캐싱 정보 보관 클래스
 */
class T1PROJECT_API FGsGameObjectData
{
public:
	// Message 시스템으로 처리 하기 애매한 부분이 있어 델리게이트 처리
	// GameObjectData를 소유하고있는 객체에서(UGsGameObjectClass) 바인딩 하여 쓰기 때문에 Multi일 필요가 없을수 있다.
	DECLARE_MULTICAST_DELEGATE_OneParam(FGsGameObjectDataDelegate, EGsGameObjectDataMember);

protected:
	// Object Guid
	int64 _gameId;
	// Object Table Id
	int _tId;
	SpawnId _spawnId;

	// 나의 몸집의 반 (실제 Actor의 UCapsuleComponent 설정값)
	float _scaledCapsuleRadius = 1.0f;
	// 캡슐 높이 반 (실제 Actor의 UCapsuleComponent 설정값)
	float _scaledCapsuleHalfHeight = 1.0f;
	// 전투 근접거리 활용변수
	float _approachDistanceSquared = 1.0f;

	// Iff
	IffFactionActor _iffFaction;
	//
	TerritoryType _territoryType = TerritoryType::NONE;

	// 유저이름
	FString _name;
	// 레벨
	uint16 _level = 0;
	
	FGsGameObjectDataDelegate _onChangeGameObjectDataDelegate;

public:
	FGsGameObjectData(const struct FGsSpawnParam* SpawnParam);
	virtual ~FGsGameObjectData()	= default;

public:
	virtual void Initialize();
	virtual void Finalize();

public:
	int64 GetGameId() const						{ return _gameId; }
	//배치툴에서 사용하는 unique id
	SpawnId GetSpawnId() const					{ return _spawnId;}
	int GetTableId() const						{ return _tId; }

	float GetScaledCapsuleRadius() const		{ return _scaledCapsuleRadius; }
	float GetScaledCapsuleHalfHeight() const	{ return _scaledCapsuleHalfHeight; }
	float GetApproachDistanceSquared() const	{ return _approachDistanceSquared; }

	IffFactionActor& GetIff()					{ return _iffFaction; }	
	PartyId GetPartyId() const					{ return _iffFaction.mPartyId; }
	GuildDBId GetGuildId() const				{ return _iffFaction.mGuildId; }

	// 삭제 예정
	IffTeamType GetIffTeam() const { return IffTeamType::MAX; }

	virtual FString GetName() const				{ return _name; }
	virtual uint16 GetLevel() const				{ return _level; }
	virtual TerritoryType GetTerritoryType() const { return _territoryType; }

public:
	FGsGameObjectDataDelegate& OnDelegate()		{ return _onChangeGameObjectDataDelegate; }

public:
	void SetIffFaction(const IffFactionActor& FactionActor)	{ _iffFaction = FactionActor; }

public:
	void SetScaledCapsuleRadius(float Value)	 { _scaledCapsuleRadius = Value; }
	void SetScaledCapsuleHalfHeight(float Value) { _scaledCapsuleHalfHeight = Value; }
	void SetApproachDistanceSquared(float Value) { _approachDistanceSquared = Value; }
	
	virtual void SetTerritoryType(TerritoryType inTerritoryType) { _territoryType = inTerritoryType; }
	virtual void SetName(const FString& InName)	 { _name = InName; }
	virtual void SetLevel(uint16 InLevel);

	virtual WorldId GetHomeWorldId() const { return 0; }

protected:
	void OnSend(EGsGameObjectDataMember MemberType);
};
