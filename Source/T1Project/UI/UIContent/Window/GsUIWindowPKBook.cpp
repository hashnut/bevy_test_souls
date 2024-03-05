
#include "GsUIWindowPKBook.h"
#include "PKBook/GsPKBookData.h"
#include "PKBook/GsPKRecordData.h"
#include "PKBook/GsPKBookHelper.h"
#include "PKBook/GsUIPKBookRecordDataEntry.h"
#include "PKBook/GsUIPKTargetDetectionDataEntry.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"

#include "../Message/GsMessageContents.h"
#include "../Helper/GsUIHelper.h"

#include "TextBlock.h"
#include "ScrollBox.h"
#include "WidgetSwitcher.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "../Popup/GsUIPopupPKBookRecordDetail.h"
#include "../Popup/GsUIPopupPKBookPKMessageSetting.h"
#include "../Popup/PKBook/GsUIPopupPKBookAddBoundaryTarget.h"
#include "../Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "DataCenter/Public/DataSchema/PKBook/GsSchemaPKBookConfig.h"

#include "../Net/GsNetSendServiceWorld.h"




void UGsUIWindowPKBook::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::PKBook;

	// 상단 버튼
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowPKBook::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowPKBook::OnClickCloseAllStack);

	_slotHelperRecordData = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperRecordData->Initialize(_subClassOfRecordDataEntry, _scrollBoxRecordSet);
	_slotHelperRecordData->OnRefreshEntry.AddDynamic(this, &UGsUIWindowPKBook::OnRefreshEntryRecordData);

	_slotHelperDetectionTargetData = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperDetectionTargetData->Initialize(_subClassOfDetectionTargetDataEntry, _scrollBoxDetectionTargetSet);
	_slotHelperDetectionTargetData->OnRefreshEntry.AddDynamic(this, &UGsUIWindowPKBook::OnRefreshEntryDetectionTarget);

	_toggleGroupRecordDataFilter.OnSelectChanged.BindUObject(this, &UGsUIWindowPKBook::OnSelectedBtnFilter);

	_btnCheckOnlyLogIn->OnSwitcherButtonSelected.AddDynamic(this, &UGsUIWindowPKBook::OnSelectedDetectionFilter);
	_btnCheckOnlyLogIn->OnClicked.AddDynamic(this, &UGsUIWindowPKBook::OnClickedDetectionFilter);

	_btnPKMessageSetting->OnClicked.AddDynamic(this, &UGsUIWindowPKBook::OnClcikedOpenPopupMessageSetting);

	_btnAddBoundaryTarget->OnClicked.AddDynamic(this, &UGsUIWindowPKBook::OnClcikedAddBoundaryTarget);

	int32 maxRecordListCount = 0;
	if (FGsPKBookManager* PKBookManager = GsPKBook())
	{
		const FGsSchemaPKBookConfig* config = PKBookManager->GetConfig();
		if (config)
		{
			maxRecordListCount = config->maximumCountOfPKRecord;
		}
	}
	
	FText format;
	FGsPKBookHelper::FindText(TEXT("Dest_RecordList_Count"), format);
	
	// 2022/09/22 PKT - "PK 기록은 최근 {0}개까지 표시됩니다."
	FText message = FText::Format(format, maxRecordListCount);
	_textBoxDestPKCount->SetText(message);
}

void UGsUIWindowPKBook::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == IsStackRecoverFlag())
	{

	}
	
	// 2022/09/22 PKT - Filter
	_toggleGroupRecordDataFilter.Clear();
	_toggleGroupRecordDataFilter.AddToggleWidget(_toggleAll);
	_toggleGroupRecordDataFilter.AddToggleWidget(_toggleKill);
	_toggleGroupRecordDataFilter.AddToggleWidget(_toggleAssist);
	_toggleGroupRecordDataFilter.AddToggleWidget(_toggleDead);

	static const int32 DEFAULT_RECORD_DATA_SET_FILTER = 0;
	_toggleGroupRecordDataFilter.SetSelectedIndex(DEFAULT_RECORD_DATA_SET_FILTER, true);

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	_btnCheckOnlyLogIn->SetIsSelected(PKBookManager->IsShowLogInDetectionTargetOnly());

	_contentsDelegate = GMessage()->GetContents().AddUObject(MessageContents::CLOSE_WINDOW_PK_BOOK, this, &UGsUIWindowPKBook::OnCloseWindow);
}

void UGsUIWindowPKBook::NativeDestruct()
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	if (_btnCheckOnlyLogIn->GetIsSelected() != PKBookManager->IsShowLogInDetectionTargetOnly())
	{	// 2022/10/06 PKT - 옵션이 달라 졌다면 서버에 저장		
		FGsNetSendServiceWorld::Send_ReqPKBookSavePKMessage(
			_btnCheckOnlyLogIn->GetIsSelected()
			, PKBookManager->IsPKMessageActive()
			, PKBookManager->GetTeasePKMessage()
			, PKBookManager->GetRevengePKMessage()
		);
	}

	GMessage()->GetContents().Remove(_contentsDelegate);

	Super::NativeDestruct();
}

bool UGsUIWindowPKBook::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsPKBook))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_PK_BOOK);
		return true;
	}

	return false;
}

void UGsUIWindowPKBook::BeginDestroy()
{
	// 2021/11/23 PKT - Event UnBind
	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowPKBook::OnClickBack);
		_btnBack = nullptr;
	}

	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowPKBook::OnClickCloseAllStack);
		_btnClose = nullptr;
	}

	if (_slotHelperRecordData)
	{
		_slotHelperRecordData->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowPKBook::OnRefreshEntryRecordData);
		_slotHelperRecordData = nullptr;
	}

	if (_slotHelperDetectionTargetData)
	{
		_slotHelperDetectionTargetData->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowPKBook::OnRefreshEntryDetectionTarget);
		_slotHelperDetectionTargetData = nullptr;
	}

	if (_btnCheckOnlyLogIn)
	{
		_btnCheckOnlyLogIn->OnSwitcherButtonSelected.RemoveDynamic(this, &UGsUIWindowPKBook::OnSelectedDetectionFilter);
		_btnCheckOnlyLogIn->OnClicked.RemoveDynamic(this, &UGsUIWindowPKBook::OnClickedDetectionFilter);
		_btnCheckOnlyLogIn = nullptr;
	}

	if (_btnPKMessageSetting)
	{
		_btnPKMessageSetting->OnClicked.RemoveDynamic(this, &UGsUIWindowPKBook::OnClcikedOpenPopupMessageSetting);
		_btnPKMessageSetting = nullptr;
	}		
	
	Super::BeginDestroy();
}

void UGsUIWindowPKBook::BuildRecordDataViewList(EPKBookFilter InFilter)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const int32 maxCount = PKBookManager->MakeRecordDataViewSet(InFilter);

	const static int32 EMPTY_COUNT = 0;
	if (EMPTY_COUNT >= maxCount)
	{
		const static int32 SWITCHER_EMPTY_INDEX = 1;
		_switcherRecordList->SetActiveWidgetIndex(SWITCHER_EMPTY_INDEX);
	}
	else
	{
		const static int32 SWITCHER_ENOUGHT_INDEX = 0;
		_switcherRecordList->SetActiveWidgetIndex(SWITCHER_ENOUGHT_INDEX);

		_slotHelperRecordData->RefreshAll(maxCount);
	}	
}

void UGsUIWindowPKBook::BuildTargetDetectionViewList(EPKDetectionTargetFilter InFilter)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const int32 setCount = PKBookManager->MakeDetectionTargetViewSet(InFilter);
	const static int32 EMPTY_COUNT = 0;
	if (EMPTY_COUNT >= setCount)
	{
		const static int32 SWITCHER_EMPTY_INDEX = 1;
		_switcherDetectionTarget->SetActiveWidgetIndex(SWITCHER_EMPTY_INDEX);
	}
	else
	{
		const static int32 SWITCHER_ENOUGHT_INDEX = 0;
		_switcherDetectionTarget->SetActiveWidgetIndex(SWITCHER_ENOUGHT_INDEX);

		_slotHelperDetectionTargetData->RefreshAll(setCount);
	}
	

	// 2022/09/23 PKT - Count
	const FGsSchemaPKBookConfig* config = PKBookManager->GetConfig();
	const int32 maximumCountOfDetectionTarget = (nullptr == config) ? 0 : config->maximumCountOfDetectionTarget;

	FText text = FText::FromString(FString::Format(TEXT("{0} / {1}"), { setCount, maximumCountOfDetectionTarget }));
	_textBlockDetectionTargetCount->SetText(text);

	bool isAddBoundaryTarget = (PKBookManager->GetDetectionTargetCountAll() < maximumCountOfDetectionTarget) ? true : false;
	_btnAddBoundaryTarget->SetIsEnabled(isAddBoundaryTarget);
}

void UGsUIWindowPKBook::OnTryOenpPopupRecordDetail(PKRecordId InPKRecordId)
{
	if (INVALID_PK_RECORD_ID == InPKRecordId)
	{
		GSLOG(Error, TEXT("INVALID_PK_RECORD_ID == InPKRecordId"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}
	
	const FGsPKRecordData* PKRecordData = PKBookManager->FindPKRecordDataByPKRecordId(InPKRecordId);
	if (nullptr == PKRecordData)
	{
		GSLOG(Error, TEXT("nullptr == PKRecordData"));
		return;
	}
	
	if (false == PKRecordData->IsDetailDataCaching())
	{
		// 2022/09/23 PKT - 캐싱 된 디테일 Data가 없으면 서버에 요청
		FGsNetSendServiceWorld::Send_ReqPKBookDetailInfo(InPKRecordId);
	}
	else
	{
		OenpPopupRecordDetail(PKRecordData);
	}
}

void UGsUIWindowPKBook::OenpPopupRecordDetail(const class FGsPKRecordData* InPKRecordData)
{
	if (nullptr == InPKRecordData)
	{
		GSLOG(Error, TEXT("nullptr == InPKRecordData"));
		return;
	}

	// 2022/06/07 PKT - 안내 팝업창을 띄운다.
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupPKBookDetailInfo"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupPKBookRecordDetail* popup = Cast<UGsUIPopupPKBookRecordDetail>(widget.Get()))
	{	
		popup->SetData(InPKRecordData->GetId());
	}
}

void UGsUIWindowPKBook::RefreshDetailData()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupPKBookDetailInfo"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupPKBookRecordDetail* popup = Cast<UGsUIPopupPKBookRecordDetail>(widget.Get()))
	{
		if (popup->IsActive())
		{	// 2022/09/28 PKT - 디테일 창 갱신
			popup->Invalidate();
		}
	}
}

void UGsUIWindowPKBook::OnRefreshEntryRecordData(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIPKBookRecordDataEntry> entry = Cast<UGsUIPKBookRecordDataEntry>(InEntry);
	if (false == entry.IsValid())
	{
		GSLOG(Warning, TEXT("false == entry.IsValid()"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const TArray<const FGsPKRecordData*> recordDataViewSet = PKBookManager->GetRecordDataViewSet();
	if (false == recordDataViewSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == recordDataViewSet.IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	// 2022/09/22 PKT - PK Record Data
	entry->SetData(recordDataViewSet[InIndex]);

	if (false == entry->OnTryOpenPopUpDelegate.IsBoundToObject(this))
	{	// 2021/12/21 PKT - selected main
		entry->OnTryOpenPopUpDelegate.BindUObject(this, &UGsUIWindowPKBook::OnTryOenpPopupRecordDetail);
	}	
}

void UGsUIWindowPKBook::OnRefreshEntryDetectionTarget(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIPKTargetDetectionDataEntry> entry = Cast<UGsUIPKTargetDetectionDataEntry>(InEntry);
	if (false == entry.IsValid())
	{
		GSLOG(Warning, TEXT("false == entry.IsValid()"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const TArray<const FGsPKDetectionTargetData*>& detectionTargetViewSet = PKBookManager->GetDetectionTargetDataViewSet();
	if (false == detectionTargetViewSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == detectionTargetViewSet.IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	// 2022/09/22 PKT - Detection Target Data
	entry->SetData(detectionTargetViewSet[InIndex]);
}

void UGsUIWindowPKBook::OnSelectedBtnFilter(int32 InIndex)
{
	BuildRecordDataViewList(StaticCast<EPKBookFilter>(InIndex));
}

void UGsUIWindowPKBook::OnSelectedDetectionFilter(bool bIsSelected)
{
	BuildTargetDetectionViewList(StaticCast<EPKDetectionTargetFilter>(bIsSelected));
}

void UGsUIWindowPKBook::OnClickedDetectionFilter()
{
	_btnCheckOnlyLogIn->SetIsSelected(false == _btnCheckOnlyLogIn->GetIsSelected());
}

void UGsUIWindowPKBook::OnClcikedOpenPopupMessageSetting()
{
	// 2022/06/07 PKT - PK Message
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupPKBookPKMessageSetting"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}
}

void UGsUIWindowPKBook::OnClcikedAddBoundaryTarget()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		auto widget = uiMgr->OpenAndGetWidget(TEXT("PopupPKBookAddBoundaryTarget"));
		if (widget.IsValid())
		{
			FText buff;
			UGsUIPopupPKBookAddBoundaryTarget* uiPopup = Cast<UGsUIPopupPKBookAddBoundaryTarget>(widget.Get());
			uiPopup->SetOkCallback([this](WorldId InWorldId, const FString& InNickName)
				{
					OnAddBoundaryTarget(InWorldId, InNickName);
				});
		}
	}
}

void UGsUIWindowPKBook::OnAddBoundaryTarget(WorldId InWorldId, const FString& InNickName)
{
	bool isAlreadyDetectionTarget = GsPKBook()->CheckDetectionTarget(0, InWorldId, FText::FromString(InNickName));

	if (false == isAlreadyDetectionTarget)
	{	// 2023/8/1 PKT - 이름으로 등록하는 것은 유저 DBId를 알수 없다.
		FGsNetSendServiceWorld::Send_ReqPKBookAddDetectionTarget(InWorldId, INVALID_USER_DB_ID, FText::FromString(InNickName));
	}
	else
	{
		// 2023/1/18 PKT - 이미 경계 대상으로 등록된 캐릭터 입니다
		FText msg;
		FGsPKBookHelper::FindText(TEXT("Already_BoundartyTarget"), msg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
	}
}

void UGsUIWindowPKBook::OnCloseWindow()
{
	/**
	 * 복수 하기 눌렀을 경우 윈도우가 자동으로 닫힌다.
	 */
	 // 2022/06/07 PKT - 안내 팝업창을 띄운다.
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupPKBookDetailInfo"));
	if (widget.IsValid())
	{
		widget->Close();
	}

	Close();
}

void UGsUIWindowPKBook::InvalidRecoredDataSet()
{
	int32 selectedIndex = _toggleGroupRecordDataFilter.GetSelectedIndex();
	BuildRecordDataViewList(StaticCast<EPKBookFilter>(selectedIndex));
}

void UGsUIWindowPKBook::OpenRecordDetail(PKRecordId InRecordId)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const FGsPKRecordData* PKRecordData = PKBookManager->FindPKRecordDataByPKRecordId(InRecordId);
	if (nullptr == PKRecordData)
	{
		GSLOG(Error, TEXT("nullptr == PKRecordData"));
		return;
	}

	OenpPopupRecordDetail(PKRecordData);
}

void UGsUIWindowPKBook::InvalidRecordDetail(PKRecordId InRecordId)
{
	if (INVALID_PK_RECORD_ID == InRecordId)
	{
		return;
	}

	RefreshDetailData();
}

void UGsUIWindowPKBook::InvalidDetectionTarget(UserDBId InUserDBId)
{
	// 2022/09/28 PKT - 경계 대상 로그인/로그 아웃 했을 시 옴. Filter옵션에 따라 리스트를 다시 만들어 줘야 하는 경우도 있으니 전체 목록을 갱신
	BuildTargetDetectionViewList(StaticCast<EPKDetectionTargetFilter>(_btnCheckOnlyLogIn->GetIsSelected()));
}

void UGsUIWindowPKBook::InvalidDetectionTargetSet()
{
	// 2022/09/28 PKT - 경계 대상이 추가 되거나 석제 되면 디테일 창에 있는 경계대상 목록 애들도 갱신해야 함.
	RefreshDetailData();

	// 2022/09/28 PKT - 전체 목록 갱신
	BuildTargetDetectionViewList(StaticCast<EPKDetectionTargetFilter>(_btnCheckOnlyLogIn->GetIsSelected()));
}