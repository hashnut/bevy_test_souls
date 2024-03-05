// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyPage.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyBase.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyEnemy.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyAlly.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyState.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"


void UGsUIGuildDiplomacyPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIGuildDiplomacyPage::OnSelectTab);
	_toggleGroupTab.AddToggleWidget(_sBtnEnemy);
	_toggleGroupTab.AddToggleWidget(_sBtnAlly);
	_toggleGroupTab.AddToggleWidget(_sBtnState);

	_pageList.Empty();
	_pageList.Emplace(_uiPageEnemy);
	_pageList.Emplace(_uiPageAlly);
	_pageList.Emplace(_uiPageState);

	// 순서에 유의(탭 순서와 동일)
	_redDotList.Empty();
	_redDotList.Emplace(_uiRedDotEnemy);
	_redDotList.Emplace(_uiRedDotAlly);
	_redDotList.Emplace(_uiRedDotState);

	_currTabIndex = 0;
}

void UGsUIGuildDiplomacyPage::NativeDestruct()
{
	_currTabIndex = 0;
	_prevTabIndex = -1;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyPage::Show(bool bIsMyGuild)
{
	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	_bIsMyGuild = bIsMyGuild;

	if (false == bIsMyGuild)
	{
		// 선택된 항목이 없게 만들기 위함
		_toggleGroupTab.SetSelectedIndex(EGsDiplomacyTabType::OtherGuild);
		_panelRootTab->SetIsEnabled(false);

		_switcherPage->SetActiveWidgetIndex(EGsDiplomacyTabType::OtherGuild);

		for (UGsUIRedDotBase* redDot : _redDotList)
		{
			redDot->SetRedDot(false);
		}

		return;
	}

	_panelRootTab->SetIsEnabled(true);

	// bInForced가 true 이므로 OnSelectTab이 반드시 불리며, 거기서 필요항 패킷을 요청함
	_toggleGroupTab.SetSelectedIndex(_currTabIndex, true);

	_uiRedDotEnemy->SetRedDot(guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyEnemy));
	_uiRedDotAlly->SetRedDot(guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyAlly));

	bool bCanDiplomacy = (FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority())) ? true : false;
	if (bCanDiplomacy)
	{
		_sBtnState->SetIsEnabled(true);
		_uiRedDotState->SetRedDot(guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyState));
	}
	else
	{
		_sBtnState->SetIsEnabled(false);
		_uiRedDotState->SetRedDot(false);
	}
}

void UGsUIGuildDiplomacyPage::Hide()
{
	if (_bIsMyGuild)
	{
		// 현재 페이지 레드닷 처리 위함
		if (UGsUIGuildDiplomacyBase* currPage = GetCurrentPage())
		{
			currPage->Hide();
		}
	}

	_prevTabIndex = -1;
}

void UGsUIGuildDiplomacyPage::OnSelectTab(int32 InIndex)
{
	if (_pageList.IsValidIndex(InIndex))
	{
		_pageList[InIndex]->SendRequestList(false);
	}

	SetUIPage(InIndex);
}

void UGsUIGuildDiplomacyPage::SetUIPage(int32 InIndex)
{
	_currTabIndex = InIndex;

	if (_currTabIndex != _prevTabIndex)
	{
		if (_pageList.IsValidIndex(_prevTabIndex))
		{
			_pageList[_prevTabIndex]->Hide();

			UpdateRedDot(_prevTabIndex);

			OnChangeDiplomacyTab.ExecuteIfBound();
		}
	}

	_switcherPage->SetActiveWidgetIndex(InIndex);

	if (_pageList.IsValidIndex(InIndex))
	{
		_pageList[InIndex]->Show();
		_prevTabIndex = InIndex;
	}
}

UGsUIGuildDiplomacyBase* UGsUIGuildDiplomacyPage::GetCurrentPage() const
{
	if (_pageList.IsValidIndex(_currTabIndex))
	{
		return  _pageList[_currTabIndex];
	}

	return nullptr;
}

void UGsUIGuildDiplomacyPage::UpdateRedDot(int32 InTabIndex)
{
	if (EGsDiplomacyTabType::ALL == InTabIndex)
	{
		for (int i = 0; i < _redDotList.Num(); ++i)
		{
			if (_pageList.IsValidIndex(i))
			{
				_redDotList[i]->SetRedDot(_pageList[i]->IsRedDot());
			}
		}

		return;
	}

	if (_pageList.IsValidIndex(InTabIndex) &&
		_redDotList.IsValidIndex(InTabIndex))
	{
		_redDotList[InTabIndex]->SetRedDot(_pageList[InTabIndex]->IsRedDot());
	}
}

void UGsUIGuildDiplomacyPage::OnChangeMyAuthority()
{
	bool bCanDiplomacy = (FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority())) ? true : false;
	_sBtnState->SetIsEnabled(bCanDiplomacy);

	// 권한 박탈 상황
	if (false == bCanDiplomacy)
	{
		// 하위 팝업들이 떠있다면 강제로 닫기
		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->CloseByKeyName(TEXT("PopupGuildDiplomacySearch"));
			uiMgr->CloseByKeyName(TEXT("PopupGuildDiplomacyConfirm"));
			uiMgr->CloseByKeyName(TEXT("PopupCommonPay"));
		}

		// 동맹 신청 현황 페이지를 보던 중 권한이 박탈 될 경우, 강제로 첫번째 탭으로 변경 		
		if (EGsDiplomacyTabType::AllyState == _currTabIndex)
		{
			_toggleGroupTab.SetSelectedIndex(0, true);
			return;
		}
	}

	// 현재 페이지에 이벤트 전달
	if (UGsUIGuildDiplomacyBase* page = GetCurrentPage())
	{
		page->OnChangeMyAuthority();
	}
}

void UGsUIGuildDiplomacyPage::OnDiplomacyInvalidateList()
{
	if (UGsUIGuildDiplomacyBase* currPage = GetCurrentPage())
	{
		currPage->OnDiplomacyInvalidateList();
	}

	// 레드닷 전체 갱신
	for (int i = 0; i < _redDotList.Num(); ++i)
	{
		UpdateRedDot(i);
	}
}

void UGsUIGuildDiplomacyPage::OnDiplomacyEnemyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	if (UGsUIGuildDiplomacyBase* currPage = GetCurrentPage())
	{
		currPage->OnDiplomacyEnemyStateNotify(InParam);
	}

	UpdateRedDot(EGsDiplomacyTabType::Enemy);
}

void UGsUIGuildDiplomacyPage::OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	if (UGsUIGuildDiplomacyBase* currPage = GetCurrentPage())
	{
		currPage->OnDiplomacyAllyStateNotify(InParam);
	}

	UpdateRedDot(EGsDiplomacyTabType::Ally);
	UpdateRedDot(EGsDiplomacyTabType::AllyState);
}

void UGsUIGuildDiplomacyPage::OnDiplomacyResendList()
{
	if (UGsUIGuildDiplomacyBase* currPage = GetCurrentPage())
	{
		currPage->OnDiplomacyResendList();
	}
}
