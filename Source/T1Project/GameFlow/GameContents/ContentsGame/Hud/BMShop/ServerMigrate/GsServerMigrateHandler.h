// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsServerMigrateData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"

/*
* file		GsServerMigrateHandler.h
* date		2023/10/26 11:22
* author	PKT
* brief		서버 이전 관련 Handler. 서버 이전 관련이 BMShop에 붙어 있다보니 별도 메니징 없이 Handler에서 처리 하도록 
*/

struct FGsSchemaServerMigrateConfigData;

using	MsgMigrationHandle = TPair<MessageContentsMigration, FDelegateHandle>; 
using	MsgMigrationHandleArray = TArray<MsgMigrationHandle>;

class T1PROJECT_API FGsServerMigrateHandler
{
public:
	FDateTime _endTIme = FDateTime(0);

public:
	struct Migrate
	{
		ServerMigrateType type{ ServerMigrateType::NONE };

		int32 count{ 0 };
		TArray<TWeakPtr<MigrateGroupItem>> groupData;
		TMap<ServerMigrateConditionType, TWeakPtr<MigrateConditionItem>> checkConditonList;

		void SetType(ServerMigrateType miType)
		{
			type = miType;
		}

		void Reset()
		{
			type = ServerMigrateType::NONE;
			count = 0;
			checkConditonList.Reset();

			groupData.Reset();
		}
	};

private:
	TArray<TSharedPtr<MigrateServer>> _serverList;
	TArray<TSharedPtr<MigrateConditionItem>> _migrateConditions;
	TArray<TSharedPtr<MigrateGroupItem>> _migrateGroups;

	TMap<ServerMigrateType, Migrate> _migrateData;
	TWeakPtr<MigrateServer> _selectedMigrateServer{ nullptr };

	bool _reqServerList{ false };

private:
	MsgGameObjHandle _msgGameObjHander;
	MsgMigrationHandleArray _msgMigrationHandlerList;

public:
	void Init();
	void Close();
	void OnReconnection();
	void ClearConditionInfo();

protected:
	// 2023/10/27 PKT - Local Player 생성 시 Data 초기화
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

public:
	// 2023/11/3 PKT - index -> ServerMigrateType
	ServerMigrateType ConvertIntToServerMigrateType(int32 InIndex) const;

	// 2023/11/3 PKT - 외부에서 특정 조건에 대한 상태를 변경 한다.( 조건에 대한 상태는 서버에서만 변경 할 수 있으나, 예외 적으로 서버 선택은 클라에서 변경 해야 함.)
	void SetConditionState(ServerMigrateType InType, ServerMigrateConditionType InConditionType, bool InFlag, int32 InParam = 0);

	// 2023/10/27 PKT - 서버 이주 관련 모든 조건을 만족 하였는지 체크
	enum class ETryReason { Valid, InvalidServerList, InvalidWaitRequest, InvalidDate, InvalidCondition, InvalidGuildMarster, InvalidOverCount, };
	ETryReason IsCheckedMigrateStateAll(ServerMigrateType InType) const;

	/**
	 * Config Data
	 */
public:
	// 2023/11/3 PKT - 개인 / 기사단 가이드
	EGsHelpContents FindHelpContetns(ServerMigrateType InType) const;
	// 2023/11/3 PKT - 남은 시간 Text로 표기
	FText GetTextRemainingTime() const;
	// 2023/11/3 PKT - 계정 통합 현재 카운트 / 전체 카운트
	TPair<int32, int32> CountInfo() const;
	// 2023/11/3 PKT - 이주 비용
	CurrencyPair GetCurrencyInfo(ServerMigrateType InType) const;

private:
	// 2023/10/31 PKT - Data ClearAll
	void ClearAll();
	// 2023/10/27 PKT - Load Config Data
	static const FGsSchemaServerMigrateConfigData* GetConfigData();

	// 서버 조건 테이블 읽어두기
	void LoadMigrateConditions();
public:
	// Get Type 별 서버 조건 Group 
	int32 GetCheckGroups(ServerMigrateType type, OUT TArray<TWeakPtr<MigrateGroupItem>>& data);

public:
	// 이전 가능 서버 리스트
	int32 GetServerList(OUT TArray<TWeakPtr<MigrateServer>>& serverList);
	// 현재 선택된 서버 리스트
	TWeakPtr<MigrateServer> GetSelectedServer() { return _selectedMigrateServer; }
	// 특정 서버 리스트 정보
	TWeakPtr<MigrateServer> FindMigrateServer(WorldId worldId);

public:
	// 패킷을 받아 필요한 정보 Setting
	void SetInitServerMigrateInfo(FDateTime endTime, int accCount, int guildCount);
	void AddMigrateServerList(MigrateServer data);
	void SetAccountServerMigrateData(ServerMigrateConditionType miCondType, bool possible);
	void SetGuildServerMigrateData(ServerMigrateConditionType miCondType, bool possible);
	void SetEndServerMigrateInfo();

public:
	// 2023/11/7 PKT - 서버에게 부터 이주 조건 상태를 요청한다.
	void ReqServerMigrateConditionState();
	// 서버 이주 요청
	void SendServerMigrate(ServerMigrateType type);
public:
	// 서버 이전 Ack. SendServerMigrate의 응답
	bool OnAckServerMigration(int result);

public:
	// 서버 이전 이후 상태에 따를 이름수정 요구
	void OnReqMigrationNameChange(const struct IGsMessageParam* InParam);
	void OnReqMigrationGuildNameChange(const struct IGsMessageParam* InParam);
};