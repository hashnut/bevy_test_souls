#include "GsUIPageCostumeSummonConfirm.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "GsUICostumeSummonWaitItem.h"
#include "Costume/GsCostumeData.h"

#include "Message/MessageParam/GsCostumeMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Data/GsDataContainManager.h"

#include "Costume/GsCostumeSummonConfirmData.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupCostumeCollection.h"
#include "UI/UIContent/Popup/GsUIPopupSummonConfirmProbability.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "PanelWidget.h"
#include "ScrollBox.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Currency/GsCostPackage.h"

void UGsUIPageCostumeSummonConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeSummonWaitItem>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageCostumeSummonConfirm::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageCostumeSummonConfirm::OnRefreshEntry);
	_scrollBoxHelper->RefreshAll(0);
}

void UGsUIPageCostumeSummonConfirm::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageCostumeSummonConfirm::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageCostumeSummonConfirm::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageCostumeSummonConfirm::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	if (auto userData = GGameData()->GetUserData())
		_genderType = userData->mGender;

	_costumeData.Empty();
	auto& list = GSCostume()->GetSummonWaitingList();
	for (auto e : list)
	{
		_costumeData.Add(e);
	}

	_scrollBoxHelper->RefreshAll(_costumeData.Num());
}

void UGsUIPageCostumeSummonConfirm::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCommonPay")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCostumeCollection")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCostumeSummonProbability")).Get())
		popup->Close();
}

void UGsUIPageCostumeSummonConfirm::OnSelected()
{
	_scrollBoxHelper->RefreshAll(_costumeData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonCostumeWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_costumeData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageCostumeSummonConfirm::OnUnselected()
{

}

void UGsUIPageCostumeSummonConfirm::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeSummonWaitItem>(InEntry))
	{
		entry->OnSummonChange.BindUObject(this, &UGsUIPageCostumeSummonConfirm::OnChangeSummon);
		entry->OnSummonConfirm.BindUObject(this, &UGsUIPageCostumeSummonConfirm::OnComfirmSummon);
		entry->OnCollectionPopup.BindUObject(this, &UGsUIPageCostumeSummonConfirm::OnCollectionPopup);
		entry->OnFindCostumeInfo.BindUObject(this, &UGsUIPageCostumeSummonConfirm::OnFindCostumeInfo);
		entry->OnProbabilityPopup.BindUObject(this, &UGsUIPageCostumeSummonConfirm::OnChangeSummonInfoPopup);
	}
}

void UGsUIPageCostumeSummonConfirm::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeSummonWaitItem>(InEntry))
	{
		if (_costumeData.IsValidIndex(index))
		{
			entry->SetData(_costumeData[index], _genderType);
			entry->RefreshUI();
		}
	}
}

void UGsUIPageCostumeSummonConfirm::OnComfirmSummon(TWeakPtr<FGsCostumeSummonConfirmData> data)
{
	if (auto useData = data.Pin())
	{
		FText costumeName;
		if (auto costumeData = useData->_costumeData.Pin())
		{
			if (costumeData->GetAmount() >= MAX_COSTUME_STACK_AMOUNT)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull_Confirm"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
				return;
			}

			costumeName = costumeData->GetCostumeName();
		}

		auto confirmID = useData->_confirmId;

		FText textMsg; //XXX으로 확정하시겠습니까?
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Popup_SummonConfirm_Main"), textMsg);

		textMsg = FText::Format(textMsg, costumeName);

		FGsUIHelper::PopupYesNo(textMsg, [confirmID](bool isOK) {
			if (isOK) FGsNetSendServiceWorld::SendReqCostumeSummonConfirm(confirmID);
			});
	}
}

void UGsUIPageCostumeSummonConfirm::OnChangeSummon(TWeakPtr<FGsCostumeSummonConfirmData> data)
{
	if (auto useData = data.Pin())
	{
		auto confirmID = useData->_confirmId;

		FText textMsg;
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Popup_Replace_Main"), textMsg);

		const FText costumeName = useData->GetCostumeData().IsValid() ? useData->GetCostumeData().Pin()->GetCostumeName() : FText::GetEmpty();
		//교체를 진행하시겠습니까
		textMsg = FText::Format(textMsg, costumeName, useData->_remainChangeCount); //메세지에 (남은회수{0}회) 처리
		if (useData->_remainChangeCount <= 1)
		{
			//이번에 교체를 진행하면 더 이상 교체가 불가능합니다
			FText CostumeUI_Popup_LastChange;
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Popup_LastChange"), CostumeUI_Popup_LastChange);

			textMsg = FText::FromString(FString::Format(TEXT("{0}\n{1}"), { *textMsg.ToString(),  *CostumeUI_Popup_LastChange.ToString() }));
		}

		if (auto popupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"))))
		{
			popupCommonPay->SetData(textMsg, FGsCostPackage::CreateSimple(useData->_currencyType, useData->_currencyValue),
				[confirmID](bool isOk) {
					if (isOk)
					{
						GSCostume()->SetEffectPlaying(true);
						FGsNetSendServiceWorld::SendReqCostumeSummonChange(confirmID);
					}
				});
		}

		if (TSharedPtr<FGsCostumeData> costumeData = useData->_costumeData.Pin())
		{
			GSSummon()->SetCostumeSummonWaitChangeParticleGrade(costumeData->GetCostumeGrade());
		}
		GSSummon()->SetNotUseRetry();
	}
}

void UGsUIPageCostumeSummonConfirm::OnCollectionPopup(CostumeId id)
{
	if (auto PopupCollection = Cast<UGsUIPopupCostumeCollection>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeCollection"))))
	{
		_openPopup.Emplace(TEXT("PopupCostumeCollection"), PopupCollection);

		CreatureGenderType genderType{ CreatureGenderType::ALL };
		if (auto userData = GGameData()->GetUserData())
			genderType = userData->mGender;

		UGsUIPopupCostumeCollection::PopupInitData param = { id, genderType };
		PopupCollection->InitializeData(&param);
	}
}

void UGsUIPageCostumeSummonConfirm::OnFindCostumeInfo(CostumeId id)
{
	TArray<CostumeId> elem = { id };
	FGsCostumeMsgParamUpdateList param(elem);
	GMessage()->GetCostume().SendMessage(MessageCostume::FIND_COSTUME, &param);
}

void UGsUIPageCostumeSummonConfirm::OnChangeSummonInfoPopup(TWeakPtr<FGsCostumeSummonConfirmData> data)
{
	if (auto popup = Cast<UGsUIPopupSummonConfirmProbability>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeSummonProbability"))))
	{
		_openPopup.Emplace(TEXT("PopupCostumeSummonProbability"), popup);
		if (auto summonData = data.Pin())
		{
			FText CostumeUI_SummonConfirm_ChangeList;
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_SummonConfirm_ChangeList"), CostumeUI_SummonConfirm_ChangeList);
			FText CostumeUI_SummonConfirm_ChangeListGuide;
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_SummonConfirm_ChangeListGuide"), CostumeUI_SummonConfirm_ChangeListGuide);

			popup->SetTitleDesc(CostumeUI_SummonConfirm_ChangeList, CostumeUI_SummonConfirm_ChangeListGuide);
			popup->SetCostumeData(summonData->GetCostumeSummonChangeList());
		}
	}
}

void UGsUIPageCostumeSummonConfirm::OnUpdateSummonWaiting(CostumeConfirmId summonWaitDBId)
{
	_costumeData.Empty();
	auto& list = GSCostume()->GetSummonWaitingList();
	for (auto e : list)
	{
		_costumeData.Add(e);
	}

	auto findData = _costumeData.FindByPredicate([summonWaitDBId](TWeakPtr<FGsCostumeSummonConfirmData> e) {
		return e.IsValid() && e.Pin()->_confirmId == summonWaitDBId;
		});

	//if (findData && findData->IsValid() && findData->Pin()->_remainChangeCount < 1)
	//{
	//	FGsNetSendServiceWorld::SendReqCostumeSummonConfirm(findData->Pin()->_confirmId);
	//	return;
	//}

	_scrollBoxHelper->RefreshAll(0);
	_scrollBoxHelper->RefreshAll(_costumeData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonCostumeWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_costumeData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageCostumeSummonConfirm::OnConfirmSummonWaiting(CostumeConfirmId summonWaitDBId)
{
	_costumeData.Empty();
	auto& list = GSCostume()->GetSummonWaitingList();
	for (auto e : list)
	{
		_costumeData.Add(e);
	}
	_scrollBoxHelper->RefreshAll(0);
	_scrollBoxHelper->RefreshAll(_costumeData.Num());

	auto maxCount = GData()->GetGlobalData()->_summonCostumeWaitMaxCount;
	_textCount = FText::FromString(FString::FromInt(_costumeData.Num()) / FString::FromInt(maxCount));
}

void UGsUIPageCostumeSummonConfirm::OnNotiComfirmChanged()
{
	auto widget = GUI()->GetWidgetByKey(TEXT("PopupSkillDetailInfo"));
	if(widget.IsValid())
	{
		_openPopup.Add(TEXT("PopupSkillDetailInfo"), widget);
	}

	Algo::ForEachIf(_openPopup, [](auto e) { return e.Value.IsValid();}, [](auto e) {e.Value->Close();});
	_openPopup.Empty();
}
