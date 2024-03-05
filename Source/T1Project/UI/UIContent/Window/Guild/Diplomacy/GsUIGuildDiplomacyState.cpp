// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyState.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyStateListEntry.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Message/MessageParam/GsGuildMessageParam.h"


void UGsUIGuildDiplomacyState::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildDiplomacyState::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildDiplomacyState::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);	
	_slotHelper->SetEmptyListPanel(_panelRootEmpty);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildDiplomacyState::OnRefreshEntry);
}

void UGsUIGuildDiplomacyState::Show()
{
	InvalidateList();
}

void UGsUIGuildDiplomacyState::Hide()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateDiplomacyStateCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyState);
	}
}

void UGsUIGuildDiplomacyState::SendRequestList(bool bInResetCoolTime)
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (bInResetCoolTime)
		{
			guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyStateList);
		}

		guildMgr->GetSendController().SendReqAllyStateList();
	}
}

bool UGsUIGuildDiplomacyState::IsRedDot()
{
	return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyState);
}

void UGsUIGuildDiplomacyState::OnDiplomacyInvalidateList()
{
	InvalidateList();
}

void UGsUIGuildDiplomacyState::OnDiplomacyResendList()
{
	SendRequestList(true);
}

void UGsUIGuildDiplomacyState::OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	// 삭제되었을 경우
	if (false == InParam->_bIsDelete)
	{
		if (FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildDataNoConst())
		{
			if (FGsGuildDiplomacyDataState* stateData = myGuildData->GetAllyStateData(InParam->_guildDBId))
			{
				if (false == stateData->IsValidData())
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

void UGsUIGuildDiplomacyState::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildDiplomacyStateListEntry* entry = Cast<UGsUIGuildDiplomacyStateListEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex]);
	}
}

void UGsUIGuildDiplomacyState::InvalidateList()
{
	// 데이터 세팅
	_dataList.Empty();
	int requestCount = 0;
	int requestedCount = 0;

	if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
	{
		for (const FGsGuildDiplomacyDataState& stateData : myGuildData->_stateList)
		{
			if (stateData.IsRequest())
			{
				++requestCount;
			}
			else
			{
				++requestedCount;
			}

			_dataList.Emplace(&stateData);
		}
	}

	SortByRemainTime(false);

	if (_slotHelper)
	{
		_slotHelper->RefreshAll(_dataList.Num());
	}

	// 스크롤 리셋
	_scroll->ScrollToStart();

	// 기사단 수 표시
	if (const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig())
	{
		// 보낸 요청
		FString strRequestCount = FString::Format(TEXT("{0}/{1}"), { requestCount, config->maxAllySendCount });
		_tbRequestCount->SetText(FText::FromString(strRequestCount));

		// 받은 요청
		FString strRequestedCount = FString::Format(TEXT("{0}/{1}"), { requestedCount, config->maxAllyReceiveCount });
		_tbRequestedCount->SetText(FText::FromString(strRequestedCount));
	}
}

void UGsUIGuildDiplomacyState::SortByRemainTime(bool bIsAscending)
{
	if (bIsAscending)
	{
		_dataList.Sort([](const FGsGuildDiplomacyDataState& A, const FGsGuildDiplomacyDataState& B)
			{
				return (A._endTime > B._endTime) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildDiplomacyDataState& A, const FGsGuildDiplomacyDataState& B)
			{
				return (A._endTime <= B._endTime) ? true : false;
			});
	}
}
