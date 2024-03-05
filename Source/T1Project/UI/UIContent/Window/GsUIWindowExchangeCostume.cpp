// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowExchangeCostume.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Costume/GsCostumeData.h"
#include "Costume/GsCostumeSummonConfirmData.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/Costume/GsUICostumeItem.h"
#include "UI/UIContent/Window/Costume/GsUICostumeDetail.h"
#include "UI/UIContent/Popup/GsUIPopupSkillDetailInfo.h"
#include "UI/UIContent/Popup/GsUIPopupCostumeCollection.h"
#include "UI/UIContent/Popup/GsUIPopupFlavorText.h"
#include "UI/UIContent/Popup/GsUIPopupExchangeCostumeSelect.h"
#include "UI/UIContent/Popup/GsUIPopupSummonConfirmProbability.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Algo/Transform.h"

void UGsUIWindowExchangeCostume::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIWindowExchangeCostume::OnCreateCostumeItemEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowExchangeCostume::OnRefreshCostumeItemEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIWindowExchangeCostume::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickCloseAllStack);

	_btnSelect->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickPopupSelect);
	_collectionButton->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnOpenCollection);
	_flavorButton->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnOpenFlavorText);
	_btnDetailInfo->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickPopupCostumeDetail);
	_btnProbabilityInfo->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickPopupProbability);
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIWindowExchangeCostume::OnClickExchange);

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeItem>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowExchangeCostume::OnCreateCostumeItemEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowExchangeCostume::OnRefreshCostumeItemEntry);
}

void UGsUIWindowExchangeCostume::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto userData = GGameData()->GetUserData())
		_genderType = userData->mGender;

	SetInitExchangeInfo();
}

void UGsUIWindowExchangeCostume::NativeDestruct()
{
	_selectedItem = nullptr;
	Super::NativeDestruct();
}

bool UGsUIWindowExchangeCostume::OnBack()
{
	if (auto costumeManager = GSCostume())
	{
		if (costumeManager->IsEffectPlaying())
		{
			return false;
		}
	}

	return Super::OnBack();
}

void UGsUIWindowExchangeCostume::SetInitExchangeInfo()
{
	_scrollBoxHelper->RefreshAll(0);
	_switcherInfo->SetActiveWidgetIndex(1);
	_switcherCostumeList->SetActiveWidgetIndex(1);
	_portrait->SetVisibility(ESlateVisibility::Hidden);

	_btnConfirm->SetIsEnabled(false);
	_btnDetailInfo->SetIsEnabled(false);
	_btnProbabilityInfo->SetIsEnabled(false);
	_collectionButton->SetIsEnabled(false);
	_flavorButton->SetIsEnabled(false);
}

void UGsUIWindowExchangeCostume::OnClickPopupSelect()
{
	if (auto PopupSelection = Cast<UGsUIPopupExchangeCostumeSelect>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeSelection"))))
	{
		if (_selectedItem)
		{
			UGsUIPopupExchangeCostumeSelect::PopupInitData param = { _selectedItem->GetTID(), INVALID_COSTUME_ID, true };
			PopupSelection->InitializeData(&param, [this](TWeakPtr<FGsCostumeData> data, const TArray<TPair<TWeakPtr<FGsCostumeData>, uint64>>& probabilityList) {
					SetSelectCostume(data, probabilityList);
				});
		}
	}
}

void UGsUIWindowExchangeCostume::OnClickPopupCostumeDetail()
{
	if (auto PopupSelection = Cast<UGsUIPopupExchangeCostumeSelect>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeSelection"))))
	{
		if (_selectedItem)
		{
			UGsUIPopupExchangeCostumeSelect::PopupInitData param = { _selectedItem->GetTID(), INVALID_COSTUME_ID, false };
			PopupSelection->InitializeData(&param);
		}
	}
}

void UGsUIWindowExchangeCostume::OnClickPopupProbability()
{
	TArray<FGsCostumeSummonChangeData> enchangeProbabilityList;

	static auto SetProbability = [](TArray<FGsCostumeSummonChangeData>& enchangeList) {
		int64 sumProbability = 0;
		Algo::ForEach(enchangeList, [&sumProbability](auto& e) { sumProbability += e._probability; });
		Algo::ForEach(enchangeList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability); });
	};

	if (_exchangeList.IsValidIndex(0))
	{
		Algo::TransformIf(_exchangeList, enchangeProbabilityList, [](auto& pair) { return pair.Key.IsValid(); }, [](auto& pair) {
				return FGsCostumeSummonChangeData{ pair.Key, pair.Value, 0 };
			});

		SetProbability(enchangeProbabilityList);

		// 전체 확률 계산을 위해 계산 이후 재료 아이템이 있다면 빼준다
		//auto selectId = _selectData.Pin()->_id;
		//enchangeProbabilityList.RemoveAll([selectId](auto& e) { return e._costumeData.Pin()->_id == selectId; });
	}

	if (auto popup = Cast<UGsUIPopupSummonConfirmProbability>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeSummonProbability"))))
	{
		FText CostumeUI_Ceiling_SuccessReward;
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Ceiling_SuccessReward"), CostumeUI_Ceiling_SuccessReward);
		FText CostumeUI_ProbDesc;
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_ProbDesc"), CostumeUI_ProbDesc);

		popup->SetTitleDesc(CostumeUI_Ceiling_SuccessReward, CostumeUI_ProbDesc);
		popup->SetCostumeData(enchangeProbabilityList);
	}
}

void UGsUIWindowExchangeCostume::OnOpenCollection()
{
	if (auto PopupCollection = Cast<UGsUIPopupCostumeCollection>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeCollection"))))
	{
		if (auto costumeData = _selectData.Pin())
		{
			UGsUIPopupCostumeCollection::PopupInitData param = { costumeData->_id, _genderType, true, UGsUIPopupCostumeCollection::PopupInitData::InExchangeCostume };
			PopupCollection->InitializeData(&param);
		}
	}
}

void UGsUIWindowExchangeCostume::OnOpenFlavorText()
{
	if (auto costumeData = _selectData.Pin())
	{
		if (auto popup = Cast<UGsUIPopupFlavorText>(GUI()->OpenAndGetWidget(TEXT("PopupFlavorText")).Get()))
		{
			UGsUIPopupFlavorText::PopupInitData param{ costumeData->GetCostumeName(), costumeData->GetFlavorText() };
			popup->InitializeData(&param);
		}
	}
}

void UGsUIWindowExchangeCostume::OnCreateCostumeItemEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::EXCHANGE;
		entry->OnItemSelected.BindUObject(this, &UGsUIWindowExchangeCostume::OnSelectedCostumeItem);
	}
}

void UGsUIWindowExchangeCostume::OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		if (_exchangeList.IsValidIndex(index))
		{
			entry->SetData(_exchangeList[index].Key, _genderType);
			entry->SetSelected(false);
			entry->RefreshUI();
		}
	}
}

void UGsUIWindowExchangeCostume::OnSelectedCostumeItem(CostumeId id)
{
	if (auto PopupSelection = Cast<UGsUIPopupExchangeCostumeSelect>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeSelection"))))
	{
		if (_selectedItem)
		{	
			auto selectCostume = _exchangeList.FindByPredicate([id](auto& pair) { return pair.Key.IsValid() && pair.Key.Pin()->_id == id; });
			UGsUIPopupExchangeCostumeSelect::PopupInitData param = { _selectedItem->GetTID(), selectCostume ? id : INVALID_COSTUME_ID, false };
			PopupSelection->InitializeData(&param);
		}
	}
}

void UGsUIWindowExchangeCostume::OnClickExchange()
{
	if(_selectData.IsValid() && GSCostume()->IsEnableStackCostume() == false)
	{
		TMap<CostumeId, ItemAmount> exchangeList;
		exchangeList.Add(TPair<CostumeId, ItemAmount>{ _selectData.Pin()->_id, 1 });
		if (GSCostume()->CheckComposeStackableCostume(exchangeList) == false)
		{
			FText findText;
			FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
	}

	if (auto popupYesNo = Cast<UGsUIPopupYesNo>(GUI()->OpenAndGetWidget(TEXT("PopupYesNo"))))
	{
		FText msg;
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_CostumeChange_Popup_Desc"), msg);
		popupYesNo->SetData(msg, [this](bool yes){
			if (yes && _selectedItem && _selectData.IsValid())
			{
				GSCostume()->SetEffectPlaying(true);
				FGsNetSendServiceWorld::SendReqCostumeExchange(_selectedItem->GetDBID(), _selectData.Pin()->_id);
			}
		
		});
	}
}

void UGsUIWindowExchangeCostume::SetCostumeExchangeTicketInfo(const FGsItem* item)
{
	_selectedItem = item;
	if (_selectedItem)
	{
		SetInitExchangeInfo();
	}
}

void UGsUIWindowExchangeCostume::SetSelectCostume(TWeakPtr<FGsCostumeData> selectData, const TArray<TPair<TWeakPtr<FGsCostumeData>, uint64>>& probabilityList)
{
	_exchangeList.Reset();

	_selectData = selectData;
	_exchangeList.Append(probabilityList);
	if (auto costumeData = _selectData.Pin())
	{	
		_portrait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_portrait->SetData(_selectData, _genderType);
		_portrait->RefreshUI();


		_switcherInfo->SetActiveWidgetIndex(0);
		FLinearColor gradeColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
		FText gradeText;
		switch (costumeData->GetCostumeGrade())
		{
		case CostumeGrade::NORMAL:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Normal"), gradeText);	break;
		case CostumeGrade::MAGIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Magic"), gradeText);	break;
		case CostumeGrade::RARE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Rare"), gradeText);		break;
		case CostumeGrade::EPIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Epic"), gradeText);		break;
		case CostumeGrade::UNIQUE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Unique"), gradeText);	break;
		case CostumeGrade::LEGEND:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Legend"), gradeText);	break;
		default: break;
		}
		_costumeName->SetText(costumeData->GetCostumeName());
		_costumeGrade->SetText(gradeText);
		_costumeName->SetColorAndOpacity(gradeColor);
		_costumeGrade->SetColorAndOpacity(gradeColor);

		_costumeDetail->SetSelectedCostumeDetail(_selectData);

		_switcherCostumeList->SetActiveWidgetIndex(probabilityList.IsValidIndex(0) ? 0 : 1);
		_scrollBoxHelper->RefreshAll(probabilityList.Num());

		_btnProbabilityInfo->SetIsEnabled(probabilityList.IsValidIndex(0));
		_collectionButton->SetIsEnabled(true);
		_btnDetailInfo->SetIsEnabled(true);
		_btnConfirm->SetIsEnabled(true);

		auto flavorText = costumeData->GetFlavorText();
		_flavorButton->SetIsEnabled(flavorText.IsEmpty() == false);
	}

}

void UGsUIWindowExchangeCostume::OnAckExchangedCostume()
{

}
