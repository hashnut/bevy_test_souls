/**
 * Automation Test 가 유효할 때만 빌드에 포함. 에디터/런타임 환경에서 테스트 실행.
 */

#include "Runtime/Core/Public/Delegates/IDelegateInstance.h"
#include "Runtime/Core/Public/Misc/AutomationTest.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/ScopeExit.h"
#include "Runtime/Engine/Public/Tests/AutomationCommon.h"

#include "Cinematic/GsSchemaSequenceResData.h"
#include "Event/EventAction/GsSchemaEventAction.h"
#include "Event/EventAction/GsSchemaEventActionGroup.h"
#include "GsSchemaRowBase.h"
#include "GsTableManager.h"

#include "Management/ScopeGame/GsEventManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsEventMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"

#if WITH_DEV_AUTOMATION_TESTS

#define NS_TEST "Raon.이벤트액션.이벤트"

/**
 * 테스트 완료한 이벤트액션그룹 테이블을 반환.
 */
const UGsTableEventActionGroup* GetEventActionGroupTableTested(FAutomationTestBase& Context)
{
	const UGsTableManager& TableManager = UGsTableManager::GetInstance();
	Context.TestTrue(TEXT("TableManager.IsValidLowLevel()"), TableManager.IsValidLowLevel());
	if (!TableManager.IsValidLowLevel())
	{
		return nullptr;
	}

	const UGsTableEventActionGroup* EventActionGroupTable = Cast<UGsTableEventActionGroup>(TableManager.GetTable(FGsSchemaEventActionGroup::StaticStruct()));
	Context.TestNotNull(TEXT("EventActionGroupTable"), EventActionGroupTable);

	return EventActionGroupTable;
}

/**
 * 테스트 완료한 첫번째 이벤트액션그룹을 반환.
 */
const FGsSchemaEventActionGroup* GetEventActionGroupTested(FAutomationTestBase& Context, const int32 OrderIndex)
{
	const UGsTableEventActionGroup* EventActionGroupTable = GetEventActionGroupTableTested(Context);
	if (nullptr == EventActionGroupTable)
	{
		return nullptr;
	}

	TArray<const FGsSchemaEventActionGroup*> Rows;
	const bool ValidRows = EventActionGroupTable->GetAllRows(Rows);
	Context.TestTrue(TEXT("!ValidRows"), ValidRows);
	if (!ValidRows)
	{
		return nullptr;
	}

	const int32 RowsNum = Rows.Num();
	Context.TestNotSame(TEXT("0, RowsNum"), 0, RowsNum);
	if (0 > RowsNum || OrderIndex >= RowsNum)
	{
		return nullptr;
	}

	const FGsSchemaEventActionGroup* EventActionGroup = Rows[OrderIndex];
	Context.TestNotNull(TEXT("EventActionGroup"), EventActionGroup);
	return EventActionGroup;
}

/**
 * 테스트 완료한 이벤트매니저를 반환.
 */
UGsEventManager* GetEventManagerTested(FAutomationTestBase& Context)
{
	UGsEventManager* EventManager = GSEvent();
	Context.TestNotNull(TEXT("EventManager"), EventManager);
	return EventManager;
}

/*
const FGsSchemaQuest* GetQuestManagerTested(FAutomationTestBase& Context, const QuestId InQuestId)
{
	const UGsTableQuest* QuestTable
		= Cast<UGsTableQuest>(UGsTableManager::GetInstance().GetTable(FGsSchemaQuest::StaticStruct()));
	Context.TestNotNull(TEXT("QuestTable"), QuestTable);
	if (nullptr == QuestTable)
	{
		return nullptr;
	}

	const FGsSchemaQuest* Quest = nullptr;
	Context.TestTrue(TEXT("QuestTable->FindRowById(InQuestId, Quest)"), QuestTable->FindRowById(InQuestId, Quest));
	Context.TestNotNull(TEXT("Quest"), Quest);
	return Quest;
}*/

////////////////////////////////////////////////////////////////////////// commands

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FGsEventActionRequest, FGsReqEventActionParam, _param);
bool FGsEventActionRequest::Update()
{
	// 이벤트매니저에게 이벤트액션플레이 명령
	GMessage()->GetEvent().SendMessage(MessageContentEvent::EVENT_ACTION_PLAY, _param);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FGsEventActionResponse, FGsReqEventActionParam, _param);
bool FGsEventActionResponse::Update()
{
	// 서버에서 응답 보낸 것 처럼 호출
	GSEvent()->EventActionPlayResponse(true, _param._questId, _param._eventActionState, _param._eventActionGroupId, _param._lastEventActionIndex);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FGsSequencePlayStop, FGsSequenceMessageParam, _param);
bool FGsSequencePlayStop::Update()
{
	// GMessage()->GetGameObject().SendMessage(MessageGameObject::SEQUENCE_PLAYER_STOP, &_param);
	// GetDefault<UGsCheatManager>()->StopSequence(_param._sequenceId);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FGsWaitForEventActionFinished);
bool FGsWaitForEventActionFinished::Update()
{
	const UGsEventManager* EventManager = GSEvent();
	if (nullptr == EventManager || !EventManager->IsValidLowLevel())
	{
		GSLOG(Error, TEXT("nullptr == EventManager || !EventManager->IsValidLowLevel()"));
		return true;
	}

	return !EventManager->EventActionIsPlaying();
}

//DEFINE_LATENT_AUTOMATION_COMMAND(FGsWaitForLastEventActionFinished);
//bool FGsWaitForLastEventActionFinished::Update()
//{
//	FGsEventActionFinishedParam Param(_reqEventActionParam._questId, _reqEventActionParam._eventActionGroupId);
//	GMessage()->GetEvent().SendMessage(MessageContentEvent::LAST_EVENT_ACTION_FINISH, Param);
//
//	const UGsEventManager* EventManager = GSEvent();
//	if (nullptr == EventManager || !EventManager->IsValidLowLevel())
//	{
//		GSLOG(Error, TEXT("nullptr == EventManager || !EventManager->IsValidLowLevel()"));
//		return true;
//	}
//
//	return !EventManager->EventActionIsPlaying();
//}

////////////////////////////////////////////////////////////////////////// tests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGsEventActionStartTest, TEXT(NS_TEST "01 Event Action Start")
	, EAutomationTestFlags::EditorContext
	| EAutomationTestFlags::MediumPriorityAndAbove
	| EAutomationTestFlags::EngineFilter)
bool FGsEventActionStartTest::RunTest(const FString& parameters)
{
	constexpr QuestId QUEST_ID = 250310;

	UGsEventManager* EventManager = GetEventManagerTested(*this);
	if (nullptr == EventManager)
	{
		return false;
	}

	const FGsSchemaEventActionGroup* EventActionGroup = GetEventActionGroupTested(*this, 0);
	if (nullptr == EventActionGroup)
	{
		return false;
	}

	FGsReqEventActionParam Param(QUEST_ID, EventActionState::PRE, EventActionGroup->id, 0, false);

	// 이벤트매니저에게 이벤트액션플레이 명령
	GMessage()->GetEvent().SendMessage(MessageContentEvent::EVENT_ACTION_PLAY, Param);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGsEventActionBasicTest, TEXT(NS_TEST "02 Event Action Basic Test")
	, EAutomationTestFlags::EditorContext
	| EAutomationTestFlags::MediumPriorityAndAbove
	| EAutomationTestFlags::EngineFilter)
bool FGsEventActionBasicTest::RunTest(const FString& parameters)
{
	constexpr QuestId QUEST_ID = 250310;

	UGsEventManager* EventManager = GetEventManagerTested(*this);
	if (nullptr == EventManager)
	{
		return false;
	}

	const FGsSchemaEventActionGroup* EventActionGroup = GetEventActionGroupTested(*this, 0);
	if (nullptr == EventActionGroup)
	{
		return false;
	}

	FGsReqEventActionParam Param(QUEST_ID, EventActionState::PRE, EventActionGroup->id, 0, false);
	ADD_LATENT_AUTOMATION_COMMAND(FGsEventActionRequest(Param));

	int32 EventActionCount = EventActionGroup->eventActionList.Num();
	for (int32 i = 0; i < EventActionCount; i++)
	{
		// 네트워크 딜레이 시뮬레이션
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));

		ADD_LATENT_AUTOMATION_COMMAND(FGsEventActionResponse(Param));

		const FGsSchemaEventAction& EventActionData = EventActionGroup->eventActionList[i];
		if (EventActionType::PLAY_CINEMATIC == EventActionData.type)
		{
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));

			// (임시) 시네마틱의 경우, 지정 시간 후에 시네마틱 종료 이벤트를 강제로 발생시킴
			const FGsSchemaSequenceResData* SchemaSequenceResData = EventActionData.typeInfo.GetRowByType<FGsSchemaSequenceResData>();
			check(SchemaSequenceResData);
			FGsSequenceMessageParam SequenceParam(SchemaSequenceResData->sequenceResId);
			ADD_LATENT_AUTOMATION_COMMAND(FGsSequencePlayStop(SequenceParam));
		}

		ADD_LATENT_AUTOMATION_COMMAND(FGsWaitForEventActionFinished());

		++Param._lastEventActionIndex;
	}

	// 이벤트 액션 종료할 때까지 대기
	ADD_LATENT_AUTOMATION_COMMAND(FGsWaitForEventActionFinished());

	EventActionGroup = GetEventActionGroupTested(*this, 1);

	Param._eventActionState = EventActionState::POST;
	Param._eventActionGroupId = EventActionGroup->id;
	Param._lastEventActionIndex = 0;
	ADD_LATENT_AUTOMATION_COMMAND(FGsEventActionRequest(Param));

	EventActionCount = EventActionGroup->eventActionList.Num();
	for (int32 i = 0; i < EventActionCount; i++)
	{
		// 네트워크 딜레이 시뮬레이션
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));

		ADD_LATENT_AUTOMATION_COMMAND(FGsEventActionResponse(Param));

		const FGsSchemaEventAction& EventActionData = EventActionGroup->eventActionList[i];
		if (EventActionType::PLAY_CINEMATIC == EventActionData.type)
		{
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));

			// (임시) 시네마틱의 경우, 지정 시간 후에 시네마틱 종료 이벤트를 강제로 발생시킴
			const FGsSchemaSequenceResData* SchemaSequenceResData = EventActionData.typeInfo.GetRowByType<FGsSchemaSequenceResData>();
			check(SchemaSequenceResData);
			FGsSequenceMessageParam SequenceParam(SchemaSequenceResData->sequenceResId);
			ADD_LATENT_AUTOMATION_COMMAND(FGsSequencePlayStop(SequenceParam));
		}

		ADD_LATENT_AUTOMATION_COMMAND(FGsWaitForEventActionFinished());

		++Param._lastEventActionIndex;
	}

	return true;
}

#endif
