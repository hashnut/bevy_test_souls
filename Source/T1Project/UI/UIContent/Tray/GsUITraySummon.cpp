#include "GsUITraySummon.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/GsUIItemButton.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Tray/Summon/GsUISummonCardItem.h"
#include "UI/UIContent/Tray/Summon/GsUISummonSkipToggleButton.h"
#include "UI/UIContent/Tray/Summon/GsUISummonMileageAddedEffect.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupSummonMileage.h"

#include "GameMode/GsGameModeWorld.h"

#include "Summon/GsSummonCardData.h"
#include "Summon/GsSummonDefine.h"
#include "Summon/GsSummonCardSlotInfo.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Message/MessageParam/GsTutorialMessageParam.h"

#include "Option/GsGameUserSettings.h"
#include "Option/GsOptionEnumType.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "DataSchema/GsSchemaEnums.h"

#include "DataSchema/Summon/GsSchemaSummonConfig.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Currency/GsCurrencyHelper.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"

void UGsUITraySummon::BeginDestroy()
{
	if (_timerHandleTestRetrySummon.IsValid() == true)
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandleTestRetrySummon);
			_timerHandleTestRetrySummon.Invalidate();
		}
	}

	Super::BeginDestroy();
}

void UGsUITraySummon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOpenAll->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickOpenAll);
	_btnClose->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickClose);
	_btnClose2->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickClose);
	_btnClose3->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickClose);
	_btnShowResult->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickShowResult);
	_btnRetry->OnClickCurrencyButton.BindUObject(this, &UGsUITraySummon::OnClickRetry);
	_btnRetryItem->OnClickItemButton.BindUObject(this, &UGsUITraySummon::OnClickRetry);

	//_targetSummonMileageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	//_targetSummonMileageIconSlotHelper->Initialize(_targetSummonMileageItemIconSelector);
	//_targetSummonMileageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITraySummon::OnRefreshTargetIcon);
	//_targetSummonMileageIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITraySummon::OnCreateTargetIcon);


	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("Retry"), findText);
	_btnRetry->SetButtonName(findText);
	// 아이템 다시하기 버튼 텍스트 세팅
	_btnRetryItem->SetButtonName(findText);

	_btnRetryItem->SetItemCount(1);

	_toggleSkipButton->SetToggleCallback([this](int32 In_index, bool In_selected)
	{
		OnSelectSkip(In_selected);
	});

	_btnOpenPopupSummonMileage->OnClicked.AddDynamic(this, &UGsUITraySummon::OnClickOpenPopupSummonMileage);
}
void UGsUITraySummon::NativeConstruct()
{

	_summonHandler = UGsSummonManager::GetSummonHandler();
	
	ClearList();

	Super::NativeConstruct();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_SUMMON);
	}
	bool isComposeType = GSSummon()->GetIsComposeSkipType();
	bool isSkip = false;
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{		
		EGsOptionOutput optionType =
			(isComposeType == true) ?
			EGsOptionOutput::SUMMON_SKIP_COMPOSE : EGsOptionOutput::SUMMON_SKIP;

		const int32 intIsSkip =
			gameUserSettings->GetOutputSetting(optionType);

		isSkip = (bool)intIsSkip;
	}

	_toggleSkipButton->SetIsSelected(isSkip);

	FText findText;
	FTextKey textKey = (isComposeType == true)? TEXT("FairyUI_Compose_Skip") :TEXT("FairyUI_Summon_Skip");
	FText::FindText(TEXT("FairyText"), textKey, findText);

	_toggleSkipButton->SetText(findText);

	FGsMessageHolder* msg = GMessage();

	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUITraySummon::OnUpdateCurrency);

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_SUMMON_MILEAGE_DATA, 
		this, &UGsUITraySummon::OnChangedSummonMileageData));

	EGsSummonType summonType = GSSummon()->GetSummonType();

	int milegeIndex = (summonType == EGsSummonType::SUMMON_COSTUME)? 0: 1;
	_switcherBtnSummonMileage->SetActiveWidgetIndex(milegeIndex);
}

void UGsUITraySummon::NativeDestruct()
{
	_summonHandler = nullptr;

	ClearList();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_SUMMON);
	}

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	// 메시지 해제
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);

		if (0 != _uiMsgArray.Num())
		{
			for (auto iter : _uiMsgArray)
			{
				msgMgr->GetUI().Remove(iter);
			}
			_uiMsgArray.Empty();
		}
	}

	Super::NativeDestruct();

	// 옵션 저장
	UGsGameUserSettings* settings = GGameUserSettings();
	if (settings)
	{
		settings->SaveUserSettings();
	}
}

void UGsUITraySummon::OnClickOpenAll()
{

	// https://jira.com2us.com/jira/browse/C2URWQ-4063
	// 최초 전체 카드 애니 + 각자 카드 애니 스킵

	// 모든 카드 애니 중인지
	if (_totalCardAniStart == true)
	{
		// 애니 멈춤 및 제일 뒷프레임으로 이동
		SkipAnimation();
	}
	// 카드 애니매이션 동시에 다 돌리기
	StartCardAnimation(true);

	// 버튼 상태 갱신
	ChangeButtonState();

	PlayBackCardEffectAnimation();

}
// 닫기 클릭
void UGsUITraySummon::OnClickClose()
{
	CloseTraySummon();
}

// 버튼 상태 세팅
void UGsUITraySummon::SetButtonType(EGsSummonBtnType In_type)
{
	_switcherBtnType->SetActiveWidgetIndex((int)In_type);
}
//https://jira.com2us.com/jira/browse/C2URWQ-4063
// 합성에서도 보이게 처리
void UGsUITraySummon::SetSkipButtonVisible(bool In_isVisible)
{
	ESlateVisibility skipButtonVisibility = (In_isVisible == true)?ESlateVisibility::Visible : ESlateVisibility::Hidden;
	_toggleSkipButton->SetVisibility(skipButtonVisibility);
}
void UGsUITraySummon::CloseTraySummon()
{
	if (_summonHandler)
	{
		// https://jira.com2us.com/jira/browse/C2URWQ-4530
		// 다시 시도후 바로 나가기 클릭시 이상 현상
		if (_summonHandler->IsClickRetry() == true)
		{
			return;
		}

		_summonHandler->EndSummon(); 
	}
	// ui 닫기
	Close();
}
// 결과 보기(고등급용)
void UGsUITraySummon::OnClickShowResult()
{
	ShowRemainVeilCard();
}

// 다시 뽑기
void UGsUITraySummon::OnClickRetry()
{
	if (_summonHandler)
	{
		_summonHandler->StartRetry();
	}
}

// 리스트 초기화
void UGsUITraySummon::InitList()
{
	TArray<FGsSummonCardData*> arrayCardOrderData = GSSummon()->GetCardOrderData();

	_countCard = arrayCardOrderData.Num();
	_countOpenCard = 0;

	_currentSlotId.Empty();

	if (_countCard == 0)
	{
		return;
	}

	if (GData()->GetGlobalData()->_cardSlotInfo.Num() >= _countCard)
	{
		FGsSummonCardSlotInfo slotInfo = GData()->GetGlobalData()->_cardSlotInfo[_countCard - 1];
		_currentSlotId = slotInfo._slotInfo;
	}

	int dataIndex = 0;
	_lowCardCount = 0;

	float summonCardOpenAniDelay = GData()->GetGlobalData()->_summonCardOpenAniDelay;

	for (; dataIndex < _countCard; ++dataIndex)
	{		
		int slotIndex = GetCurrentCardItemSlotIndex(dataIndex);

		if (_arrayCardItem.Num() <= slotIndex ||
			slotIndex == -1)
		{
			return;
		}

		UGsUISummonCardItem* settingDataItem = _arrayCardItem[slotIndex];
		if (settingDataItem == nullptr)
		{
			continue;
		}
		
		if (arrayCardOrderData.Num() <= dataIndex ||
			dataIndex < 0)
		{
			continue;
		}

		FGsSummonCardData* data = arrayCardOrderData[dataIndex];
		if (data == nullptr)
		{
			continue;
		}

		bool isHighCard = data->IsHighCard();

		float timeAnyDelay = 0.0f;
		if(isHighCard == false)
		{
			timeAnyDelay = _lowCardCount * summonCardOpenAniDelay;
			++_lowCardCount;
		}
		
		SetCardItemData(data, settingDataItem, timeAnyDelay, isHighCard);

		_arrayNowUseCardItem.Add(settingDataItem);
	}
	// 
	for (int itemIndex = 0; itemIndex < _arrayCardItem.Num(); ++itemIndex)
	{
		UGsUISummonCardItem* hideItem = _arrayCardItem[itemIndex];
		if (hideItem == nullptr)
		{
			continue;
		}
		// show item skip
		if (true == _arrayNowUseCardItem.Contains(hideItem))
		{
			continue;
		}

		hideItem->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 한번 클리어 해준다
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	StartAnimation();

	_totalCardAniStart = true;

	_cardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	EGsSummonType summonType = GSSummon()->GetSummonType();
	int bgIndex = (summonType == EGsSummonType::SUMMON_FAIRY) ? 0 : 1;

	_switcherBG->SetActiveWidgetIndex(bgIndex);
	_switcherBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);


	int itemCount = GSSummon()->GetSummonMileageRewardItemCountCaculated();

	// 보상 받을 아이템 갯수
	// 이때 저장하는게 애매한데 다른데 좋은데 있으면 거길로 바꾸자
	GSSummon()->SetSummonMileageRewardItemCountSaved(itemCount);
	// 이전 보상 갯수
	int preItemCount = GSSummon()->GetSummonMileageRewardItemPreCountCaculated();


	CurrencyType currencyType =
		(summonType == EGsSummonType::SUMMON_FAIRY) ?
		CurrencyType::SUMMON_FAIRY_MILEAGE : CurrencyType::SUMMON_COSTUME_MILEAGE;

	int addedMileage = GSSummon()->GetAddedSummonMileage();
	Currency oldCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyType) - addedMileage;

	// 마일리지 세팅
	SetMileageData(oldCurrency, preItemCount);

	// 초기화
	_isLastStep = false;
}
// 리스트 삭제
void UGsUITraySummon::ClearList()
{
	_cardPanel->SetVisibility(ESlateVisibility::Hidden);

	// 아직 카드 애니매이션 중이라면 finish callback unbind 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-6044
	UnbindCardAnimationFunction();

	_arrayNowUseCardItem.Empty();

	_currentSlotId.Empty();

	_switcherBG->SetVisibility(ESlateVisibility::Hidden);	
}
// 반복 데이터 갱신
void UGsUITraySummon::InitRetryData()
{
	// 반복 타입이면
	if (GSSummon()->GetIsSummonRetry() == true)
	{
		if (GSSummon()->GetSummonBuyType() == EGsSummonBuyType::BM_SHOP)
		{
			_btnRetry->SetData(GSSummon()->GetSummonCurrencyType(),
				GSSummon()->GetSummonCurrencyCost());
		}
		else
		{
			int itemId = GSSummon()->GetRetryItemId();
			_btnRetryItem->SetItemId(itemId);
		}
	}
}

// 마지막 단계 버튼 세팅
void UGsUITraySummon::SetButtonLastStep()
{
	EGsSummonBtnType lastBtnType = EGsSummonBtnType::QUIT;
	if (GSSummon()->GetIsSummonRetry() == true)
	{
		lastBtnType = 
			(GSSummon()->GetSummonBuyType() == EGsSummonBuyType::ITEM)?
			EGsSummonBtnType::RETRY_ITEM:EGsSummonBtnType::RETRY;
	}

	SetButtonType(lastBtnType);	

	FGsTutorialMsgParamSummonButtonState param(lastBtnType);
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::SUMMON_BUTTON_STATE, &param);

	EGsSummonType summonType = GSSummon()->GetSummonType();
	if (EGsSummonType::SUMMON_FAIRY == summonType)
	{
		GSStatChangeNotify()->CheckNotify(StatUpdateReason::FAIRY_COLLECTION);
	}
	else if (EGsSummonType::SUMMON_COSTUME == summonType)
	{
		GSStatChangeNotify()->CheckNotify(StatUpdateReason::COSTUME_COLLECTION);
	}

	SetSkipButtonVisible(true);

	_isLastStep = true;


	EGsSummonOriginContentType originContentType = GSSummon()->GetSummonOriginContent();
	if (originContentType == EGsSummonOriginContentType::SUMMON_ITEM)
	{	
		// 마일리지 보상 연출 시작
		// 이전 아이템보다 갯수가 늘었는가
		int newCount = GSSummon()->GetSummonMileageRewardItemCountCaculated();
		int oldCount = GSSummon()->GetSummonMileageRewardItemPreCountCaculated();
		bool newRewardAdded = (newCount > oldCount)? true : false;

		// 보상 추가 이펙트 틀기
		if (newRewardAdded == true)
		{
			_mileageAddedEffect->StartAnimation();
		}

		SetNowMileageData();
	}
}

void UGsUITraySummon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return;
		}

		UGsUISummonCardItem* startAnimationItem = _arrayNowUseCardItem[dataIndex];
		if (startAnimationItem == nullptr)
		{
			continue;
		}
		startAnimationItem->Tick(InDeltaTime);
	}
}

// 카드 오픈 애니 끝 호출
void UGsUITraySummon::OnFinishCardOpenAni()
{
	++_countOpenCard;

	if (_lowCardCount <= _countOpenCard)	
	{
		ChangeButtonState();
	}	
}

// 스킵할지 선택
void UGsUITraySummon::OnSelectSkip(bool In_isSelect)
{
	_toggleSkipButton->SetIsSelected(In_isSelect);

	int saveVal = (In_isSelect) ? 1 : 0;
	bool isComposeType = GSSummon()->GetIsComposeSkipType();
	if (UGsOptionManager* opt = GOption())
	{
		EGsOptionItem optionItem =
			(isComposeType == true)?
			EGsOptionItem::SummonSkipCompose : EGsOptionItem::SummonSkip;
		opt->SetValue(optionItem, saveVal);
	}

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		
		EGsOptionOutput optionType = 
			(isComposeType == true)? 
			EGsOptionOutput::SUMMON_SKIP_COMPOSE: EGsOptionOutput::SUMMON_SKIP;
		gameUserSettings->SetOutputSetting(optionType, saveVal, false);
	}
}

EGsSummonBtnType UGsUITraySummon::GetButtonType() const
{
	return static_cast<EGsSummonBtnType>(_switcherBtnType->GetActiveWidgetIndex());
}
void UGsUITraySummon::TestAutoRetrySkip()
{
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
}

void UGsUITraySummon::SetCardItemData(FGsSummonCardData* In_cardData, UGsUISummonCardItem* In_cardItem,
	float In_timeAnyDelay, bool In_isHighCard)
{
	if (In_cardData == nullptr)
	{
		return;
	}

	if (In_cardItem == nullptr)
	{
		return;
	}


	EGsSummonType summonType = GSSummon()->GetSummonType();
	In_cardItem->SetData(summonType, In_cardData, In_timeAnyDelay, In_isHighCard);

	In_cardItem->_callbackFinishCardOpenAni = [this]()
	{
		OnFinishCardOpenAni();
	};

	In_cardItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUITraySummon::StartAnimation()
{
	UWidgetAnimation* nowPlayAnimation = GetNowAnimation();

	PlayAnimation(nowPlayAnimation);


	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	BindToAnimationFinished(nowPlayAnimation, FWidgetAnimationDynamicEvent(animationDelegate));
}

void UGsUITraySummon::FinishAnimationBP()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UWidgetAnimation* nowPlayAnimation = GetNowAnimation();

	UnbindFromAnimationFinished(nowPlayAnimation, FWidgetAnimationDynamicEvent(animationDelegate));

	StartCardAnimation();

	_totalCardAniStart = false;
}

UWidgetAnimation* UGsUITraySummon::GetNowAnimation()
{
	UWidgetAnimation* nowPlayAnimation = (_countCard > 6) ? _animation11Card : _animation6Card;
	return nowPlayAnimation;
}

void UGsUITraySummon::StartCardAnimation(bool In_isSkip)
{	
	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return;
		}

		UGsUISummonCardItem* startAnimationItem = _arrayNowUseCardItem[dataIndex];
		if (startAnimationItem == nullptr ||
		true == startAnimationItem->GetIsHighCard())
		{
			continue;
		}
		// 1. 스킵일때(처음 전체 카드 애니메이션 도중)
		// 2. 애니 시작 요청 했는지
		if (In_isSkip == true &&
			true == startAnimationItem->GetIsCardAnimationStart())
		{
			continue;
		}

		// 스킵일때
		if (In_isSkip == true)
		{
			// 대기 시간 0으로(바로 다음틱 시작)
			startAnimationItem->SetTimeTickWaitAniPlay(0.0f);
		}

		startAnimationItem->StartTimmer();
	}
}

// get slot index by data index
int UGsUITraySummon::GetCurrentCardItemSlotIndex(int In_dataIndex)
{
	if (_currentSlotId.Num() <= In_dataIndex)
	{
		return -1;
	}

	return _currentSlotId[In_dataIndex];
}
// check all card opened(high card: over rare grade)
bool UGsUITraySummon::IsAllCardOpened()
{
	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return false;
		}

		UGsUISummonCardItem* card = _arrayNowUseCardItem[dataIndex];
		if (card == nullptr ||
			false == card->GetIsHighCard())
		{
			continue;
		}
		if (card->GetIsOpended() == false)
		{
			return false;
		}
	}
	return true;
}

// open remain card
void UGsUITraySummon::ShowRemainVeilCard()
{
	int dataIndex = 0;
	int aniPlayCount = 0;

	float summonCardOpenAniDelay = GData()->GetGlobalData()->_summonCardOpenAniDelay;

	bool isOpenHighSequenceCard = false;

	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return;
		}

		UGsUISummonCardItem* card = _arrayNowUseCardItem[dataIndex];
		if (card == nullptr ||
			false == card->GetIsHighCard())
		{
			continue;
		}
		if (card->GetIsOpended() == false)
		{
			// U1: 에픽 이상이면 연출을 무조건 보여준다
			if (card->GetIsHighSequenceCard() == true)
			{
				// 한번에 하나씩만
				if(isOpenHighSequenceCard == false)
				{ 
					card->OnClickCard();
					isOpenHighSequenceCard = true;
				}
			}
			else
			{ 
				float timeAnyDelay = aniPlayCount * summonCardOpenAniDelay;
				++aniPlayCount;

				card->StartAnimationRemainHighCard(timeAnyDelay);
			}
		}
	}
}

void UGsUITraySummon::PlayHighGetCardAnimation()
{
	int dataIndex = GSSummon()->GetCurrentHighGetDataIndex();

	if (_arrayNowUseCardItem.Num() <= dataIndex)
	{
		return;
	}

	UGsUISummonCardItem* card = _arrayNowUseCardItem[dataIndex];
	if (card == nullptr ||
		false == card->GetIsHighCard())
	{
		return;;
	}

	card->StartAnimation();
}

void UGsUITraySummon::SkipAnimation()
{
	// 1. 콜백 삭제
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UWidgetAnimation* nowPlayAnimation = GetNowAnimation();

	UnbindFromAnimationFinished(nowPlayAnimation, FWidgetAnimationDynamicEvent(animationDelegate));
	_totalCardAniStart = false;


	// 2. 마지막 프레임으로 이동
	float endTime = nowPlayAnimation->GetEndTime();
	PlayAnimation(nowPlayAnimation, endTime - 0.1f);

}

// https://jira.com2us.com/jira/browse/C2URWQ-4063
// 버튼 상태 갱신
// 1. 고등급 다열림(QUIT)
// 2. 하나라도 안열림(SHOW_RESULT)
void UGsUITraySummon::ChangeButtonState()
{
	if (true == IsAllCardOpened())
	{
		SetButtonLastStep();
	}
	else
	{
		SetButtonType(EGsSummonBtnType::SHOW_RESULT);
	}
}

//https://jira.com2us.com/jira/browse/C2URWQ-5123
//고등급만 있을때 두번 터치 해야되는 이슈
void UGsUITraySummon::SetFirstButtonState()
{
	// 모두 고등급이면 바로 세팅
	if (true == IsAllCardHigh())
	{
		SetButtonType(EGsSummonBtnType::SHOW_RESULT);
	}
	// 아니면 일반 카드들 애니매이션 끝날때 or skip 할때 또 바꿔줌
	else
	{
		SetButtonType(EGsSummonBtnType::OPEN_ALL);
	}
}

// 모든 카드가 고등급인지
bool UGsUITraySummon::IsAllCardHigh()
{
	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return false;
		}

		UGsUISummonCardItem* card = _arrayNowUseCardItem[dataIndex];
		if (card == nullptr ||
			false == card->GetIsHighCard())
		{
			return false;
		}
	}
	return true;
}

// 뒷면 카드 이펙트 애니 플레이(플레이 안된것들)
// 타임라인까지 도달 못하고 스킵되어서 이벤트 호출 안된것들 처리
void UGsUITraySummon::PlayBackCardEffectAnimation()
{
	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return;
		}

		UGsUISummonCardItem* startAnimationItem = _arrayNowUseCardItem[dataIndex];
		if (startAnimationItem == nullptr )
		{
			continue;
		}
		
		startAnimationItem->PlayNotCalledEventBackCardEffect();
	}
}

//void UGsUITraySummon::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
//{
//	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
//	if (nullptr == itemIcon)
//	{
//		return;
//	}
//
//
//	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
//	{
//		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUITraySummon::OnLongPressMileageReward);
//	}
//
//	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
//	{
//		itemIcon->OnClickSlot.AddUObject(this, &UGsUITraySummon::OnClickMileageReward);
//	}
//
//	itemIcon->SetNotOwnItem(_rewardItemTblId, _rewardItemCount);
//	// 갯수 0, 1은 표시를 안해서 강제로 하는 처리 추가
//	itemIcon->SetItemCountEX( _rewardItemCount, true);
//	itemIcon->SetItemNameVisibility(false);
//	itemIcon->SetItemEnchantLevel(0);
//	itemIcon->SetActiveIconBg(false);
//}

//void UGsUITraySummon::OnCreateTargetIcon(UGsUIIconBase* InIcon)
//{
//	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
//	if (nullptr == itemIcon)
//	{
//		return;
//	}
//
//	itemIcon->SetItemNameVisibility(false);
//	itemIcon->SetActiveIconBg(false);
//}

// 보상 아이콘 클릭
//void UGsUITraySummon::OnClickMileageReward(UGsUIIconItem& InIcon)
//{
//	// 보상 획득 가능한지
//	// 1. 획득할 보상이 있는지
//	int itemCount = GSSummon()->GetSummonMileageRewardItemCountCaculated();
//	if (itemCount == 0)
//	{
//		FText findText;
//		// 마일리지가 부족합니다.
//		FText::FindText(TEXT("SummonText"), TEXT("SummonMileage_NotEnough"), findText);
//		FGsUIHelper::TrayMessageTicker( findText);
//		return;
//	}
//	
//	// 2. 모든카드 오픈 되었는지
//	if (_isLastStep == false)
//	{
//		FText findText;
//		// 소환 결과를 확인해야 보상을 획득할 수 있습니다.
//		FText::FindText(TEXT("SummonText"), TEXT("SummonMileage_NotYet"), findText);
//		FGsUIHelper::TrayMessageTicker( findText);
//		return;
//	}
//
//	
//	// 서버 요청
//	EGsSummonType summonType = GSSummon()->GetSummonType();
//	if (summonType == EGsSummonType::SUMMON_FAIRY)
//	{
//		FGsNetSendServiceWorld::SendReqFairySummonMileage();
//	}
//	else
//	{
//		FGsNetSendServiceWorld::SendReqCostumeSummonMileage();
//	}
//
//}
//// 보상 아이콘 롱프레스
//void UGsUITraySummon::OnLongPressMileageReward(UGsUIIconItem& InIcon)
//{
//	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
//}

// 보상 데이터 세팅
void UGsUITraySummon::SetRewardData(int In_rewardItemId, int In_rewardItemCount)
{
	_rewardItemTblId = In_rewardItemId;
	_rewardItemCount = In_rewardItemCount;

	//_targetSummonMileageIconSlotHelper->RefreshAll(1);
}

// 마일리지 제목 텍스트 세팅(코스튬, 페어리)
void UGsUITraySummon::SetMileageTitleText()
{
	EGsSummonType summonType = GSSummon()->GetSummonType();
	
	FText findText;
	FText::FindText(TEXT("SummonText"), 
		(summonType == EGsSummonType::SUMMON_FAIRY)? 
		TEXT("SummonMileage_Fairy"): TEXT("SummonMileage_Costume"), findText);

	_textMileageTitle = findText;
}
// 마일리지 보상 텍스트 세팅(획득 가능, 불가)
void UGsUITraySummon::SetMileageRewardInfoText(bool In_isRewardOn)
{
	FText findText;
	FText::FindText(TEXT("SummonText"),
		(In_isRewardOn == true) ?
		TEXT("SummonMileage_Guide2") : TEXT("SummonMileage_Guide1"), findText);

	_textMileageRewardInfo = findText;
}

// 마일리지 데이터 세팅
void UGsUITraySummon::SetMileageData(Currency In_mileage, int In_itemCount, bool In_isProgressAnim)
{
	EGsSummonOriginContentType originContentType =  GSSummon()->GetSummonOriginContent();

	if(originContentType == EGsSummonOriginContentType::SUMMON_ITEM )
	{ 
		// 리워드 세팅
		const FGsSchemaItemCommon* rewardItemTbl = GSSummon()->FindSummonMileageRewardItemTable();
		if (rewardItemTbl == nullptr)
		{
			return;
		}

		int itemId = rewardItemTbl->id;	
		int maxSummonMileage = GSSummon()->GetMaxSummonMileage();

		SetRewardData(itemId, In_itemCount);

		// max치 를 나눈 나머지가 나와야 되므로
		Currency progressMileage = In_mileage - (In_itemCount * maxSummonMileage);

		// 프로그래스바 세팅
		_summonMileageProgressBar->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { int32(progressMileage), int32(maxSummonMileage) })));
		//_summonMileageProgressBar->SetPercent(FMath::Clamp<float>(float(In_mileage) / maxSummonMileage, 0.f, 1.0f));
		//_summonMileageProgressBar->SetSecondPercent(0.f);

		_summonMileageProgressBar->SetProgress(FMath::Clamp<float>(float(progressMileage) / maxSummonMileage, 0.f, 1.0f), 0.0f, In_isProgressAnim);


		// 타이틀 텍스트 세팅
		SetMileageTitleText();
		// 정보 세팅

		bool epicRewardExist = (In_itemCount > 0) ? true : false;
		SetMileageRewardInfoText(epicRewardExist);

		EGsSummonType summonType = GSSummon()->GetSummonType();
		bool isPossibleUniqueMileageReward = GSSummon()->IsPossibleUniqueMileageRewardByType(summonType);

		// 에픽 or 유니크 보상이 있는지
		ESlateVisibility rewardEffectVisibility = 
			(epicRewardExist == true || isPossibleUniqueMileageReward == true)? 
				ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
		_mileageRewardEffectWidget->SetVisibility(rewardEffectVisibility);

		// 전체 visible 세팅
		_mileageVisibleWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		// 전체 visible 세팅
		_mileageVisibleWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUITraySummon::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);
	// 유니크꺼도 체크
	if (targetCurrency != CurrencyType::SUMMON_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_FAIRY_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE&&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE
		)
	{
		return;
	}
	SetNowMileageData();
}

// 현재 마일리지 데이터로 세팅
void UGsUITraySummon::SetNowMileageData()
{
	int itemCount = GSSummon()->GetSummonMileageRewardItemCountCaculated();

	EGsSummonType summonType = GSSummon()->GetSummonType();

	CurrencyType currencyType =
		(summonType == EGsSummonType::SUMMON_FAIRY) ?
		CurrencyType::SUMMON_FAIRY_MILEAGE : CurrencyType::SUMMON_COSTUME_MILEAGE;
	Currency currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyType);

	SetMileageData(currentCurrency, itemCount, true);
}

// 최초에 마일리지 관련 위젯 hide 하기
void UGsUITraySummon::SetMileageWidgetHide()
{
	// 전체 visible 세팅
	_mileageVisibleWidget->SetVisibility(ESlateVisibility::Collapsed);
}

// 카드 애니메이션 unbind 처리
// 빠른 연타로 다시뽑기로 넘어갔는데 카드 애니메이션 콜백이 호출되어 버튼이 보이는 이슈 수정
// clear 시점에 bind 된 animation callback 정리하기
// https://jira.com2us.com/jira/browse/C2URWQ-6044
void UGsUITraySummon::UnbindCardAnimationFunction()
{
	int dataIndex = 0;
	for (; dataIndex < _countCard; ++dataIndex)
	{
		if (_arrayNowUseCardItem.Num() <= dataIndex)
		{
			return;
		}

		UGsUISummonCardItem* startAnimationItem = _arrayNowUseCardItem[dataIndex];
		if (startAnimationItem == nullptr)
		{
			continue;
		}
		
		startAnimationItem->UnbindAnimation();
	}
}

void UGsUITraySummon::OnClickOpenPopupSummonMileage()
{
	// 1. 모든카드 오픈 되었는지
	if (_isLastStep == false)
	{
		FText findText;
		// 소환 결과를 확인해야 보상을 획득할 수 있습니다.
		FText::FindText(TEXT("SummonText"), TEXT("SummonMileage_NotYet"), findText);
		FGsUIHelper::TrayMessageTicker( findText);
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->OpenAndGetWidget(TEXT("PopupSummonMileage"));

	if (wigdet.IsValid() == false)
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupSummonMileage> popupSummonMileage = Cast<UGsUIPopupSummonMileage>(wigdet);

	EGsSummonType summonType = GSSummon()->GetSummonType();
	popupSummonMileage->InitSummonMileage(summonType);
}

// 소환 마일리지 데이터 갱신
void UGsUITraySummon::OnChangedSummonMileageData(const  IGsMessageParam* inParam)
{
	SetNowMileageData();
}