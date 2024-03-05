// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
//#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
//#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

//----------------------------------------------------------------------------------------------------------------------
enum class EGsGuildQuestStateType
{
	NONE, // 수락전
	ACCEPTED, // 수락됨. 진행중
	COMPLETED, // 완료(보상 지급 전 - 내 퀘스트일 때만 존재함)
	REWARDED, // 완료(보상 지급까지 완료)
};
//----------------------------------------------------------------------------------------------------------------------
struct FGsGuildQuestData final
{
public:
	StoryId _storyId = INVALID_STORY_ID; // 이거 없어도 되나...흠
	QuestId _questId = INVALID_QUEST_ID;
	QuestState _questState = QuestState::NONE;

public:
	void Clear();

	EGsGuildQuestStateType GetGuildQuestState() const;	
	void SetQuestState(QuestState InState) { _questState = InState; }
	EGsGuildQuestStateType GetQuestState() const;

	// DELETED, GIVEUP 이 아니어야 함
	bool IsValidQuest() const;

	// ACCEPTED 상태
	bool IsAccepted() const;
	
	// COMPLETED, REWARDED 상태 (취소 불가)
	bool IsComplete() const;
};
