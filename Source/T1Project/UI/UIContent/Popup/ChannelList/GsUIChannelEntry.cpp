// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChannelEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Net/GsNetSendServiceWorld.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIChannelEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIChannelEntry::OnClickSlot);
}

void UGsUIChannelEntry::OnShow()
{
	// 미리 찾아둔다
	// TEXT: {0} 채널
	FText::FindText(TEXT("UICommonText"), TEXT("NChannel"), _textChannelFormat);
}

void UGsUIChannelEntry::SetData(const FGsMapChannelData& InData, bool bInIsCurrentChannel)
{
	_data = &InData;
	_bIsCurrentChannel = bInIsCurrentChannel;

	// 내 채널인지 표시
	_panelMyChannelRoot->SetVisibility((_bIsCurrentChannel) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	// 채널 번호 표시
	if (_textChannelFormat.IsEmpty())
	{
		_textChannel = FText::GetEmpty();
	}
	else
	{
		_textChannel = FText::Format(_textChannelFormat, InData._channel);
		FText::AsNumber(InData._channel);
	}

	// 혼잡도 표시
	SetUICongestion(InData._state);
}

void UGsUIChannelEntry::OnClickSlot()
{
	if (false == CheckCanChangeChannel())
	{
		return;
	}

	GSMinimap()->ReserveChannelMove(_data->_channel, _bIsCurrentChannel, _data->_state);
}

void UGsUIChannelEntry::SetUICongestion(ChannelCongestionStatus InStatus)
{
	FText text;
	FLinearColor color;
	switch (InStatus)
	{
	// 부족은 표시 안함(원활로 표시)
	case ChannelCongestionStatus::LOW:
	//{
	//	FText::FindText(TEXT("SystemText"), TEXT("CongestionLow"), text);
	//	color = FGsUIColorHelper::GetColor(EGsUIColorType::CONGESTION_LOW);
	//}
	//break;
	case ChannelCongestionStatus::MEDIUM:
	{
		FText::FindText(TEXT("SystemText"), TEXT("CongestionMedium"), text);
		color = FGsUIColorHelper::GetColor(EGsUIColorType::CONGESTION_MEDIUM);
	}
	break;	
	case ChannelCongestionStatus::HIGH:
	{
		FText::FindText(TEXT("SystemText"), TEXT("CongestionHigh"), text);
		color = FGsUIColorHelper::GetColor(EGsUIColorType::CONGESTION_HIGH);
	}
	break;
	case ChannelCongestionStatus::FULL:
	{
		FText::FindText(TEXT("SystemText"), TEXT("CongestionFull"), text);
		color = FGsUIColorHelper::GetColor(EGsUIColorType::CONGESTION_FULL);
	}
	break;
	}

	_textBlockState->SetText(text);
	_textBlockState->SetColorAndOpacity(color);
}

void UGsUIChannelEntry::SendRequestSelectChannel()
{
	// 채널 이동 확인 팝업을 오랫동안 띄워놓고 있으면 상태가 바뀔 수 있으므로 한 번 더 체크한다
	if (false == CheckCanChangeChannel())
	{
		return;
	}

	// 채널이동 패킷 전송
	FGsNetSendServiceWorld::SendRequestSelectChannel(_data->_channel);
}

bool UGsUIChannelEntry::CheckCanChangeChannel()
{
	// 현재 채널일 경우 버튼 막기
	if (_bIsCurrentChannel)
	{
		return false;
	}

	// 꽉참일 경우 이동 불가 팝업 띄움
	if (ChannelCongestionStatus::FULL == _data->_state)
	{
		// TEXT: 해당 채널에 유저가 많아서 이동할 수 없습니다.
		FText message;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelChangeDeny"), message);

		FGsUIHelper::PopupOK(message);
		return false;
	}

	// 쿨타임 체크. 쿨타임은 팝업에서 보여주고 있으므로 별도 안내가 필요 없음
	if (false == GGameData()->CanChangeChannel())
	{
		return false;
	}

	// 내 캐릭터가 평화 상태인지 체크
	if (false == IsPeaceState())
	{
		// TEXT: 전투 중에는 채널을 이동할 수 없습니다.
		FText message;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelCombatDeny"), message);
		
		FGsUIHelper::PopupOK(message);
		return false;
	}

	return true;
}

bool UGsUIChannelEntry::IsPeaceState() const
{
	if (UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			// 전투상태면 불가
			if (local->IsBattleMode())
			{
				return false;
			}
		}
	}

	return true;
}
