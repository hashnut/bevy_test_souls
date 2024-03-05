// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentManage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "DataSchema/Guild/GsSchemaGuildSnsShortcutUrl.h"
#include "DataSchema/Guild/GsSchemaGuildSnsShortcutPlatform.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeEmblem.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeMessage.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeSnsUrl.h"
#include "UI/UIContent/Popup/GsUIPopupCommonChangeNumber.h"
#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Data/GsDataContainManager.h"
#include "PaperSprite.h"
#include "Runtime/UMG/Public/Components/ListView.h"
#include "UTIL/GsText.h"
#include "Sns/GsUIGuildSnsUrlListEntry.h"
#include "T1Project.h"

void UGsUIGuildContentManage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupJoinType.AddToggleWidgetByParentPanel(_panelJoinType);
	_toggleGroupJoinType.OnSelectChanged.BindUObject(this, &UGsUIGuildContentManage::OnSelectJoinType);
	_toggleGroupJoinType.SetSelectedIndex(0);

	_btnLimitLevel->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickLimitLevel);
	_btnAuthority->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickAuthority);
	_btnApproval->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickApproval);
	_btnDisband->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickDisband);
	_btnLeave->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickLeave);
	_btnIntroduce->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickIntroduce);
	_btnNotice->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickNotice);
	_btnEmblem->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickEmblem);
	_btnAcquisition->OnClicked.AddDynamic(this, &UGsUIGuildContentManage::OnClickAcquisition);
}

void UGsUIGuildContentManage::NativeDestruct()
{
	_bClickedApproval = false;

	Super::NativeDestruct();
}

void UGsUIGuildContentManage::SendRequestPacket()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		// 승인 권한이 있는 유저일때만 진행
		uint32 myAuthority = guildMgr->GetAuthority();
		if (FGsGuildHelper::CanManageMember(myAuthority))
		{
			// 요청을 취소하는 사람의 경우 따로 노티가 오지 않아 레드닷에 카운트 동기화를 위해 보냄
			guildMgr->GetSendController().SendRequestJoinWaitingUserList();
		}
	}
}

void UGsUIGuildContentManage::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
{
	// null일 수 없는 데이터라 null체크 안함
	_guildData = GGuild()->GetMyGuildData();
	if (nullptr == _guildData)
	{
		return;
	}

	SetUIGuildEmblem(_guildData->_emblemId);

	_textBlockGuildName->SetText(FText::FromString(_guildData->_guildName));
	_textBlockLimitLevel->SetText(FText::AsNumber(_guildData->_joinLimitLevel));

	_toggleGroupJoinType.SetSelectedIndex(static_cast<int32>(_guildData->_joinType));

	auto thiz = MakeWeakObjectPtr(this);
	FGsTextUtil::ChangeBanSentence(_guildData->GetIntroduction(), [thiz](bool success, FString inText, FString outText) 
		{
			if (thiz.IsValid())
			{
				thiz->SetUIGuildIntroduce(outText);
			}
		});

	FGsTextUtil::ChangeBanSentence(_guildData->GetNotice(), [thiz](bool success, FString inText, FString outText) 
		{
			if (thiz.IsValid())
			{
				thiz->SetUIGuildNotice(outText);
			}
		});

	InvalidateAuthority();
	InvalidateAcquisition();
	RequestInvalidateSnsLinks();

	_scrollNotice->ScrollToStart();
}

void UGsUIGuildContentManage::Hide()
{
	_guildData = nullptr;
	_bClickedApproval = false;
}

void UGsUIGuildContentManage::OnChangeMyAuthority()
{
	InvalidateAuthority();
}

void UGsUIGuildContentManage::OnReceiveChangeGuildSetting(const FGsGuildMsgParamSetting* InParam)
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (nullptr == guildData)
	{
		return;
	}

	switch (InParam->_settingType)
	{
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Introduction:
	{
		auto thiz = MakeWeakObjectPtr(this);
		FGsTextUtil::ChangeBanSentence(guildData->GetIntroduction(), [thiz](bool success, FString inText, FString outText)
			{
				if (thiz.IsValid())
				{
					thiz->SetUIGuildIntroduce(outText);
				}
			});
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Notice:
	{
		auto thiz = MakeWeakObjectPtr(this);
		FGsTextUtil::ChangeBanSentence(guildData->GetNotice(), [thiz](bool success, FString inText, FString outText) 
			{
				if (thiz.IsValid())
				{
					thiz->SetUIGuildNotice(outText);
				}
			});
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::LimitLevel:
	{
		_textBlockLimitLevel->SetText(FText::AsNumber(_guildData->_joinLimitLevel));
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::JoinType:
	{
		_toggleGroupJoinType.SetSelectedIndex(static_cast<int32>(_guildData->_joinType));
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Emblem:
	{
		SetUIGuildEmblem(_guildData->_emblemId);
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Acquisition:
	{
		InvalidateAcquisition();
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::SNSLink:
	{
		RequestInvalidateSnsLinks();
	}
	break;
	}
}

void UGsUIGuildContentManage::OnNotifyChangeNotice()
{
	if (const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData())
	{
		auto thiz = MakeWeakObjectPtr(this);
		FGsTextUtil::ChangeBanSentence(guildData->GetNotice(), [thiz](bool success, FString inText, FString outText)
			{
				if (thiz.IsValid())
				{
					thiz->SetUIGuildNotice(outText);
				}
			});
	}
}

void UGsUIGuildContentManage::OnReceiveJoinWaitingUserList()
{
	UpdateRedDotApproval();

	if (_bClickedApproval)
	{
		GUI()->Open(TEXT("PopupGuildApproveJoin"));
		_bClickedApproval = false;
	}
}

void UGsUIGuildContentManage::OnNotifyJoinRequest()
{
	UpdateRedDotApproval();
}

void UGsUIGuildContentManage::OnReceiveJoinApproveResult()
{
	UpdateRedDotApproval();
}

void UGsUIGuildContentManage::OnReceiveAuthorityList()
{
	GUI()->Open(TEXT("PopupGuildAuthority"));
}

void UGsUIGuildContentManage::InvalidateAuthority()
{
	if (nullptr == _guildData)
	{
		return;
	}
	
	uint32 myAuthority = _guildData->_authority;
	
	_btnEmblem->SetIsEnabled(FGsGuildHelper::CanManagePost(myAuthority));
	_btnApproval->SetIsEnabled(FGsGuildHelper::CanManageMember(myAuthority));
	
	// 권한변경, 해산, 길드 레벨 변경, 가입유형은 길드장만 가능
	bool bIsMaster = GGuild()->IsGuildMaster();
	_btnAuthority->SetIsEnabled(bIsMaster);
	_btnDisband->SetIsEnabled(bIsMaster);
	_btnLimitLevel->SetIsEnabled(bIsMaster);
	_panelJoinType->SetIsEnabled(bIsMaster);

	// 길드장 탈퇴 불가(넘기고 가야함)
	_btnLeave->SetIsEnabled(!bIsMaster);	
	
	_btnIntroduce->SetIsEnabled(FGsGuildHelper::CanManagePost(myAuthority));
	_btnNotice->SetIsEnabled(FGsGuildHelper::CanManagePost(myAuthority));

	UpdateRedDotApproval();
}

void UGsUIGuildContentManage::InvalidateAcquisition()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	if (false == IsBranch(EGsGameClientBranchType::GUILD_AUCTION))
	{
		_panelAcquisition->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_panelAcquisition->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText textAcquisition;
	if (auctionData->IsAcquisitionOn())
	{
		// TEXT: 적용중
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Info_Status_On"), textAcquisition);
	}
	else
	{
		// TEXT: 미적용
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Info_Status_Off"), textAcquisition);
	}
	_textBlockAcquisitionInfo->SetText(textAcquisition);

	UpdateRedDotAcquisition();
}

TArray<UGsUIGuildSnsUrlListItem*> UGsUIGuildContentManage::MakeSnsLinkListItems(const bool InIsMyGuild)
{
	TArray<UGsUIGuildSnsUrlListItem*> ListItems;

	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
	}
	else
	{
		const TArray<FString> SnsLinks = GuildManager->MakeSnsLinks(InIsMyGuild);
		for (int32 i = 0; i < SnsLinks.Num(); ++i)
		{
			const FString& SnsLink = SnsLinks[i];
			const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = GuildManager->FindSnsLinkInfo(SnsLink);

			UGsUIGuildSnsUrlListItem* ListItem = UGsUIGuildSnsUrlListItem::CreateInstance(SnsLink);
			ListItem->_index = i;
			ListItem->_url = SnsLink;
			ListItem->_enableButton = InIsMyGuild && GuildManager->IsGuildMaster();
			ListItem->_onClick.Unbind();
			if (ListItem->_enableButton)
			{
				ListItem->_onClick.BindUObject(this, &UGsUIGuildContentManage::OnClickSnsLink);
			}

			if (nullptr != SnsShortcutUrl)
			{
				if (!SnsShortcutUrl->platform.IsNull())
				{
					ListItem->_iconIndex = SnsShortcutUrl->platform.GetRow()->iconIndex;
				}

				// 체크가 필요한 경우에만 URL체크를 실행
				ListItem->_urlCheckCompleted = !SnsShortcutUrl->checkBanWord;
			}
			else
			{
				// 미지정 URL 인 경우, 비속어 체크 필요
				ListItem->_urlCheckCompleted = false;
			}

			ListItems.Emplace(ListItem);
		}
	}

	return ListItems;
}

void UGsUIGuildContentManage::RequestInvalidateSnsLinks()
{
	_snsUrlListItems = MakeSnsLinkListItems(true);

	TWeakObjectPtr<UGsUIGuildContentManage> This = MakeWeakObjectPtr(this);
	for (const UGsUIGuildSnsUrlListItem* ListItem : _snsUrlListItems)
	{
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}

		if (ListItem->_urlCheckCompleted)
		{
			UpdateSnsLink(ListItem->_index, ListItem->_url);
		}
		else
		{
			const int32 Index = ListItem->_index;
			FGsTextUtil::ChangeBanSentence(ListItem->_url, [This, Index](bool InSuccess, FString InOriginal, FString InChanged)
			{
				if (This.IsValid())
				{
					This->UpdateSnsLink(Index, InChanged);
				}
			});
		}
	}
}

void UGsUIGuildContentManage::InvalidateSnsLinks()
{
	_snsLinkListView->SetListItems(_snsUrlListItems);
}

bool UGsUIGuildContentManage::IsEnableDisband(bool bInShowMessage) const
{
	if (nullptr == _guildData)
	{
		return false;
	}

	if (1 < _guildData->_memberNum)
	{
		// TEXT: 기사단에 1명 남았을 때만 해산할 수 있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotDeleteGuild"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return false;
	}

	if (false == GGuild()->IsGuildMaster())
	{
		// TEXT: 기사단장만 해산할 수 있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotDeleteGuild_NotMaster"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return false;
	}

	return true;
}

bool UGsUIGuildContentManage::IsEnableLeave(bool bInShowMessage) const
{
	if (GGuild()->IsGuildMaster())
	{
		// TEXT: 기사단장은 탈퇴할 수 없습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotQuitGuild_Master"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return false;
	}

	return true;
}

void UGsUIGuildContentManage::UpdateSnsLink(const int32 InIndex, const FString& InSnsLink)
{
	bool AllSnsLinkChecked = true;

	for (UGsUIGuildSnsUrlListItem* ListItem : _snsUrlListItems)
	{
		if (nullptr == ListItem)
		{
			GSLOG(Error, TEXT("nullptr == ListItem"));
			continue;
		}

		if (ListItem->_index == InIndex && !ListItem->_urlCheckCompleted)
		{
			ListItem->_url = InSnsLink;
			ListItem->_urlCheckCompleted = true;
		}

		if (!ListItem->_urlCheckCompleted)
		{
			AllSnsLinkChecked = false;
		}
	}

	if (AllSnsLinkChecked)
	{
		InvalidateSnsLinks();
	}
}

void UGsUIGuildContentManage::OnClickLimitLevel()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonChangeNumber"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonChangeNumber* popup = Cast<UGsUIPopupCommonChangeNumber>(widget.Get()))
		{
			// TEXT: 가입 제한 레벨을 변경하시겠습니까?\r\n\r\n<p>숫자만 입력 가능합니다.</>
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_LevelLimitChange_Text"), textMsg);
			popup->SetMessageText(textMsg);

			popup->SetData(1, MAX_LEVEL, [](int32 InNumber)
				{
					FGsNetSendServiceGuild::SendReqChangeLimitLevel(InNumber);
				});
		}
	}
}

void UGsUIGuildContentManage::OnClickAuthority()
{
	if (false == GGuild()->GetSendController().SendRequestAuthorityList())
	{
		 GUI()->Open(TEXT("PopupGuildAuthority"));
	}
}

void UGsUIGuildContentManage::OnClickApproval()
{
	if (GGuild()->GetSendController().SendRequestJoinWaitingUserList())
	{
		_bClickedApproval = true;
	}
	else
	{
		GUI()->Open(TEXT("PopupGuildApproveJoin"));
		_bClickedApproval = false;
	}
}

void UGsUIGuildContentManage::OnClickDisband()
{
	// 해산 가능한지 체크
	if (false == IsEnableDisband())
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (widget.IsValid())
	{
		if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
		{
			// TEXT: 기사단을 해산하시겠습니까?
			FText textMsg1;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Disband_Check"), textMsg1);
			// TEXT: 기사단을 해산 후 되돌릴 수 없습니다. 기사단을 해산하려면 ...
			FText textMsg2;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Disband_Text"), textMsg2);

			popup->SetData(textMsg1, textMsg2, [this](bool InIsOk)
				{
					if (InIsOk)
					{
						// 해산 가능한지 한번 더 체크
						if (IsEnableDisband())
						{
							FGsNetSendServiceGuild::SendReqDeleteGuild();
						}
					}
				});
		}
	}
}

void UGsUIGuildContentManage::OnClickLeave()
{
	if (false == IsEnableLeave())
	{
		return;
	}

	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (widget.IsValid())
	{
		if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
		{
			// TEXT: 기사단을 탈퇴하시겠습니까?
			FText textMsg1;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Leave_Check"), textMsg1);
			// TEXT: 기사단을 탈퇴하고 {0}시간 동안은 ...
			FText textFormat;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Leave_Text"), textFormat);
			FText textMsg2 = FText::Format(textFormat, guildConfig->reJoinCooldownHour);

			popup->SetData(textMsg1, textMsg2, [this](bool InIsOk)
				{
					if (InIsOk)
					{
						if (IsEnableLeave())
						{
							FGsNetSendServiceGuild::SendReqLeave();
						}						
					}
				});
		}
	}
}

void UGsUIGuildContentManage::OnClickIntroduce()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildChangeMessage"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildChangeMessage* popupMsg = Cast<UGsUIPopupGuildChangeMessage>(widget.Get()))
		{
			popupMsg->SetMessageType(UGsUIPopupGuildChangeMessage::EGsGuildMsgType::INTRODUCTION);
		}
	}
}

void UGsUIGuildContentManage::OnClickNotice()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildChangeMessage"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildChangeMessage* popupMsg = Cast<UGsUIPopupGuildChangeMessage>(widget.Get()))
		{
			popupMsg->SetMessageType(UGsUIPopupGuildChangeMessage::EGsGuildMsgType::NOTICE);
		}
	}
}

void UGsUIGuildContentManage::OnClickEmblem()
{
	if (nullptr == _guildData)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> popup = GUI()->OpenAndGetWidget(TEXT("PopupGuildChangeEmblem"));
	if (popup.IsValid())
	{
		if (UGsUIPopupGuildChangeEmblem* popupEmblem = Cast<UGsUIPopupGuildChangeEmblem>(popup.Get()))
		{
			// 콜백 등록
			popupEmblem->SetData(true, _guildData->_emblemId, [this](int32 InEmblem)
				{
					FGsNetSendServiceGuild::SendReqChangeEmblem(static_cast<GuildEmblemId>(InEmblem));
				});
		}
	}
}

void UGsUIGuildContentManage::OnClickAcquisition()
{
	 GUI()->Open(TEXT("PopupGuildOption"));

	 // 레드닷 체크
	 GGuild()->GetRedDotCheckData().UpdateAcquisitionCheckTime();
	 GGuild()->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::AcquisitionSetting);

	 // 레드닷 메인 탭 및 HUD 갱신 요청
	 OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::Manage));
	 GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
	 UpdateRedDotAcquisition();
}

void UGsUIGuildContentManage::OnClickSnsLink(const int32 InIndex)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
		return;
	}

	const TArray<FString> SnsLinks = GuildManager->MakeSnsLinks(true);
	if (!SnsLinks.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("!SnsLinks.IsValidIndex(%d)"), InIndex);
		return;
	}

	const FString SnsLink = SnsLinks[InIndex];

	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> Popup = UiManager->OpenAndGetWidget(UGsUIPopupGuildChangeSnsUrl::WIDGET_KEY);
	if (Popup.IsValid())
	{
		UGsUIPopupGuildChangeSnsUrl* GuildChangeSnsUrlPopup = Cast<UGsUIPopupGuildChangeSnsUrl>(Popup.Get());
		TUniquePtr<UGsUIPopupGuildChangeSnsUrl::Parameters> Parameters = MakeUnique<UGsUIPopupGuildChangeSnsUrl::Parameters>();
		Parameters->_index = InIndex;
		Parameters->_oldSnsLink = SnsLink;
		GuildChangeSnsUrlPopup->SetParameters(MoveTemp(Parameters), UGsUIPopupGuildChangeSnsUrl::FOnClosePopup::CreateUObject(this, &UGsUIGuildContentManage::OnClosePopupGuildChangeSnsLink));
	}
}

void UGsUIGuildContentManage::OnClosePopupGuildChangeSnsLink(const UGsUIPopupGuildChangeSnsUrl::Parameters* InParameters)
{
	if (nullptr == InParameters)
	{
		GSLOG(Error, TEXT("nullptr == InParameters"));
		return;
	}

	if (!InParameters->_result)
	{
		return;
	}

	FGsNetSendServiceGuild::SendReqSaveSNSLink(InParameters->_index, InParameters->_newSnsLink);
}

void UGsUIGuildContentManage::OnSelectJoinType(int32 InIndex)
{
	GuildJoinType joinType = static_cast<GuildJoinType>(InIndex);
	if (_guildData &&
		_guildData->_joinType != joinType)
	{
		if (false == GGuild()->GetSendController().SendRequestChangeJoinType(static_cast<GuildJoinType>(InIndex)))
		{
			// 실패했으므로 다시 돌려놓음
			_toggleGroupJoinType.SetSelectedIndex(static_cast<int32>(_guildData->_joinType));

			// TEXT: 잠시 후 다시 시도해주세요.
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}
}

void UGsUIGuildContentManage::SetUIGuildEmblem(GuildEmblemId InEmblemId)
{
	const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InEmblemId);
	if (emblemData)
	{
		_imgEmblem->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(emblemData->iconPathBig));
	}
}

void UGsUIGuildContentManage::SetUIGuildIntroduce(const FString& InIntroduce)
{
	_textBlockIntroduce->SetText(FText::FromString(InIntroduce));
	_panelEmptyIntroduce->SetVisibility(InIntroduce.IsEmpty() ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildContentManage::SetUIGuildNotice(const FString& InNotice)
{
	_textBlockNotice->SetText(FText::FromString(InNotice));
	_panelEmptyNotice->SetVisibility(InNotice.IsEmpty() ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildContentManage::UpdateRedDotApproval()
{
	int32 count = GGuild()->GetRedDotApproval();
	_uiRedDotApproval->SetRedDot((0 < count) ? true : false, EGsIconRedDotType::NUMBER, count);
}

void UGsUIGuildContentManage::UpdateRedDotAcquisition()
{
	bool isChanged = GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::AcquisitionSetting);
	_uiRedDotAcquisition->SetVisibility(isChanged ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
