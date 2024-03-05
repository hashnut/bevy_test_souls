// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/GsUIWindowOfflinePlay.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsServerOption.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Net/GsNetSendServiceWorld.h"
#include "../../../Management/GsMessageHolder.h"
#include "../../../Message/GsMessageContents.h"


void UGsUIWindowOfflinePlay::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 2021/06/29 PKT - bind..
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowOfflinePlay::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowOfflinePlay::OnClickCloseAllStack);

	_btnOffLinePlayStart->OnClicked.AddDynamic(this, &UGsUIWindowOfflinePlay::OnStartButtonClicked);
}

void UGsUIWindowOfflinePlay::BeginDestroy()
{
	// 2021/06/29 PKT - release...
	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowOfflinePlay::OnClickBack);
	}
	_btnBack = nullptr;
	
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowOfflinePlay::OnClickCloseAllStack);
	}
	_btnClose = nullptr;

	if (_btnOffLinePlayStart)
	{
		_btnOffLinePlayStart->OnClicked.RemoveDynamic(this, &UGsUIWindowOfflinePlay::OnStartButtonClicked);
	}
	_btnOffLinePlayStart = nullptr;
	

	Super::BeginDestroy();
}

void UGsUIWindowOfflinePlay::NativeConstruct()
{
	Super::NativeConstruct();

	// 2021/06/29 PKT - 서버와 동기화 된 옵션으로 체크박스 표기
	const FGsServerOption* serverOption = GServerOption();
	if (nullptr != serverOption)
	{
		const OffLineOptionInfo& optionData = serverOption->GetOfflineOption();
		_checkBoxUsePlay->SetIsSelected(optionData.mAutoPlay);
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIWindowOfflinePlay::NativeConstruct : invaild Server Option"));
#endif
	}

	// 2021/06/28 PKT - 남은 시간 업데이트
	OnRefreshTime();
}

void UGsUIWindowOfflinePlay::NativeDestruct()
{
	// 2021/06/29 PKT - 옵션변경이 있다면 서버와 동기화
	FGsServerOption* serverOption = GServerOption();
	if (nullptr != serverOption)
	{
		const OffLineOptionInfo& optionData = serverOption->GetOfflineOption();
		bool isChecked = _checkBoxUsePlay->IsChecked();
		if (isChecked != optionData.mAutoPlay)
		{
			OffLineOptionInfo data;
			data.mAutoPlay = isChecked;
			serverOption->SetOfflineOption(data);
			serverOption->SendToServerGameOption();		// 2021/06/25 PKT - 서버 정보 갱신..
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIWindowOfflinePlay::NativeDestruct : invaild Server Option"));
#endif
	}

	Super::NativeDestruct();
}

bool UGsUIWindowOfflinePlay::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsOfflinePlay))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_OFFLINEPLAY);
		return true;
	}

	return false;
}

void UGsUIWindowOfflinePlay::OnStartButtonClicked()
{
	OnClickCloseAllStack();

	FGsOfflinePlayManager* offlinePlayManager = GSOfflinePlay();
	if (nullptr != offlinePlayManager)
	{
		offlinePlayManager->Request_ReadyState();
	}
}

void UGsUIWindowOfflinePlay::OnRefreshTime()
{
	time_t _freeRemainingSecounds = 0;
	time_t _paidRemainingSecounds = 0;
	bool _isBuyTime = false;

	if (nullptr != GSOfflinePlay())
	{
		_freeRemainingSecounds = GSOfflinePlay()->FreeRemainingSecounds();
		_paidRemainingSecounds = GSOfflinePlay()->PaidRemainingSecounds();
		_isBuyTime = GSOfflinePlay()->IsBuyTime();
	}

	/**
	 * 현재는 무료 시간에 대한 표기만 존재 함.(충전 시간에 대한 기획은 없음.)
	 */
	FDateTime dateTime = FGsTimeSyncUtil::ConvertToDateTime(_freeRemainingSecounds);
	FString textRemainingSecounds = FString::Printf(TEXT("%02d:%02d:%02d"), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
	// 2021/06/23 PKT - set..
	_textFreeRemainTimeNum ->SetText(FText::FromString(textRemainingSecounds));

	// 2021/06/28 PKT - 남은 시간이 없으면 Enable를 꺼준다.
	/*bool isEnable = (_freeRemainingSecounds > 0 || _paidRemainingSecounds > 0);
	OnRefreshButtonEnable(isEnable);*/
}

void UGsUIWindowOfflinePlay::OnRefreshButtonEnable(bool isEnable)
{	
	// 2021/06/28 PKT - 버튼 상태 변경
	_btnOffLinePlayStart->SetIsEnabled(isEnable);
}

void UGsUIWindowOfflinePlay::InvalidateTime()
{
	// window 비활성화 중이면 굳이 할 필요 없음.
	if (false == IsActive())
	{
		return;
	}

	// 2021/06/28 PKT - 시간 값 변경으로 인한 시간 갱신( with.. Button Enable 갱신 )
	OnRefreshTime();
}