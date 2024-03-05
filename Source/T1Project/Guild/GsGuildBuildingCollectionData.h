// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Classes/Collection/GsCollectionData.h"
#include "Guild/GsGuildBuildingData.h"

/**
 * 길드 건물 정보 관리, 콜렉션 버프 표시를 위해 IGsCollectionData 상속
 */
struct T1PROJECT_API FGsGuildBuildingCollectionData final : public IGsCollectionData
{
public:
	// 오픈된 빌딩 정보만 있음.
	TArray<FGsGuildBuildingData> _dataList;

	// 건물 패시비티 컬렉션 정보(스탯만. 버프로 활성화 되는 것은 제외)
	TArray<PassivityId> _collectionList;

	// 버프 재활성 대기시간
	time_t _buffReactivateTime = 0;

public:
	FGsGuildBuildingCollectionData() = default;
	virtual ~FGsGuildBuildingCollectionData() = default;

public:
	virtual PassivityOwnerType GetPassivityOwnerType() const override { return PassivityOwnerType::GUILD_BUILDING; }
	virtual int32 GetPassivityCount() const override;
	virtual PassivityId GetPassivityId(int32 InIndex) const override;

public:
	void Empty();
	void SetBuildingData(const GuildBuildingData& InData);
	void SetBuffReactivateTime(float InRemainSecond);
	void SetUpgradeLevel(GuildBuildingId InId, GuildBuildingLevel InNextLevel);
	void SetActiveGuildBuff(GuildBuildingId InId, PassivityId InPassivityId, float InCoolDownTime);

	bool IsExistActiveBuff() const;
	// 외부엔 수정 못하게 const 붙여서 보냄
	const FGsGuildBuildingData* GetBuildingData(GuildBuildingId InId) const;	

protected:
	// 내부 활용용
	FGsGuildBuildingData* GetBuildingDataInter(GuildBuildingId InId);

	void RemoveCollection(FGsGuildBuildingData* InData);
	void AddCollection(FGsGuildBuildingData* InData);
};