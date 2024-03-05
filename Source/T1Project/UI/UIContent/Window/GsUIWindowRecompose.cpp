#include "GsUIWindowRecompose.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Item/GsItemManager.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/Recompose/GsUIRecomposeListEntry.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Net/GsNetSendServiceItem.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsSummonManager.h"


void UGsUIWindowRecompose::BeginDestroy()
{
	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowRecompose::OnClickCloseAllStack);
	}
	_btnClose = nullptr;

	if (nullptr != _btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowRecompose::OnClickBack);
	}
	_btnBack = nullptr;

	if (nullptr != _btnRecompose)
	{
		_btnRecompose->OnClicked.RemoveDynamic(this, &UGsUIWindowRecompose::OnClickRecompose);
	}
	_btnRecompose = nullptr;

	if (nullptr != _slotHelperGradeSlot)
	{
		_slotHelperGradeSlot->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRecompose::OnRefreshGradeSlot);
	}
	_slotHelperGradeSlot = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowRecompose::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowRecompose::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowRecompose::OnClickBack);
	_btnRecompose->OnClicked.AddDynamic(this, &UGsUIWindowRecompose::OnClickRecompose);

	_slotHelperGradeSlot = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperGradeSlot)
	{
		_slotHelperGradeSlot->Initialize(_subClassOfGradeSlot, _panelGradeList);
		_slotHelperGradeSlot->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRecompose::OnRefreshGradeSlot);
	}
}

void UGsUIWindowRecompose::NativeConstruct()
{
	Super::NativeConstruct();

	_selectedGradeSlot.Reset();
	_btnRecompose->SetIsEnabled(false);

	FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
	if (deliveryBoxData.IsActiveFairy())
	{
		SetFairyData();
	}
	else if (deliveryBoxData.IsActiveCostume())
	{
		SetCostumeData();
	}
	else
	{
		OnClickCloseAllStack();
	}

	_msgFairyHandleList.Emplace(GMessage()->GetFairy().AddUObject(MessageFairy::SUMMON_COMPLETE,
		this, &UGsUIWindowRecompose::OnSummonComplete));

	_msgCostumeHandleList.Emplace(GMessage()->GetCostume().AddUObject(MessageCostume::SUMMON_COMPLETE,
		this, &UGsUIWindowRecompose::OnSummonComplete));
}

void UGsUIWindowRecompose::NativeDestruct()
{
	auto& fairyMsgManager = GMessage()->GetFairy();
	for (auto handler : _msgFairyHandleList)
	{
		fairyMsgManager.Remove(handler);
	}

	auto& costumeMsgManager = GMessage()->GetCostume();
	for (auto handler : _msgCostumeHandleList)
	{
		costumeMsgManager.Remove(handler);
	}

	Super::NativeDestruct();
}

bool UGsUIWindowRecompose::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsRecompose))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_RECOMPOSE);
		return true;
	}

	return false;
}

void UGsUIWindowRecompose::SetCostumeData()
{
	FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();

	_recomposeType = RecomposeType::COSTUME;

	// Window 상단 타이틀 텍스트 설정
	FText titleText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Costume_Title"), titleText);
	_textBlockTitle->SetText(titleText);

	// 버튼 텍스트 설정
	FText repairText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Costume_Button_Repair"), repairText);
	_textBlockRecompose->SetText(repairText);

	// 기간 텍스트 설정
	FDateTime startTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetStartTime());
	FDateTime endTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetEndTime());

	FString beginDateText;
	FString endDateText;
	FGsTimeStringHelper::GetTimeStringNYMDHM(startTime, beginDateText);
	FGsTimeStringHelper::GetTimeStringNYMDHM(endTime, endDateText);
	FText dateText = FText::FromString(FString::Format(TEXT("{0} ~ {1}"), { *beginDateText, *endDateText }));

	FText noticeText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Costume_Notice"), noticeText);
	noticeText = FText::Format(noticeText, dateText);
	_textBlockNotice->SetText(noticeText);

	// 데이터 설정 및 갱신
	_validCostumeGradeList = deliveryBoxData.GetRecomposableCostumeList();
	deliveryBoxData.GetCostumeGradeRange(_minCostumeGrade, _maxCostumeGrade);
	uint8 gradeCount = static_cast<uint8>(_maxCostumeGrade) - static_cast<uint8>(_minCostumeGrade) + 1;

	_slotHelperGradeSlot->RefreshAll(gradeCount);
}

void UGsUIWindowRecompose::SetFairyData()
{
	FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();

	_recomposeType = RecomposeType::FAIRY;

	// Window 상단 타이틀 텍스트 설정
	FText titleText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Fairy_Title"), titleText);
	_textBlockTitle->SetText(titleText);

	// 버튼 텍스트 설정
	FText repairText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Fairy_Button_Repair"), repairText);
	_textBlockRecompose->SetText(repairText);

	// 기간 텍스트 설정
	FDateTime startTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetStartTime());
	FDateTime endTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(deliveryBoxData.GetEndTime());

	FString beginDateText;
	FString endDateText;
	FGsTimeStringHelper::GetTimeStringNYMDHM(startTime, beginDateText);
	FGsTimeStringHelper::GetTimeStringNYMDHM(endTime, endDateText);
	FText dateText = FText::FromString(FString::Format(TEXT("{0} ~ {1}"), { *beginDateText, *endDateText }));

	FText noticeText;
	FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Fairy_Notice"), noticeText);
	noticeText = FText::Format(noticeText, dateText);
	_textBlockNotice->SetText(noticeText);

	// 데이터 설정 및 갱신
	_validFairyGradeList = deliveryBoxData.GetRecomposableFairyList();
	deliveryBoxData.GetFairyGradeRange(_minFairyGrade, _maxFairyGrade);
	uint8 gradeCount = static_cast<uint8>(_maxFairyGrade) - static_cast<uint8>(_minFairyGrade) + 1;

	_slotHelperGradeSlot->RefreshAll(gradeCount);
}

void UGsUIWindowRecompose::OnRefreshGradeSlot(int32 InIndex, class UWidget* InSlot)
{
	TWeakObjectPtr<UGsUIRecomposeListEntry> slot = Cast<UGsUIRecomposeListEntry>(InSlot);
	if (slot.IsValid())
	{
		if (_recomposeType == RecomposeType::COSTUME)
		{
			int32 gradeValue = static_cast<int32>(_minCostumeGrade) + InIndex;
			CostumeGrade grade = static_cast<CostumeGrade>(gradeValue);
			bool canRecompose = _validCostumeGradeList.Contains(grade);
			slot->SetCostumeGrade(grade, canRecompose);
			slot->SetIsEnabled(canRecompose);
		}
		else
		{
			int32 gradeValue = static_cast<int32>(_minFairyGrade) + InIndex;
			FairyGrade grade = static_cast<FairyGrade>(gradeValue);
			bool canRecompose = _validFairyGradeList.Contains(grade);
			slot->SetFairyGrade(grade, canRecompose);
			slot->SetIsEnabled(canRecompose);
		}

		if (false == slot->OnClickGradeSlot.IsBoundToObject(this))
		{
			slot->OnClickGradeSlot.BindUObject(this, &UGsUIWindowRecompose::ChangeGradeSlotFocus);
		}
	}
}

void UGsUIWindowRecompose::OnClickRecompose()
{
	if (_selectedGradeSlot.IsValid())
	{
		TWeakObjectPtr<UGsUIRecomposeListEntry> slot = Cast<UGsUIRecomposeListEntry>(_selectedGradeSlot);

		FText alertText;
		if (_recomposeType == RecomposeType::COSTUME)
		{
			FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Popup_Costume_Alert"), alertText);

			
			CostumeGrade grade = slot->GetCostumeGrade();
			uint8 gradeInt = static_cast<uint8>(grade) - 1;
			CostumeGrade prevGrade = static_cast<CostumeGrade>(gradeInt);
			
			GSSummon()->SetCostumeSummonComposeTryGrade(prevGrade);
			FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
			ItemDBId usedBoxItemDBId = deliveryBoxData.GetUsedBoxItemDBId();
			FGsUIHelper::PopupYesNo(alertText, [usedBoxItemDBId, grade](bool bInYes)
				{
					if (bInYes)
					{
						if (GItem()->TryUseItem(usedBoxItemDBId, 1, true))
						{
							FGsNetSendServiceItem::SendItemUseDeliveryBoxCostumeCompose(usedBoxItemDBId, grade);
						}
						else
						{
							TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowRecompose"));
							if (widget.IsValid())
							{
								widget->Close();
							}
						}
					}
				});
		}
		else
		{
			FText::FindText(DELIVERY_BOX_TEXT, TEXT("DeliveryBoxText_Popup_Fairy_Alert"), alertText);

			FairyGrade grade = slot->GetFairyGrade();
			uint8 gradeInt = static_cast<uint8>(grade) - 1;
			FairyGrade prevGrade = static_cast<FairyGrade>(gradeInt);
			GSSummon()->SetFairySummonComposeTryGrade(prevGrade);
			FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
			ItemDBId usedBoxItemDBId = deliveryBoxData.GetUsedBoxItemDBId();
			FGsUIHelper::PopupYesNo(alertText, [usedBoxItemDBId, grade](bool bInYes)
				{
					if (bInYes)
					{
						if (GItem()->TryUseItem(usedBoxItemDBId, 1, true))
						{
							FGsNetSendServiceItem::SendItemUseDeliveryBoxFairyCompose(usedBoxItemDBId, grade);
						}
						else
						{
							TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowRecompose"));
							if (widget.IsValid())
							{
								widget->Close();
							}
						}
					}
				});
		}
	}
	else
	{
		// 슬롯이 선택되지 않았습니다.
		FText findText;
		FText::FindText(TEXT(""), TEXT(""), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void UGsUIWindowRecompose::ChangeGradeSlotFocus(UUserWidget* InWidget)
{
	if (_selectedGradeSlot.Get() == InWidget)
		return;

	SetIsSelected(_selectedGradeSlot.Get(), false);

	_selectedGradeSlot = InWidget;
	SetIsSelected(_selectedGradeSlot.Get(), true);
}

void UGsUIWindowRecompose::OnSummonComplete(const IGsMessageParam* InParam)
{
	Close();
}

void UGsUIWindowRecompose::SetIsSelected(UUserWidget* InWidget, bool InIsSelected)
{
	TWeakObjectPtr<UGsUIRecomposeListEntry> slot = Cast<UGsUIRecomposeListEntry>(InWidget);
	if (slot.IsValid())
	{
		slot->SetIsSelected(InIsSelected);
		_btnRecompose->SetIsEnabled(true);
	}
}
