#pragma once

#include "GsPlatformManager.h"


#if PLATFORM_WINDOWS
// Windows 전용 헤더 선언을 이곳 안에서 정의 한다.


// warning C4005 : 'WIN32_LEAN_AND_MEAN' 대응
#ifdef WIN32_LEAN_AND_MEAN
__pragma (warning(push)) \
__pragma (warning(disable: 4005))
#endif

#include "WindowsApplication.h"

#ifdef WIN32_LEAN_AND_MEAN
__pragma (warning(pop))
#endif

#endif

#include "GsWindowPlatformManager.generated.h"

//------------------------------------------------------------------------------
// 플렛폼별 관리 매니저 (Windows)
//------------------------------------------------------------------------------
UCLASS()
class T1PROJECT_API UGsWindowPlatformManager :
	public UGsPlatformManager
{
	GENERATED_BODY()

#if PLATFORM_WINDOWS
private:	
	class FGsWindowMessageHandler : public IWindowsMessageHandler
	{
	public:
		virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult) override;
	} _windowMessageHandler;


private:
	FDelegateHandle _gameInstanceStartHandle;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	void OnStartGameInstance(UGameInstance* GameInstance);
	void DestroyWindowOverride(const TSharedRef<SWindow>& WindowBeingDestroyed);

private:
	void DestroyWindowPopupOverride();
#endif
};
