#include "GsUIPageFairySummonConfirm.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "Message/MessageParam/GsFairyMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Data/GsDataContainManager.h"

#include "Fairy/GsFairyData.h"
#include "Fairy/GsFairySummonConfirmData.h"

#include "GsUIFairySummonWaitItem.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollection.h"
#include "UI/UIContent/Popup/GsUIPopupSummonConfirmProbability.h"
#include "UI/UIContent/Popup/GsUIPopupFairySkillTooltip.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "PanelWidget.h"
#include "ScrollBox.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Currency/GsCostPackage.h"
#include "ForEach.h"


void UGsUIPageFairySummonConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairySummonWaitItem>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageFairySummonConfirm::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageFairySummonConfirm::OnRefreshEntry);
	_scrollBoxHelper->RefreshAll(0);
}

void UGsUIPageFairySummonConfirm::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageFairySummonConfirm::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageFairySummonConfirm::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageFairySummonConfirm::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_fairyData.Empty();
	auto& list = GSFairy()->GetFairySummonWaitingList();
	for (auto e : list)
	{
		_fairyData.Add(e);
	}

	_scrollBoxHelper->RefreshAll(_fairyData.Num());
}

void UGsUIPageFairySummonConfirm::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCommonPay")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCollection")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairySkillTooltip")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairySummonProbability")).Get())
		popup->Close(); 
}

void UGsUIPageFairySummonConfirm::OnSelected()
{
	_scrollBoxHelper->RefreshAll(_fairyData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonFairyWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_fairyData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageFairySummonConfirm::OnUnselected()
{

}

void UGsUIPageFairySummonConfirm::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySummonWaitItem>(InEntry))
	{
		entry->OnSummonChange.BindUObject(this, &UGsUIPageFairySummonConfirm::OnChangeSummon);
		entry->OnSummonConfirm.BindUObject(this, &UGsUIPageFairySummonConfirm::OnComfirmSummon);
		entry->OnCollectionPopup.BindUObject(this, &UGsUIPageFairySummonConfirm::OnCollectionPopup);
		entry->OnFindFairyInfo.BindUObject(this, &UGsUIPageFairySummonConfirm::OnFindFairyInfo);
		entry->OnProbabilityPopup.BindUObject(this, &UGsUIPageFairySummonConfirm::OnChangeSummonInfoPopup);
		entry->OnTooltipSkillFairy.BindUObject(this, &UGsUIPageFairySummonConfirm::OnTooltipSkillDetail);
		entry->OnTooltipVehicleSkillFairy.BindUObject(this, &UGsUIPageFairySummonConfirm::OnTooltipVehicleSkillDetail);
	}
}

void UGsUIPageFairySummonConfirm::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySummonWaitItem>(InEntry))
	{
		if (_fairyData.IsValidIndex(index))
		{
			entry->SetData(_fairyData[index]);
			entry->RefreshUI();
		}
	}
}

void UGsUIPageFairySummonConfirm::OnComfirmSummon(TWeakPtr<FGsFairySummonConfirmData> data)
{
	//if (GSFairy()->IsEnableStackFairy() == false)
	//{
	//	FText findText;
	//	FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull_Confirm"), findText);
	//	FGsUIHelper::TrayMessageTicker(findText);
	//	return;
	//}

	if (auto useData = data.Pin())
	{
		FText fairyName;
		if (auto fairyData = useData->GetFairyData().Pin())
		{
			if (fairyData->GetAmount() >= MAX_FAIRY_STACK_AMOUNT)
			{
				FText findText;
				FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull_Confirm"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
				return;
			}
			fairyName = fairyData->GetFairyName();
		}

		auto confirmID = useData->_fairyConfirmId;

		FText textMsg; //XXX으로 확정하시겠습니까?
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Popup_SummonConfirm_Main"), textMsg);
		
		textMsg = FText::Format(textMsg, fairyName);

		FGsUIHelper::PopupYesNo(textMsg, [confirmID](bool isOK) {
				if (isOK) FGsNetSendServiceWorld::SendReqFairySummonConfirm(confirmID);
			});
	}
}

void UGsUIPageFairySummonConfirm::OnChangeSummon(TWeakPtr<FGsFairySummonConfirmData> data)
{
	if (auto useData = data.Pin())
	{
		auto confirmID = useData->_fairyConfirmId;

		FText textMsg;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Popup_Replace_Main"), textMsg);
		//교체를 진행하시겠습니까
		if (auto popupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"))))
		{
			const FText fairyName = useData->GetFairyData().IsValid() ? useData->GetFairyData().Pin()->GetFairyName() : FText::GetEmpty();
			textMsg = FText::Format(textMsg, fairyName, useData->_remainChangeCount); //메세지에 (남은회수{0}회) 처리
			if (useData->_remainChangeCount <= 1)
			{
				//이번에 교체를 진행하면 더 이상 교체가 불가능합니다
				FText FairyUI_Popup_LastChange;
				FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Popup_LastChange"), FairyUI_Popup_LastChange);

				textMsg = FText::FromString(FString::Format(TEXT("{0}\n{1}"), { *textMsg.ToString(),  *FairyUI_Popup_LastChange.ToString() }));
			}

			popupCommonPay->SetData(textMsg, FGsCostPackage::CreateSimple(useData->_currencyType, useData->_currencyValue),
				[confirmID](bool isOk) {
					if (isOk)
					{
						GSFairy()->SetEffectPlaying(true);
						FGsNetSendServiceWorld::SendReqFairySummonChange(confirmID);
					}
				});	
		}	

		if (TSharedPtr<FGsFairyData> fairyData = useData->GetFairyData().Pin())
		{
			GSSummon()->SetFairySummonWaitChangeParticleGrade(fairyData->GetFairyGrade());
		}
		GSSummon()->SetNotUseRetry();
	}
}

void UGsUIPageFairySummonConfirm::OnCollectionPopup(FairyId id)
{
	if (auto PopupCollection = Cast<UGsUIPopupFairyCollection>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCollection"))))
	{
		_openPopup.Emplace(TEXT("PopupFairyCollection"), PopupCollection);
		UGsUIPopupFairyCollection::PopupInitData param = { id };
		PopupCollection->InitializeData(&param);
	}
}

void UGsUIPageFairySummonConfirm::OnFindFairyInfo(FairyId id)
{
	TArray<FairyId> elem = { id };
	FGsFairyMsgParamUpdateList param(elem);
	GMessage()->GetFairy().SendMessage(MessageFairy::FIND_FAIRY, &param);
}

void UGsUIPageFairySummonConfirm::OnChangeSummonInfoPopup(TWeakPtr<FGsFairySummonConfirmData> data)
{
	if (auto popup = Cast<UGsUIPopupSummonConfirmProbability>(GUI()->OpenAndGetWidget(TEXT("PopupFairySummonProbability"))))
	{
		_openPopup.Emplace(TEXT("PopupFairySummonProbability"), popup);
		if (auto summonData = data.Pin())
		{	
			FText FairyUI_SummonConfirm_ChangeList;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_SummonConfirm_ChangeList"), FairyUI_SummonConfirm_ChangeList);
			FText FairyUI_SummonConfirm_ChangeListGuide;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_SummonConfirm_ChangeListGuide"), FairyUI_SummonConfirm_ChangeListGuide);

			popup->SetTitleDesc(FairyUI_SummonConfirm_ChangeList, FairyUI_SummonConfirm_ChangeListGuide);
			popup->SetFairyData(summonData->GetFairySummonChangeList());
		}
	}
}

void UGsUIPageFairySummonConfirm::OnTooltipSkillDetail(TWeakPtr<FGsFairySummonConfirmData> data)
{
	if (auto summonData = data.Pin())
	{
		if (auto fairyData = summonData->GetFairyData().Pin())
		{
			const FGsSchemaPassivitySet* baseStatPassivity;
			TArray<FGsFairyLevelInfo> passivityList;
			fairyData->GetFairyLevelPassivityList(baseStatPassivity, passivityList);

			TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillList;
			Algo::ForEachIf(passivityList, [](auto& e) {return e._skillPassivities.IsValidIndex(0);}, [&skillList](auto& e) {
				skillList.Add(TPair<FairyLevelId, const FGsSchemaPassivitySet*>(e._level, e._skillPassivities[0])); });

			if (skillList.IsValidIndex(0))
			{
				if (auto popup = Cast<UGsUIPopupFairySkillTooltip>(GUI()->OpenAndGetWidget(TEXT("PopupFairySkillTooltip")).Get()))
				{
					_openPopup.Emplace(TEXT("PopupFairySkillTooltip"), popup);
					UGsUIPopupFairySkillTooltip::PopupInitData param{ fairyData, skillList };
					popup->InitializeData(&param);
				}
			}
		}
	}
}

void UGsUIPageFairySummonConfirm::OnTooltipVehicleSkillDetail(TWeakPtr<FGsFairySummonConfirmData> data)
{
	if (auto summonData = data.Pin())
	{
		if (auto fairyData = summonData->GetFairyData().Pin())
		{
			TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillList;

			auto curLevel = fairyData->GetCurrentLevel();
			skillList.Add(TPair<FairyLevelId, const FGsSchemaPassivitySet*>(curLevel, fairyData->GetByVehiclePassivity(curLevel)));

			if (skillList.IsValidIndex(0) && skillList[0].Value)
			{
				if (auto popup = Cast<UGsUIPopupFairySkillTooltip>(GUI()->OpenAndGetWidget(TEXT("PopupFairySkillTooltip")).Get()))
				{
					_openPopup.Emplace(TEXT("PopupFairySkillTooltip"), popup);
					UGsUIPopupFairySkillTooltip::PopupInitData param{ fairyData, skillList, INVALID_FAIRY_LEVEL, true };
					popup->InitializeData(&param);
				}
			}
		}
	}
}

void UGsUIPageFairySummonConfirm::OnUpdateSummonWaitingFairy(FairyConfirmId summonWaitDBId)
{
	_fairyData.Empty();
	auto& list = GSFairy()->GetFairySummonWaitingList();
	for (auto e : list)
	{
		_fairyData.Add(e);
	}

	auto findData = _fairyData.FindByPredicate([summonWaitDBId](TWeakPtr<FGsFairySummonConfirmData> e) {
			return e.IsValid() && e.Pin()->_fairyConfirmId == summonWaitDBId;
		});

	//if (findData && findData->IsValid() && findData->Pin()->_remainChangeCount < 1)
	//{
	//	FGsNetSendServiceWorld::SendReqFairySummonConfirm(findData->Pin()->_fairyConfirmId);
	//	return;
	//}

	_scrollBoxHelper->RefreshAll(0);
	_scrollBoxHelper->RefreshAll(_fairyData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonFairyWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_fairyData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageFairySummonConfirm::OnConfirmSummonWaitingFairy(FairyConfirmId summonWaitDBId)
{
	_fairyData.Empty();
	auto& list = GSFairy()->GetFairySummonWaitingList();
	for (auto e : list)
	{
		_fairyData.Add(e);
	}
	_scrollBoxHelper->RefreshAll(0);
	_scrollBoxHelper->RefreshAll(_fairyData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonFairyWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_fairyData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageFairySummonConfirm::OnNotiComfirmChanged()
{
	Algo::ForEachIf(_openPopup, [](auto e) { return e.Value.IsValid();}, [](auto e) {e.Value->Close();});
	_openPopup.Empty();
}
