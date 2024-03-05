// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowAccumulateTax.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Common/ListEntry/GsUIOneTextBoxListEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsSiegeWarfareManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/InterServer/GsSchemaWorldGroupSiegeWarfare.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"


void UGsUIWindowAccumulateTax::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowAccumulateTax::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowAccumulateTax::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 설정
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowAccumulateTax::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowAccumulateTax::OnClickCloseAllStack);

	_btnTooltip->OnPressed.AddDynamic(this, &UGsUIWindowAccumulateTax::OnPressTooltip);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowAccumulateTax::OnRefreshEntry);
}

void UGsUIWindowAccumulateTax::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	InvalidateUI();

	// 누적 세금 정보 요청
	if (FGsSiegeWarfareManager* siegeMgr = GSSiegeWarfare())
	{
		if (FGsTaxDataHandler* taxHandler = siegeMgr->GetTaxHandler())
		{
			taxHandler->SendReqAccumTax(false);
		}
	}
}

void UGsUIWindowAccumulateTax::NativeDestruct()
{
	UnbindMessages();

	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	_totalTime = 0;
	_accumEndTime = 0;

	Super::NativeDestruct();
}

bool UGsUIWindowAccumulateTax::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsSiegeWarfare))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SIEGE_WARFARE);
		return true;
	}

	return false;
}

void UGsUIWindowAccumulateTax::BindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
	{
		return;
	}

	_msgHandleSystem = messageMgr->GetSystemParam().AddUObject(MessageSystem::RESET_TIME_CONTENTS,
		this, &UGsUIWindowAccumulateTax::OnResetTimeContents);

	MMarket& msgMarket = messageMgr->GetMarket();
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ACCUM_TAX_LIST,
		this, &UGsUIWindowAccumulateTax::OnRcvAccumTaxList));

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	_msgHandleUnlockList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIWindowAccumulateTax::OnContentsUnlock));
	_msgHandleUnlockList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIWindowAccumulateTax::OnContentsUnlock));
}

void UGsUIWindowAccumulateTax::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
	{
		return;
	}

	messageMgr->GetSystemParam().Remove(_msgHandleSystem);

	MMarket& msgMarket = messageMgr->GetMarket();
	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	for (MsgHandleUnlock& handle : _msgHandleUnlockList)
	{
		msgUnlock.Remove(handle);
	}
	_msgHandleUnlockList.Empty();
}

void UGsUIWindowAccumulateTax::OnResetTimeContents(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		ResetTimeTicket resetType = static_cast<ResetTimeTicket>(param->_data);
		if (ResetTimeTicket::RTT_CALCUATION_TAX == resetType)
		{
			// 누적 세금 정보 요청
			if (FGsSiegeWarfareManager* siegeMgr = GSSiegeWarfare())
			{
				if (FGsTaxDataHandler* taxHandler = siegeMgr->GetTaxHandler())
				{
					// 강제 쿨타임 리셋
					taxHandler->SendReqAccumTax(true);
				}
			}
		}
	}
}

void UGsUIWindowAccumulateTax::OnRcvAccumTaxList(const IGsMessageParam*)
{
	InvalidateUI();
}

void UGsUIWindowAccumulateTax::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
		{
			if (static_cast<int32>(EGsUnlockContentsType::SIEGE_WARFARE) != param->_first)
			{
				// 파라미터가 있을 경우엔 공성전 항목이 아니면 체크할 필요가 없음
				return;
			}
		}
	}

	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{	
		// 컨텐츠가 막히면 창을 닫아버림(CloseAllStack 처리)
		if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::SIEGE_WARFARE))
		{			
			if (UGsUIManager* uiMgr = GUI())
			{
				uiMgr->CloseAllStack(this);
			}
		}
	}
}

void UGsUIWindowAccumulateTax::InvalidateUI()
{
	FGsTaxDataHandler* taxHandler = nullptr;
	if (FGsSiegeWarfareManager* siegeMgr = GSSiegeWarfare())
	{
		taxHandler = siegeMgr->GetTaxHandler();
	}

	if (nullptr == taxHandler)
	{
		return;
	}

	SetTextAccumAmount(taxHandler->GetAccumGold(), _textBlockGold);
	SetTextAccumAmount(taxHandler->GetAccumDia(), _textBlockDia);
		
	_accumEndTime = taxHandler->GetAccumEndTime();
	_totalTime = _accumEndTime - taxHandler->GetAccumStartTime();
	
	if (0 < _totalTime)
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
			world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIWindowAccumulateTax::OnTimerUpdateTime,
				CHECK_TIME_DURATION_SEC, true);
		}
	}

	InvalidateTime();
	InvalidateServerList();
}

void UGsUIWindowAccumulateTax::InvalidateTime()
{
	if (0 == _totalTime ||
		0 == _accumEndTime)
	{
		_textBlockRemainTime->SetVisibility(ESlateVisibility::Hidden);
		SetProgress(0.f);
		return;
	}

	time_t nowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainSec = _accumEndTime - nowSec;
	if (0 >= remainSec)
	{
		_textBlockRemainTime->SetVisibility(ESlateVisibility::Hidden);
		SetProgress(0.f);
		return;
	}	

	FString strTime;
	FGsTimeStringHelper::GetTimeStringCeil(FTimespan::FromSeconds(remainSec), strTime, false);

	_textBlockRemainTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textBlockRemainTime->SetText(FText::FromString(strTime));

	float progress = 1.f - (static_cast<float>(remainSec) / static_cast<float>(_totalTime));
	SetProgress(progress);
}

void UGsUIWindowAccumulateTax::InvalidateServerList()
{
	_serverList.Empty();	

	if (const FGsSchemaWorldGroupSiegeWarfare* myGroup = GetMyWorldGroup())
	{
		for (const FGsSchemaWorldDataRow& itData : myGroup->worldList)
		{
			if (const FGsSchemaWorldData* worldData = itData.GetRow())
			{
				_serverList.Emplace(worldData);
			}
		}
	}	

	_slotHelper->RefreshAll(_serverList.Num());
	
	_scroll->ScrollToStart();
}

const FGsSchemaWorldGroupSiegeWarfare* UGsUIWindowAccumulateTax::GetMyWorldGroup() const
{
	WorldId myWorldId = INVALID_WORLD_ID;
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
		{
			myWorldId = userData->mHomeWorldId;
		}
	}

	if (INVALID_WORLD_ID != myWorldId)
	{
		if (const UGsTable* table = FGsSchemaWorldGroupSiegeWarfare::GetStaticTable())
		{
			TArray<const FGsSchemaWorldGroupSiegeWarfare*> rows;
			table->GetAllRows(rows);

			for (const FGsSchemaWorldGroupSiegeWarfare* itGroup : rows)
			{
				for (const FGsSchemaWorldDataRow& itData : itGroup->worldList)
				{
					if (const FGsSchemaWorldData* worldData = itData.GetRow())
					{
						if (worldData->worldId == myWorldId)
						{
							return itGroup;
						}
					}
				}
			}
		}
	}

	return nullptr;
}

void UGsUIWindowAccumulateTax::SetProgress(float InValue)
{
	_progressBar->SetPercent(InValue);
	_sliderBar->SetValue(InValue);
}

void UGsUIWindowAccumulateTax::SetTextAccumAmount(Currency InAmount, OUT UTextBlock* OutTextBlock)
{
	if (0 == InAmount)
	{
		// TEXT: 집계중
		FText textCounting;
		FText::FindText(TEXT("SiegeWarfareText"), TEXT("UI_AccumulateTax_Counting"), textCounting);

		OutTextBlock->SetText(textCounting);
		return;
	}

	OutTextBlock->SetText(FText::AsNumber(InAmount));
}

void UGsUIWindowAccumulateTax::OnTimerUpdateTime()
{
	InvalidateTime();
}

void UGsUIWindowAccumulateTax::OnPressTooltip()
{
	if (_uiTooltip->IsOpen())
	{
		_uiTooltip->Close();
		return;
	}

	// TEXT: 누적된 세금은...(생략)
	FText textDesc;
	FText::FindText(TEXT("SiegeWarfareText"), TEXT("Notice_AccumulateTax_Info_Desc"), textDesc);
	_uiTooltip->SetDescText(textDesc);
	
	_uiTooltip->Open();
}

void UGsUIWindowAccumulateTax::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _serverList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIOneTextBoxListEntry* entry = Cast<UGsUIOneTextBoxListEntry>(InEntry))
	{
		entry->SetData(_serverList[InIndex]->worldName);
	}
}
