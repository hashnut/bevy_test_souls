#include "GsWindowPlatformManager.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

#if PLATFORM_WINDOWS
bool UGsWindowPlatformManager::FGsWindowMessageHandler::ProcessMessage(
	HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult)
{
// 	switch (msg)
// 	{
// 	default:
// 		break;
// 	}
	return false;
}

void UGsWindowPlatformManager::Initialize()
{
	Super::Initialize();

	FWindowsApplication* windowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
	check(windowsApplication);
	windowsApplication->AddMessageHandler(_windowMessageHandler);

	_gameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGsWindowPlatformManager::OnStartGameInstance);	
}

void UGsWindowPlatformManager::Finalize()
{
	Super::Finalize();

	FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
	check(WindowsApplication);
	WindowsApplication->RemoveMessageHandler(_windowMessageHandler);

	FWorldDelegates::OnStartGameInstance.Remove(_gameInstanceStartHandle);
}

void UGsWindowPlatformManager::OnStartGameInstance(UGameInstance* GameInstance)
{
	// 에디터 환경에서는 처리하지 않음
#if !WITH_EDITOR
	DestroyWindowPopupOverride();
#endif
}

void UGsWindowPlatformManager::DestroyWindowOverride(const TSharedRef<SWindow>& WindowBeingDestroyed)
{
	FGsUIHelper::PopupQuitGame();
}

void UGsWindowPlatformManager::DestroyWindowPopupOverride()
{
	// 2023/5/26 PKT - 게임을 종료 할지 문의 하는 코드(PIE 에서 실행)
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return;
	}

	const UGameInstance* gameInstance = world->GetGameInstance();
	if (nullptr == gameInstance)
	{
		return;
	}

	UGameViewportClient* viewPortClient = gameInstance->GetGameViewportClient();
	if (nullptr == viewPortClient)
	{
		return;
	}

	if (false == viewPortClient->GetWindow().IsValid())
	{
		return;
	}

	viewPortClient->GetWindow()->SetRequestDestroyWindowOverride(FRequestDestroyWindowOverride::CreateUObject(this, &UGsWindowPlatformManager::DestroyWindowOverride));
}
#endif