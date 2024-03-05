
#include "AutomationTest.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"
#include "GameMode/GsGameModeWorldBase.h"
#include "GameMode/GsGameModeWorld.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Tests\AutomationCommon.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

/**
 * 에디터 포스트빌드 테스트 샘플
 * 
 * 포스트빌드에 자동 포함하기 위한 유일한 조건은 테스트 이름이 "Raon.EditorPostBuild" 로 시작하는 것입니다.
 * 아래 코드를 참고하세요.
 * 
 * 포스트빌드는 보통 CI에서 클라이언트 빌드가 성공하면, 본 테스트를 모두 실행합니다.
 * 이후에 변경된 사항들을 SCM 으로 자동 커밋합니다.
 * 
 * 이 테스트를 로컬에서 실행하고자 하면, 아래 스크립트를 참조하세요.
 * UE4Editor-Cmd.exe T1Project.uproject -NoP4 -Run -StdOut -NullRHI -Unattended  -NoPause -UTF8Output -NoXge -ExecCmds="Automation RunTests Raon.EditorPostBuild; Quit"
 * 
 * 또한, 본 포스트빌드는 테스트 프로세스의 일부이므로, Unreal Frontend 에서 원하는 작업을 직접 선택하여 실행할 수 있습니다.
 * 
 * 참고로, 테스트 프로세스는 에디터에만 가능한 것은 아닙니다. 당연히 런타임에도 테스트 프로세스를 실행할 수 있으나, 현재 본 샘플은 에디터 전용 포스트빌드에 한 합니다.
 */

DEFINE_LATENT_AUTOMATION_COMMAND(FSpawnStart);

//SpawnMeComplete가 True면 끝
bool FSpawnStart::Update()
{
	static int32 count = 0;
	count++;
	UE_LOG(LogTemp, Log, TEXT("FallingTest - FSpawnStart - count : %d"), count);

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FSpawnFinish);

//SpawnMeComplete가 True면 끝
bool FSpawnFinish::Update()
{
	static int32 count = 0;
	count++;
	UE_LOG(LogTemp, Log, TEXT("FallingTest - SpawnFinish - count : %d"), count);

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FWarpLocTest, int32, inMapId, FVector, inWarpLoccation);

//SpawnMeComplete가 True면 끝
bool FWarpLocTest::Update()
{
	UGsLevelManager::SetIsPlayerSpawnComplete(false);

	FString command = FString::Printf(TEXT("warp %d %d %d %d"), inMapId, (int32)inWarpLoccation.X, (int32)inWarpLoccation.Y, (int32)inWarpLoccation.Z);

	UWorld* world = UGsLevelManager::GetGameWorld();
	if (nullptr == world)
	{
		return false;
	}

	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		//wait spawn me complete
		controller->ConsoleCommand(command);
	}	

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FWarpSpotTest, int32, inMapId, int32, inSpotId);

//SpawnMeComplete가 True면 끝
bool FWarpSpotTest::Update()
{
	UGsLevelManager::SetIsPlayerSpawnComplete(false);

	FString command = FString::Printf(TEXT("warpSpot %d %d"), inMapId, inSpotId);

	UWorld* world = UGsLevelManager::GetGameWorld();
	if (nullptr == world)
	{
		return false;
	}

	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		//wait spawn me complete
		controller->ConsoleCommand(command);
	}

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCompleteQuestTest, int32, inQuestId);

//SpawnMeComplete가 True면 끝
bool FCompleteQuestTest::Update()
{	
	FString command = FString::Printf(TEXT("completequest %d"), inQuestId);

	UWorld* world = UGsLevelManager::GetGameWorld();
	if (nullptr == world)
	{
		return false;
	}

	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		//wait spawn me complete
		controller->ConsoleCommand(command);
	}

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitCompleteQuestTest, int32, inQuestId);

//SpawnMeComplete가 True면 끝
bool FWaitCompleteQuestTest::Update()
{
	FString command = FString::Printf(TEXT("completequest %d"), inQuestId);

	UWorld* world = UGsLevelManager::GetGameWorld();
	if (nullptr == world)
	{
		return false;
	}

	if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
	{
		//wait spawn me complete
		controller->ConsoleCommand(command);
	}

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FWaitUntilSpawnMeComplete);

//SpawnMeComplete가 True면 끝
bool FWaitUntilSpawnMeComplete::Update()
{
	return UGsLevelManager::GetIsPlayerSpawnComplete();		
}

DEFINE_LATENT_AUTOMATION_COMMAND(FCheckPlayerFalling);

//SpawnMeComplete가 True면 끝
bool FCheckPlayerFalling::Update()
{
	UWorld* world = UGsLevelManager::GetGameWorld();
	if (nullptr == world)
	{
		return false;
	}

	//is player falling?	
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(world, 0);
	if (false == character)
	{
		return false;
	}

	bool isFalling = character->GetMovementComponent()->IsFalling();

	if (isFalling)
	{
		UE_LOG(LogTemp, Log, TEXT("FallingTest - Player is falling"));
	}	

	return !isFalling;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(InGameTestPlayerSpawnTest, TEXT("Raon.InGameTest.SpawnPlayer")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)

bool InGameTestPlayerSpawnTest::RunTest(const FString& Parameters)
{			
	//레오성 워프
	ADD_LATENT_AUTOMATION_COMMAND(FSpawnStart);
	ADD_LATENT_AUTOMATION_COMMAND(FWarpLocTest(3021011, FVector(26406, 46030, -568)));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitUntilSpawnMeComplete());
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	//아이리스 외곽 비석 워프
	ADD_LATENT_AUTOMATION_COMMAND(FWarpLocTest(3021011, FVector(33467, 28732, - 2054)));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitUntilSpawnMeComplete());
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FCheckPlayerFalling());
	ADD_LATENT_AUTOMATION_COMMAND(FSpawnFinish);

	return true;
}