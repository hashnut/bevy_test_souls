// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentOperation.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBase.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBuilding.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBuff.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationSanctum.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"


void UGsUIGuildContentOperation::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleSubTab.OnSelectChanged.BindUObject(this, &UGsUIGuildContentOperation::OnSelectTab);
	_toggleSubTab.AddToggleWidgetByParentPanel(_panelRootTab);

	_pageMap.Empty();
	_pageMap.Emplace(EGsOperationTab::BUILDING, _uiPageBuilding);
	_pageMap.Emplace(EGsOperationTab::SANCTUM, _uiPageSanctum);
	_pageMap.Emplace(EGsOperationTab::BUFF, _uiPageBuff);

	_redDotMap.Empty();
	_redDotMap.Emplace(EGsOperationTab::BUILDING, FGsRedDotChecker(_uiRedDotBuilding, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buildings);
		}));
	_redDotMap.Emplace(EGsOperationTab::SANCTUM, FGsRedDotChecker(_uiRedDotSanctum, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Sanctum);
		}));
	_redDotMap.Emplace(EGsOperationTab::BUFF, FGsRedDotChecker(_uiRedDotBuff, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buffs);
		}));
}

void UGsUIGuildContentOperation::NativeDestruct()
{
	_prevTab = EGsOperationTab::MAX;

	Super::NativeDestruct();
}

void UGsUIGuildContentOperation::SendRequestPacket()
{
	// 빌딩, 버프 모두 건물 정보로 처리
	GGuild()->GetSendController().SendRequestBuildingList();
}

void UGsUIGuildContentOperation::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	_bIsMyGuild = bIsMyGuild;

	_toggleSubTab.SetSelectedIndex(0, true);

	if (_bIsMyGuild)
	{
		_sBtnBuff->SetVisibility(ESlateVisibility::Visible);
		_sBtnSanctum->SetVisibility(ESlateVisibility::Visible);

		UpdateRedDot(EGsOperationTab::ALL);
	}
	else
	{
		_sBtnBuff->SetVisibility(ESlateVisibility::Collapsed);
		_sBtnSanctum->SetVisibility(ESlateVisibility::Collapsed);

		_uiRedDotBuilding->SetRedDot(false, EGsIconRedDotType::NORMAL);
		_uiRedDotBuff->SetRedDot(false, EGsIconRedDotType::NORMAL);
	}
}

void UGsUIGuildContentOperation::Hide()
{
	// 레드닷에 영향을 주는 요소 저장
	if (_bIsMyGuild)
	{
		if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
		{
			currPage->Hide();
		}
	}

	// 페이지
	_prevTab = EGsOperationTab::MAX;
}

void UGsUIGuildContentOperation::OnChangeCurrency(CurrencyType InType)
{
	if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
	{
		currPage->OnChangeCurrency(InType);
	}
}

void UGsUIGuildContentOperation::OnChangeGuildLevel()
{
	if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
	{
		currPage->OnChangeGuildLevel();
	}
}

void UGsUIGuildContentOperation::OnUpdateBuilding(GuildBuildingId InBuildingId)
{
	if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
	{
		currPage->OnUpdateBuilding(InBuildingId);
	}
}

void UGsUIGuildContentOperation::OnChangeMyAuthority()
{
	if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
	{
		currPage->OnChangeMyAuthority();
	}
}

void UGsUIGuildContentOperation::OnReceiveBuildingList()
{
	if (UGsUIGuildOperationBase* currPage = GetCurrentPage())
	{
		currPage->OnReceiveBuildingList();
	}
}

void UGsUIGuildContentOperation::OnCampConstruction()
{
	//// 건물 리스트 다시 요청
	//if (FGsGuildManager* guildMgr = GGuild())
	//{
	//	guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::BuildingList);
	//	guildMgr->GetSendController().SendRequestBuildingList();
	//}
}

void UGsUIGuildContentOperation::OnCampDestruction()
{
	//if (EGsOperationTab::CAMP == _toggleSubTab.GetSelectedIndex())
	//{
	//	_uiPageCamp->OnCampDestruction();
	//}
}

void UGsUIGuildContentOperation::OnSanctumConstructionStateChanged()
{
	// 갱신을 위해 정보를 다시 요청함(BuildingList 패킷쪽에 성소정보가 같이와서 이렇게 받는다)
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::BuildingList);
		guildMgr->GetSendController().SendRequestBuildingList();
	}
}

void UGsUIGuildContentOperation::OnSanctumDestruction()
{
	if (EGsOperationTab::SANCTUM == _toggleSubTab.GetSelectedIndex())
	{
		_uiPageSanctum->OnNexusDestruction();
	}
}

void UGsUIGuildContentOperation::OnSelectTab(int32 InIndex)
{	
	if (UGsUIGuildOperationBase*const* prevPage = _pageMap.Find(_prevTab))
	{
		(*prevPage)->Hide();
	}

	UpdateRedDot(_prevTab);
	OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::Operation));

	EGsOperationTab selectedTab = static_cast<EGsOperationTab>(InIndex);
	if (UGsUIGuildOperationBase* const* selectedPage = _pageMap.Find(selectedTab))
	{
		_switcherPage->SetActiveWidgetIndex(InIndex);

		(*selectedPage)->Show(_bIsMyGuild);
		_prevTab = selectedTab;
	}
}

void UGsUIGuildContentOperation::UpdateRedDot(EGsOperationTab InTab)
{
	if (EGsOperationTab::ALL == InTab)
	{
		for (TPair<EGsOperationTab, FGsRedDotChecker>& itPair : _redDotMap)
		{
			itPair.Value.UpdateRedDot();
		}
	}
	else
	{
		if (FGsRedDotChecker* redDot = _redDotMap.Find(InTab))
		{
			redDot->UpdateRedDot();
		}
	}
}

UGsUIGuildOperationBase* UGsUIGuildContentOperation::GetCurrentPage() const
{
	EGsOperationTab currTab = static_cast<EGsOperationTab>(_toggleSubTab.GetSelectedIndex());
	if (UGsUIGuildOperationBase*const* page = _pageMap.Find(currTab))
	{
		return *page;
	}

	return nullptr;
}
