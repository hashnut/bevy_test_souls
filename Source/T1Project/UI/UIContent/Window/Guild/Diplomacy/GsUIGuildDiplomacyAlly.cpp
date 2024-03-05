// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyAlly.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyAllyListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacySearch.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Message/MessageParam/GsGuildMessageParam.h"


void UGsUIGuildDiplomacyAlly::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildDiplomacyAlly::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildDiplomacyAlly::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->SetEmptyListPanel(_panelRootEmpty);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildDiplomacyAlly::OnRefreshEntry);

	_btnAlly->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyAlly::OnClickAlly);
}

void UGsUIGuildDiplomacyAlly::NativeDestruct()
{
	_myGuildData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyAlly::Show()
{
	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	_myGuildData = guildMgr->GetMyGuildData();
	
	// 최대 동맹 수 받아 둠
	if (const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig())
	{
		_maxAllyCount = config->maxAllyCount;
	}

	_scroll->ScrollToStart();
	InvalidateList();
}

void UGsUIGuildDiplomacyAlly::Hide()
{
	// 페이지 확인을 하면 레드닷을 제거
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateDiplomacyAllyCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyAlly);
	}
}

void UGsUIGuildDiplomacyAlly::SendRequestList(bool bInResetCoolTime)
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (bInResetCoolTime)
		{
			guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyList);
		}

		guildMgr->GetSendController().SendReqAllyList();
	}
}

bool UGsUIGuildDiplomacyAlly::IsRedDot()
{
	return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyAlly);
}

void UGsUIGuildDiplomacyAlly::OnChangeMyAuthority()
{
	_btnAlly->SetIsEnabled(CanAlly());

	InvalidateList();
}

void UGsUIGuildDiplomacyAlly::OnDiplomacyInvalidateList()
{
	InvalidateList();
}

void UGsUIGuildDiplomacyAlly::OnDiplomacyResendList()
{
	SendRequestList(true);
}

void UGsUIGuildDiplomacyAlly::OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	// 삭제되었을 경우
	if (false == InParam->_bIsDelete)
	{
		if (FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildDataNoConst())
		{
			if (FGsGuildDiplomacyDataAlly* allyData = myGuildData->GetAllyData(InParam->_guildDBId))
			{
				if (false == allyData->IsValidData())
				{
					// 노티로 새로 추가된 경우, 누락된 정보들이 있어 리스트를 재요청 해야한다.
					SendRequestList(true);
					return;
				}
			}
		}
	}

	InvalidateList();
}

void UGsUIGuildDiplomacyAlly::OnClickAlly()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacySearch"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacySearch* popup = Cast<UGsUIPopupGuildDiplomacySearch>(widget.Get()))
		{
			popup->SetData(true);
		}
	}
}

void UGsUIGuildDiplomacyAlly::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _myGuildData ||
		false == _myGuildData->_allyList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildDiplomacyAllyListEntry* entry = Cast<UGsUIGuildDiplomacyAllyListEntry>(InEntry))
	{
		entry->SetData(&_myGuildData->_allyList[InIndex]);
	}
}

void UGsUIGuildDiplomacyAlly::InvalidateList()
{	
	if (nullptr == _myGuildData)
	{
		return;
	}

	// 리스트 표시
	int32 allyCount = _myGuildData->_allyList.Num();
	_slotHelper->RefreshAll(allyCount);
	_scroll->ScrollToStart();

	// 기사단 수 표시
	FString strGuildCount = FString::Format(TEXT("{0}/{1}"), { allyCount, _maxAllyCount });
	_tbGuildCount->SetText(FText::FromString(strGuildCount));

	// 권한에 따라 버튼 활성화
	_btnAlly->SetIsEnabled(CanAlly());
}

bool UGsUIGuildDiplomacyAlly::CanAlly() const
{
	if (_myGuildData)
	{
		// 권한 체크
		if (FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority()))
		{
			// 최대인원 체크
			if (_maxAllyCount > _myGuildData->_allyList.Num())
			{
				return true;
			}
		}
	}

	return false;
}
