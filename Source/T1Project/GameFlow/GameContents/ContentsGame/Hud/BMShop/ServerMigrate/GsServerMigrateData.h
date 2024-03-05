#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateConditionData.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateGroupData.h"

/**
 * 2023/11/1 PKT - 서버 이주에 대한 각 서버들의 상태 정보
 */
struct MigrateConditionItem
{
	ServerMigrateConditionType _type{ ServerMigrateConditionType::NONE };
	const FGsSchemaServerMigrateConditionData* _data{ nullptr };

	bool _isPossible{ false };

	MigrateConditionItem(const FGsSchemaServerMigrateConditionData* data) : _type(data->type), _data(data) {};

	FText GetConditionTitle() { return _data != nullptr ? _data->conditionText : FText::GetEmpty(); }
	FText GetNotFitConditionDesc() { return _data != nullptr ? _data->failDesc : FText::GetEmpty(); }

	void Reset() { _isPossible = false; }
};

struct MigrateGroupItem
{
	ServerMigrateType _type{ ServerMigrateType::NONE };
	const FGsSchemaServerMigrateGroupData* _data{ nullptr };
	TArray<TWeakPtr<MigrateConditionItem>> _conditions;

	MigrateGroupItem(const FGsSchemaServerMigrateGroupData* data) : _type(data->type), _data(data) {};

	FText GetGroupTitle() { return _data == nullptr ? FText::GetEmpty() : _data->name; }
	const TArray<TWeakPtr<MigrateConditionItem>>& GetConditions() { return _conditions; }
	ServerMigrateType GetMigrateType() { return _data == nullptr ? ServerMigrateType::NONE : _data->type; }
};

struct MigrateServer
{
	PlanetId _planetId{ INVALID_PLANET_ID };
	WorldId _worldId{ INVALID_WORLD_ID };
	ServerCongestion _congestion{ ServerCongestion::ON_STANBY };
	ServerMigrateLimitStatus _status{ ServerMigrateLimitStatus::NONE };

	bool _isRecommended{ false };
	bool _isNewOpenServer{ false };
	int32 _userCount{ 0 };

	bool _useCheck{ false };

	MigrateServer() = default;
	MigrateServer(PlanetId planetId, WorldId worldId, ServerCongestion congestion, ServerMigrateLimitStatus status, bool isRecommended, bool isNewOpenServer, int32 userCount)
		: _planetId(planetId), _worldId(worldId), _congestion(congestion), _status(status)
		, _isRecommended(isRecommended), _isNewOpenServer(isNewOpenServer), _userCount(userCount), _useCheck(true)
	{
	}

	void Reset()
	{
		_useCheck = false;
	}

	void SetData(ServerCongestion congestion, ServerMigrateLimitStatus status, bool isRecommended, bool isNewOpenServer, int32 userCount)
	{
		_congestion = congestion; 
		_status = status;
		_isRecommended = isRecommended; 
		_isNewOpenServer = isNewOpenServer;
		_userCount = userCount;

		_useCheck = true;
	}
};