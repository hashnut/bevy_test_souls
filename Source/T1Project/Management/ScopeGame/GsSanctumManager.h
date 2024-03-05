// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "Classes/GsPool.h"
#include "Management/GsScopeHolder.h"
//#include "RedDot/GsRedDotCheck.h"
#include "Sanctum/GsSanctumData.h"


namespace PD
{	
	//namespace SC
	//{
	//	struct PKT_SC_ACK_MAP_SANCTUM_INFO_READ;
	//}

	namespace GC
	{
		struct PKT_GC_ACK_MAP_SANCTUM_INFO_READ;
	}
}

struct FGsSchemaSanctumConfig;
struct FGsSchemaSanctumData;
struct FGsSchemaSanctumNexusData;

/**
 * 성소 데이터 매니저
 */
class T1PROJECT_API FGsSanctumManager :
	public IGsManager
{
public:
	using DefSanctumList = TArray<TWeakPtr<FGsSanctumData>>;

private:
	// 원본
	TMap<SanctumAreaId, TSharedPtr<FGsSanctumData>> _sanctumDataMap; // Key:SanctumAreaId,

public:
	FGsSanctumManager() = default;
	virtual ~FGsSanctumManager();

	//  테이블 가져오는 로직 
public:
	static const FGsSchemaSanctumConfig* GetTableSanctumConfig();
	static const FGsSchemaSanctumData* GetTableSanctumData(SanctumAreaId InSanctumId);
	static const FGsSchemaSanctumNexusData* GetTableSanctumNexusDataBySanctumId(SanctumAreaId InSanctumId);
	static const FGsSchemaSanctumNexusData* GetTableSanctumNexusData(SanctumNexusId InNexusId);

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void InitializeAfterResourceDownload() override;

public:
	void SetMapSanctumInfo(PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ* InPacket);

public:
	//  TODO: 각 성소의 기사단 정보 날리고 상태 변경
	void PrepareBattle();

public:
	TWeakPtr<FGsSanctumData> GetSanctumData(SanctumAreaId InSanctumId);
};

#define GSSanctum() UGsScopeHolder::GetGameManagerFType<FGsSanctumManager>(UGsScopeGame::EManagerType::Sanctum)
