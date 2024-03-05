// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowGuildCreate.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Classes/Engine/Texture2D.h"
#include "Classes/PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeEmblem.h"
#include "UI/UIContent/Window/GsUIWindowGuildStart.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UTIL/GsText.h"


void UGsUIWindowGuildCreate::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnChangeEmblem->OnClicked.AddDynamic(this, &UGsUIWindowGuildCreate::OnClickChangeEmblem);

	_toggleGroupJoinType.AddToggleWidgetByParentPanel(_panelJoinType);
	_toggleGroupJoinType.OnSelectChanged.BindUObject(this, &UGsUIWindowGuildCreate::OnClickTabJoinType);

	_btnCreateGuild->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowGuildCreate::OnClickCreateGuild);
	
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowGuildCreate::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowGuildCreate::OnClickCloseAllStack);

	_inputBoxName->OnTextChanged.AddDynamic(this, &UGsUIWindowGuildCreate::OnTextChangedGuildName);
	_inputBoxName->OnTextCommitted.AddDynamic(this, &UGsUIWindowGuildCreate::OnTextCommittedGuildName);

	_inputBoxIntroduction->OnTextChanged.AddDynamic(this, &UGsUIWindowGuildCreate::OnTextChangedIntroduction);
}

void UGsUIWindowGuildCreate::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterMessages();

	// 메뉴들 비우고 기본값으로 세팅
	// 로컬라이징을 위해 Init에서 세팅하지 않음	

	// TEXT: 창설하기
	FText textCreate;
	FText::FindText(TEXT("GuildText"), TEXT("GuildCreate"), textCreate);
	_btnCreateGuild->SetButtonName(textCreate);

	const FGsSchemaGuildConfig* configData = FGsGuildHelper::GetGuildConfig();
	if (configData)
	{
		_createGoldAmount = configData->creationCost;
		_btnCreateGuild->SetData(CurrencyType::GOLD, _createGoldAmount);

		// 재화량에 따른 버튼 색상, 활성화 체크
		InvalidateCreateButton();
	}
	
	SetUIGuildEmblem(1);

	_inputBoxName->SetText(FText::GetEmpty());
	_inputBoxIntroduction->SetText(FText::GetEmpty());

	_toggleGroupJoinType.SetSelectedIndex(0);
}

void UGsUIWindowGuildCreate::NativeDestruct()
{
	DeregisterMessages();

	Super::NativeDestruct();
}

void UGsUIWindowGuildCreate::RegisterMessages()
{
	MUserBasicInfo& msgUserInfo = GMessage()->GetUserBasicInfo();
	_msgUserInfoHandle = msgUserInfo.AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIWindowGuildCreate::OnChangeCurrency);
}

void UGsUIWindowGuildCreate::DeregisterMessages()
{
	MUserBasicInfo& msgUserInfo = GMessage()->GetUserBasicInfo();
	msgUserInfo.Remove(_msgUserInfoHandle);
}

bool UGsUIWindowGuildCreate::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsGuild))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);

		// 정해진 로직으로 열어야해서...
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowGuildStart"));
		if (widget.IsValid())
		{
			if (UGsUIWindowGuildStart* windowStart = Cast<UGsUIWindowGuildStart>(widget.Get()))
			{
				windowStart->StartOpenCreate();
			}
		}

		return true;
	}

	return false;
}

void UGsUIWindowGuildCreate::OnClickChangeEmblem()
{
	TWeakObjectPtr<UGsUIWidgetBase> popup = GUI()->OpenAndGetWidget(TEXT("PopupGuildChangeEmblem"));
	if (popup.IsValid())
	{
		if (UGsUIPopupGuildChangeEmblem* popupEmblem = Cast<UGsUIPopupGuildChangeEmblem>(popup.Get()))
		{
			// 콜백 등록
			popupEmblem->SetData(false, _guildEmblemId, [this](uint8 InEmblem)
				{
					// 아이콘 세팅
					SetUIGuildEmblem(InEmblem);
				});
		}
	}
}

void UGsUIWindowGuildCreate::OnTextChangedGuildName(const FText& InText)
{
	FString changedString;
	bool isTextChanged = FGsUIStringHelper::GetFormatedEditableText(InText.ToString(), changedString, MAX_GUILD_NAME_LEN, false, true);
	if (isTextChanged)
	{
		_inputBoxName->SetText(FText::FromString(changedString));
	}
}

void UGsUIWindowGuildCreate::OnTextCommittedGuildName(const FText& InText, ETextCommit::Type InCommitType)
{
	FString strText = InText.ToString();
	if (false == FGsTextUtil::IsValidNameType(strText))
	{
		// TEXT: 잘못된 문자가 포함되어 있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLetter"), textMsg);
		FGsUIHelper::PopupOK(textMsg);
	}
}

void UGsUIWindowGuildCreate::OnTextChangedIntroduction(const FText& InText)
{
	FString changedString;
	bool isTextChanged = FGsUIStringHelper::GetFormatedEditableText(InText.ToString(), changedString, MAX_GUILD_DESC_LEN);
	if (isTextChanged)
	{
		_inputBoxIntroduction->SetText(FText::FromString(changedString));
	}
}

void UGsUIWindowGuildCreate::OnClickTabJoinType(int32 InIndex)
{
	_toggleGroupJoinType.SetSelectedIndex(InIndex);
}

void UGsUIWindowGuildCreate::OnClickCreateGuild()
{
	// 재화량 검사
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, _createGoldAmount, false))
	{
		Currency myGold = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
		Currency lackAmount = _createGoldAmount - myGold;

		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, lackAmount);
		return;
	}
	
	FString strName = _inputBoxName->GetText().ToString();
	
	// 길드 이름 글자수 검사
	int32 length = FGsTextUtil::GetStringLengthNameType(strName);
	if (MIN_GUILD_NAME_LEN > length ||
		MAX_GUILD_NAME_LEN < length)
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
		FGsUIHelper::PopupOK(textMsg);
		return;
	}

	// 숫자와 지원 언어 외의 문자가 포함되었는지 체크(공백, 특수문자, 이모티콘, 미지원언어)
	if (false == FGsTextUtil::IsValidNameType(strName))
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
		FGsUIHelper::PopupOK(textMsg);
		return;
	}

	auto emblemId = _guildEmblemId;
	FString textIntroduction = _inputBoxIntroduction->GetText().ToString();
	GuildJoinType joinType = static_cast<GuildJoinType>(_toggleGroupJoinType.GetSelectedIndex());
	
	// 길드 이름 금칙어 검사
	FGsTextUtil::IsBanWord(strName, BannedWordCheckUsage::name, [emblemId, strName, textIntroduction, joinType](bool success, FString inText, FString outText) {

			if (success)
			{
				// TODO: 딜레이 처리
				FGsNetSendServiceGuild::SendReqCreateGuild(strName, emblemId, joinType, textIntroduction);
			}
			else
			{
				FText textMsg;
				FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLetter"), textMsg);
				FGsUIHelper::PopupOK(textMsg);
			}
		});
	

	//// TODO: 딜레이 처리
	//FGsNetSendServiceGuild::SendReqCreateGuild(strName, _guildEmblemId, 
	//	static_cast<GuildJoinType>(_toggleGroupJoinType.GetSelectedIndex()), textIntroduction);
}

void UGsUIWindowGuildCreate::OnChangeCurrency(uint64 InParam)
{
	// 골드 변화 있을 때 체크
	if (static_cast<int32>(CurrencyType::GOLD) == InParam)
	{
		InvalidateCreateButton();
	}
}

void UGsUIWindowGuildCreate::SetUIGuildEmblem(int32 InGuildEmblemId)
{
	_guildEmblemId = InGuildEmblemId;

	if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InGuildEmblemId))
	{
		if (UTexture2D* texture = FGsUIHelper::LoadTextureFromPath(emblemData->iconPathBig))
		{
			_imgGuildEmblem->SetBrushFromAtlasInterface(texture);
		}
	}
}

void UGsUIWindowGuildCreate::InvalidateCreateButton()
{
	bool bIsEnoughCurrency = FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, _createGoldAmount, false);

	_btnCreateGuild->SetAmountTextColor((bIsEnoughCurrency) ?
		EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST);
}