
#include "GsGameStateOfflinePlay.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsMessageParam.h"



FGsGameStateOfflinePlay::FGsGameStateOfflinePlay() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsOfflinePlay)
{
	/*None*/
}

void FGsGameStateOfflinePlay::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("WindowOfflinePlay"));
		if (Widget.IsValid())
		{
			_window = Cast<UGsUIWindowOfflinePlay>(Widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();
}

void FGsGameStateOfflinePlay::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_window = nullptr;

	// 메세지 해지
	UnregisterMessages();

	FGsContentsGameBase::Exit();
}

void FGsGameStateOfflinePlay::RegisterMessages()
{
	// 2021/06/25 PKT - bind...	
	MOfflinePlay& messageHolder = GMessage()->GetOfflinePlay();
	// 2021/06/25 PKT - HUDD 모드 변경시 마다 이벤트를 받는다.
	_listMessageDelegate.Emplace(messageHolder.AddRaw(MessageContentOfflinePlay::INVALIDATE_AVAILABLE_TIME, this, &FGsGameStateOfflinePlay::OnUpdateTime));
}

void FGsGameStateOfflinePlay::UnregisterMessages()
{
	// 2021/06/25 PKT - release...
	MOfflinePlay& messageHolder = GMessage()->GetOfflinePlay();
	for (const TPair< MessageContentOfflinePlay, FDelegateHandle>& delegate : _listMessageDelegate)
	{
		messageHolder.Remove(delegate);
	}	
}

void FGsGameStateOfflinePlay::OnUpdateTime(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{	
		return;
	}
	// 2021/06/29 PKT - 이용 가능 시간 업데이트
	_window->InvalidateTime();
}
