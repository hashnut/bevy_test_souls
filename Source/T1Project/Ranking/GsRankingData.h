#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "UTIL/GsClassUtil.h"


// 보상등급
enum class EGsRankingGrade : uint8
{
	First,
	Second,
	Third,
	Other,
	NONE,

	MAX
};

enum class EGsRankingDataType : uint8
{
	Local,
	World,
	Guild,
	GuildDungeon,

	MAX
};

struct FGsRankingDetail
{
public:
	FGsRankingDetail() = default;
	virtual ~FGsRankingDetail() = default;

	virtual EGsRankingDataType GetDataType() { return EGsRankingDataType::MAX; }
};

struct FGsRankingDetailPersonal : public FGsRankingDetail
{
public:
	FString _userName;	// 캐릭터 이름
	WorldId _worldId;	// 서버 정보
	FString _guildName;	// 기사단 이름
	GuildEmblemId _guildEmblemId;	// 기사단 문양
	uint64 _param1; // 랭킹 기준 값


public:
	FGsRankingDetailPersonal() = default;
	virtual ~FGsRankingDetailPersonal() = default;

	FGsRankingDetailPersonal(const PD::GC::PKT_GC_ACK_RANK_READ::RankListIterator& InRankingData)
		: _userName(InRankingData->UserName()), _guildName(InRankingData->GuildName()),
		_guildEmblemId(InRankingData->GuildEmblemId()), _param1(InRankingData->Param1())
	{
		PlanetWorldId planetWorldId(InRankingData->PlanetWorldId());
		_worldId = planetWorldId.st.worldId;
	}

	virtual EGsRankingDataType GetDataType() override { return EGsRankingDataType::Local; }
};

struct FGsRankingDetailWorld : public FGsRankingDetail
{
public:
	FString _userName;	// 캐릭터 이름
	WorldId _worldId;	// 서버 정보
	uint64 _param1;		// 랭킹 기준 값

public:
	FGsRankingDetailWorld() = default;
	virtual ~FGsRankingDetailWorld() = default;

	FGsRankingDetailWorld(const PD::GC::PKT_GC_ACK_RANK_READ::RankListIterator& InRankingData)
		: _userName(InRankingData->UserName()), _param1(InRankingData->Param1())
	{
		PlanetWorldId planetWorldId(InRankingData->PlanetWorldId());
		_worldId = planetWorldId.st.worldId;
	}

	virtual EGsRankingDataType GetDataType() override { return EGsRankingDataType::World; }
};

struct FGsRankingDetailGuild : public FGsRankingDetail
{
public:
	FString _guildName;				// 기사단 이름
	GuildEmblemId _guildEmblemId;	// 기사단 문양
	uint64 _param1;	// 랭킹 기준 값 1

public:
	FGsRankingDetailGuild() = default;
	virtual ~FGsRankingDetailGuild() = default;

	FGsRankingDetailGuild(const PD::GC::PKT_GC_ACK_RANK_READ::RankListIterator& InRankingData)
		: _guildName(InRankingData->GuildName()), _guildEmblemId(InRankingData->GuildEmblemId()),
		_param1(InRankingData->Param1()) {}

	virtual EGsRankingDataType GetDataType() override { return EGsRankingDataType::Guild; }
};

struct FGsRankingDetailGuildDungeon : public FGsRankingDetail
{
public:
	FString _guildName;				// 기사단 이름
	GuildEmblemId _guildEmblemId;	// 기사단 문양
	uint64 _param1;	// 랭킹 기준 값 1
	uint64 _param2;	// 랭킹 기준 값 2

public:
	FGsRankingDetailGuildDungeon() = default;
	virtual ~FGsRankingDetailGuildDungeon() = default;

	FGsRankingDetailGuildDungeon(const PD::GC::PKT_GC_ACK_RANK_READ::RankListIterator& InRankingData)
		: _guildName(InRankingData->GuildName()), _guildEmblemId(InRankingData->GuildEmblemId()),
		_param1(InRankingData->Param1()), _param2(InRankingData->Param2()) {}

	virtual EGsRankingDataType GetDataType() override { return EGsRankingDataType::GuildDungeon; }
};

struct FGsRankingData
{
public:
	uint32 _ranking;	// 순위
	uint32 _previousRank;	// 순위 변동
	TSharedPtr<FGsRankingDetail> _detailData; // 랭킹 상세 정보
};

struct FGsRankingMyData
{
public:
	uint32 _highRanking = 0; // 순위
	uint32 _previousRank = 0;
	bool _hasHighRanking = false;
	float _ranking = 0;
	uint64 _param1 = 0;
	uint64 _param2 = 0;
};