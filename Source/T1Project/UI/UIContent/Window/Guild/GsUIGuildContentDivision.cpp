//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildContentDivision.h"
//
//#include "Components/WidgetSwitcher.h"
//#include "Components/ListView.h"
//#include "Components/TextBlock.h"
//#include "Components/PanelWidget.h"
//
//#include "Guild/GsGuildAuctionData.h"
//#include "Guild/GsGuildHelper.h"
//
//#include "Management/GsMessageHolder.h"
//
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//
//#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBegin.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionAppointment.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionUser.h"
//#include "UI/UIContent/Helper/GsUIHelper.h"
//#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
//#include "UI/UIContent/Window/Guild/Auction/GsGuildDivisionListItem.h"
//#include "UI/UIContent/Window/Guild/Auction/GsUIGuildDivisionListEntry.h"
//#include "UI/UIContent/Window/Guild/Auction/GsUIGuildDivisionComboBox.h"
//
//#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
//#include "UI/UIControlLib/ContentWidget/GsButton.h"
//
//#include "UTIL/GsTimeSyncUtil.h"
//
//#include "Blueprint/SlateBlueprintLibrary.h"
//
//
//void UGsUIGuildContentDivision::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	if (_btnCheckAll)
//	{
//		_btnCheckAll->OnClicked.AddDynamic(this, &UGsUIGuildContentDivision::OnClickCheckAll);
//	}
//	if (_btnDivisionTooltip)
//	{
//		_btnDivisionTooltip->OnClicked.AddDynamic(this, &UGsUIGuildContentDivision::OnClickTooltipDivision);
//	}
//	if (_btnAuctionTooltip)
//	{
//		_btnAuctionTooltip->OnClicked.AddDynamic(this, &UGsUIGuildContentDivision::OnClickTooltipResult);
//	}
//	if (_btnStart)
//	{
//		_btnStart->OnClicked.AddDynamic(this, &UGsUIGuildContentDivision::OnClickStart);
//	}
//
//	if (_comboBoxDivision)
//	{
//		_comboBoxDivision->OnChangeComboBox.BindUObject(this, &UGsUIGuildContentDivision::OnChangedComboBox);
//		_comboBoxDivision->OnClickComboBox.BindUObject(this, &UGsUIGuildContentDivision::OnClickedComboBoxDivision);
//	}
//	if (_comboBoxResult)
//	{
//		_comboBoxResult->OnChangeComboBox.BindUObject(this, &UGsUIGuildContentDivision::OnChangedComboBox);
//		_comboBoxResult->OnClickComboBox.BindUObject(this, &UGsUIGuildContentDivision::OnClickedComboBoxResult);
//	}
//}
//
//void UGsUIGuildContentDivision::SendRequestPacket()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	guildManager->GetSendController().SendRequestMemberList();
//
//	auctionData->SendReqDivisionList();
//}
//
//void UGsUIGuildContentDivision::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	/*데이터 초기화*/
//	_currentDivisionType = GuildAuctionType::NORMAL;
//	_currentResultType = GuildAuctionResultType::NORMAL;
//	auctionData->SetCurrentDivisionType(GuildAuctionType::NORMAL);
//	auctionData->RemoveCheckedItemAll();
//
//	/*스크롤 감추기 및 상단 이동*/
//	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
//	_listView->ScrollToTop();
//
//	/*전체 선택/해제 체크 버크 갱신*/
//	InvalidateCheckAll();
//
//	/*목록 갱신*/
//	InvalidateList();
//
//	/*하단 버튼 갱신*/
//	_comboBoxDivision->SetDivisionType();
//	_comboBoxResult->SetResultType();
//
//	/*분배 버튼 갱신*/
//	InvalidateStart();
//
//	_uiTooltip->Close();
//}
//
//void UGsUIGuildContentDivision::Hide()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	_currentDivisionType = GuildAuctionType::NORMAL;
//	_currentResultType = GuildAuctionResultType::NORMAL;
//	auctionData->SetCurrentDivisionType(GuildAuctionType::NORMAL);
//	auctionData->RemoveCheckedItemAll();
//
//	_comboBoxDivision->Hide();
//	_comboBoxResult->Hide();
//
//	_uiTooltip->Close();
//
//	//OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::Division));
//}
//
//void UGsUIGuildContentDivision::OnChangeMyAuthority()
//{
//	InvalidateStart();
//}
//
//void UGsUIGuildContentDivision::OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam)
//{
//	InvalidateStart();
//}
//
//void UGsUIGuildContentDivision::OnNotifyJoinMember()
//{
//	// 해당 멤버의 정보를 얻기 위해 리스트 요청
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	guildManager->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);
//	guildManager->GetSendController().SendRequestMemberList();
//}
//
//void UGsUIGuildContentDivision::OnReceiveDivisionList()
//{
//	InvalidateList();
//}
//
//void UGsUIGuildContentDivision::OnChangedDivisionListCheck()
//{
//	InvalidateCheckAll();
//	InvalidateListEntry();
//	InvalidateStart();
//}
//
//void UGsUIGuildContentDivision::OnUpdatedDivisionData()
//{
//	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
//	for (int32 i = 0; i < entries.Num(); ++i)
//	{
//		UGsUIGuildDivisionListEntry* entry = Cast<UGsUIGuildDivisionListEntry>(entries[i]);
//		if (nullptr == entry)
//			continue;
//
//		entry->InvalidateAll();
//	}
//}
//
//void UGsUIGuildContentDivision::OnReceiveDivisionDetail()
//{
//	UGsUIManager* uiManager = GUI();
//	if (nullptr == uiManager)
//		return;
//
//	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupGuildAuctionCalculate"));
//	if (widget.IsValid())
//	{
//		if (UGsUIPopupGuildAuctionResult* popup = Cast<UGsUIPopupGuildAuctionResult>(widget.Get()))
//		{
//			popup->InvalidateList();
//		}
//	}
//
//	widget = uiManager->GetWidgetByKey(TEXT("PopupGuildDivisionJoin"));
//	if (widget.IsValid())
//	{
//		if (UGsUIPopupGuildAuctionUser* popup = Cast<UGsUIPopupGuildAuctionUser>(widget.Get()))
//		{
//			popup->InvalidateList();
//		}
//	}
//}
//
//void UGsUIGuildContentDivision::OnRegisterDivision()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	auctionData->SendReqDivisionList();
//}
//
//void UGsUIGuildContentDivision::InvalidateItem(uint64 InItemDBId)
//{
//	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
//	for (int32 i = 0; i < entries.Num(); ++i)
//	{
//		UGsUIGuildDivisionListEntry* entry = Cast<UGsUIGuildDivisionListEntry>(entries[i]);
//		if (entry->GetItemDBId() == InItemDBId)
//		{
//			entry->InvalidateAll();
//		}
//	}
//}
//
//void UGsUIGuildContentDivision::OnChangedComboBox()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	auctionData->SetCurrentDivisionType(_comboBoxDivision->GetDivisionType());
//	_currentDivisionType = _comboBoxDivision->GetDivisionType();
//	_currentResultType = _comboBoxResult->GetResultType();
//
//	InvalidateCheckAll();
//	InvalidateList();
//	InvalidateStart();
//}
//
//void UGsUIGuildContentDivision::OnClickedComboBoxDivision()
//{
//	_comboBoxResult->Hide();
//}
//
//void UGsUIGuildContentDivision::OnClickedComboBoxResult()
//{
//	_comboBoxDivision->Hide();
//}
//
//void UGsUIGuildContentDivision::InvalidateCheckAll()
//{
//	// 지정 경매일 경우 일괄 체크 기능을 사용할 수 없다. (딤드)
//	if (_currentDivisionType == GuildAuctionType::APPOINTMENT)
//	{
//		_btnCheckAll->SetIsEnabled(false);
//		_btnCheckAll->SetIsSelected(false);
//	}
//	else
//	{
//		FGsGuildManager* guildManager = GGuild();
//		if (nullptr == guildManager)
//			return;
//
//		FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//		if (nullptr == auctionData)
//			return;
//
//		_btnCheckAll->SetIsEnabled(true);
//		_btnCheckAll->SetIsSelected(auctionData->IsCheckedAllItem());
//	}
//}
//
//void UGsUIGuildContentDivision::InvalidateList()
//{
//	// 분배 목록 정보를 받기 전에는 빈 목록이 보여진다.
//	// 분배 목록 정보를 다 받았을 때부터 목룍이 표시된다.- OnReceiveDivisionList
//
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//	
//	const int32 DIVISION_EXIST = 0;
//	const int32 DIVISION_NONE = 1;
//
//	TArray<UGsGuildDivisionListItem*> divisionList;
//
//	const TArray<FGsGuildDivisionItem>& divisionDataList = auctionData->GetDivisionList();
//	for (int32 i = 0; i < divisionDataList.Num(); ++i)
//	{
//		UGsGuildDivisionListItem* listItem = NewObject<UGsGuildDivisionListItem>(this);
//		listItem->SetData(&divisionDataList[i]);
//		divisionList.Emplace(listItem);
//	}
//
//	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
//	divisionList.Sort([currentTime](const UGsGuildDivisionListItem& lhs, const UGsGuildDivisionListItem& rhs)
//		{
//			// 분배 상태가 다르다면 - 보관중이거나 경매 진행중인 슬롯을 상단 출력
//			if (lhs._divisionItem->GetDivisionState() != rhs._divisionItem->GetDivisionState())
//			{
//				if (lhs._divisionItem->GetDivisionState() == GuildDivisionState::STORAGE)
//				{
//					return true;
//				}
//				if (rhs._divisionItem->GetDivisionState() == GuildDivisionState::STORAGE)
//				{
//					return false;
//				}
//				if (lhs._divisionItem->GetDivisionState() == GuildDivisionState::AUCTION)
//				{
//					return true;
//				}
//				if (rhs._divisionItem->GetDivisionState() == GuildDivisionState::AUCTION)
//				{
//					return false;
//				}
//			}
//
//			if (lhs._divisionItem->GetAcquisitionDateTime() == rhs._divisionItem->GetAcquisitionDateTime())
//			{
//				return lhs._divisionItem->GetItemData().mItemDBId < rhs._divisionItem->GetItemData().mItemDBId;
//			}
//
//			// 나중에 획득한 아이템을 상단 출력
//			return lhs._divisionItem->GetAcquisitionDateTime() > rhs._divisionItem->GetAcquisitionDateTime();
//		});
//
//	// 획득 내역이 존재한다.
//	if (divisionList.Num() > 0)
//	{
//		_switcherList->SetActiveWidgetIndex(DIVISION_EXIST);
//		_listView->SetListItems(divisionList);
//	}
//	// 획득 내역이 존재하지 않는다.
//	else
//	{
//		_switcherList->SetActiveWidgetIndex(DIVISION_NONE);
//		_listView->ClearListItems();
//	}
//
//	// 획득 내역 수
//	FString strRegist = FString::Printf(TEXT("%d/%d"), divisionList.Num(), auctionData->GetMaxDivisionCount());
//	_textBlockRegister->SetText(FText::FromString(strRegist));
//}
//
//void UGsUIGuildContentDivision::InvalidateListEntry()
//{
//	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
//	for (int32 i = 0; i < entries.Num(); ++i)
//	{
//		UGsUIGuildDivisionListEntry* entry = Cast<UGsUIGuildDivisionListEntry>(entries[i]);
//		entry->InvalidateCheck();
//	}
//}
//
//void UGsUIGuildContentDivision::InvalidateStart()
//{
//	const int32 CAN_START = 0;
//	const int32 CANNOT_START = 1;
//
//	bool canStartDivision = false;
//	do 
//	{
//		FGsGuildManager* guildManager = GGuild();
//		if (nullptr == guildManager)
//			break;
//
//		FGsGuildAuctionData * auctionData = guildManager->GetMyGuildAuctionData();
//		if (nullptr == auctionData)
//			break;
//
//		uint32 myAuthority = guildManager->GetAuthority();
//		if (false == FGsGuildHelper::CanDivision(myAuthority))
//			break;
//
//		canStartDivision = auctionData->HasCheckedItem();
//
//	} while (0);
//
//	_switcherStart->SetActiveWidgetIndex(canStartDivision ? CAN_START : CANNOT_START);
//}
//
//void UGsUIGuildContentDivision::OnClickCheckAll()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	// 지정 경매라면 다중 선택 불가능 & 일괄체크 비활성화 및
//	if (_currentDivisionType == GuildAuctionType::APPOINTMENT)
//		return;
//
//	if (_btnCheckAll->GetIsSelected())
//	{
//		auctionData->RemoveCheckedItemAll();
//	}
//	else
//	{
//		auctionData->AddCheckedItemAll();
//	}
//	
//	InvalidateCheckAll();
//	InvalidateListEntry();
//	InvalidateStart();
//}
//
//void UGsUIGuildContentDivision::OnClickTooltipDivision()
//{
//	if (_uiTooltip->IsOpen() && _uiTooltip->GetRenderOpacity() == 1.0f)
//	{
//		_uiTooltip->Close();
//		return;
//	}
//
//	FText findText;
//	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Tooltip_Info_Division"), findText);
//	_uiTooltip->SetDescText(findText);
//	_uiTooltip->SetLifeTime(_toolTipLifetime);
//	_uiTooltip->Open();
//	_uiTooltip->SetRenderOpacity(0.0f);
//	_uiTooltip->SetIsNeedRefresh(true);
//	if (_uiTooltip->OnInvalidatePosition.IsBound())
//	{
//		_uiTooltip->OnInvalidatePosition.Unbind();
//	}
//	_uiTooltip->OnInvalidatePosition.BindUObject(this, &UGsUIGuildContentDivision::SetTooltipPositionDivision);
//}
//
//void UGsUIGuildContentDivision::OnClickTooltipResult()
//{
//	if (_uiTooltip->IsOpen() && _uiTooltip->GetRenderOpacity() == 1.0f)
//	{
//		_uiTooltip->Close();
//		return;
//	}
//
//	FText findText;
//	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Tooltip_info_calculate"), findText);
//	_uiTooltip->SetDescText(findText);
//	_uiTooltip->SetLifeTime(_toolTipLifetime);
//	_uiTooltip->Open();
//	_uiTooltip->SetRenderOpacity(0.0f);
//	_uiTooltip->SetIsNeedRefresh(true);
//	if (_uiTooltip->OnInvalidatePosition.IsBound())
//	{
//		_uiTooltip->OnInvalidatePosition.Unbind();
//	}
//	_uiTooltip->OnInvalidatePosition.BindUObject(this, &UGsUIGuildContentDivision::SetTooltipPositionResult);
//}
//
//void UGsUIGuildContentDivision::SetTooltipPositionDivision()
//{
//	SetTooltipPosition(true);
//}
//
//void UGsUIGuildContentDivision::SetTooltipPositionResult()
//{
//	SetTooltipPosition(false);
//}
//
//void UGsUIGuildContentDivision::SetTooltipPosition(bool InIsDivision)
//{
//	_uiTooltip->SetRenderOpacity(1.0f);
//	UWidget* targetWidget = InIsDivision ? _comboBoxDivision : _comboBoxResult;
//	FWidgetTransform panelTransform = _uiTooltip->RenderTransform;
//	if (targetWidget)
//	{
//		FVector2D targetTopLeftPixel;
//		FVector2D targetTopLeftViewport;
//		USlateBlueprintLibrary::LocalToViewport(GetParent(), targetWidget->GetCachedGeometry(), FVector2D::ZeroVector, targetTopLeftPixel, targetTopLeftViewport);
//
//		FVector2D targetTopRightViewport = targetTopLeftViewport;
//		targetTopRightViewport.X += targetWidget->GetCachedGeometry().GetLocalSize().X;
//
//		FVector2D topLeftPixel;
//		FVector2D topLeftViewport;
//		USlateBlueprintLibrary::LocalToViewport(GetParent(), _uiTooltip->GetCachedGeometry(), FVector2D::ZeroVector, topLeftPixel, topLeftViewport);
//
//		// UserInfoAnchor::TOP_RIGHT_UP
//		panelTransform.Translation += -topLeftViewport + targetTopRightViewport - _uiTooltip->GetCachedGeometry().GetLocalSize();
//
//		_uiTooltip->SetRenderTransform(panelTransform);
//	}
//}
//
//void UGsUIGuildContentDivision::OnClickStart()
//{
//	UGsUIManager* uiManager = GUI();
//	if (nullptr == uiManager)
//		return;
//
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	bool canStartDivision = false;
//	do
//	{
//		uint32 myAuthority = guildManager->GetAuthority();
//		if (false == FGsGuildHelper::CanDivision(myAuthority))
//		{
//			// 기사단 분배 권한이 없습니다.
//			FText errorText;
//			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Authority_Notify"), errorText);
//			FGsUIHelper::TrayMessageTicker(errorText);
//			break;
//		}
//			
//		if (false == auctionData->HasCheckedItem())
//		{
//			// 선택된 획득 내역이 없어 분배를 시작할 수 없습니다.
//			FText errorText;
//			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_NoCheck_Notify"), errorText);
//			FGsUIHelper::TrayMessageTicker(errorText);
//			break;
//		}
//
//		canStartDivision = true;
//
//	} while (0);
//
//	if (false == canStartDivision)
//		return;
//
//	switch (_currentDivisionType)
//	{
//	case GuildAuctionType::NORMAL:
//	case GuildAuctionType::PARTICIPANTS:
//	{
//		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildNormalAuction"));
//		if (widget.IsValid())
//		{
//			if (UGsUIPopupGuildAuctionBegin* popup = Cast<UGsUIPopupGuildAuctionBegin>(widget.Get()))
//			{
//				TArray<const FGsGuildDivisionItem*> checkedItemList = auctionData->GetCheckedDivisionItemList();
//				popup->SetData(checkedItemList , _currentDivisionType, _currentResultType);
//			}
//		}
//		break;
//	}
//	case GuildAuctionType::APPOINTMENT:
//	{
//		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildAuction"));
//		if (widget.IsValid())
//		{
//			if (UGsUIPopupGuildAuctionAppointment* popup = Cast<UGsUIPopupGuildAuctionAppointment>(widget.Get()))
//			{
//				TArray<const FGsGuildDivisionItem*> checkedItemList = auctionData->GetCheckedDivisionItemList();
//				if (checkedItemList.IsValidIndex(0))
//				{
//					popup->SetData(checkedItemList[0], _currentResultType);
//				}
//			}
//		}
//		break;
//	}
//	default:
//		break;
//	}
//}