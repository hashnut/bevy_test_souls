// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentStorage.h"

#include "Blueprint/SlateBlueprintLibrary.h"

#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

#include "Guild/GsGuildHelper.h"

#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"

#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildDivision.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuction.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildCalculate.h"

#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBegin.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionAppointment.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionUser.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionDirect.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildDivisionComboBox.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Net/GsNetSendServiceGuild.h"


void UGsUIGuildContentStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleSubTab.OnSelectChanged.BindUObject(this, &UGsUIGuildContentStorage::OnSelectTab);
	_toggleSubTab.AddToggleWidgetByParentPanel(_panelRootTab);

	_pageMap.Empty();
	_pageMap.Emplace(EGsStorageTab::DIVISION, _uiPageDivision);
	_pageMap.Emplace(EGsStorageTab::AUCTION, _uiPageAuction); 
	_pageMap.Emplace(EGsStorageTab::CALCULATE, _uiPageCalculate);

	_redDotMap.Empty();
	_redDotMap.Emplace(EGsStorageTab::DIVISION, FGsRedDotChecker(_uiRedDotDivision, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division);
		}));
	_redDotMap.Emplace(EGsStorageTab::AUCTION, FGsRedDotChecker(_uiRedDotAuction, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);
		}));
	_redDotMap.Emplace(EGsStorageTab::CALCULATE, FGsRedDotChecker(_uiRedDotCalculate, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);
		}));

	_uiPageDivision->SetContentStorage(this);
	_uiPageAuction->SetContentStorage(this);
	_uiPageCalculate->SetContentStorage(this);

	// 분배 페이지 하단
	if (_btnDivisionTooltip)
	{
		_btnDivisionTooltip->OnPressed.AddDynamic(this, &UGsUIGuildContentStorage::OnClickTooltipDivision);
	}
	if (_btnAuctionTooltip)
	{
		_btnAuctionTooltip->OnPressed.AddDynamic(this, &UGsUIGuildContentStorage::OnClickTooltipResult);
	}
	if (_btnDivision)
	{
		_btnDivision->OnClicked.AddDynamic(this, &UGsUIGuildContentStorage::OnClickDivision);
	}

	if (_comboBoxDivision)
	{
		_comboBoxDivision->OnChangeComboBox.BindUObject(this, &UGsUIGuildContentStorage::OnChangedComboBox);
		_comboBoxDivision->OnClickComboBox.BindUObject(this, &UGsUIGuildContentStorage::OnClickedComboBoxDivision);
	}
	if (_comboBoxResult)
	{
		_comboBoxResult->OnChangeComboBox.BindUObject(this, &UGsUIGuildContentStorage::OnChangedComboBox);
		_comboBoxResult->OnClickComboBox.BindUObject(this, &UGsUIGuildContentStorage::OnClickedComboBoxResult);
	}

	// 경매 페이지 하단
	if (_btnPayback)
	{
		_btnPayback->OnClicked.AddDynamic(this, &UGsUIGuildContentStorage::OnClickPayback);
	}
}

void UGsUIGuildContentStorage::NativeDestruct()
{
	_prevTab = EGsStorageTab::MAX;

	Super::NativeDestruct();
}

void UGsUIGuildContentStorage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGsUIGuildStorageBase* currPage = GetCurrentPage())
	{
		currPage->InvalidateTick();
	}
}

void UGsUIGuildContentStorage::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
{
	_prevTab = EGsStorageTab::MAX;
	_isStackRecoverFlag = IsStackRecoverFlag;
	_toggleSubTab.SetSelectedIndex(IsStackRecoverFlag ? _lastTab : 0, true);
	_isStackRecoverFlag = false;
}

void UGsUIGuildContentStorage::Hide()
{
	if (UGsUIGuildStorageBase* currPage = GetCurrentPage())
	{
		currPage->Hide();
	}

	_prevTab = EGsStorageTab::MAX;
	OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::GuildStorage));

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("PopupGuildAuctionCalculate"));
	uiManager->CloseByKeyName(TEXT("PopupGuildDivisionJoin"));
	uiManager->CloseByKeyName(TEXT("PopupGuildNormalAuction"));
	uiManager->CloseByKeyName(TEXT("PopupGuildAuction"));
	uiManager->CloseByKeyName(TEXT("PopupGuildAuctionDirect"));
}

void UGsUIGuildContentStorage::OnNotifyJoinMember()
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		_uiPageDivision->OnNotifyJoinMember();
	}
}

void UGsUIGuildContentStorage::OnReceiveRemoveMember()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("PopupGuildAuction"));
	uiManager->CloseByKeyName(TEXT("PopupGuildAuctionDirect"));
}

void UGsUIGuildContentStorage::OnChangeMyAuthority()
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		InvalidateDivisionButton();
	}
}

void UGsUIGuildContentStorage::OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam)
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		InvalidateDivisionButton();
	}
}

void UGsUIGuildContentStorage::OnInvalidateList()
{
	switch (_prevTab)
	{
	case UGsUIGuildContentStorage::DIVISION:
		_uiPageDivision->OnReceiveDivisionList();
		break;
	case UGsUIGuildContentStorage::AUCTION:
		_uiPageAuction->OnReceiveAuctionList();
		break;
	case UGsUIGuildContentStorage::CALCULATE:
		_uiPageCalculate->OnReceiveCalculateList();
		break;
	default:
		break;
	}
}

void UGsUIGuildContentStorage::OnUpdatedDivisionItem()
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		_uiPageDivision->OnUpdatedDivisionItem();
	}
}

void UGsUIGuildContentStorage::OnUpdatedAuctionItem(uint64 InAuctionId)
{
	if (EGsStorageTab::AUCTION == _prevTab)
	{
		_uiPageAuction->OnUpdatedAuctionItem(InAuctionId);
	}
}

void UGsUIGuildContentStorage::OnRemovedAuctionItem(uint64 InAuctionId)
{
	if (EGsStorageTab::AUCTION == _prevTab)
	{
		_uiPageAuction->OnRemovedAuctionItem(InAuctionId);
	}
}

void UGsUIGuildContentStorage::OnChangedDivisionListCheck()
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		_uiPageDivision->OnChangedDivisionListCheck();
	}
}

void UGsUIGuildContentStorage::OnRegisterDivision()
{
	if (EGsStorageTab::DIVISION == _prevTab)
	{
		_uiPageDivision->SendRequestPacket();
	}
}

void UGsUIGuildContentStorage::OnResetAuctionList()
{
	if (EGsStorageTab::AUCTION == _prevTab)
	{
		_uiPageAuction->SendRequestPacket();
	}
}

void UGsUIGuildContentStorage::OnUpdatedAuctionPayback()
{
	if (EGsStorageTab::AUCTION == _prevTab)
	{
		InvalidateAuctionPayback();
	}
}

void UGsUIGuildContentStorage::OnReceiveStorageDetail()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widgetResult = uiManager->GetWidgetByKey(TEXT("PopupGuildAuctionCalculate"));
	if (widgetResult.IsValid())
	{
		if (UGsUIPopupGuildAuctionResult* popup = Cast<UGsUIPopupGuildAuctionResult>(widgetResult.Get()))
		{
			popup->InvalidateList();
		}
	}

	TWeakObjectPtr<UGsUIWidgetBase> widgetUser = uiManager->GetWidgetByKey(TEXT("PopupGuildDivisionJoin"));
	if (widgetUser.IsValid())
	{
		if (UGsUIPopupGuildAuctionUser* popup = Cast<UGsUIPopupGuildAuctionUser>(widgetUser.Get()))
		{
			popup->InvalidateList();
		}
	}
}

void UGsUIGuildContentStorage::InvalidateRedDot()
{
	UpdateRedDot(EGsStorageTab::ALL);
}

void UGsUIGuildContentStorage::OnSelectTab(int32 InIndex)
{
	if (UGsUIGuildStorageBase* const* prevPage = _pageMap.Find(_prevTab))
	{
		(*prevPage)->Hide();
	}
	UpdateRedDot(_prevTab);

	EGsStorageTab selectedTab = static_cast<EGsStorageTab>(InIndex);
	if (UGsUIGuildStorageBase* const* selectedPage = _pageMap.Find(selectedTab))
	{
		_switcherPage->SetActiveWidgetIndex(InIndex);
		_switcherPageBottom->SetActiveWidgetIndex(InIndex);

		(*selectedPage)->Show(_isStackRecoverFlag);
		_prevTab = selectedTab;
		_lastTab = selectedTab;
	}
	UpdateRedDot(_prevTab);

	OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::GuildStorage));
}

void UGsUIGuildContentStorage::UpdateRedDot(EGsStorageTab InTab)
{
	if (EGsStorageTab::ALL == InTab)
	{
		for (TPair<EGsStorageTab, FGsRedDotChecker>& itPair : _redDotMap)
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

UGsUIGuildStorageBase* UGsUIGuildContentStorage::GetCurrentPage() const
{
	EGsStorageTab currTab = static_cast<EGsStorageTab>(_toggleSubTab.GetSelectedIndex());
	if (UGsUIGuildStorageBase* const* page = _pageMap.Find(currTab))
	{
		return *page;
	}

	return nullptr;
}

void UGsUIGuildContentStorage::OnShowDivision()
{
	/*하단 버튼 갱신*/
	_comboBoxDivision->SetDivisionType();
	_comboBoxResult->SetResultType();
	_comboBoxResult->SetIsEnabled(_comboBoxDivision->GetDivisionType() != GuildAuctionType::DIRECT_DISTRIBUTION);

	/*분배 버튼 갱신*/
	InvalidateDivisionButton();

	_uiTooltip->Close();
}

void UGsUIGuildContentStorage::OnShowAuction()
{
	InvalidateAuctionCommission();
	InvalidateAuctionPayback();
}

void UGsUIGuildContentStorage::OnHideDivision()
{
	_comboBoxDivision->Hide();
	_comboBoxResult->Hide();
	_uiTooltip->Close();
}

void UGsUIGuildContentStorage::OnHideAuction()
{
}

void UGsUIGuildContentStorage::InvalidateDivisionCount(int32 InCount, int32 InMaxCount)
{
	FString strRegist = FString::Printf(TEXT("%d/%d"), InCount, InMaxCount);
	_textBlockDivisionCount->SetText(FText::FromString(strRegist));
}

void UGsUIGuildContentStorage::InvalidateDivisionButton()
{
	bool canStartDivision = false;
	do
	{
		FGsGuildManager* guildManager = GGuild();
		if (nullptr == guildManager)
			break;

		FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
		if (nullptr == auctionData)
			break;

		uint32 myAuthority = guildManager->GetAuthority();
		if (false == FGsGuildHelper::CanDivision(myAuthority))
			break;

		canStartDivision = auctionData->HasCheckedItem();

	} while (0);

	const int32 CAN_START = 0;
	const int32 CANNOT_START = 1;
	_switcherDivision->SetActiveWidgetIndex(canStartDivision ? CAN_START : CANNOT_START);
}

void UGsUIGuildContentStorage::InvalidateAuctionCount(int32 InCount, int32 InMaxCount)
{
	FString strRegister = FString::Printf(TEXT("%d/%d"), InCount, InMaxCount);
	_textBlockAuctionCount->SetText(FText::FromString(strRegister));
}

void UGsUIGuildContentStorage::InvalidateAuctionCommission()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	/* 경매 수수료 설정 */
	FText textCommission;
	auctionData->GetCommissionRateText(textCommission);
	_textBlockCommission->SetText(textCommission);
}

void UGsUIGuildContentStorage::InvalidateAuctionPayback()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	/* 회수금 영역 초기화 */
	_btnPayback->SetIsEnabled(auctionData->GetRemainPayback() > 0);
	_textBlockPayback->SetText(FText::AsNumber(auctionData->GetRemainPayback()));
}

void UGsUIGuildContentStorage::OnClickTooltipDivision()
{
	if (_uiTooltip->IsOpen() && _uiTooltip->GetRenderOpacity() == 1.0f)
	{
		_uiTooltip->Close();
		return;
	}

	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Tooltip_Info_Division"), findText);
	_uiTooltip->SetDescText(findText);
	_uiTooltip->SetLifeTime(_toolTipLifetime);
	_uiTooltip->Open();
	_uiTooltip->SetRenderOpacity(0.0f);
	_uiTooltip->SetIsNeedRefresh(true);
	if (_uiTooltip->OnInvalidatePosition.IsBound())
	{
		_uiTooltip->OnInvalidatePosition.Unbind();
	}
	_uiTooltip->OnInvalidatePosition.BindUObject(this, &UGsUIGuildContentStorage::SetTooltipPositionDivision);
}

void UGsUIGuildContentStorage::OnClickTooltipResult()
{
	if (_uiTooltip->IsOpen() && _uiTooltip->GetRenderOpacity() == 1.0f)
	{
		_uiTooltip->Close();
		return;
	}

	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Tooltip_info_calculate"), findText);
	_uiTooltip->SetDescText(findText);
	_uiTooltip->SetLifeTime(_toolTipLifetime);
	_uiTooltip->Open();
	_uiTooltip->SetRenderOpacity(0.0f);
	_uiTooltip->SetIsNeedRefresh(true);
	if (_uiTooltip->OnInvalidatePosition.IsBound())
	{
		_uiTooltip->OnInvalidatePosition.Unbind();
	}
	_uiTooltip->OnInvalidatePosition.BindUObject(this, &UGsUIGuildContentStorage::SetTooltipPositionResult);
}

void UGsUIGuildContentStorage::OnClickDivision()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	bool canStartDivision = false;
	do
	{
		uint32 myAuthority = guildManager->GetAuthority();
		if (false == FGsGuildHelper::CanDivision(myAuthority))
		{
			// 기사단 분배 권한이 없습니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Authority_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		}

		if (false == auctionData->HasCheckedItem())
		{
			// 선택된 획득 내역이 없어 분배를 시작할 수 없습니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_NoCheck_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		}

		canStartDivision = true;

	} while (0);

	if (false == canStartDivision)
		return;

	GuildAuctionType currentDivisionType = auctionData->GetCurrentDivisionType();
	GuildAuctionResultType currentResultType = auctionData->GetCurrentResultType();
	switch (currentDivisionType)
	{
	case GuildAuctionType::NORMAL:
	case GuildAuctionType::PARTICIPANTS:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildNormalAuction"));
		if (widget.IsValid())
		{
			if (UGsUIPopupGuildAuctionBegin* popup = Cast<UGsUIPopupGuildAuctionBegin>(widget.Get()))
			{
				TArray<ItemDBId> checkedItemList = auctionData->GetCheckedDivisionItemList();
				popup->SetData(checkedItemList, currentDivisionType, currentResultType);
			}
		}
		break;
	}
	case GuildAuctionType::APPOINTMENT:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildAuction"));
		if (widget.IsValid())
		{
			if (UGsUIPopupGuildAuctionAppointment* popup = Cast<UGsUIPopupGuildAuctionAppointment>(widget.Get()))
			{
				TArray<ItemDBId> checkedItemList = auctionData->GetCheckedDivisionItemList();
				popup->SetData(checkedItemList, currentResultType);
			}
		}
		break;
	}
	case GuildAuctionType::DIRECT_DISTRIBUTION:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildAuctionDirect"));
		if (widget.IsValid())
		{
			if (UGsUIPopupGuildAuctionDirect* popup = Cast<UGsUIPopupGuildAuctionDirect>(widget.Get()))
			{
				TArray<ItemDBId> checkedItemList = auctionData->GetCheckedDivisionItemList();
				popup->SetData(checkedItemList);
			}
		}
		break;
	}
	default:
		break;
	}
}

void UGsUIGuildContentStorage::OnChangedComboBox()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	auctionData->SetCurrentDivisionType(_comboBoxDivision->GetDivisionType());
	auctionData->SetCurrentResultType(_comboBoxResult->GetResultType());

	_comboBoxResult->SetIsEnabled(_comboBoxDivision->GetDivisionType() != GuildAuctionType::DIRECT_DISTRIBUTION);

	_uiPageDivision->OnChangedComboBox();
}

void UGsUIGuildContentStorage::OnClickedComboBoxDivision()
{
	_comboBoxResult->Hide();
}

void UGsUIGuildContentStorage::OnClickedComboBoxResult()
{
	_comboBoxDivision->Hide();
}

void UGsUIGuildContentStorage::SetTooltipPositionDivision()
{
	SetTooltipPosition(true);
}

void UGsUIGuildContentStorage::SetTooltipPositionResult()
{
	SetTooltipPosition(false);
}

void UGsUIGuildContentStorage::SetTooltipPosition(bool InIsDivision)
{
	_uiTooltip->SetRenderOpacity(1.0f);
	UWidget* targetWidget = InIsDivision ? _comboBoxDivision : _comboBoxResult;
	FWidgetTransform panelTransform = _uiTooltip->RenderTransform;
	if (targetWidget)
	{
		FVector2D targetTopLeftPixel;
		FVector2D targetTopLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), targetWidget->GetCachedGeometry(), FVector2D::ZeroVector, targetTopLeftPixel, targetTopLeftViewport);

		FVector2D targetTopRightViewport = targetTopLeftViewport;
		targetTopRightViewport.X += targetWidget->GetCachedGeometry().GetLocalSize().X;

		FVector2D topLeftPixel;
		FVector2D topLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), _uiTooltip->GetCachedGeometry(), FVector2D::ZeroVector, topLeftPixel, topLeftViewport);

		// UserInfoAnchor::TOP_RIGHT_UP
		panelTransform.Translation += -topLeftViewport + targetTopRightViewport - _uiTooltip->GetCachedGeometry().GetLocalSize();

		_uiTooltip->SetRenderTransform(panelTransform);
	}
}

void UGsUIGuildContentStorage::OnClickPayback()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	if (auctionData->GetRemainPayback() <= 0)
		return;

	FGsNetSendServiceGuild::SendReqGuildAuctionReturnCurrency();
}
