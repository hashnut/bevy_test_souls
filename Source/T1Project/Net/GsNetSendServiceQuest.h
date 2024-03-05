// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

/**
 * 퀘스트 관련 패킷 Send 함수 모음 
 */
class T1PROJECT_API FGsNetSendServiceQuest
{
public:
	static void SendReqQuestList(const CreatureId inNpcId, const QuestBoardType inQuestBoardType);
	static void SendReqBoardQuestRefresh(const CreatureId inNpcId, const QuestBoardType inQuestBoardType);
	static void SendReqBoardQuestAccept(const CreatureId inNpcId, const QuestStoryIdPair& inQuest, const QuestBoardType inQuestBoardType);
	static void SendReqQuestRepeatGiveUp(const QuestStoryIdPair& inQuest);
	static void SendReqQuestRepeatRefresh(const QuestIndex inQuestIndex);		// 반복퀘스트 새로고침
	static void SendReqQuestRepeatRewardBox(const QuestIndex inQuestIndex);		// 반복퀘스트 단일 보상목록 요청 시
	static void SendReqQuestRepeatRewardBoxList();								// 모든 반복퀘스트 보상목록 요청 시

	// 길드 퀘스트 관련 패킷
public:
	static void SendReqQuestGuildAccept(QuestId InQuestId);
	static void SendReqQuestGuildReward(QuestId InQuestId);
	static void SendReqQuestGuildGiveUp(QuestId InQuestId);
	static void SendReqQuestGuildRefresh();
};
