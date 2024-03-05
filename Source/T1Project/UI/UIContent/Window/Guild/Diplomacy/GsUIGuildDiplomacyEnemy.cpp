// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyEnemy.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyEnemyListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacySearch.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Message/MessageParam/GsGuildMessageParam.h"


void UGsUIGuildDiplomacyEnemy::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildDiplomacyEnemy::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildDiplomacyEnemy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->SetEmptyListPanel(_panelRootEmpty);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildDiplomacyEnemy::OnRefreshEntry);

	_btnHostile->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyEnemy::OnClickHostile);
}

void UGsUIGuildDiplomacyEnemy::NativeDestruct()
{
	_myGuildData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyEnemy::Show()
{
	// null일 수 없는 값이라 널체크 안함
	_myGuildData = GGuild()->GetMyGuildData();
	
	if (const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig())
	{
		_maxEnemyCount = config->maxEnemyCount;
	}

	InvalidateList();
}

void UGsUIGuildDiplomacyEnemy::Hide()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateDiplomacyEnemyCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyEnemy);
	}
}

void UGsUIGuildDiplomacyEnemy::SendRequestList(bool bInResetCoolTime)
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (bInResetCoolTime)
		{
			guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::EnemyList);
		}

		guildMgr->GetSendController().SendReqEnemyList();
	}
}

bool UGsUIGuildDiplomacyEnemy::IsRedDot()
{
	return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyEnemy);
}

void UGsUIGuildDiplomacyEnemy::OnChangeMyAuthority()
{
	// 적대 버튼
	_btnHostile->SetIsEnabled(CanHostile());

	InvalidateList();
}

void UGsUIGuildDiplomacyEnemy::OnDiplomacyInvalidateList()
{
	InvalidateList();
}

void UGsUIGuildDiplomacyEnemy::OnDiplomacyResendList()
{
	SendRequestList(true);
}

void UGsUIGuildDiplomacyEnemy::OnDiplomacyEnemyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	// 지워졌을 경우
	if (InParam->_bIsDelete)
	{
		// 적대 취소 팝업 닫기 처리
		TWeakObjectPtr<UGsUIWidgetBase> popupConfirm = GUI()->GetWidgetByKey(TEXT("PopupGuildDiplomacyConfirm"));
		if (popupConfirm.IsValid())
		{
			// TEXT: 해당 길드의 상태가 변경되었습니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Changed_GuildState"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			popupConfirm->Close();
		}
	}
	else
	{
		// 추가되었을 경우, 새로운 정보를 받기 위한 처리
		if (FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildDataNoConst())
		{
			if (FGsGuildDiplomacyDataEnemy* enemyData = myGuildData->GetEnemyData(InParam->_guildDBId))
			{
				if (false == enemyData->IsValidData())
				{
					// 노티로 새로 추가될 경우, 일부 정보(길드레벨, 인원 수, 시간정보 등)가 누락되어 있다. 
					// 쿨타임을 강제로 리셋 후 리스트를 다시 요청한다.		
					SendRequestList(true);
					return;
				}
			}
		}
	}

	InvalidateList();
}

void UGsUIGuildDiplomacyEnemy::OnClickHostile()
{
	if (nullptr == _myGuildData)
	{
		return;
	}

	// 최대 인원수 체크
	const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig();
	if (nullptr == config)
	{
		return;
	}

	if (config->maxEnemyCount <= _myGuildData->_enemyList.Num())
	{
		// TEXT: 적대 기사단 목록이 가득차서 더 이상 적대 기사단을 등록할 수 없습니다.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Full_Enemy"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacySearch"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacySearch* popup = Cast<UGsUIPopupGuildDiplomacySearch>(widget.Get()))
		{
			popup->SetData(false);
		}
	}
}

void UGsUIGuildDiplomacyEnemy::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (_myGuildData &&
		_myGuildData->_enemyList.IsValidIndex(InIndex))
	{
		if (UGsUIGuildDiplomacyEnemyListEntry* entry = Cast<UGsUIGuildDiplomacyEnemyListEntry>(InEntry))
		{
			entry->SetData(&_myGuildData->_enemyList[InIndex]);
		}
	}
}

void UGsUIGuildDiplomacyEnemy::InvalidateList()
{
	if (nullptr == _myGuildData)
	{
		return;
	}

	// 버튼 활성화 세팅
	_btnHostile->SetIsEnabled(CanHostile());

	// 적대 길드가 없으면 빈 화면 처리
	if (0 == _myGuildData->_enemyList.Num())
	{
		if (_slotHelper)
		{
			_slotHelper->RefreshAll(0);
		}

		SetUIGuildCount(0);

		return;
	}

	// 리스트 세팅
	if (_slotHelper)
	{
		_slotHelper->RefreshAll(_myGuildData->_enemyList.Num());
	}
	_scroll->ScrollToStart();

	// 인원수 세팅
	SetUIGuildCount(_myGuildData->_enemyList.Num());
}

void UGsUIGuildDiplomacyEnemy::SetUIGuildCount(int32 InCount)
{
	FString strGuildCount = FString::Format(TEXT("{0}/{1}"), { InCount, _maxEnemyCount });
	_tbGuildCount->SetText(FText::FromString(strGuildCount));
}

bool UGsUIGuildDiplomacyEnemy::CanHostile() const
{
	if (_myGuildData)
	{
		// 권한 체크
		if (FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority()))
		{
			// 최대인원 체크
			if (_maxEnemyCount > _myGuildData->_enemyList.Num())
			{
				return true;
			}
		}
	}

	return false;
}
