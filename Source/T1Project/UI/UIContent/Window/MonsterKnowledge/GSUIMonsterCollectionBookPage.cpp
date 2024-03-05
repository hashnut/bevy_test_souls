#include "GSUIMonsterCollectionBookPage.h"
#include "GSUIMonsterCollectionPortrait.h"
#include "GsUIMonsterCollectionPortraitSet.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Classes/GsPoolUObject.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Common/GsUITabButtonCommon.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterBookDetail.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterBookUsedItem.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterCollectionBookResult.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterCollectionBookGradeRewardInfo.h"
#include "Object.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/TileView.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"



void UGSUIMonsterCollectionBookPage::BeginDestroy()
{
	if (_slotHelperCategoryView)
	{
		_slotHelperCategoryView->OnRefreshEntry.RemoveDynamic(this, &UGSUIMonsterCollectionBookPage::OnRefreshEntryCategoryTab);
	}
	_slotHelperCategoryView = nullptr;

	if (_btnLevelUpAll)
	{
		_btnLevelUpAll->OnClicked.RemoveDynamic(this, &UGSUIMonsterCollectionBookPage::OnClickedBtnLevelUpAll);
	}
	_btnLevelUpAll = nullptr;

	if (_btnUseItemAll)
	{
		_btnUseItemAll->OnClicked.RemoveDynamic(this, &UGSUIMonsterCollectionBookPage::OnClickedBtnUseItemAll);
	}
	_btnUseItemAll = nullptr;

	if (_btnRewardDetail)
	{
		_btnRewardDetail->OnClicked.RemoveDynamic(this, &UGSUIMonsterCollectionBookPage::OnOpenRewardDetail);		
	}
	_btnRewardDetail = nullptr;

	if (_poolPortraitDataSet)
	{
		_poolPortraitDataSet->RemovePool();
		_poolPortraitDataSet = nullptr;
	}

	Super::BeginDestroy();
}

void UGSUIMonsterCollectionBookPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_poolPortraitDataSet = NewObject<UGsPoolUObject>(this);

	const static int32 POOL_WARM_UP_SIZE = 1000;
	_poolPortraitDataSet->WarmUp<UGsMonsterCollectionPortraitEntryData>(POOL_WARM_UP_SIZE);
	
	_toggleGroupCategoryTab.Clear();
	_toggleGroupCategoryTab.OnSelectChanged.BindUObject(this, &UGSUIMonsterCollectionBookPage::OnSelectedCategoryTab);
	
	_slotHelperCategoryView = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCategoryView->Initialize(_subClassOfCategoryView, _scrollCategoryView);
	_slotHelperCategoryView->OnRefreshEntry.AddDynamic(this, &UGSUIMonsterCollectionBookPage::OnRefreshEntryCategoryTab);

	_tileViewPortaitSet->OnListViewScrolled().AddUObject(this, &UGSUIMonsterCollectionBookPage::OnScrollListView);
	_tileViewPortaitSet->OnItemClicked().AddUObject(this, &UGSUIMonsterCollectionBookPage::OnClickedTileViewItem);
	_tileViewPortaitSet->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_btnLevelUpAll->OnClicked.AddDynamic(this, &UGSUIMonsterCollectionBookPage::OnClickedBtnLevelUpAll);
	_btnUseItemAll->OnClicked.AddDynamic(this, &UGSUIMonsterCollectionBookPage::OnClickedBtnUseItemAll);
	_btnRewardDetail->OnClicked.AddDynamic(this, &UGSUIMonsterCollectionBookPage::OnOpenRewardDetail);
}

void UGSUIMonsterCollectionBookPage::NativeConstruct()
{
	Super::NativeConstruct();

	// 2023/2/8 PKT - 0(NONE) 포함 갯수로 정의 함.(전체 Tab이 있으므로.. )
	_slotHelperCategoryView->RefreshAll(MaxTypeCount());
}

void UGSUIMonsterCollectionBookPage::NativeDestruct()
{
	SetTutorial(false);
	SetIsTutorialInteraction(false);

	Super::NativeDestruct();
}

void UGSUIMonsterCollectionBookPage::Enter(struct FGsMonsterKnowledgeUICaptureData* InData)
{
	Super::Enter(InData);

	_savedOptionData = 0;
	_prevActiveUseItemPopup = false;

	_listScrollOffset = InData->_categoryScrollOffset;
	_toggleGroupCategoryTab.SetSelectedIndex(StaticCast<int32>(InData->_categoryTab), true);

	if (INVALID_MONSTER_KNOWLEDGE_ID < StaticCast<uint32>(InData->_lastViewParam))
	{
		ActiveDetailPopup(StaticCast<uint32>(InData->_lastViewParam));
	}

	InvalidateRedDot();
}

void UGSUIMonsterCollectionBookPage::Leave()
{
	Super::Leave();
}

void UGSUIMonsterCollectionBookPage::Capture(FGsMonsterKnowledgeUICaptureData* OutData)
{
	Super::Capture(OutData);

	OutData->_categoryTab = StaticCast<uint32>(_toggleGroupCategoryTab.GetSelectedIndex());
	OutData->_categoryScrollOffset = _listScrollOffset;
	OutData->_lastViewParam = _lastBookId;
}

void UGSUIMonsterCollectionBookPage::InvalidateBook(uint32 InData)
{
	// 2023/2/3 PKT - Book Page RedDot 갱신
	Super::InvalidateBook(InData);
	
	RefreshDisplayListViewEntryWidget(InData);

	// 2023/2/1 PKT - Popup Detail
	TWeakObjectPtr<UGsUIPopupMonsterBookDetail> detailWidget = Cast<UGsUIPopupMonsterBookDetail>(GUI()->GetWidgetByKey("PopupMonsterBookDetail"));
	if (detailWidget.IsValid() && detailWidget->GetData() && detailWidget->GetData()->Id() == InData)
	{
		detailWidget->Invalidate();
	}	
}

void UGSUIMonsterCollectionBookPage::InvalidateBookSet(const TArray<FGsMonsterCollectionBookResult>* InData)
{
	// 2023/2/3 PKT - Book Page RedDot 갱신
	Super::InvalidateBookSet(InData);

	// 2023/2/3 PKT - View List 복수 갱신
	for (const auto& item : (*InData))
	{
		RefreshDisplayListViewEntryWidget(item.id);
	}

	TWeakObjectPtr<UGsUIPopupMonsterBookUsedItem> usedItemWidget = Cast<UGsUIPopupMonsterBookUsedItem>(GUI()->GetWidgetByKey("PopupMonsterBookUsedItem"));
	if (usedItemWidget.IsValid())
	{
		// 2023/2/6 PKT - 아이템 일괄 사용 창에 활성화 중이라면 닫어준다.
		usedItemWidget->Close(true);

		_prevActiveUseItemPopup = true;
	}

	TWeakObjectPtr<UGsUIPopupMonsterCollectionBookResult> resultWidget = Cast<UGsUIPopupMonsterCollectionBookResult>(GUI()->OpenAndGetWidget("PopupMonsterBookResult"));
	if (resultWidget.IsValid())
	{
		resultWidget->SetData((*InData));
		if (false == resultWidget->OnCloseDelegate.IsBoundToObject(this))
		{
			resultWidget->OnCloseDelegate.BindUObject(this, &UGSUIMonsterCollectionBookPage::OnCloseResultPopup);
		}
	}
}

void UGSUIMonsterCollectionBookPage::InvalidateRedDot()
{
	Super::InvalidateRedDot();

	int32 maxCount = _slotHelperCategoryView->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(_slotHelperCategoryView->GetEntry(idx));
		if (false == tabButton.IsValid())
		{
			GSLOG(Error, TEXT("false == tabButton.IsValid()"));
			continue;
		}

		bool isRedDot = GsMonsterCollection()->CheckRedDot(ConvertIndexToType(idx));
		tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, isRedDot);
	}
}

void UGSUIMonsterCollectionBookPage::BuildData(int32 InCategoryIndex)
{
	Super::BuildData(InCategoryIndex);

	GsMonsterCollection()->BuildBookViewSet(ConvertIndexToType(InCategoryIndex));

	_poolPortraitDataSet->ReleaseAll();
	TArray<UGsMonsterCollectionPortraitEntryData*> entrySet;

	for (auto item : GsMonsterCollection()->BookViewSet())
	{
		UGsMonsterCollectionPortraitEntryData* data = _poolPortraitDataSet->Claim<UGsMonsterCollectionPortraitEntryData>();
		data->SetData(item);
		
		// 2023/2/1 PKT - Add..
		entrySet.Emplace(data);
	}
	_tileViewPortaitSet->SetListItems(entrySet);
	_tileViewPortaitSet->RegenerateAllEntries();
	_tileViewPortaitSet->SetScrollOffset(_listScrollOffset);
	_tileViewPortaitSet->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGSUIMonsterCollectionBookPage::OnRefreshEntryCategoryTab(int32 InIndex, UWidget* InEntry)
{
	//TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	//if (false == tabButton.IsValid())
	//{
	//	GSLOG(Error, TEXT("false == tabButton.IsValid()"));
	//	return;
	//}

	//FText textCategoryName;

	//MonsterKnowledgeBookGrade bookGrade = ConvertIndexToType(InIndex);
	//if (MonsterKnowledgeBookGrade::MAX == bookGrade)
	//{
	//	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("BookGradeAll"), textCategoryName);
	//}
	//else
	//{
	//	auto schemaDetailInfo = GsMonsterCollection()->BookGradeDetailInfo(bookGrade);
	//	if (schemaDetailInfo)
	//	{
	//		textCategoryName = schemaDetailInfo->gardeName;
	//	}
	//}

	//tabButton->SetTitleText(textCategoryName);
	//tabButton->SetIsRedDot(EGsIconRedDotType::NORMAL, false);

	//// 2021/08/02 PKT - bind widget
	//_toggleGroupCategoryTab.AddToggleWidget(tabButton->GetToggleGroupEntry());
}

void UGSUIMonsterCollectionBookPage::OnSelectedCategoryTab(int32 InIndex)
{
	BuildData(InIndex);
}

void UGSUIMonsterCollectionBookPage::OnClickedBtnLevelUpAll()
{
	// 2021/11/23 PKT - 서버에게 레벨업 가능 모든 몬스터들에 대한 레벨업을 요청한다.
	FGsNetSendServiceWorld::Send_MonsterKnowledgeLevelAll();
}

void UGSUIMonsterCollectionBookPage::OnClickedBtnUseItemAll()
{
	ActiveUsedItemPopup();
}

void UGSUIMonsterCollectionBookPage::OnScrollListView(float InOffsetInItems, float InDistanceRemaining)
{
	// 튜토리얼 스크롤 막음
	if (IsTutorial())
	{
		_tileViewPortaitSet->SetScrollOffset(_listScrollOffset);
	}
	else
	{
		_listScrollOffset = InOffsetInItems;
	}
}

void UGSUIMonsterCollectionBookPage::OnClickedTileViewItem(UObject* InListItem)
{
	TWeakObjectPtr<UGsMonsterCollectionPortraitEntryData> item = Cast<UGsMonsterCollectionPortraitEntryData>(InListItem);
	if (false == item.IsValid() && nullptr == item->Get())
	{
		GSLOG(Error, TEXT("false == item.IsValid() && nullptr == item->Get()"));
		return;
	}

	if (EMonsterKnowledgeState::LevelUpPossible == item->Get()->State())
	{	// 2023/2/1 PKT - Send Level Up
		FGsNetSendServiceWorld::Send_MonsterKnowledgeLevelUp(item->Get()->Id());
	}
	else
	{
		// 2023/2/20 PKT - Detail Popup
		ActiveDetailPopup(item->Get()->Id());

		if (GUI())
		{
			GUI()->PlayPressSound();
		}
	}
}

void UGSUIMonsterCollectionBookPage::OnSavedUsedItemOption(uint64 InData)
{
	_savedOptionData = InData;
}

void UGSUIMonsterCollectionBookPage::OnCloseResultPopup()
{
	if (_prevActiveUseItemPopup)
	{	 // 2023/2/6 PKT - 이전에 아이템 일괄 사용 팝업 활성화 중이였다면 
		ActiveUsedItemPopup();
	}
}

void UGSUIMonsterCollectionBookPage::OnOpenRewardDetail()
{
	TWeakObjectPtr<UGsUIPopupMonsterCollectionBookGradeRewardInfo> item 
		= Cast<UGsUIPopupMonsterCollectionBookGradeRewardInfo>(GUI()->OpenAndGetWidget("PopupMonsterCollectionBookGradeRewardInfo"));
	if (item.IsValid())
	{
		item->Open();
	}
}

void UGSUIMonsterCollectionBookPage::OnCloseDetailPopup(uint32 InId)
{
	if (_lastBookId == InId)
	{
		_lastBookId = INVALID_MONSTER_KNOWLEDGE_ID;
	}
}

void UGSUIMonsterCollectionBookPage::RefreshDisplayListViewEntryWidget(uint32 InId)
{
	const TArray<UUserWidget*>& displayWidgets = _tileViewPortaitSet->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : displayWidgets)
	{
		TWeakObjectPtr<UGSUIMonsterCollectionPortrait> item = Cast<UGSUIMonsterCollectionPortrait>(widget);
		if (false == item.IsValid() && nullptr == item->GetData())
		{
			continue;
		}

		if (item->GetData()->Id() == InId)
		{
			item->Invalidate();
			break;
		}
	}
}

void UGSUIMonsterCollectionBookPage::ActiveDetailPopup(uint32 InId)
{
	TWeakObjectPtr<UGsMonsterCollectionPortraitEntryData> entryData;

	for (int32 idx = 0; idx < _tileViewPortaitSet->GetNumItems(); ++idx)
	{
		TWeakObjectPtr<UGsMonsterCollectionPortraitEntryData> item = Cast<UGsMonsterCollectionPortraitEntryData>(_tileViewPortaitSet->GetItemAt(idx));
		if (item.IsValid() && (InId == item->Get()->Id()))
		{
			entryData = item;
			break;
		}
	}

	if (entryData.IsValid())
	{	TWeakObjectPtr<UGsUIPopupMonsterBookDetail> detailWidget = Cast<UGsUIPopupMonsterBookDetail>(GUI()->OpenAndGetWidget("PopupMonsterBookDetail"));
		if (false == detailWidget.IsValid())
		{
			return;
		}
		detailWidget->SetData(entryData->Get());

		if (false == detailWidget->_OnClickedCloseDelegate.IsBoundToObject(this))
		{	// 2023/2/20 PKT - Long Press Bind
			detailWidget->_OnClickedCloseDelegate.BindUObject(this, &UGSUIMonsterCollectionBookPage::OnCloseDetailPopup);
		}

		_lastBookId = entryData->Get()->Id();
	}
}

void UGSUIMonsterCollectionBookPage::ActiveUsedItemPopup()
{	
	TWeakObjectPtr<UGsUIPopupMonsterBookUsedItem> usedItemWidget = Cast<UGsUIPopupMonsterBookUsedItem>(GUI()->OpenAndGetWidget("PopupMonsterBookUsedItem"));
	if (usedItemWidget.IsValid())
	{
		usedItemWidget->SetData(_savedOptionData);
		if (false == usedItemWidget->OnSavedOptionDelegate.IsBoundToObject(this))
		{
			usedItemWidget->OnSavedOptionDelegate.BindUObject(this, &UGSUIMonsterCollectionBookPage::OnSavedUsedItemOption);
		}

		_prevActiveUseItemPopup = false;
	}
}

void UGSUIMonsterCollectionBookPage::SetIsTutorialInteraction(bool bIsTutorial)
{ 
	_bIsTutorialInteraction = bIsTutorial; 

	if (false == _bIsTutorialInteraction)
	{
		OnTutorialInteraction.Unbind();
	}
}

void UGSUIMonsterCollectionBookPage::OnClickTutorialRegion()
{
	if (IsTutorialInteraction())
	{
		if (UObject* firstItem = _tileViewPortaitSet->GetItemAt(0))
		{
			TWeakObjectPtr<UGsMonsterCollectionPortraitEntryData> item = Cast<UGsMonsterCollectionPortraitEntryData>(firstItem);
			if (false == item.IsValid() && nullptr == item->Get())
			{
				GSLOG(Error, TEXT("false == item.IsValid() && nullptr == item->Get()"));
				return;
			}

			ActiveDetailPopup(item->Get()->Id());
		}
	}
}

UWidget* UGSUIMonsterCollectionBookPage::SetTutorial(bool bIsTutorial)
{ 
	_bIsTutorial = bIsTutorial;

	if (_bIsTutorial)
	{
		if (UObject* firstItem = _tileViewPortaitSet->GetItemAt(0))
		{
			if (UWidget* firstEntry = _tileViewPortaitSet->GetEntryWidgetFromItem(firstItem))
			{
				return firstEntry;
			}
		}
	}

	return nullptr;
}
