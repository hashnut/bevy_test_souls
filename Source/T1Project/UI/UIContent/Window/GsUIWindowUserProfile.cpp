// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIWindowUserProfile.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"

#include "DataSchema/Social/GsSchemaSocialKeyword.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "Guild/GsGuildHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"

#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Net/GsNetSendService.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Option/GsGameUserSettings.h"
#include "Option/GsServerOption.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Window/UserProfile/GsUIUserProfileStatEntry.h"
#include "UI/UIContent/Window/UserProfile/GsUIUserProfileCommentListEntry.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"

#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupKeywordVote.h"
#include "UI/UIContent/Popup/GsUIPopupKeywordVoteMy.h"
#include "UI/UIContent/Popup/GsUIPopupCommentWrite.h"
#include "UI/UIContent/Popup/GsUIPopupUserProfileSetting.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"

#include "Framework/Application/SlateApplication.h"

#include "Ranking/GsRankingHelper.h"
#include "UTIL/GsTimeSyncUtil.h"


void UGsUIWindowUserProfile::BeginDestroy()
{
	if (nullptr != _slotHelperComment)
	{
		_slotHelperComment->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowUserProfile::OnRefreshCommentSlot);
		_slotHelperComment = nullptr;
	}

	for (int32 i = 0;i < _iconSlotHelperList.Num();++i)
	{
		if (nullptr != _iconSlotHelperList[i])
		{
			_iconSlotHelperList[i]->OnRefreshIcon.RemoveDynamic(this, &UGsUIWindowUserProfile::OnRefreshIconEquipItem);
		}
	}
	_iconSlotHelperList.Empty();

	if (_slotHelperNickNameHistory)
	{
		_slotHelperNickNameHistory->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowUserProfile::OnRefreshNickNameHistory);
	}
	_slotHelperNickNameHistory = nullptr;

	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickBack);
	}
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickCloseAllStack);
	}
	if (_btnKeywordVote)
	{
		_btnKeywordVote->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickKeywordVote);
	}
	if (_btnCommentWrite)
	{
		_btnCommentWrite->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickCommentWrite);
	}
	if (_btnStat)
	{
		_btnStat->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickStat);
	}
	if (_btnIntroduction)
	{
		_btnIntroduction->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickIntroduction);
	}
	if (_btnUserInfo)
	{
		_btnUserInfo->OnClicked.RemoveDynamic(this, &UGsUIWindowUserProfile::OnClickUserInfo);
	}

	if (_scrollBoxComment)
	{
		_scrollBoxComment->OnUserScrolled.RemoveDynamic(this, &UGsUIWindowUserProfile::OnUserScrolled);
	}
	
	Super::BeginDestroy();
}

void UGsUIWindowUserProfile::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickCloseAllStack);

	_btnKeywordVote->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickKeywordVote);
	_btnCommentWrite->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickCommentWrite);
	_btnStat->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickStat);
	_btnIntroduction->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickIntroduction);
	_btnUserInfo->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickUserInfo);
	_btnVisible->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickVisible);
	_btnEquipFirst->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickPageFirst);
	_btnEquipSecond->OnClicked.AddDynamic(this, &UGsUIWindowUserProfile::OnClickPageSecond);

	_iconSlotHelperList.Empty();
	FindIconSelecter(_panelIconRootLeft, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootRight, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootBMLeft, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootBMRight, _iconSlotHelperList);

	if (nullptr == _slotHelperComment)
	{
		_slotHelperComment = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperComment->Initialize(_subClassOfComment, _scrollBoxComment);
		_slotHelperComment->OnRefreshEntry.AddDynamic(this, &UGsUIWindowUserProfile::OnRefreshCommentSlot);
	}
	_scrollBoxComment->OnUserScrolled.AddDynamic(this, &UGsUIWindowUserProfile::OnUserScrolled);

	if (nullptr == _slotHelperNickNameHistory)
	{
		_slotHelperNickNameHistory = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperNickNameHistory->Initialize(_entryClassNickNameHistory, _scrollBoxNickNameHistory);
		_slotHelperNickNameHistory->OnRefreshEntry.AddDynamic(this, &UGsUIWindowUserProfile::OnRefreshNickNameHistory);
		_slotHelperNickNameHistory->SetEmptyListPanel(_panelEmptyNickNameHistory);
	}

	_toggleGroupMenu.OnSelectChanged.BindUObject(this, &UGsUIWindowUserProfile::OnSelectMenu);
}

void UGsUIWindowUserProfile::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == GSUserProfile()->IsExistProfile())
	{
		Close();
		return;
	}

	_statEntryList.Empty();
	_statEntryList.Emplace(_statEntry01);
	_statEntryList.Emplace(_statEntry02);
	_statEntryList.Emplace(_statEntry03);
	_statEntryList.Emplace(_statEntry04);
	_statEntryList.Emplace(_statEntry05);
	_statEntryList.Emplace(_statEntry06);
	_statEntryList.Emplace(_statEntry07);

	// 순서 유의
	_equipSlotList.Empty();
	_equipSlotList.Emplace(EGsEquipSlotType::HELMET_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::ARMOR_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SHOES_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::NECKLACE_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::EAR_RING_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::RING_LEFT_TYPE);

	_equipSlotList.Emplace(EGsEquipSlotType::WEAPON_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BELT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::GLOVE_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SCARF_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::EAR_RING_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::RING_RIGHT_TYPE);

	_equipSlotList.Emplace(EGsEquipSlotType::CHARM_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BRACELET_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::DURAL_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::MARBLE_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);

	_equipSlotList.Emplace(EGsEquipSlotType::CHARM_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BRACELET_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::MARBLE_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);

	_isStatOpened = false;
	_reqOptionSave = false;

	_btnEquipFirst->SetSwitcherIndex(0);
	_btnEquipSecond->SetSwitcherIndex(1);

	PlayAnimation(_animMoveToSecond, _animMoveToSecond->GetEndTime() - 0.1f, 1, EUMGSequencePlayMode::Reverse);

	_toggleGroupMenu.Clear();
	_toggleGroupMenu.AddToggleWidget(_sBtnComment);
	_toggleGroupMenu.AddToggleWidget(_sBtnNickNameHistory);
	_toggleGroupMenu.SetSelectedIndex(0, true);

	InvalidateAllInternal();
}

void UGsUIWindowUserProfile::NativeDestruct()
{
	_statEntryList.Empty();

	_equipSlotList.Empty();

	_nickNameHistoryList.Empty();

	ClosePopupAll();

	Super::NativeDestruct();
}

void UGsUIWindowUserProfile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isStatOpened)
	{
		if (0 < FSlateApplication::Get().GetPressedMouseButtons().Num())
		{
			CloseStatTooltip();
		}
	}
}

void UGsUIWindowUserProfile::CloseInternal()
{
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::USER_PROFILE, true);

	Super::CloseInternal();
}

bool UGsUIWindowUserProfile::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsUserProfile))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_USER_PROFILE);
		return true;
	}

	return false;
}

void UGsUIWindowUserProfile::FindIconSelecter(UPanelWidget* InPanel, OUT TArray<UGsDynamicIconSlotHelper*>& OutIconSlotHelperList)
{
	for (int32 i = 0, childCount = InPanel->GetChildrenCount(); i < childCount; ++i)
	{
		UWidget* childWidget = InPanel->GetChildAt(i);
		if (childWidget->IsA<UGsHorizontalBoxIconSelector>())
		{
			UGsDynamicIconSlotHelper* iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
			iconSlotHelper->Initialize(Cast<UGsHorizontalBoxIconSelector>(childWidget));
			iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowUserProfile::OnRefreshIconEquipItem);
			OutIconSlotHelperList.Emplace(iconSlotHelper);
		}
	}
}

void UGsUIWindowUserProfile::InvalidateAllInternal()
{
	FGsUserProfileManager* profileManager = GSUserProfile();
	if (nullptr == profileManager)
		return;

	bool isVisible = profileManager->IsVisible();
	bool isMyProfile = profileManager->IsMyProfile();

	SetTargetUserExist(true);

	_panelPublic->SetVisibility(isVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_panelPrivate->SetVisibility(isVisible ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	InvalidateIntroduction();	// 자기소개 멘트 및 정보공개 버튼
	InvalidateEquipItem();		// 장착한 아이템
	InvalidateStat();			// 캐릭터 스탯 정보
	InvalidateStatTooltip();	// 캐릭터 스탯 툴팁

	InvalidateCharacterInfo();	// 캐릭터 정보
	InvalidateGuildInfo();		// 소속 기사단
	InvalidateKeyword();		// 나의 키워드

	_scrollBoxComment->ScrollToStart();
	InvalidateComment();		// 한마디 목록
	InvalidateNickNameHistory(); // 닉네임 변경 이력

	_btnUserInfo->SetVisibility(isMyProfile ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); // 컨텍스트 UI 버튼
	_btnUserInfo->SetIsEnabled(false == profileManager->IsGM());
}

void UGsUIWindowUserProfile::InvalidateIntroduction()
{
	// 자기 소개 텍스트 설정
	FString introductionText = GSUserProfile()->GetIntroductionText();
	if (introductionText.IsEmpty())
	{
		// 자기소개가 없습니다.
		FText findText;
		FText::FindText(TEXT("SocialText"), TEXT("introduction_bubbleEmpty"), findText);
		_textBlockIntroduction->SetText(findText);
	}
	else
	{
		auto thiz = MakeWeakObjectPtr(this);
		GLocalization()->ChangeBannedSentence(introductionText, [thiz](bool success, FString inText, FString outText) {
				if(thiz.IsValid())
				{
					thiz->_textBlockIntroduction->SetText(FText::FromString(outText));
				}
			});
	}

	if (GSUserProfile()->IsMyProfile())
	{
		_btnIntroduction->SetVisibility(ESlateVisibility::Visible);

		_btnVisible->SetVisibility(ESlateVisibility::Visible);
		_switcherVisible->SetActiveWidgetIndex(GServerOption()->GetSocialMediaOption().mNormal ? 0 : 1);
	}
	else
	{
		_btnIntroduction->SetVisibility(ESlateVisibility::Hidden);

		_btnVisible->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowUserProfile::InvalidateEquipItem()
{
	if (GSUserProfile()->IsVisible())
	{
		for (int32 i = 0;i < _iconSlotHelperList.Num();++i)
		{
			if (nullptr != _iconSlotHelperList[i])
			{
				if (_equipSlotList.IsValidIndex(i))
				{
					_refreshSlotType = _equipSlotList[i];
					_iconSlotHelperList[i]->RefreshAll(1);
				}
			}
		}
	}
}

void UGsUIWindowUserProfile::InvalidateStat()
{
	FGsUserProfileManager* profileManager = GSUserProfile();
	if (nullptr == profileManager)
		return;

	if (profileManager->IsVisible() && false == profileManager->IsGM())
	{
		TArray<StatInfo> statInfoList;
		if (profileManager->GetTargetUserStat(statInfoList))
		{
			FGsStatHelper::SortUiOrder(statInfoList);

			if (statInfoList.Num() <= _statEntryList.Num())
			{
				uint8 maxCount = statInfoList.Num();
				for (uint8 i = 0; i < maxCount; ++i)
				{
					const StatInfo& statInfo = statInfoList[i];
					UGsUIUserProfileStatEntry* statEntry = _statEntryList[maxCount - i - 1];

					FText statName;
					FText statValue;
					FString iconImagePath;
					if (FGsStatHelper::GetStatIconImagePath(statInfo.mType, iconImagePath))
					{
						if (FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statInfo.mType, statInfo.mValue, statValue))
						{
							if (FGsStatHelper::GetDisPlayStatName(statInfo.mType, statName))
							{
								statEntry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
								statEntry->SetData(iconImagePath, statName, statValue);
							}
						}
					}
				}
				for (uint8 i = maxCount; i < _statEntryList.Num();++i)
				{
					_statEntryList[i]->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

void UGsUIWindowUserProfile::InvalidateStatTooltip()
{
	FGsUserProfileManager* profileManager = GSUserProfile();
	if (nullptr == profileManager)
		return;

	if (profileManager->IsGM())
	{
		_btnStat->SetVisibility(ESlateVisibility::Visible);
		_btnStat->SetIsEnabled(false);
		_panelStat->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (profileManager->IsVisible())
	{
		TArray<StatInfo> statInfoList;
		profileManager->GetTargetUserStat(statInfoList);
		if (statInfoList.Num() > 0)
		{
			_btnStat->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			_btnStat->SetVisibility(ESlateVisibility::Hidden);
		}
		
		// 스탯 정보 설정
		if (_isStatOpened)
		{
			_panelStat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_panelStat->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		_btnStat->SetVisibility(ESlateVisibility::Hidden);
		_panelStat->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowUserProfile::InvalidateCharacterInfo()
{
	FGsUserProfileManager* profileManager = GSUserProfile();
	if (nullptr == profileManager)
		return;

	// 유저 이름
	_textBlockCharacterName->SetText(FText::FromString(profileManager->GetTargetUserName()));

	// 유저 레벨
	if (profileManager->IsVisible() && false == profileManager->IsGM())
	{
		_switcherLevel->SetActiveWidgetIndex(0);

		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("LevelShort"), findText);

		FString levelText = FString::Format(TEXT("{0}{1}"), { *findText.ToString(), profileManager->GetTargetUserLevel() });
		_textBlockCharacterLevel->SetText(FText::FromString(levelText));
	}
	else
	{
		_switcherLevel->SetActiveWidgetIndex(1);
	}

	// 뱃지
	if (_imageBadge)
	{
		const FSoftObjectPath* badgeIconPath = nullptr;
		if (profileManager->IsGM())
		{
			badgeIconPath = FGsUIHelper::GetBadgeIconPath(AccountGrade::GM);
		}
		else
		{
			badgeIconPath = FGsRankingHelper::GetBadgeIconPath(profileManager->GetRank());
		}
		_imageBadge->SetVisibility(ESlateVisibility::Collapsed);
		if (nullptr != badgeIconPath)
		{
			TWeakObjectPtr<UImage> weakThis(_imageBadge);
			FGsUIHelper::AsyncLoadSpriteFromPath(*badgeIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
				{
					if (UImage* strongThis = weakThis.Get())
					{
						if (UPaperSprite* badgeTexture = Cast<UPaperSprite>(inRes))
						{
							strongThis->SetBrushFromAtlasInterface(badgeTexture);
							strongThis->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						}
					}
				}));
		}
	}
}

void UGsUIWindowUserProfile::InvalidateGuildInfo()
{
	FGsUserProfileManager* manager = GSUserProfile();
	if (nullptr == manager)
		return;

	if (false == manager->IsExistGuild())
	{
		_switcherGuild->SetActiveWidgetIndex(1);
		return;
	}

	const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(manager->GetTargetGuildEmblemId());
	if (nullptr == emblemData)
		return;

	_switcherGuild->SetActiveWidgetIndex(0);
	const FSoftObjectPath& iconPath = emblemData->iconPathBig;
	TWeakObjectPtr<UImage> weakThis(_imageGuildEmblem);
	FGsUIHelper::AsyncLoadTextureFromPath(iconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				if (UTexture2D* emblemTexture = Cast<UTexture2D>(inRes))
				{
					strongThis->SetBrushFromSoftTexture(emblemTexture);
				}
			}
		}));
	_textBlockGuildName->SetText(FText::FromString(manager->GetTargetGuildName()));

	FText gradeText;
	FGsGuildHelper::GetMemberGradeText(manager->GetTargetGuildMemberGrade(), gradeText);
	_textBlockGuildGrade->SetText(gradeText);
}

void UGsUIWindowUserProfile::InvalidateKeyword()
{
	const UGsTableSocialKeyword* table = Cast<UGsTableSocialKeyword>(FGsSchemaSocialKeyword::GetStaticTable());
	if (nullptr == table)
	{
		_switcherKeyword->SetActiveWidgetIndex(1);
		return;
	}
		
	uint64 keywordId = GSUserProfile()->GetMostKeywordId();
	uint64 keywordCount = GSUserProfile()->GetMostKeywordCount();
	if (0 >= keywordCount || 0 == keywordId)
	{
		_switcherKeyword->SetActiveWidgetIndex(1);
		return;
	}
	else
	{
		_switcherKeyword->SetActiveWidgetIndex(0);
	}

	const FGsSchemaSocialKeyword* row = nullptr;
	if (table->FindRowById(static_cast<int32>(keywordId), row))
	{
		if (nullptr != row)
		{
			const FSoftObjectPath& iconPath = row->icon;
			TWeakObjectPtr<UImage> weakThis(_imageKeyword);
			FGsUIHelper::AsyncLoadSpriteFromPath(iconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
				{
					if (UImage* strongThis = weakThis.Get())
					{
						strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
					}
				}));

			_textBlockKeywordName->SetText(row->keywordText);
			if (MOST_KEYWORD_COUNT_MAX < keywordCount)
			{
				FString countText = FString::Printf(TEXT("%lu+"), MOST_KEYWORD_COUNT_MAX);
				_textBlockKeywordCount->SetText(FText::FromString(countText));
			}
			else
			{
				_textBlockKeywordCount->SetText(FText::AsNumber(keywordCount));
			}
		}
	}
	else
	{
		_switcherKeyword->SetActiveWidgetIndex(1);
	}
}

void UGsUIWindowUserProfile::InvalidateComment()
{
	// {0} 님에게 한마디
	FText findText;
	FText::FindText(TEXT("SocialText"), TEXT("comment_title"), findText);

	FText formatText = FText::Format(findText, FText::FromString(GSUserProfile()->GetTargetUserName()));
	_textBlockComment->SetText(formatText);
	_textBlockCommentOff->SetText(formatText);

	// 한마디 작성 버튼
	_btnCommentWrite->SetVisibility(GSUserProfile()->IsMyProfile()
		? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	// 한마디 목록 요청 전 데이터가 없으면 빈 한마디 표시
	if (0 >= GSUserProfile()->GetCommentCount())
	{
		_panelNoComment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_slotHelperComment->RefreshAll(0);
	}
	else
	{
		_panelNoComment->SetVisibility(ESlateVisibility::Collapsed);
		_slotHelperComment->RefreshAll(GSUserProfile()->GetCommentCount());
	}
}

void UGsUIWindowUserProfile::InvalidateNickNameHistory()
{
	_nickNameHistoryList.Empty();
	_nickNameHistoryList.Append(GSUserProfile()->GetNickNameChangeList());

	_slotHelperNickNameHistory->RefreshAll(_nickNameHistoryList.Num());

	_scrollBoxNickNameHistory->ScrollToStart();
}

void UGsUIWindowUserProfile::SetTargetUserExist(bool InIsExist)
{
	_panelBackground->SetVisibility(InIsExist ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_panelExistCharacter->SetVisibility(InIsExist ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_panelNoExistCharacter->SetVisibility(InIsExist ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (!InIsExist)
	{
		ClosePopupAll();
	}
}

void UGsUIWindowUserProfile::OpenKeywordVote()
{
	if (GSUserProfile()->IsMyProfile())
	{
		TWeakObjectPtr<UGsUIWidgetBase> popupKeywordVote = GUI()->OpenAndGetWidget(TEXT("PopupKeywordVoteMy"));
		if (popupKeywordVote.IsValid())
		{
			_popupKeywordVoteMyUI = Cast<UGsUIPopupKeywordVoteMy>(popupKeywordVote);
		}
	}
	else
	{
		TWeakObjectPtr<UGsUIWidgetBase> popupKeywordVote = GUI()->OpenAndGetWidget(TEXT("PopupKeywordVote"));
		if (popupKeywordVote.IsValid())
		{
			_popupKeywordVoteUI = Cast<UGsUIPopupKeywordVote>(popupKeywordVote);
		}
	}
}

void UGsUIWindowUserProfile::CloseStatTooltip()
{
	_isStatOpened = false;
	InvalidateStatTooltip();
}

void UGsUIWindowUserProfile::ClosePopupAll()
{
	if (_popupKeywordVoteUI.IsValid())
	{
		_popupKeywordVoteUI->Close(true);
	}
	if (_popupKeywordVoteMyUI.IsValid())
	{
		_popupKeywordVoteMyUI->Close(true);
	}
	if (_popupCommentWriteUI.IsValid())
	{
		_popupCommentWriteUI->Close(true);
	}
	if (_popupIntroductionUI.IsValid())
	{
		_popupIntroductionUI->Close(true);
	}
}

void UGsUIWindowUserProfile::OnClickBack()
{
	UserDBId lastOpenedUserDBId = GSUserProfile()->PopLastOpenedUserDBID();
	if (INVALID_USER_DB_ID != lastOpenedUserDBId)
	{
		CloseStatTooltip();
		FGsNetSendService::SendReqUserProfile(lastOpenedUserDBId);
	}
	else
	{
		Close();
	}
}

bool UGsUIWindowUserProfile::OnBack()
{
	UserDBId lastOpenedUserDBId = GSUserProfile()->PopLastOpenedUserDBID();
	if (INVALID_USER_DB_ID != lastOpenedUserDBId)
	{
		CloseStatTooltip();
		FGsNetSendService::SendReqUserProfile(lastOpenedUserDBId);
	}
	else
	{
		Close();
	}

	return true;
}

void UGsUIWindowUserProfile::OnClickKeywordVote()
{
	CloseStatTooltip();
	FGsNetSendService::SendReqUserProfileKeywordInfo(GSUserProfile()->GetTargetUserDBID());
}

void UGsUIWindowUserProfile::OnClickCommentWrite()
{
	CloseStatTooltip();
	if (GSUserProfile()->IsMyProfile())
		return;

	TWeakObjectPtr<UGsUIWidgetBase> popupCommentWrite = GUI()->OpenAndGetWidget(TEXT("PopupCommentWrite"));
	if (popupCommentWrite.IsValid())
	{
		_popupCommentWriteUI = Cast<UGsUIPopupCommentWrite>(popupCommentWrite);
	}
}

void UGsUIWindowUserProfile::OnClickStat()
{
	_isStatOpened = !_isStatOpened;
	InvalidateStat();
	InvalidateStatTooltip();
}

void UGsUIWindowUserProfile::OnClickIntroduction()
{
	CloseStatTooltip();
	TWeakObjectPtr<UGsUIWidgetBase> popupIntroduction = GUI()->OpenAndGetWidget(TEXT("PopupUserProfileSetting"));
	if (popupIntroduction.IsValid())
	{
		_popupIntroductionUI = Cast<UGsUIPopupUserProfileSetting>(popupIntroduction);
	}
}

void UGsUIWindowUserProfile::OnClickVisible()
{
	if (false == _reqOptionSave)
	{
		if (FGsServerOption* serverOption = GServerOption())
		{
			_reqOptionSave = true;
			SocialMediaInfo socialMediaInfo = serverOption->GetSocialMediaOption();
			socialMediaInfo.mNormal = !socialMediaInfo.mNormal;
			serverOption->SetSocialMediaInfoOption(socialMediaInfo);
			serverOption->SendToServerGameOption();
			InvalidateIntroduction();
		}
	}
}

void UGsUIWindowUserProfile::OnClickEquipItemSlot(UGsUIIconItem& InIcon)
{
	CloseStatTooltip();
	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	int32 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
	bool isUseLongPress = (0 == value) ? false : true;

	if (false == isUseLongPress)
	{
		FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, true);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
}

void UGsUIWindowUserProfile::OnLongPressEquipItemSlot(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, true);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUIWindowUserProfile::OnRefreshIconEquipItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	int32 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
	bool isUseLongPress = (0 == value) ? false : true;

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowUserProfile::OnLongPressEquipItemSlot);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIWindowUserProfile::OnClickEquipItemSlot);
	}

	if (GSUserProfile()->IsMyProfile())
	{
		ItemDBId itemDBId = GItem()->GetEquipedItemDBIDBySlotType(_refreshSlotType);
		if (INVALID_ITEM_DB_ID == itemDBId)
		{
			itemIcon->SetOwnedItem(0);
			itemIcon->SetActiveIconBg(false);
			itemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			_refreshSlotType = EGsEquipSlotType::SLOT_MAX;
			return;
		}

		itemIcon->SetOwnedItem(itemDBId);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetSlotEquipedImage(false);
		itemIcon->SetPresetImage(false);
		itemIcon->SetRedDot(false);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetItemNameToolTipFlag(isUseLongPress);
		itemIcon->SetOnChangeDisplayDetailWndType((true == isUseLongPress) ?
			UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
		itemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		const ItemDataEquip* itemDataEquip = GSUserProfile()->GetTargetUserItemDataEquip(_refreshSlotType);
		if (nullptr != itemDataEquip)
		{
			itemIcon->SetNotOwnItem(itemDataEquip->mTemplateId, itemDataEquip->mItemAmount, itemDataEquip->mLevel, itemDataEquip->mEnchantShield, itemDataEquip->mEnchantBonusIndex);
			FGsItem* itemData = itemIcon->GetItemIconData();
			itemData->SetIsOtherUserItem(true);
			FGsItemEquipment* itemIconEquipData = itemData->GetCastItem<FGsItemEquipment>();
			itemIconEquipData->UpdateBy(itemDataEquip);
			itemIconEquipData->SetDBID(INVALID_ITEM_DB_ID);
			itemIcon->SetActiveIconBg(true);
			itemIcon->SetItemNameVisibility(false);
			itemIcon->SetSlotEquipedImage(false);
			itemIcon->SetPresetImage(false);
			itemIcon->SetRedDot(false);
			itemIcon->SetItemNameToolTipHoverMode(true);
			itemIcon->SetItemNameToolTipFlag(isUseLongPress);
			itemIcon->SetOnChangeDisplayDetailWndType((true == isUseLongPress) ?
				UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
			itemIcon->SetSlotEffectMagicalForge(itemData);
			itemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			itemIcon->SetOwnedItem(0);
			itemIcon->SetActiveIconBg(false);
			itemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	_refreshSlotType = EGsEquipSlotType::SLOT_MAX;
}

void UGsUIWindowUserProfile::OnSelectMenu(int32 InIndex)
{
	_switcherMenu->SetActiveWidgetIndex(InIndex);

	// 변경 이력을 창을 열면 받기 때문에, 타이밍이 늦을 수 있어 다시 갱신을 수행함
	if (1 == InIndex)
	{
		InvalidateNickNameHistory();
	}
}

void UGsUIWindowUserProfile::OnRefreshCommentSlot(int32 InIndex, UWidget* InEntry)
{
	UGsUIUserProfileCommentListEntry* commentEntry = Cast<UGsUIUserProfileCommentListEntry>(InEntry);
	if (nullptr == commentEntry)
		return;

	const UserComment* commentData = GSUserProfile()->GetComment(InIndex);
	if (nullptr == commentData)
		return;

	commentEntry->SetData(*commentData);
}

void UGsUIWindowUserProfile::OnUserScrolled(float InScrollOffset)
{
	FGsUserProfileManager* profileManager = GSUserProfile();
	if (nullptr == profileManager)
		return;

	float commentCount = _scrollBoxComment->GetChildrenCount();
	float maxOffset = _scrollBoxComment->GetScrollOffsetOfEnd();
	bool isRequestOffset = (InScrollOffset > (commentCount - 6) * (maxOffset / commentCount));
	if (profileManager->CanRequestComment() && isRequestOffset)
	{
		profileManager->RequestComment();
	}
}

void UGsUIWindowUserProfile::OnRefreshNickNameHistory(int32 InIndex, UWidget* InEntry)
{
	if (false == _nickNameHistoryList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
	{
		// time_t를 날짜 포맷으로 변환
		FDateTime date = FGsTimeSyncUtil::ConvertToDateTime(_nickNameHistoryList[InIndex].Value);
		FString strDate;
		FGsTimeStringHelper::GetTimeStringNYMD(date, strDate);

		entry->SetTextFront(FText::FromString(_nickNameHistoryList[InIndex].Key));
		entry->SetTextBack(FText::FromString(strDate));
	}
}

void UGsUIWindowUserProfile::OnClickUserInfo()
{
	if (FGsUserProfileManager* profileManager = GSUserProfile())
	{
		TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->OpenAndGetWidget(TEXT("PopupChatUserInfo")));
		if (contextPopup.IsValid())
		{
			// 2023/9/13 PKT - 유저 프로필은 현재 내 로컬 서버랑 같은 애들만 가능함.
			UGsUIPopupChatUserInfo::PopupInitData initData{ profileManager->GetTargetUserDBID(), profileManager->GetTargetUserName(), TEXT(""), true, EGsUserContentsOwner::USER_PROFILE };
			contextPopup->SetData(&initData, true);
			contextPopup->SetPosition(_btnUserInfo);
		}
	}
}

void UGsUIWindowUserProfile::OnOptionSaveCompleted()
{
	if (_reqOptionSave)
	{
		_reqOptionSave = false;
		InvalidateIntroduction();

		FText findText;

		if (GServerOption()->GetSocialMediaOption().mNormal)
		{
			FText::FindText(TEXT("SocialText"), TEXT("ProfileOpenOnTicker"), findText);
		}
		else
		{
			FText::FindText(TEXT("SocialText"), TEXT("ProfileOpenOffTicker"), findText);
		}
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void UGsUIWindowUserProfile::OnClickPageFirst()
{
	if (_btnEquipFirst->GetSwitcherIndex() == 1)
	{
		_btnEquipFirst->SetSwitcherIndex(0);
		_btnEquipSecond->SetSwitcherIndex(1);

		PlayAnimation(_animMoveToFirst);
	}
}

void UGsUIWindowUserProfile::OnClickPageSecond()
{
	if (_btnEquipFirst->GetSwitcherIndex() == 0)
	{
		_btnEquipFirst->SetSwitcherIndex(1);
		_btnEquipSecond->SetSwitcherIndex(0);

		PlayAnimation(_animMoveToSecond);
	}
}
