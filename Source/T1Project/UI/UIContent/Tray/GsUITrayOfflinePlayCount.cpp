// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITrayOfflinePlayCount.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/Image.h"
#include "Net/GsNetSendServiceWorld.h"



void UGsUITrayOfflinePlayCount::NativeConstruct()
{
	Super::NativeConstruct();

	// 2023/8/17 PKT - Init
	Reset();

	// 2023/8/17 PKT - Add Event Delegate
	_buttonCancel->OnClicked.AddDynamic(this, &UGsUITrayOfflinePlayCount::OnClickedScreen);
	
	// 2023/8/17 PKT - Message Bind
	if (GMessage())
	{
		_autoStateDelegate = GMessage()->GetGameObject().AddUObject(MessageGameObject::AI_MODE_ACTIVE, this, &UGsUITrayOfflinePlayCount::OnChangedAutoState);
	}
}

void UGsUITrayOfflinePlayCount::NativeDestruct()
{
	// 2023/8/17 PKT - Clear
	Reset();

	// 2023/8/17 PKT - Remove Event Delegate
	_buttonCancel->OnClicked.RemoveDynamic(this, &UGsUITrayOfflinePlayCount::OnClickedScreen);

	// 2023/8/17 PKT - Message Unbind
	if (GMessage())
	{
		GMessage()->GetGameObject().Remove(_autoStateDelegate);
	}

	Super::NativeDestruct();
}

void UGsUITrayOfflinePlayCount::Reset()
{
	_elapsedTime = 0.f;

	_delaySecounds = 0;

	// 2023/8/17 PKT - Text Clean
	SetTextCount(_delaySecounds);
	_textCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 2023/8/17 PKT - Button Clicked Active
	_buttonCancel->SetVisibility(ESlateVisibility::Visible);
	
	// 2023/8/17 PKT - Image Alpha
	auto color = _blockOutImag->ColorAndOpacity;
	color.A = _begineAlphaValue;
	_blockOutImag->SetColorAndOpacity(color);

	// 2023/8/17 PKT - IOS 전용 공지 메세지
	_noticeExit->SetVisibility(ESlateVisibility::Collapsed);

	// 2023/8/17 PKT - Timer Handler 초기화
	TWeakObjectPtr<UWorld> world = GetWorld();
	if (world.IsValid())
	{
		if (world.Get()->GetTimerManager().IsTimerActive(_Timerhandler))
		{
			world.Get()->GetTimerManager().ClearTimer(_Timerhandler);
		}
	}
}

void UGsUITrayOfflinePlayCount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ESlateVisibility::Collapsed != _noticeExit->GetVisibility() || 0 >= _delaySecounds)
	{	// 2023/8/17 PKT - 공지가 활성화 되어 있는 상태라면 카운트 하지 않음.( 카운트가 없고 공지만 보일 테니깐.. )
		return;
	}

	// 2023/8/17 PKT - min : 0, max : _delaySecounds
	_elapsedTime = FMath::Min(_elapsedTime + InDeltaTime, static_cast<float>(_delaySecounds));
		
	// 2023/8/17 PKT - 0 ~ 1
	float ratio = FMath::Clamp(_elapsedTime / static_cast<float>(_delaySecounds), 0.f, 1.f);

	// 2023/8/17 PKT - Render Alpha
	// 2023/8/17 PKT - Image Alpha
	auto color = _blockOutImag->ColorAndOpacity;
	color.A = _begineAlphaValue + ((1.f - _begineAlphaValue) * ratio);
	_blockOutImag->SetColorAndOpacity(color);
}

void UGsUITrayOfflinePlayCount::SetTextCount(int32 InDelaySeconds)
{
	InDelaySeconds = FMath::Max(InDelaySeconds, 0);
	_textCount->SetText(FText::AsNumber(InDelaySeconds));
}

void UGsUITrayOfflinePlayCount::OnChangedAutoState(const IGsMessageParam* InParam /*= nullptr*/)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	// 2021/08/26 PKT - AI 비활성화면 오토 중지
	if (false == isActive)
	{
		CancelOfflinePlay();
	}
}

void UGsUITrayOfflinePlayCount::OnCount()
{
	int32 currentCount = _delaySecounds - static_cast<int32>(_elapsedTime);
	if (0 >= currentCount)
	{
		FGsOfflinePlayManager* offlineManager = GSOfflinePlay();
		if (nullptr != offlineManager)
		{
			FGsNetSendServiceWorld::SendReqOfflinePlayStartPlay();
		}
	}
	else
	{
		SetTextCount(currentCount);
	}
}

void UGsUITrayOfflinePlayCount::CancelOfflinePlay()
{
	Close();
	// 2023/8/17 PKT - HUD 원복
	ShowHUD();
	// 2023/8/17 PKT -  서버에게 오프라인 플레이 취소를 요청 한다.
	FGsNetSendServiceWorld::SendReqOfflinePlayCancel();
}

void UGsUITrayOfflinePlayCount::OnClickedScreen()
{
	CancelOfflinePlay();
}

void UGsUITrayOfflinePlayCount::HideHUD()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr != uiManager)
	{
		_currentHUDMode = GUI()->GetHUDMode();
		GUI()->SetHideFlags(EGsUIHideFlags::STATE_OFFLINE_PLAY);
	}
}

void UGsUITrayOfflinePlayCount::ShowHUD()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr != uiManager)
	{
		if (true == EnumHasAnyFlags(EGsUIHideFlags::STATE_OFFLINE_PLAY, uiManager->GetHideFlagsCurrent()))
		{	// 2021/06/30 PKT - Flag가 설정되어 있으면 지운다.
			GUI()->ClearHideFlags(EGsUIHideFlags::STATE_OFFLINE_PLAY);
		}

		GUI()->SetHUDMode(_currentHUDMode, true);
	}	
}

void UGsUITrayOfflinePlayCount::SetData(int32 InDelaySeconds)
{	
	_delaySecounds = InDelaySeconds;
	
	// 2023/8/17 PKT - HUD 숨김
	HideHUD();
	// 2023/8/17 PKT - 
	OnCount();

	TWeakObjectPtr<UWorld> world = GetWorld();
	if (world.IsValid())
	{
		const static float INTERVAL = 1.0f;
		world->GetTimerManager().SetTimer(_Timerhandler, this, &UGsUITrayOfflinePlayCount::OnCount, INTERVAL, true);
	}
}

void UGsUITrayOfflinePlayCount::UIBlockAndNotice()
{
	Reset();

	auto color = _blockOutImag->ColorAndOpacity;
	color.A = 1.f;
	_blockOutImag->SetColorAndOpacity(color);

	// 2023/8/17 PKT - 종료 해도 좋다는 공지를 보여 주도록 한다.
	_noticeExit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 2023/8/17 PKT - 카운트 텍스트 박스 비활성화
	_textCount->SetVisibility(ESlateVisibility::Collapsed);

	// 2023/8/17 PKT - 버튼 클릭 이벤트 끔
	_buttonCancel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}