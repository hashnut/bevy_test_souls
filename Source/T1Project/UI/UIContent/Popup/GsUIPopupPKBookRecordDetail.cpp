

#include "GsUIPopupPKBookRecordDetail.h"
#include "../PKBook/GsPKRecordData.h"
#include "../PKBook/GsPKBookData.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../Guild/GsGuildData.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "GsUIPopupPKBookTryToRevenge.h"
#include "GsUIPopupPKBookTryToTease.h"
#include "PKBook/GsUIPKBookDetailContributorEntry.h"

#include "DataCenter/Public/DataSchema/PKBook/GsSchemaPKBookConfig.h"

#include "Image.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "PaperSprite.h"
#include "HorizontalBox.h"
#include "WidgetSwitcher.h"
#include "Classes/Engine/Texture2D.h"
#include "../Common/GsUICurrencyButton.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../Helper/GsUIHelper.h"
#include "../UTIL/GsText.h"

#include "T1Project.h"


void UGsUIPopupPKBookRecordDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 2022/09/26 PKT - Close
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupPKBookRecordDetail::OnClose);
	// 2022/09/26 PKT - Action On
	_btnButtonCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupPKBookRecordDetail::OnClickedAction);

	_slotHelperContributorSetData = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperContributorSetData->Initialize(_subClassOfContributorDataEntry, _verticalBoxContributorSet);
	_slotHelperContributorSetData->OnRefreshEntry.AddDynamic(this, &UGsUIPopupPKBookRecordDetail::OnRefreshEntryContributorData);
}

void UGsUIPopupPKBookRecordDetail::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookRecordDetail::OnClose);
		_btnClose = nullptr;
	}

	if (_slotHelperContributorSetData)
	{
		_slotHelperContributorSetData->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupPKBookRecordDetail::OnRefreshEntryContributorData);
		_slotHelperContributorSetData = nullptr;
	}
	
	Super::BeginDestroy();
}

void UGsUIPopupPKBookRecordDetail::NativeConstruct()
{
	Super::NativeConstruct();

	_targetId = INVALID_PK_RECORD_ID;
}

void UGsUIPopupPKBookRecordDetail::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (false == IsActionEnableState())
	{	// 2022/09/23 PKT - 업뎃 필요 없음.
		return;
	}

	if (const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(_targetId))
	{
		// 2022/09/26 PKT - 대응 시간 업데이트
		static const int32 SWITCHER_SPAN_TIME_INDEX = 0;
		if (SWITCHER_SPAN_TIME_INDEX == _switcherActionTime->GetActiveWidgetIndex())
		{
			SetTextActionTime(recordData->GetOccurrenceTime());
		}
	}
}

void UGsUIPopupPKBookRecordDetail::OnClose()
{
	Close();
}

void UGsUIPopupPKBookRecordDetail::OnClickedAction()
{
	const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(_targetId);
	if (nullptr == recordData)
	{
		GSLOG(Error, TEXT("nullptr == recordData"));
		return;
	}

	if (PKBookResult::DIE == recordData->GetReason())
	{
		// 2022/06/07 PKT - 복수 창을 띄운다.
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupPKBookTryToRevenge"));
		if (!widget.IsValid())
		{
			GSLOG(Error, TEXT("!widget.IsValid()"));
			return;
		}

		if (UGsUIPopupPKBookTryToRevenge* popup = Cast<UGsUIPopupPKBookTryToRevenge>(widget.Get()))
		{	
			popup->SetData(recordData->GetId(), &recordData->GetMainContributorData());
		}
	}
	else
	{
		// 2022/06/07 PKT - 조롱 창을 띄운다.
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupPKBookTryToTease"));
		if (!widget.IsValid())
		{
			GSLOG(Error, TEXT("!widget.IsValid()"));
			return;
		}

		if (UGsUIPopupPKBookTryToTease* popup = Cast<UGsUIPopupPKBookTryToTease>(widget.Get()))
		{	
			popup->SetData(recordData->GetId(), &(recordData->GetMainContributorData()), GsPKBook()->IsUseMockeryMessage(), GsPKBook()->GetMockeryMessage());
		}
	}
}

void UGsUIPopupPKBookRecordDetail::OnRefreshEntryContributorData(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIPKBookDetailContributorEntry> entry = Cast<UGsUIPKBookDetailContributorEntry>(InEntry);
	if (false == entry.IsValid())
	{
		GSLOG(Warning, TEXT("false == entry.IsValid()"));
		return;
	}

	const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(_targetId);
	if (nullptr == recordData)
	{
		GSLOG(Error, TEXT("nullptr == recordData"));
		return;
	}

	const FGsPKDetailRecordData* detailData = (nullptr != recordData) ? recordData->GetDetailData() : nullptr;
	if (nullptr == detailData || false == detailData->_contributorSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("nullptr == detailData || false == detailData->_contributorSet.IsValidIndex(InIndex)"));
		return;
	}

	entry->SetData(recordData->GetReason() , &detailData->_contributorSet[InIndex]);
}

void UGsUIPopupPKBookRecordDetail::SetMainContributorData(const FGsPKRecordData* InData)
{
	if (nullptr == InData || false == InData->IsDetailDataCaching())
	{
		GSLOG(Error, TEXT("nullptr == InData || false == InData->IsDetailDataCaching()"));
		return;
	}

	/**
	 * 기록 사유가 사망이라면 나의 정보를.. 사망이 아니라면 서버에서 보내준 메인 정보를 토대로 찍어야 함.
	 */
	const FGsPKRecordingContributorData& mainContributorData = InData->GetMainContributorData();

	FText deadName = mainContributorData._name;
	GuildEmblemId guildEmblemId = mainContributorData._guildEmblemId;
	FText guildName = mainContributorData._guildName;
	WorldId worldId = mainContributorData._homeWorldId;

	if (PKBookResult::DIE == InData->GetReason())
	{	// 2022/10/05 PKT - 기록 사유가 사망이라면 나의 정보로 치환한다.
		FGsGameDataManager* gameDataManager = GGameData();
		if (nullptr == gameDataManager)
		{
			GSLOG(Error, TEXT("nullptr == gameDataManager"));
			return;
		}

		// 2022/10/05 PKT -  내 이름
		deadName = FText::FromString(gameDataManager->GetUserData()->mUserName);
		worldId = INVALID_WORLD_ID;

		// 2022/10/05 PKT - 길드 아이콘 정보
		if (GGuild() && GGuild()->IsGuild() && GGuild()->GetMyGuildData())
		{	// 2022/10/06 PKT - 길드에 가입되어 있고, 길드 데이터가 있는 경우
			guildEmblemId = GGuild()->GetMyGuildData()->_emblemId;
			guildName = FText::FromString(GGuild()->GetMyGuildData()->_guildName);
		}
		else
		{
			guildEmblemId = INVALID_GUILD_EMBLEM_ID;
			guildName = FText::GetEmpty();
		}		
	}
		
	// 2022/09/26 PKT - 이름.
	_textBlockName->SetText(FText::FromString(FGsTextUtil::CombineUserAndPrefix(deadName, worldId)));

	// 2022/09/26 PKT - 길드 정보
	if (INVALID_GUILD_EMBLEM_ID != guildEmblemId && false == guildName.IsEmpty())
	{
		// 2022/09/22 PKT - 길드 정보
		_iconGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockGuildName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_textBlockGuildName->SetText(guildName);

		FSoftObjectPath path;
		if (FGsPKBookHelper::FindPathGuildIcon(guildEmblemId, true, path))
		{
			_iconGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
		}
	}
	else
	{
		_iconGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
		_textBlockGuildName->SetVisibility(ESlateVisibility::Hidden);
	}

	// 2022/09/26 PKT - 손실 경험치
	if (PKBookResult::DIE == InData->GetReason())
	{	// 2022/10/06 PKT - 내가 사망 할 경우 손실 경험치를 표기 한다.
		_horizontalBoxExpInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText message = FText::FromString(
			FString::Printf(TEXT("%s(%0.2f%%)"), *FText::AsNumber(InData->GetLostExp()).ToString(), InData->GetLostExpPercent())
		);

		_textBlockPaneltyExp->SetText(message);
	}
	else
	{	// 2022/10/06 PKT - 내가 사망하지 않았을 경우 손실 경험치를 표시 하지 않는다.
		_horizontalBoxExpInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UGsUIPopupPKBookRecordDetail::SetCurrecntBtn(const FGsPKRecordData* InData)
{
	if (nullptr == InData || false == InData->IsDetailDataCaching())
	{
		GSLOG(Error, TEXT("nullptr == InData || false == InData->IsDetailDataCaching()"));
		return;
	}
		
	// 2022/09/26 PKT - 매니저 검사
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	CurrencyType type;
	Currency amount;
	PKBookManager->ActionCostInfo(InData->GetReason(), type, amount);

	FText text;
	if (PKBookResult::DIE == InData->GetReason())
	{	// 2022/09/27 PKT - "복수"
		FGsPKBookHelper::FindText(TEXT("Popup_Detail_Btn_Revenge"), text);
	}
	else
	{	// 2022/09/27 PKT - "조롱"
		FGsPKBookHelper::FindText(TEXT("Popup_Detail_Btn_Tease"), text);
	}

	_btnButtonCurrency->SetData(type, amount);
	_btnButtonCurrency->SetButtonName(text);
}

void UGsUIPopupPKBookRecordDetail::SetTextActionCount(int32 InRemaindCount)
{
	if (0 < InRemaindCount)
	{
		static const int32 SWITCHER_LIMIT_COUNT_INDEX = 0;
		_switcherActionCount->SetActiveWidgetIndex(SWITCHER_LIMIT_COUNT_INDEX);

		FText format;
		FGsPKBookHelper::FindText(TEXT("Popup_Detail_Limit_Action_Count_Dest"), format);
		FText message = FText::Format(format, InRemaindCount);
		_textBlockLimitActionCount->SetText(message);
	}
	else
	{	// 2022/09/26 PKT - 횟수 제한
		static const int32 SWITCHER_OVER_COUNT_INDEX = 1;
		_switcherActionCount->SetActiveWidgetIndex(SWITCHER_OVER_COUNT_INDEX);

		// 2022/09/26 PKT - 비용 버튼 비활성화
		SetDisableCurrecntBtn(true);
	}
}

void UGsUIPopupPKBookRecordDetail::SetTextActionTime(const time_t& InOccurrenceTime)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	int64 availableTimeForAction = (nullptr != PKBookManager->GetConfig()) ? PKBookManager->GetConfig()->availableTimeForAction : 0;
	
	int64 elapsedTime = FGsTimeSyncUtil::GetServerNowSecond() - InOccurrenceTime;

	int64 limitActionTime = availableTimeForAction <= elapsedTime ? 0 : (availableTimeForAction - elapsedTime);
	
	static const int32 SWITCHER_SPAN_TIME_INDEX = 0;
	static const int32 SWITCHER_OVER_TIME_INDEX = 1;

	if (0 < limitActionTime)
	{
		if (SWITCHER_OVER_TIME_INDEX == _switcherActionTime->GetActiveWidgetIndex())
		{
			_switcherActionTime->SetActiveWidgetIndex(SWITCHER_SPAN_TIME_INDEX);
		}

		FDateTime remaindTime(limitActionTime * ETimespan::TicksPerSecond);

		FText format;
		FGsPKBookHelper::FindText(TEXT("Popup_Detail_Action_LimitTime"), format);
		
		FText text = FText::Format(format, { remaindTime.GetHour(), remaindTime.GetMinute(), remaindTime.GetSecond()});		
		_textBlockLimitActionTime->SetText(text);
	}
	else
	{			
		if (SWITCHER_SPAN_TIME_INDEX == _switcherActionTime->GetActiveWidgetIndex())
		{	
			// 2022/09/28 PKT - 시간 만료
			_switcherActionTime->SetActiveWidgetIndex(SWITCHER_OVER_TIME_INDEX);
		}

		SetDisableCurrecntBtn(true);

		// 2022/10/18 PKT - 시간이 만료 되면 열려 있던 재화 팝업창을 꺼준다.
		TWeakObjectPtr<UGsUIWidgetBase> widgetRevenge = GUI()->GetWidgetByKey(TEXT("PopupPKBookTryToRevenge"), true);
		if (UGsUIPopupPKBookTryToActionBase* popup = Cast<UGsUIPopupPKBookTryToActionBase>(widgetRevenge.Get()))
		{
			popup->Close();
		}

		// 2022/10/18 PKT - 시간이 만료 되면 열려 있던 재화 팝업창을 꺼준다.
		TWeakObjectPtr<UGsUIWidgetBase> widgetTease = GUI()->GetWidgetByKey(TEXT("PopupPKBookTryToTease"), true);
		if (UGsUIPopupPKBookTryToActionBase* popup = Cast<UGsUIPopupPKBookTryToActionBase>(widgetTease.Get()))
		{
			popup->Close();
		}
	}
}

void UGsUIPopupPKBookRecordDetail::SetDisableCurrecntBtn(bool InIsDisable)
{
	if (InIsDisable == _btnButtonCurrency->GetIsEnabled())
	{
		_btnButtonCurrency->SetIsEnabled(false == InIsDisable);
	}	
}

// 2022/09/27 PKT - 대응을 할 수 있는 상황인가?
bool UGsUIPopupPKBookRecordDetail::IsActionEnableState() const
{
	const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(_targetId);
	if (nullptr == recordData)
	{
		GSLOG(Error, TEXT("nullptr == recordData"));
		return false;
	}
	
	const FGsPKRecordingContributorData& detailData = recordData->GetMainContributorData();
	if (PKContributorType::NPC == detailData._contributorType)
	{	// 2022/09/27 PKT - 메인 기여도가 NPC라면 대응 할 수 없다.
		return false;
	}

	// 2022/09/27 PKT - 자신이 누군가를 죽였거나(Kill) / 누군가가 나를 죽였거나(Die)
	return (PKBookResult::KILL == recordData->GetReason() || PKBookResult::DIE == recordData->GetReason());
}

void UGsUIPopupPKBookRecordDetail::SetData(PKRecordId InRecordId)
{
	if (INVALID_PK_RECORD_ID >= InRecordId)
	{
		GSLOG(Error, TEXT("INVALID_PK_RECORD_ID >= InRecordId"));
		return;
	}

	const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(InRecordId);
	if (nullptr == recordData)
	{
		GSLOG(Error, TEXT("nullptr == recordData"));
		return;
	}

	_targetId = InRecordId;

	SetMainContributorData(recordData);

	const FGsPKDetailRecordData* detailData = recordData->GetDetailData();
	if (detailData)
	{
		const int32 maxCount = detailData->_contributorSet.Num();
		_slotHelperContributorSetData->RefreshAll(maxCount);
	}

	if (IsActionEnableState())
	{	// 2022/09/27 PKT - 대응 할 수 있는 상황에서만 대응 정보를 세팅한다.
		_verticalBoxStatePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		SetCurrecntBtn(recordData);

		// 2022/09/27 PKT - 버튼 활성화(Default)
		SetDisableCurrecntBtn(false);

		// 2022/09/27 PKT - 대응 시간 셋팅
		SetTextActionTime(recordData->GetOccurrenceTime());

		// 2022/09/27 PKT - 대응 횟수 셋팅
		SetTextActionCount(recordData->GetActionRemaindCount());
	}
	else
	{
		// 2022/09/27 PKT - 대응 관련 정보 숨기기..
		_verticalBoxStatePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupPKBookRecordDetail::Invalidate()
{
	const FGsPKRecordData* recordData = GsPKBook()->FindPKRecordDataByPKRecordId(_targetId);
	if (nullptr == recordData)
	{
		GSLOG(Error, TEXT("nullptr == recordData"));
		return;
	}

	const FGsPKDetailRecordData* detailData = recordData->GetDetailData();
	if (detailData)
	{	// 2022/09/27 PKT - 기여도 정보 갱신
		const int32 maxCount = detailData->_contributorSet.Num();
		_slotHelperContributorSetData->RefreshAll(maxCount);
	}

	if (IsActionEnableState())
	{
		SetTextActionCount(recordData->GetActionRemaindCount());
	}
}

PKRecordId UGsUIPopupPKBookRecordDetail::GetPKRecordId() const
{
	return _targetId;
}

void UGsUIPopupPKBookRecordDetail::OnInputCancel()
{
	OnClose();
}