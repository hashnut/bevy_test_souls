#include "GsUIUserInfo.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "UI/UIContent/Popup/GsUIPopupPartyInvite.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"
#include "UI/UIContent/Popup/GsUIPopupChatReport.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Net/GsNetSendServiceWorld.h"

#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "HAL/PlatformApplicationMisc.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessagePKBook.h"

#include "Party/GsPartyFunc.h"

#include "PKBook/GsPKBookData.h"

#include "GameObject/Fence/GsFenceHandler.h"

#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"

#include "Blueprint/SlateBlueprintLibrary.h"


void UGsUIUserInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnUserProfile->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickUserProfile);
	_btnGuild->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickGuild);
	_btnAddFriend->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickAddFriend);
	_btnInviteParty->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickInviteParty);
	_btnWhisper->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickWhisper);
	_btnBlock->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickBlock);
	_btnArena->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickArena);
	_btnNameCopy->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickCopy);
	_btnReport->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickReport);
	_btnAddBoundaryTarget->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickBoundaryTarget);
	_btnRemoveBoundaryTarget->OnClicked.AddDynamic(this, &UGsUIUserInfo::OnClickBoundaryTarget);
}

void UGsUIUserInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_ownerContents = EGsUserContentsOwner::NONE;
	_requestContents = EGsContextContentsMenu::NONE;
	_chatText.Empty();

	// 2023/1/17 PKT - 경계 대상 추가에 대한 버튼 갱신
	_PKBookMessageHandler = GMessage()->GetPKBook().AddUObject(MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST, this, &UGsUIUserInfo::InvalidateBoundaryTarget);

	_targetWidget = nullptr;

	SetRenderOpacity(0.0f);
	_isOpened = false;
}

void UGsUIUserInfo::NativeDestruct()
{
	_ownerContents = EGsUserContentsOwner::NONE;
	_requestContents = EGsContextContentsMenu::NONE;
	_chatText.Empty();

	// 2023/1/17 PKT - 메세지 해제
	GMessage()->GetPKBook().Remove(_PKBookMessageHandler);

	Super::NativeDestruct();
}

void UGsUIUserInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isOpened)
	{
		if (_isDelayedOpen)
		{
			GMessage()->GetUserContext().SendMessage(MessageContentUserContext::OPEN, nullptr);
			_isDelayedOpen = false;
		}
		else
		{
			SetRenderOpacity(1.0f);
		}

		return;
	}

	FWidgetTransform panelTransform = RenderTransform;
	if (_targetWidget)
	{
		FVector2D targetTopLeftPixel;
		FVector2D targetTopLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(_targetWidget, _targetWidget->GetCachedGeometry(), FVector2D::ZeroVector, targetTopLeftPixel, targetTopLeftViewport);

		FVector2D targetTopRightViewport = targetTopLeftViewport;
		targetTopRightViewport.X += _targetWidget->GetCachedGeometry().GetLocalSize().X;

		FVector2D topLeftPixel;
		FVector2D topLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(_parent, MyGeometry, FVector2D::ZeroVector, topLeftPixel, topLeftViewport);

		if (_anchor == UserInfoAnchor::TOP_RIGHT_SIDE)
		{
			panelTransform.Translation += -topLeftViewport + targetTopRightViewport;
		}
		else if (_anchor == UserInfoAnchor::TOP_RIGHT_UP)
		{
			panelTransform.Translation += -topLeftViewport + targetTopRightViewport - MyGeometry.GetLocalSize();
		}
		else if (_anchor == UserInfoAnchor::TOP_LEFT_SIDE)
		{
			panelTransform.Translation += -topLeftViewport + targetTopLeftViewport;
			panelTransform.Translation.X -= MyGeometry.GetLocalSize().X;
		}

		if (_targetArea)
		{
			FVector2D areaTopLeftPixel;
			FVector2D areaTopLeftViewport;
			USlateBlueprintLibrary::LocalToViewport(_parent, _targetArea->GetCachedGeometry(), FVector2D::ZeroVector, areaTopLeftPixel, areaTopLeftViewport);

			float topY = areaTopLeftViewport.Y;
			float bottomY = areaTopLeftViewport.Y + _targetArea->GetCachedGeometry().GetLocalSize().Y;

			if (topY > targetTopRightViewport.Y)
			{
				panelTransform.Translation.Y += topY - targetTopRightViewport.Y;
			}

			if (bottomY < targetTopRightViewport.Y + MyGeometry.GetLocalSize().Y)
			{
				panelTransform.Translation.Y += bottomY - targetTopRightViewport.Y - MyGeometry.GetLocalSize().Y;
			}
		}
	}
	else
	{
		panelTransform.Translation = FVector2D::ZeroVector;
	}
	
	float x = FMath::Abs(RenderTransform.Translation.X - panelTransform.Translation.X);
	float y = FMath::Abs(RenderTransform.Translation.Y - panelTransform.Translation.Y);
	const float EPSLION = 0.001f;
	if (x <= EPSLION && y <= EPSLION)
	{
		_isOpened = true;
	}
	else
	{
		SetRenderTransform(panelTransform);
	}
}

void UGsUIUserInfo::SetData(EGsUserContentsOwner InOwner, FGsUserContextInfo& InUserContextInfo)
{
	_ownerContents = InOwner;
	_userContextInfo = InUserContextInfo;

	if (FGsGameDataManager* gameDataManager = GGameData())
	{
		if (const FGsNetUserData* userData = gameDataManager->GetUserData())
		{
			_userContextInfo.SetIsSameWorldUser(userData->mHomeWorldId == _userContextInfo.GetHomeWorldId());
		}
	}

	// 2023/9/13 PKT - 길드 정보
	ESlateVisibility visibility = ESlateVisibility::Collapsed;
	if (_userContextInfo.GetGuildDBId() != INVALID_GUILD_DB_ID)
	{
		visibility = ESlateVisibility::SelfHitTestInvisible;
		_textBlockGuildName->SetText(FText::FromString(_userContextInfo.GetGuildName()));

		if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(_userContextInfo.GetGuildEmblemId()))
		{
			_imageGuildEmblem->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(emblemData->iconPathBig));
		}
	}
	_imageGuildEmblem->SetVisibility(visibility);
	_textBlockGuildName->SetVisibility(visibility);
	
	// 2023/9/13 PKT - 유저 이름 & 프리픽스
	FString userNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(_userContextInfo.GetUserName()), _userContextInfo.GetHomeWorldId());
	_textBlockUserName->SetText(FText::FromString(userNameAndPrefix));

	FText findText;
	FText::FindText(TEXT("ContextUIText"), TEXT("contextButtonText9"), findText);	
	FText levelText = _userContextInfo.IsVisible() ? FText::AsNumber(_userContextInfo.GetUserLevel()) : FText::FromString(TEXT("??"));
	_textBlockUserLevel->SetText(FText::Format(findText, levelText));

	InvalidateButton();
}

void UGsUIUserInfo::SetTarget(UWidget* InTarget)
{
	_targetWidget = InTarget;
}

void UGsUIUserInfo::SetArea(UWidget* InAreaTarget)
{
	_targetArea = InAreaTarget;
}

void UGsUIUserInfo::SetIsDelayedOpen(bool InIsDelayedOpen)
{
	_isDelayedOpen = InIsDelayedOpen;
	_isOpened = false;
}

void UGsUIUserInfo::SetReportContent(const FString& InMessage)
{
	_chatText = InMessage;
}

bool UGsUIUserInfo::IsWaitingUserExist()
{
	return _requestContents != EGsContextContentsMenu::NONE;
}

void UGsUIUserInfo::ReceiveUserExist(bool InIsExist)
{
	if (InIsExist)
	{
		switch (_requestContents)
		{
		case EGsContextContentsMenu::USER_PROFILE:
			OnClickedUserProfile();
			break;
		case EGsContextContentsMenu::GUILD:
			OnClickedGuild();
			break;
		case EGsContextContentsMenu::ADD_FRIEND:
			OnClickedAddFriend();
			break;
		case EGsContextContentsMenu::INVITE_PARTY:
			OnClickedInviteParty();
			break;
		case EGsContextContentsMenu::WHISPER:
			OnClickedWhisper();
			break;
		case EGsContextContentsMenu::USER_BLOCK:
			OnClickedBlock();
			break;
		case EGsContextContentsMenu::ARENA:
			OnClickedArena();
			break;
		case EGsContextContentsMenu::NAME_COPY:
			OnClickedCopy();
			break;
		case EGsContextContentsMenu::USER_REPORT:
			OnClickedReport();
			break;
		case EGsContextContentsMenu::USER_BOUNDARYTARGET:
			OnClickedBoundaryTarget();
			break;
		}
	}
	else
	{
		// 삭제/서버이동 등에 의해 유저가 존재하지 않을 때
		FText findText;
		switch (_requestContents)
		{
		case EGsContextContentsMenu::USER_PROFILE:
		case EGsContextContentsMenu::GUILD:
			// 삭제된 유저의 정보는 열람할 수 없습니다.
			FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText4"), findText);
			break;
		case EGsContextContentsMenu::ARENA:
			// 대결이 불가능한 상태입니다.
			FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
			break;
		default:
			// 캐릭터가 존재하지 않습니다.
			FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText5"), findText);
			break;
		}
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}

	_requestContents = EGsContextContentsMenu::NONE;
}

void UGsUIUserInfo::OnClickedUserProfile()
{
	SendUserProfile(_userContextInfo.GetUserDBId());
}

void UGsUIUserInfo::OnClickedGuild()
{
	SendGuildInfo(&_userContextInfo);
}

void UGsUIUserInfo::OnClickedAddFriend()
{
	SendAddFriend(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());	
}

void UGsUIUserInfo::OnClickedInviteParty()
{
	SendInviteParty(_userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfo::OnClickedWhisper()
{
	SendWhisper(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfo::OnClickedBlock()
{
	SendUserBlock(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserName());
}

void UGsUIUserInfo::OnClickedArena()
{
	if (false == SendDuelInvite(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserLevel(), _userContextInfo.GetUserName(), _userContextInfo.IsVisible()))
	{
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void UGsUIUserInfo::OnClickedCopy()
{
	if (false == SendNameCopy(FText::FromString(_userContextInfo.GetUserName()), _userContextInfo.GetHomeWorldId()))
	{
		// 유저 아이디를 클립보드에 복사했습니다.
		FText findText;
		FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText1"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void UGsUIUserInfo::OnClickedReport()
{
	SendUserRepot(_userContextInfo.GetUserName(), _chatText);
}

void UGsUIUserInfo::OnClickedBoundaryTarget()
{
	bool isAdd = SWITCHER_BOUNDARYTARGET_INDEX::Add == StaticCast<SWITCHER_BOUNDARYTARGET_INDEX>(_switcherBoundaryTarget->GetActiveWidgetIndex());
	SendBoundaryTarget(isAdd, _userContextInfo.GetUserDBId(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfo::ClickButton(EGsContextContentsMenu InType)
{
	_requestContents = InType;
	
	SendUserContextRefresh(_userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfo::OnClickUserProfile()
{
	// 유저 DBID가 올바르면 바로 열고 / 아니면 새로 유저 정보를 요청한다.
	if (_userContextInfo.GetUserDBId() == INVALID_USER_DB_ID)
	{
		ClickButton(EGsContextContentsMenu::USER_PROFILE);
	}
	else
	{
		SendUserProfile(_userContextInfo.GetUserDBId());
	}
}

void UGsUIUserInfo::OnClickGuild()
{
	ClickButton(EGsContextContentsMenu::GUILD);
}

void UGsUIUserInfo::OnClickAddFriend()
{
	ClickButton(EGsContextContentsMenu::ADD_FRIEND);
}

void UGsUIUserInfo::OnClickInviteParty()
{
	ClickButton(EGsContextContentsMenu::INVITE_PARTY);
}

void UGsUIUserInfo::OnClickWhisper()
{
	ClickButton(EGsContextContentsMenu::WHISPER);
}

void UGsUIUserInfo::OnClickBlock()
{
	ClickButton(EGsContextContentsMenu::USER_BLOCK);
}

void UGsUIUserInfo::OnClickArena()
{
	if (GSArena()->GetArenaHandler()->CheckArenaAvailability(true))
	{
		ClickButton(EGsContextContentsMenu::ARENA);
	}
}

void UGsUIUserInfo::OnClickCopy()
{
	// 이름 정보만 사용하기 때문에 추가정보를 요청할 필요가 없다.
	//ClickButton(FunctionType::NameCopy);
	OnClickedCopy();
}

void UGsUIUserInfo::OnClickReport()
{
	// 이름 정보만 사용하기 때문에 추가정보를 요청할 필요가 없다.
	//ClickButton(FunctionType::Report);
	OnClickedReport();
}

void UGsUIUserInfo::OnClickBoundaryTarget()
{
	// 유저 DBID 정보가 없다면 정보를 요청해서 받아와서 사용한다.
	if (_userContextInfo.GetUserDBId() == INVALID_USER_DB_ID)
	{
		ClickButton(EGsContextContentsMenu::USER_BOUNDARYTARGET);
	}
	// 있으면 바로 기능을 사용한다.
	else
	{
		OnClickedBoundaryTarget();
	}
}

void UGsUIUserInfo::InvalidateBoundaryTarget(const IGsMessageParam* InParam)
{
	InvalidateButton();
}

void UGsUIUserInfo::InvalidateButton()
{
	const TMap<EGsContextContentsMenu, bool> activationList = GetActivationFeatureWithBan(_ownerContents, &_userContextInfo, nullptr);

	_btnUserProfile->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_PROFILE));
	_btnGuild->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::GUILD));
	_btnAddFriend->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::ADD_FRIEND));
	_btnInviteParty->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::INVITE_PARTY));
	_btnWhisper->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::WHISPER));
	_btnBlock->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_BLOCK));
	_btnNameCopy->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::NAME_COPY));
	_btnArena->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::ARENA));
	_btnReport->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_REPORT));
	
	_btnAddBoundaryTarget->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_BOUNDARYTARGET));
	_btnRemoveBoundaryTarget->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_BOUNDARYTARGET));

	if (activationList.Contains(EGsContextContentsMenu::ARENA))
	{
		_btnArena->SetVisibility(ESlateVisibility::Visible);
		_btnReport->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_btnArena->SetVisibility(ESlateVisibility::Collapsed);
		_btnReport->SetVisibility(ESlateVisibility::Visible);
	}

	if (activationList.Contains(EGsContextContentsMenu::USER_BOUNDARYTARGET))
	{
		_switcherBoundaryTarget->SetActiveWidgetIndex(StaticCast<int32>(activationList[EGsContextContentsMenu::USER_BOUNDARYTARGET]));
	}
}