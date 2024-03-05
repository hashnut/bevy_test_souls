// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 
 */
class T1PROJECT_API FGsNetMessageHandlerBoss : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerBoss() = default;
	virtual ~FGsNetMessageHandlerBoss();
	
public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// Channel
protected:
	void ResultDefeatFieldBoss(const FGsNet::Packet& inPacket);

	//world boss
public:
	//모든 월드 보스 
	void AckWorldBossInfo(const FGsNet::Packet& inPacket);
	void WorldBossSpawnReadyState(const FGsNet::Packet& inPacket);
	//보스 맵 생성됨, 10분 전까지 들어갈 수 있음	
	//월드 보스 입장 활성화
	void InformWorldBossMapCreate(const FGsNet::Packet& inPacket);	
	//보스 스폰됨, 월드 보스 입장 비활성화
	void AlarmWorldBossSpawn(const FGsNet::Packet& inPacket);
	//월드 보스 퇴치 성공
	void WorldBossClear(const FGsNet::Packet& inPacket);
	//시간이 시나서 월드 보스 실패
	void AlarmWorldBossTimeOver(const FGsNet::Packet& inPacket);

	// 2023/8/1 PKT - 필드 보스 스폰
	void AlarmFieldBossSpawn(const FGsNet::Packet& inPacket);

	// 2023/8/1 PKT - 필드 보스 다이
	void AlarmFieldBossDie(const FGsNet::Packet& inPacket);
	// 필드 보스 디스폰
	// https://jira.com2us.com/jira/browse/CHR-23655
	void AlarmFieldBossDespawn(const FGsNet::Packet& inPacket);
};
