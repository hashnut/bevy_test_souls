// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedEnums.h"
#include "Guild/GsGuildPlayerData.h"
#include "Quest/EGsQuestCheckType.h"
#include "Util/GsTableUtil.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GsGameObjectStruct.generated.h"


namespace PD
{
	namespace SC
	{
		struct PKT_SC_SPAWN_NPC_READ;
		struct PKT_SC_SPAWN_PROJECTILE_READ;
		struct PKT_SC_SPAWN_SPECIAL_NPC_READ;
		struct PKT_SC_SPAWN_TREASURE_NPC_READ;
		struct PKT_SC_SPAWN_USER_READ;
		struct PKT_SC_SPAWN_ME_READ;
	}
}

/**
 * 오브젝트 클래스 관련 공통, 테스트용 구조체 모음 
 */

struct FGsSchemaNpcData;

//---------------------------------------------------------------------------------------------------------------------
 // GameObjec 스폰에 필요한 정보 구조체
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParam
{
public:
	int64						_gameId = 0;
	int32						_tId = 0;
	//배치툴에서 사용하는 unique id
	SpawnId						_spawnId = 0;
	mutable EGsGameObjectType	_type = EGsGameObjectType::Base;
	FVector						_pos = FVector::ZeroVector;
	FRotator					_rot = FRotator::ZeroRotator;
	SPAWN_REASON				_spawnReason = SPAWN_REASON::NORMAL;
	NpcSpecialType				_specialType = NpcSpecialType::NONE;

	mutable bool				_async = false;
	const struct FGsSchemaNpcData* _npcData = nullptr;
	//클라이언트에서만 사용하는 변수
public:
	bool _isInitVisible = true;

public:
	virtual ~FGsSpawnParam() = default;
	FGsSpawnParam() {}
	FGsSpawnParam(PD::SC::PKT_SC_SPAWN_NPC_READ* pkt)			{ Set(pkt); }
	FGsSpawnParam(PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* pkt)	{ Set(pkt); }
	FGsSpawnParam(PD::SC::PKT_SC_SPAWN_SPECIAL_NPC_READ* pkt)	{ Set(pkt); }
	FGsSpawnParam(PD::SC::PKT_SC_SPAWN_TREASURE_NPC_READ* pkt)	{ Set(pkt); }
	//Client spawn
	FGsSpawnParam(const struct FGsSchemaSpawnElement& inSpawnElement);
	explicit FGsSpawnParam(EGsGameObjectType Type, int64 GameId, const FVector& Pos, const FRotator& Rot)
	{
		Set(Type, GameId, Pos, Rot);
	}
	FGsSpawnParam(EGsGameObjectType inType, int64 inGameId, int32 inTid, const FVector& inPos, const FRotator& inRot, bool inIsInitVisible = true)
	{
		Set(inType, inGameId, inTid, inPos, inRot, inIsInitVisible);
	}
	FGsSpawnParam(EGsGameObjectType inType, int64 inGameId, int32 inTid, const FVector& inPos, const short inRot, bool inIsInitVisible = true);

	FGsSpawnParam& operator = (PD::SC::PKT_SC_SPAWN_NPC_READ* pkt)
	{
		Set(pkt);
		return *this;
	}
	FGsSpawnParam& operator = (PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* pkt)
	{
		Set(pkt);
		return *this;
	}

	void SetType(EGsGameObjectType Type) const
	{
		_type = Type;
	}

	void Set(PD::SC::PKT_SC_SPAWN_NPC_READ* Npc);
	void Set(PD::SC::PKT_SC_SPAWN_SPECIAL_NPC_READ* inNpc);
	void Set(PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* Projectile);
	void Set(PD::SC::PKT_SC_SPAWN_TREASURE_NPC_READ* treasureNpc);
	void Set(EGsGameObjectType Type, int64 GameId, const FVector& Pos, const FRotator& Rot, bool inIsInitVisible = true);
	void Set(EGsGameObjectType Type, int64 GameId, int32 inTid, const FVector& Pos, const FRotator& Rot, bool inIsInitVisible = true);

public:	
	const struct FGsSchemaNpcData* GetNpcData(int32 id) const;
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const;
	float GetScale() const;
	

protected:
	void SetObjectType(const struct FGsSchemaNpcData* inTbl);
};

//---------------------------------------------------------------------------------------------------------------------
// FGsSpawnParamProjectile
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamProjectile : public FGsSpawnParam
{
public:
	virtual ~FGsSpawnParamProjectile() = default;
	FGsSpawnParamProjectile(PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* pkt) { Set(pkt); }
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;
};

//---------------------------------------------------------------------------------------------------------------------
// FGsSpawnParamWarp
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamWarp : public FGsSpawnParam
{
private:
	struct FGsSchemaMapWarpInfo* _warpData = nullptr;
public:
	virtual ~FGsSpawnParamWarp() = default;

	void SetWarpTable(const FGsSchemaMapWarpInfo* inTbl);
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;
};

//---------------------------------------------------------------------------------------------------------------------
// FGsSpawnParamDropObject
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamDropObject : public FGsSpawnParam
{
public:
	virtual ~FGsSpawnParamDropObject() = default;
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;
};

//---------------------------------------------------------------------------------------------------------------------
// FGsSpawnParamProp
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamProp : public FGsSpawnParam
{
private:
	mutable struct FGsSchemaPropData* _propData = nullptr;
	bool _isActiveInvasion{ false };
public:
	FGsSpawnParamProp(EGsGameObjectType inType, int64 inGameId, int32 inTid, const FVector& inPos, const short inRot, bool inIsInitVisible = true, bool isActiveInvasion = false);	
	virtual ~FGsSpawnParamProp() = default;
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;

	PropType GetPropType() const;
	const struct FGsSchemaPropData* GetPropData() const;

	bool IsInvasionActive() const { return _isActiveInvasion; }
};


//---------------------------------------------------------------------------------------------------------------------
// GameObject Player 타입 스폰에 필요한 정보 구조체
//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamPlayer : public FGsSpawnParam
{
	virtual ~FGsSpawnParamPlayer() = default;

	UserLookInfo _lookInfo;
	CustomizeFaceData _customizeFaceData;
	CustomizeBodyData _customizeBodyData;
	UserStatus _userStatus;
	FString _name;
	uint16 _level;
	UserDBId _userDBId;
	WorldId	 _homeWorldId;
	AccountGrade _accountGrade = AccountGrade::NORMAL;
	FString _gmPrefix;

	FGsGuildPlayerData _guildData;

public:
	FGsSpawnParamPlayer() :FGsSpawnParam() {}
	FGsSpawnParamPlayer(PD::SC::PKT_SC_SPAWN_USER_READ* pkt) { Set(pkt); }
	FGsSpawnParamPlayer& operator = (PD::SC::PKT_SC_SPAWN_USER_READ* pkt)
	{
		Set(pkt);
		return *this;
	}
	void Set(PD::SC::PKT_SC_SPAWN_USER_READ* Remote);
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;

};

//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsSpawnParamLocalPlayer : public FGsSpawnParam
{
	virtual ~FGsSpawnParamLocalPlayer() = default;

	// 이곳에 UserLookInfo, CustomizeFaceData, CustomizeBodyData 정보가 들어있다.
	const struct FGsNetUserData* _netUserData = nullptr;

public:
	FGsSpawnParamLocalPlayer() :FGsSpawnParam() {}
	FGsSpawnParamLocalPlayer(EGsGameObjectType Type, const FVector& Pos, const FRotator& Rot,
		const struct FGsNetUserData* UserBasicData);
	FGsSpawnParamLocalPlayer(PD::SC::PKT_SC_SPAWN_ME_READ* pkt)
	{ 
		Set(pkt); 
	}

	FGsSpawnParamLocalPlayer& operator = (PD::SC::PKT_SC_SPAWN_ME_READ* pkt)
	{
		Set(pkt);
		return *this;
	}

	void Set(PD::SC::PKT_SC_SPAWN_ME_READ* pkt);
	virtual const struct FGsSchemaShapeData* GetShapeData(uint32 id) const override;
};

// 로컬 환경에서 Rootmotion 플레이 테스트용 데이터 구조체
USTRUCT(BlueprintType)
struct FGsLocalTest_RootMotion : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* _resource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _length = 1.f;
};

struct FGsSpawnObjectShowHide
{
public:
	FGsSpawnObjectShowHide() {}

public:
	QuestId StartConditionQuestId;
	QuestId EndConditionQuestId;
	EGsQuestCheckType QuestCheckType;
};

struct FGsTM
{
	FVector		Location	= FVector::ZeroVector;
	FRotator	Rotation	= FRotator::ZeroRotator;
	FVector		Scale		= FVector::OneVector;
};

//#if	!UE_BUILD_SHIPPING
//// NPC 에 대한 디버그용 정보 구조체
//struct FGsDebugTextInfo
//{
//	int64 _gameId{ 0 };
//	int _npcId{ 0 };
//	int _aiDataId{ 0 };
//	int _skillSetId{ 0 };
//	FString _target = FString(TEXT("0"));
//	FString _state = FString(TEXT("0"));
//	int _aggroPenaltyCount{ 0 };
//};
//#endif