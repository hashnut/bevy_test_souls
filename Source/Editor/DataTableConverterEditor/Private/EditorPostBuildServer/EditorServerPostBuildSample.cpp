#if WITH_DEV_AUTOMATION_TESTS

#include "Runtime/AssetRegistry/Public/AssetRegistry/AssetRegistryModule.h"
#include "Runtime/Core/Public/Misc/AutomationTest.h"

/**
 * 서버 전용 에디터 포스트빌드 테스트 샘플
 * 
 * (동일한 역할을 하는 클라 전용 클래스가 있으나, 이 포스트빌드 샘플은 서버팀 사용 전용)
 * 
 * 포스트빌드에 자동 포함하기 위한 유일한 조건은 테스트 이름이 "Raon.EditorServerPostBuild." 로 시작하는 것 입니다.
 * 접두어 "Raon.EditorServerPostBuild." 뒤에 숫자 0~9 를 입력하여 실행 순서를 설정할 수 있습니다(작은 숫자가 먼저 실행됨).
 * 아래 코드를 참고하세요.
 * 
 * 이 테스트를 로컬에서 실행하고자 하면, 아래 스크립트를 참조하세요.
 * UE4Editor-Cmd.exe T1Project.uproject -NoP4 -Run -StdOut -NullRHI -Unattended  -NoPause -UTF8Output -NoXge -ExecCmds="Automation RunTests Raon.EditorServerPostBuild; Quit"
 * 
 * 또한, 본 포스트빌드는 테스트 프로세스의 일부이므로, Unreal Frontend 에서 원하는 작업을 직접 선택하여 실행할 수 있습니다.
 * 
 * 참고로, 테스트 프로세스는 에디터에만 가능한 것은 아닙니다. 당연히 런타임에도 테스트 프로세스를 실행할 수 있으나, 현재 본 샘플은 에디터 전용 포스트빌드에 한 합니다.
 * 
 * 참고: https://jira.com2us.com/wiki/x/u8n5Dg
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorServerPostBuildSample, TEXT("Raon.EditorServerPostBuild.0 서버 포스트빌드 샘플")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)

bool EditorServerPostBuildSample::RunTest(const FString& Parameters)
{
	AddInfo(TEXT("에디터 서버 포스트빌드 샘플 테스트"));
	return true;
}

#endif