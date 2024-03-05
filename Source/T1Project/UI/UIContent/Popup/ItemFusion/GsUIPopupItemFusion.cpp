
#include "GsUIPopupItemFusion.h"
#include "../Item/GsItemStat.h"
#include "../Item/GsItemEquipment.h"
#include "../GameObject/Stat/GsStatHelper.h"
#include "../Currency/GsCurrencyHelper.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsUIMessageParam.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/Fusion/Handler/GsItemFusionHandler.h"
#include "../UI/UIContent/Helper/GsUIColorHelper.h"
#include "../UI/UIContent/Common/GsUICurrencyButton.h"
#include "../UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "../UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "../UI/UIContent/Common/Inventory/GsInventoryLogicItemFusion.h"
#include "../UI/UIContent/Common/Inventory/GsWrapBoxInventoryIconSelector.h"
#include "../UI/UIContent/Popup/ItemFusion/GsUIItemFusionRefineStat.h"
#include "../UI/UIControlLib/Define/EGsIconType.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Animation/UMGSequencePlayer.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "../T1Project.h"


void UGsUIPopupItemFusion::BeginDestroy()
{
	SAFE_DELETE(_inventoryData);	

	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupItemFusion::OnClickedClose);
		_btnClose = nullptr;
	}

	if (_btnCurrency)
	{
		if (_btnCurrency->OnClickCurrencyButton.IsBoundToObject(this))
		{
			_btnCurrency->OnClickCurrencyButton.Unbind();
		}		
		_btnCurrency = nullptr;
	}

	if (_invenUI)
	{
		if (false == _invenUI->OnClickItem().IsBoundToObject(this))
		{
			_invenUI->OnClickItem().Unbind();
		}
		_invenUI = nullptr;
	}

	if (_listSrcRefineStat)
	{
		_listSrcRefineStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupItemFusion::OnRefreshSrcRefineStatEntry);
		_listSrcRefineStat = nullptr;
	}

	if (_listDestRefineStat)
	{
		_listDestRefineStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupItemFusion::OnRefreshDestRefineStatEntry);
		_listDestRefineStat = nullptr;
	}

	if (_btnSrcRefineInfo)
	{
		_btnSrcRefineInfo->OnClicked.RemoveDynamic(this, &UGsUIPopupItemFusion::OnClickedSrcRefine);
		_btnSrcRefineInfo = nullptr;
	}

	if (_btnDestRefineInfo)
	{
		_btnDestRefineInfo->OnClicked.RemoveDynamic(this, &UGsUIPopupItemFusion::OnClickedDestRefine);
		_btnDestRefineInfo = nullptr;
	}
	
	_idleAnimation = nullptr;

	if (_FusionAnimation)
	{
		UnbindFromAnimationFinished(_FusionAnimation, _fusionAnimationFinishDelegate);
		_FusionAnimation = nullptr;
	}	

	Super::BeginDestroy();
}

void UGsUIPopupItemFusion::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 2023/10/5 PKT - Close
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupItemFusion::OnClickedClose);
	
	// 2023/10/5 PKT - Currency
	_btnCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupItemFusion::OnClickedTryFusion);
	
	// 2023/9/26 PKT - Animation Data
	if (UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass())
	{
		for (UWidgetAnimation* anim : genClass->Animations)
		{
			if (anim->GetMovieScene())
			{
				if (0 == anim->GetMovieScene()->GetName().Compare(TEXT("Fusion"), ESearchCase::IgnoreCase))
				{
					_FusionAnimation = anim;

					_fusionAnimationFinishDelegate.BindDynamic(this, &UGsUIPopupItemFusion::OnFusionAnimationFinish);
					BindToAnimationFinished(_FusionAnimation, _fusionAnimationFinishDelegate);
				}
				else if (0 == anim->GetMovieScene()->GetName().Compare(TEXT("SlotIdle"), ESearchCase::IgnoreCase))
				{
					_idleAnimation = anim;
				}
			}
		}
	}
	
	// 2023/10/5 PKT - Src Refine Stat List
	_listSrcRefineStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_listSrcRefineStat->Initialize(_entryRefineSrcStatEntry, (UPanelWidget*)_verticalSrcRefineStatSet);
	_listSrcRefineStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemFusion::OnRefreshSrcRefineStatEntry);

	// 2023/10/5 PKT - Dest Refine Stat List
	_listDestRefineStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_listDestRefineStat->Initialize(_entryRefineDestStatEntry, (UPanelWidget*)_verticalDestRefineStatSet);
	_listDestRefineStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemFusion::OnRefreshDestRefineStatEntry);

	_btnSrcRefineInfo->OnClicked.AddDynamic(this, &UGsUIPopupItemFusion::OnClickedSrcRefine);
	_btnDestRefineInfo->OnClicked.AddDynamic(this, &UGsUIPopupItemFusion::OnClickedDestRefine);
	

	// 2023/9/27 PKT - Inventory
	_inventoryData = new FGsInventoryLogicItemFusion();

	if (false == _invenUI->OnClickItem().IsBoundToObject(this))
	{
		_invenUI->OnClickItem().BindUObject(this, &UGsUIPopupItemFusion::OnClickedInventory);
	}
}

void UGsUIPopupItemFusion::NativeConstruct()
{
	Super::NativeConstruct();

	_fusionHandler = nullptr;

	// 2023/9/26 PKT - Animation 정보 초기화
	StopEffect(_idleAnimation);
	StopEffect(_FusionAnimation);

	_panelSelectedStatGuide->SetVisibility(ESlateVisibility::Hidden);

	_textSrcItemName->SetVisibility(ESlateVisibility::Hidden);
	_textDestItemName->SetVisibility(ESlateVisibility::Hidden);

	_imgSelectedSrcRefineImg->SetVisibility(ESlateVisibility::Collapsed);
	_imgSelectedDestRefineImg->SetVisibility(ESlateVisibility::Collapsed);

	// 2023/10/4 PKT - Inventory
	_invenUI->SetData(_inventoryData);
	_invenUI->SetItemFusionInvenState();
	
	// 2023/10/10 PKT - Set Button Name
	FText buttonName;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_Fuse"), buttonName);
	_btnCurrency->SetButtonName(buttonName);
}

void UGsUIPopupItemFusion::NativeDestruct()
{
	// 2023/9/26 PKT - Animation 정보 초기화
	StopEffect(_idleAnimation);
	StopEffect(_FusionAnimation);

	_iconSrcItemSelector->RemoveAllChildren();
	_iconDestItemSelector->RemoveAllChildren();
	_iconFusionItemSelector->RemoveAllChildren();

	_fusionHandler = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupItemFusion::OnInputCancel()
{	
	if (nullptr == _fusionHandler)
	{
		GSLOG(Error, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->CloseUI();

	Close();

	Super::OnInputCancel();
}

void UGsUIPopupItemFusion::PlayEffect(UWidgetAnimation* InAnimaiton, bool InIsLoop)
{
	if (InAnimaiton)
	{
		PlayAnimation(InAnimaiton, 0.f, InIsLoop ? 0 : 1, EUMGSequencePlayMode::Forward, 1.f, false);
	}
	
	_animationEffectPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UGsUIPopupItemFusion::StopEffect(UWidgetAnimation* InAnimaiton)
{
	StopAnimation(InAnimaiton);

	_animationEffectPanel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UGsUIPopupItemFusion::IsPlayEffect(UWidgetAnimation* InAnimaiton)
{
	return (InAnimaiton) ? IsAnimationPlaying(InAnimaiton) : false;
}

UGsUIIconItemInventory* UGsUIPopupItemFusion::GetIcon(UGsWrapBoxInventoryIconSelector* InIconSelector)
{
	if (nullptr == InIconSelector)
	{
		return nullptr;
	}

	// 아이콘 세팅
	if (0 == InIconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = InIconSelector->CreateIcon();
		if (newIcon)
		{
			InIconSelector->AddChild(newIcon);
		}
	}

	const static int32 ICON_INDEX = 0;
	return Cast<UGsUIIconItemInventory>(InIconSelector->GetChildAt(ICON_INDEX));
}

void UGsUIPopupItemFusion::FillIconWithItemData(UGsWrapBoxInventoryIconSelector* OutIconSelector, class UTextBlock* OutText, ItemDBId InItemDBId)
{
	UGsUIIconItemInventory* targetIcon = GetIcon(OutIconSelector);

	if (nullptr == _fusionHandler || nullptr == targetIcon)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler || nullptr == targetIcon"));
		return;
	}

	if (INVALID_ITEM_DB_ID >= InItemDBId)
	{
		targetIcon->SetVisibility(ESlateVisibility::Collapsed);
		OutIconSelector->SetVisibility(ESlateVisibility::Collapsed);
		if (OutText)
		{
			OutText->SetVisibility(ESlateVisibility::Hidden);
		}		
	}
	else
	{
		targetIcon->ResetCoolTime();
		targetIcon->SetOwnedItem(InItemDBId);
		targetIcon->SetItemNameVisibility(false);
		targetIcon->SetActiveIconBg(true);
		targetIcon->SetItemNameToolTipFlag(false);
		targetIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);

		if (false == targetIcon->OnClickSlot.IsBoundToObject(this))
		{
			targetIcon->OnClickSlot.AddUObject(this, &UGsUIPopupItemFusion::OnSelectedMaterialItemIcon);
		}

		targetIcon->SetVisibility(ESlateVisibility::Visible);
		if (OutText && targetIcon->GetItemIconData())
		{
			FString strItemName;
			if (0 < targetIcon->GetItemIconData()->GetLevel())
			{
				strItemName = FString::Format(TEXT("+{0} {1}"), { targetIcon->GetItemIconData()->GetLevel(), *targetIcon->GetItemIconData()->GetName().ToString() });
			}
			else
			{
				strItemName = targetIcon->GetItemIconData()->GetName().ToString();
			}				 

			OutText->SetText(FText::FromString(strItemName));
			OutText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(targetIcon->GetItemIconData()->GetGradeType()));
			OutText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		targetIcon->SetVisibility(ESlateVisibility::Visible);
		OutIconSelector->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUIPopupItemFusion::FillStatUIWithItemData(class FGsItemEquipment* InItemEquipmentData, int32 InIndex, UGsUIItemFusionRefineStat* OutWidget)
{
	if (nullptr == _fusionHandler || nullptr == InItemEquipmentData  || nullptr == OutWidget)
	{
		GSLOG(Error, TEXT("nullptr == _fusionHandler || nullptr == InItemEquipmentData  || nullptr == OutWidget"));
		return;
	}

	TArray<FGsItemStat*> outItemStatList;
	InItemEquipmentData->GetAllRefineOptionStatList(outItemStatList);
	if (false == outItemStatList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == outItemStatList.IsValidIndex(InIndex)"));
		return;
	}

	FGsItemStat* itemStatData = outItemStatList[InIndex];
	if (nullptr == itemStatData)
	{
		GSLOG(Error, TEXT("nullptr == itemStatData"));
		return;
	}
	
	// 2023/10/10 PKT - 아래 코드 때문에 결국엔 ItemData가 필요하다.
	TMap<StatType, TPair<int32, int32>> statRangeMap;
	InItemEquipmentData->GetAllExpectRefineOptionStatList(ItemCategorySub::OPTION_STONE, statRangeMap);
	

	TPair<FText, FText> pairText;
	if (false == FGsStatHelper::GetDesplayStatNameAndValue(itemStatData->GetStatType(), itemStatData->GetStatOriValue(), pairText, true))
	{
		GSLOG(Error, TEXT("FGsStatHelper::GetDesplayStatNameAndValue : [%d] [%d]"), (int32)(itemStatData->GetStatType()), itemStatData->GetStatOriValue());
		return;
	}

	OutWidget->SetData(pairText.Key
		, pairText.Value
		, FGsUIColorHelper::GetColorItemRefineOption(itemStatData->GetStatOriValue(), statRangeMap[itemStatData->GetStatType()].Key, statRangeMap[itemStatData->GetStatType()].Value)
	);
}

void UGsUIPopupItemFusion::OnClickedTryFusion()
{
	if (nullptr == _fusionHandler)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler"));
		return;
	}

	// 2023/11/6 PKT - 시도
	_fusionHandler->TryFusion();
}

void UGsUIPopupItemFusion::OnClickedClose()
{
	if (_fusionHandler)
	{
		_fusionHandler->CloseUI();
	}

	Close();
}

void UGsUIPopupItemFusion::OnClickedInventory(uint64 InItemDBId)
{
	if (nullptr == _fusionHandler)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->PushMaterialItem(InItemDBId);
}

void UGsUIPopupItemFusion::OnRefreshSrcRefineStatEntry(int32 InIndex, class UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIIconItemInventory> srcItemIcon = GetIcon(_iconSrcItemSelector);
	if (false == srcItemIcon.IsValid())
	{	// 2023/10/10 PKT - Src Item이 존재 하지 않는다.
		return;
	}

	FGsItem* srcItemData = srcItemIcon.Get()->GetItemIconData();
	if (nullptr == srcItemData)
	{
		GSLOG(Error, TEXT("nullptr == srcItemData"));
		return;
	}

	FGsItemEquipment* srcItemEquipment = StaticCast<FGsItemEquipment*>(srcItemData);
	if (nullptr == srcItemEquipment)
	{
		GSLOG(Error, TEXT("nullptr == srcItemEquipment"));
		return;
	}

	FillStatUIWithItemData(srcItemEquipment, InIndex, Cast<UGsUIItemFusionRefineStat>(InEntry));
}

void UGsUIPopupItemFusion::OnRefreshDestRefineStatEntry(int32 InIndex, class UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIIconItemInventory> destItemIcon = GetIcon(_iconDestItemSelector);
	if (false == destItemIcon.IsValid())
	{	// 2023/10/10 PKT - Dest Item이 존재 하지 않는다.
		return;
	}

	FGsItem* destItemData = destItemIcon.Get()->GetItemIconData();
	if (nullptr == destItemData)
	{
		GSLOG(Error, TEXT("nullptr == destItemData"));
		return;
	}

	FGsItemEquipment* destItemEquipment = StaticCast<FGsItemEquipment*>(destItemData);
	if (nullptr == destItemEquipment)
	{
		GSLOG(Error, TEXT("nullptr == destItemEquipment"));
		return;
	}

	FillStatUIWithItemData(destItemEquipment, InIndex, Cast<UGsUIItemFusionRefineStat>(InEntry));
}

void UGsUIPopupItemFusion::OnClickedSrcRefine()
{
	if (ESlateVisibility::Visible == _imgSelectedSrcRefineImg->GetVisibility())
	{
		return;
	}

	if (nullptr == _fusionHandler)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->SelectedRefain(_fusionHandler->GetSrcItemDBId());
}

void UGsUIPopupItemFusion::OnClickedDestRefine()
{
	if (ESlateVisibility::Visible == _imgSelectedDestRefineImg->GetVisibility())
	{
		return;
	}

	if (nullptr == _fusionHandler)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->SelectedRefain(_fusionHandler->GetDestItemDBId());
}

void UGsUIPopupItemFusion::OnSelectedMaterialItemIcon(UGsUIIconItem& InIcon)
{
	if (nullptr == _fusionHandler)
	{
		GSLOG(Warning, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->PopMaterialItem(InIcon.GetDBID());
}

void UGsUIPopupItemFusion::SetData(FGsItemFusionHandler* InItemFusionHandler)
{
	// 2023/9/26 PKT - Handler Ptr
	_fusionHandler = InItemFusionHandler;

	_inventoryData->SetData(_fusionHandler);
}

void UGsUIPopupItemFusion::Invalidate()
{
	if (nullptr == _fusionHandler)
	{	// 2023/10/5 PKT - Handler Invalide 하거나 결과창이 진행 중일때는 UI갱신을 하지 않는다.
		return;
	}

	// 2023/10/4 PKT - switcher 규칙
	const static int32 SWITCH_EITHER_INDEX = 0;
	const static int32 SWITCH_EMPTY_INDEX = 1;

	// 2023/10/4 PKT - 제련 정보	(Left)
	const ItemDBId srcItemDBId = _fusionHandler->GetSrcItemDBId();
	const ItemDBId destItemDBId = _fusionHandler->GetDestItemDBId();

	FillIconWithItemData(_iconSrcItemSelector, _textSrcItemName, srcItemDBId);
	FillIconWithItemData(_iconDestItemSelector, _textDestItemName, destItemDBId);

	FText textGuide = FText::GetEmpty();
	FLinearColor textColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID);
	bool isCurrencyBtnEnable = false;

	const FGsItemFusionHandler::ECheckTry isTryEnable = _fusionHandler->IsCheckTryFusion();
	if (FGsItemFusionHandler::ECheckTry::InvalidRegist == isTryEnable)
	{	// 2023/10/11 PKT - 아직 2개 슬롯에 모두 등록이 안되어 있는 상태
		_switcherRefineSelectorPanel->SetActiveWidgetIndex(SWITCH_EMPTY_INDEX);

		// 2023/10/11 PKT - 재련 효과 Text 숨김
		_panelSelectedStatGuide->SetVisibility(ESlateVisibility::Hidden);

		textGuide = FText::GetEmpty();
		textColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID);
		isCurrencyBtnEnable = false;

		if (true == IsPlayEffect(_idleAnimation))
		{
			StopEffect(_idleAnimation);
		}
	}
	else
	{
		_switcherRefineSelectorPanel->SetActiveWidgetIndex(SWITCH_EITHER_INDEX);

		const ItemDBId refineItemDBId = _fusionHandler->GetRefinedItemDBId();

		// 2023/10/10 PKT - Src Item Refine
		const auto& statSrcSet = _fusionHandler->GetItemRefineStatSet(srcItemDBId);
		_switcherSrcRefineInfo->SetActiveWidgetIndex(0 < statSrcSet.Num() ? SWITCH_EITHER_INDEX : SWITCH_EMPTY_INDEX);
		_listSrcRefineStat->RefreshAll(statSrcSet.Num());
		_imgSelectedSrcRefineImg->SetVisibility(refineItemDBId == srcItemDBId ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		// 2023/10/10 PKT - Dest Item Refine
		const auto& statDestSet = _fusionHandler->GetItemRefineStatSet(destItemDBId);
		_switcherDestRefineInfo->SetActiveWidgetIndex(0 < statDestSet.Num() ? SWITCH_EITHER_INDEX : SWITCH_EMPTY_INDEX);
		_listDestRefineStat->RefreshAll(statDestSet.Num());
		_imgSelectedDestRefineImg->SetVisibility(refineItemDBId == destItemDBId ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		// 2023/10/11 PKT - 재련 효과 Text 보임
		_panelSelectedStatGuide->SetVisibility(ESlateVisibility::HitTestInvisible);

		switch (isTryEnable)
		{
		case FGsItemFusionHandler::ECheckTry::InvalidRefine:
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_ChoiceRefineOption"), textGuide);
			textColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID);
			isCurrencyBtnEnable = false;
			break;
		case FGsItemFusionHandler::ECheckTry::invalidCurrency:
		case FGsItemFusionHandler::ECheckTry::Valid:
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Fuse_Notice_FusePossible"), textGuide);
			textColor = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID);
			isCurrencyBtnEnable = true;
		default:
			break;
		}

		if (false == IsPlayEffect(_idleAnimation))
		{
			PlayEffect(_idleAnimation, true);
		}
	}
	
	// 2023/10/11 PKT - 하단 문구
	_textGuide->SetText(textGuide);
	_textGuide->SetColorAndOpacity(textColor);

	// 2023/10/5 PKT - Currency Button 셋팅
	CurrencyPair CurrencyInfo = _fusionHandler->GetCost();
	_btnCurrency->SetData(CurrencyInfo.mCurrencyType, CurrencyInfo.mCurrency);
	_btnCurrency->SetButtonEnable(isCurrencyBtnEnable);

	// 2023/10/12 PKT - 슬롯이 전부 다 채워지지 않는 상황에 재화가 부족한 경우도 있어. 여기서는 재화가 부족한지만 체크 하자. (isTryEnable 이 변수를 사용하지 않음. )
	bool isNotEnoughCurrency = (_fusionHandler->GetCost().mCurrency > FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD));
	_btnCurrency->SetAmountTextColor((true == isNotEnoughCurrency) ? EGsUIColorType::DEFAULT_INVALID : EGsUIColorType::DEFAULT_VALID);
	
	// 2023/10/4 PKT - Inventroy 업데이트
	FGsMessageHolder* msgSender = GMessage();
	if (msgSender)
	{
 		msgSender->GetInventoryCommon().SendMessage(MessageContentCommonInven::INVALIDATE_ALL, nullptr);
	}
}

void UGsUIPopupItemFusion::ActionFusionEffect(const ItemDBId InFusionItemDBId)
{
	// 2023/9/26 PKT - Animation 정보 초기화
	StopEffect(_idleAnimation);
	// 2023/10/5 PKT - Fusion Item
	FillIconWithItemData(_iconFusionItemSelector, _textFusionItemName, InFusionItemDBId);
	// 2023/10/5 PKT - Fusion Animation
	PlayEffect(_FusionAnimation, false);
}

void UGsUIPopupItemFusion::OnFusionAnimationFinish()
{
	if (nullptr == _fusionHandler)
	{
		GSLOG(Error, TEXT("nullptr == _fusionHandler"));
		return;
	}

	_fusionHandler->CleanView();
}