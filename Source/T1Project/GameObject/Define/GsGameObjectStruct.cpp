#include "GsGameObjectStruct.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/GameObject/DropObject/GsSchemaDropObject.h"
#include "DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"

#include "GameObject/Define/EGsDropItemCategory.h"

#include "T1Project.h"
#include "UTIL/GsDir.h"
#include "UTIL/GsItemUtil.h"
#include "UTIL/GsGameObjectUtil.h"


FGsSpawnParam::FGsSpawnParam(const FGsSchemaSpawnElement& inSpawnElement)
{
	_gameId = inSpawnElement.ClientGameId;	
	_tId = inSpawnElement.CreatureId;
	_spawnId = inSpawnElement.SpawnId;
	_type = UGsGameObjectUtil::GetObjectType(inSpawnElement.SpawnType);
	_pos = inSpawnElement.Pos;
	_rot = FGsDir::AngleToRotator(inSpawnElement.Dir);
	_isInitVisible = inSpawnElement.InitVisible;
}

FGsSpawnParam::FGsSpawnParam(EGsGameObjectType inType, int64 inGameId, int32 inTid, const FVector& inPos, 
	const short inRot, bool inIsInitVisible)
{
	Set(inType, inGameId, inTid, inPos, FGsDir::AngleToRotator(inRot), inIsInitVisible);
}

void FGsSpawnParam::Set(PD::SC::PKT_SC_SPAWN_NPC_READ* Npc)
{
	_type = EGsGameObjectType::NonPlayer;
	_gameId = Npc->GameId();
	_spawnId = Npc->NpcSpawnId();
	_tId = Npc->NpcTemplateId();
	_pos = Npc->Pos();
	_rot = FGsDir::AngleToRotator(Npc->Dir());
	_spawnReason = Npc->Reason();

	_npcData = GetNpcData(_tId);
	SetObjectType(_npcData);
	_async = true;
}

void FGsSpawnParam::Set(PD::SC::PKT_SC_SPAWN_SPECIAL_NPC_READ* inNpc)
{
	_type = EGsGameObjectType::SpaceCrackNonPlayer;
	_gameId = inNpc->GameId();
	_spawnId = inNpc->NpcSpawnId();
	_tId = inNpc->NpcTemplateId();
	_pos = inNpc->Pos();
	_rot = FGsDir::AngleToRotator(inNpc->Dir());
	_spawnReason = inNpc->Reason();
	_specialType = inNpc->SpecialType();

	_npcData = GetNpcData(_tId);

	//bak1210 : 스페이스 크랙에서 설정되는것은 값을 세팅하지 않음
	/*_specialType == NpcSpecialType::NONE*/
	/*SetObjectType(_npcData);*/
}

void FGsSpawnParam::Set(PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* Projectile)
{
	_type = EGsGameObjectType::Projectile;
	_gameId = Projectile->ProjectileGameId();
	_tId = Projectile->ProjectileId();
	_pos = Projectile->SpawnPos();
	// 프로젝타일은 방향에 높이를 고려하지 않음
	FVector dir = Projectile->DestPos() - Projectile->SpawnPos();
	dir.Z = 0.f;
	_rot = dir.Rotation();
}

void FGsSpawnParam::Set(PD::SC::PKT_SC_SPAWN_TREASURE_NPC_READ* treasureNpc)
{
	_type = EGsGameObjectType::NonPlayer;
	_gameId = treasureNpc->GameId();
	_spawnId = treasureNpc->NpcSpawnId();
	_tId = treasureNpc->NpcTemplateId();
	_pos = treasureNpc->Pos();
	_rot = FGsDir::AngleToRotator(treasureNpc->Dir());
	_spawnReason = treasureNpc->Reason();

	_npcData = GetNpcData(_tId);

	//bak1210 : 보물몬스터는 기능성을 부여하지 않음
	/*SetObjectType(_npcData);*/
}

void FGsSpawnParam::Set(EGsGameObjectType Type, int64 GameId, const FVector& Pos, const FRotator& Rot, bool inIsInitVisible)
{
	_type = Type;
	_gameId = GameId;
	_pos = Pos;
	_rot = Rot;
	_isInitVisible = inIsInitVisible;
}

void FGsSpawnParam::Set(EGsGameObjectType Type, int64 GameId, int32 inTid, const FVector& Pos, const FRotator& Rot, bool inIsInitVisible)
{
	_type = Type;
	_tId = inTid;
	_gameId = GameId;
	_pos = Pos;
	_rot = Rot;
	_isInitVisible = inIsInitVisible;
}



const struct FGsSchemaNpcData* FGsSpawnParam::GetNpcData(int32 id) const
{
	if (nullptr != _npcData)
	{
		if (_npcData->id == id)
			return _npcData;
	}
	auto npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(id);
	if (nullptr == npcData)
	{
		FString log = TEXT("Npc data missing - id : ") + FString::FromInt(id);
		GSLOG(Error, TEXT("%s"), *log);
		return nullptr;
	}
	return npcData;
}


const struct FGsSchemaShapeData* FGsSpawnParam::GetShapeData(uint32 id) const
{
	auto NpcDt = GetNpcData(id);
	if (nullptr != NpcDt)
	{
		return NpcDt->shapeId.GetRow();
	}
	return nullptr;
}

float FGsSpawnParam::GetScale() const
{
	if (nullptr != _npcData)
	{
		return _npcData->scale;
	}
	return 1.0f;
}

void FGsSpawnParam::SetObjectType(const struct FGsSchemaNpcData* inTbl)
{
	if (nullptr == inTbl) return;

	// IffFactionType::NPC_ESCORT는 EGsGameObjectType::Sandbag
	if (inTbl->iffFactionType == IffFactionType::NPC_ESCORT)
	{
		SetType(EGsGameObjectType::Sandbag);
	}
	else
	{
		SetType(UGsGameObjectUtil::GetObjectType(inTbl->npcFunctionType));
	}
}
//-------------------------------------------------------------------------------------------------
// FGsSpawnParamProjectile
//-------------------------------------------------------------------------------------------------
const struct FGsSchemaShapeData* FGsSpawnParamProjectile::GetShapeData(uint32 id) const
{
	if (const FGsSchemaProjectileSet* projectileTable =
		UGsTableUtil::FindRowById<UGsTableProjectileSet, FGsSchemaProjectileSet>(_tId))
	{
		return projectileTable->shapeId.GetRow();
	}
	return nullptr;
}

void FGsSpawnParamWarp::SetWarpTable(const FGsSchemaMapWarpInfo* inTbl)
{
	_warpData = const_cast<FGsSchemaMapWarpInfo*>(inTbl);
}

//-------------------------------------------------------------------------------------------------
// FGsSpawnParamWarp
//-------------------------------------------------------------------------------------------------
const struct FGsSchemaShapeData* FGsSpawnParamWarp::GetShapeData(uint32 id) const
{
	if (nullptr == _warpData)
		return nullptr;

	return _warpData->shapeId.GetRow();
}

//-------------------------------------------------------------------------------------------------
// FGsSpawnParamDropObject
//-------------------------------------------------------------------------------------------------
const struct FGsSchemaShapeData* FGsSpawnParamDropObject::GetShapeData(uint32 id) const
{
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaDropObject::StaticStruct());
	if (nullptr == table)
	{
		return nullptr;
	}

	const UGsTableDropObject* dropObjectTable = Cast<UGsTableDropObject>(table);
	if (nullptr == dropObjectTable)
	{
		return nullptr;
	}
	
	EGsDropItemCategory itemCategory = UGsItemUtil::GetDropObjectCategory(_tId);
	if (itemCategory == EGsDropItemCategory::Max)
	{
		GSLOG(Error, TEXT("Missing item category - item id : %d"), _tId);
		return nullptr;
	}

	const FGsSchemaDropObject* dropObject = nullptr;
	dropObjectTable->FindRowById(itemCategory, dropObject);
	if (nullptr == dropObject)
	{
		GSLOG(Error, TEXT("Missing drop object shape - item id : %d"), _tId);
		return nullptr;
	}

	return dropObject->shapeId.GetRow();	
}


//-------------------------------------------------------------------------------------------------
// FGsSpawnParamProp
//-------------------------------------------------------------------------------------------------
FGsSpawnParamProp::FGsSpawnParamProp(EGsGameObjectType inType, int64 inGameId, int32 inTid, const FVector& inPos, const short inRot, bool inIsInitVisible /*= true*/, bool isInvasionActive /*= false */)
{
	Set(inType, inGameId, inTid, inPos, FGsDir::AngleToRotator(inRot), inIsInitVisible);

	_isActiveInvasion = isInvasionActive;
}

const struct FGsSchemaShapeData* FGsSpawnParamProp::GetShapeData(uint32 id) const
{
	if (nullptr == _propData)
	{
		const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(_tId);
		_propData = const_cast<FGsSchemaPropData*>(propData);
	}
	
	if (nullptr == _propData)
	{
		FString log = TEXT("Prop data missing - id : ") + FString::FromInt(_tId);
		GSLOG(Error, TEXT("%s"), *log);
		return nullptr;
	}

	return _propData->shapeId.GetRow();
}


PropType FGsSpawnParamProp::GetPropType() const
{
	if (nullptr == _propData)
	{
		const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(_tId);
		_propData = const_cast<FGsSchemaPropData*>(propData);
	}

	if (nullptr != _propData)
	{
		return _propData->propType;
	}
	return PropType::MAX;
}

const FGsSchemaPropData* FGsSpawnParamProp::GetPropData() const
{
	if (nullptr == _propData)
	{
		const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(_tId);
		_propData = const_cast<FGsSchemaPropData*>(propData);
	}
	return _propData;
}

//-------------------------------------------------------------------------------------------------
// FGsSpawnParamPlayer
//-------------------------------------------------------------------------------------------------
void FGsSpawnParamPlayer::Set(PD::SC::PKT_SC_SPAWN_USER_READ* Remote)
{
	_type = EGsGameObjectType::RemotePlayer;
	_gameId = Remote->GameId();
	_tId = Remote->CreatureId();
	_pos = Remote->Pos();
	_rot = FGsDir::AngleToRotator(Remote->Dir());
	_lookInfo = Remote->LookInfo();
	_customizeFaceData = Remote->CustomizingFaceData();
	_customizeBodyData = Remote->CustomizingBodyData();
	_userStatus = (UserStatus)Remote->UserStatus();
	_name = Remote->UserName();
	_level = Remote->Level();
	_userDBId = Remote->UserDBId();

	_homeWorldId = Remote->HomeWorldId();

	_spawnReason = (SPAWN_REASON)Remote->Reason();
	_guildData.Set(*Remote);

	_accountGrade = Remote->AccountGrade();
	_gmPrefix = Remote->GmPrefix();
}

// param id  :  _tId = Remote->CreatureId();
const struct FGsSchemaShapeData* FGsSpawnParamPlayer::GetShapeData(uint32 id) const 
{
	const FGsSchemaUserData* pcTable = UGsTableUtil::FindRow<FGsSchemaUserData>(id);
	if (nullptr == pcTable)
	{
		GSLOG(Error, TEXT("not found UserData Table"));
		return nullptr;
	}

	const FGsSchemaPlayerShape* pcShapeTable = pcTable->shapeId.GetRow();
	if (nullptr == pcShapeTable)
	{
		GSLOG(Error, TEXT("not found PlayerShape Table"));
		return nullptr;
	}

	return pcShapeTable->remoteplayerShapeId.GetRow();
}

//-------------------------------------------------------------------------------------------------
//FGsSpawnParamLocalPlayer
//-------------------------------------------------------------------------------------------------
FGsSpawnParamLocalPlayer::FGsSpawnParamLocalPlayer(EGsGameObjectType Type, const FVector& Pos, const FRotator& Rot,
	const FGsNetUserData* UserBasicData)
{
	if (UserBasicData)
	{
		FGsSpawnParam::Set(Type, UserBasicData->mGameId, UserBasicData->mTid, Pos, Rot);
		_netUserData = UserBasicData;
	}
}

void FGsSpawnParamLocalPlayer::Set(PD::SC::PKT_SC_SPAWN_ME_READ* pkt)
{
	_type = EGsGameObjectType::LocalPlayer;
	_gameId = pkt->GameId();
	_pos = pkt->Pos();
	_rot = FGsDir::AngleToRotator(pkt->Dir());
}

// param id  :  userData->mTid;
const struct FGsSchemaShapeData* FGsSpawnParamLocalPlayer::GetShapeData(uint32 id) const
{
	const FGsSchemaUserData* pcTable = UGsTableUtil::FindRow<FGsSchemaUserData>(id);
	if (nullptr == pcTable)
	{
		GSLOG(Error, TEXT("not found UserData Table"));
		return nullptr;
	}

	const FGsSchemaPlayerShape* pcShapeTable = pcTable->shapeId.GetRow();
	if (nullptr == pcShapeTable)
	{
		GSLOG(Error, TEXT("not found PlayerShape Table"));
		return nullptr;
	}

	return pcShapeTable->localplayerShapeId.GetRow();
}


