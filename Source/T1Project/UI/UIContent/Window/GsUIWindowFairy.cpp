#include "GsUIWindowFairy.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Fairy/GsSelectedPanelInterface.h"
#include "UI/UIContent/Window/Fairy/GsUIPageFairy.h"
#include "UI/UIContent/Window/Fairy/GsUIPageFairyCompose.h"
#include "UI/UIContent/Window/Fairy/GsUIPageFairyCollection.h"
#include "UI/UIContent/Window/Fairy/GsUIPageFairySummonConfirm.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyItem.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Fairy/GsFairyData.h"

#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

void UGsUIWindowFairy::NativeOnInitialized()
{
	_helpContentsKey = EGsHelpContents::Fairy;

	TArray<UGsSwitcherButton*> btnTabs =
	{
		_btnFairy, _btnCompose, _btnCollection, _btnSummoned
	};

	TArray<IGsSelectedPanelInterface*> interfaces = 
	{ 
		_contentsFairy, _contentFairyCompose, _contentFairyCollection, _contentFairySummonComfirm
	};

	for (auto page : {	Cast<UUserWidget>(_contentsFairy), Cast<UUserWidget>(_contentFairyCompose),
						Cast<UUserWidget>(_contentFairyCollection), Cast<UUserWidget>(_contentFairySummonComfirm) })
	{
		auto index = _tabSwitcher->GetChildIndex(page);

		//interfaces[index]->InitializeTab(index);
		_calls.Emplace(index, interfaces[index]);

		btnTabs[index]->SetVisibility(ESlateVisibility::Visible);
		_toggleGroupFunction.AddToggleWidget(btnTabs[index]);
	}

	_currentFunction = -1;
	_toggleGroupFunction.OnSelectChanged.BindUObject(this, &UGsUIWindowFairy::OnFunctionTabChanged);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowFairy::OnClickCloseAllStack);

	Super::NativeOnInitialized();
}

void UGsUIWindowFairy::NativeConstruct()
{
	Super::NativeConstruct();

	_isOpend = true;
	_currentFunction = -1;
	_initTab = 0;
	for (auto call : _calls)
	{
		if (call.Value) call.Value->InitializeTab(call.Key);
	}
}

void UGsUIWindowFairy::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIWindowFairy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_currentFunction < 0)
	{
		_toggleGroupFunction.SetSelectedIndex(_initTab, true);//OnTabChanged 불리기 전은, 강제로 toggleGroup 처리를 지시해야 한다
		_initTab = 0;
	}
}

FReply UGsUIWindowFairy::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

FReply UGsUIWindowFairy::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIWindowFairy::OnFunctionTabChanged(int32 index)
{
	if (_currentFunction == index && index < 0) return;

	_tabSwitcher->SetActiveWidgetIndex(index);
	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetUnselect();
	}

	_currentFunction = index;
	_fairyBg->SetVisibility(_currentFunction == 0 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetSelect();
	}

	OnUpdateRedDot();
}

bool UGsUIWindowFairy::OnBack()
{
	if (auto fairyManager = GSFairy())
	{
		if (fairyManager->IsEffectPlaying())
		{
			return false;
		}
	}

	if (_contentFairyCollection && _contentFairyCollection->CloseChildWidget())
	{
		return true;
	}
	_isOpend = false;
	CloseProcess();

	return Super::OnBack();
}

void UGsUIWindowFairy::OnClickCloseAllStack()
{
	if (auto fairyManager = GSFairy())
	{
		if (fairyManager->IsEffectPlaying())
		{
			return;
		}
	}

	_isOpend = false;
	CloseProcess();

	Super::OnClickCloseAllStack();
}

void UGsUIWindowFairy::CloseProcess()
{
	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetUnselect();
	}

	_currentFunction = -1;
}

bool UGsUIWindowFairy::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsFairy))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_FAIRY);
		return true;
	}

	return false;
}

void UGsUIWindowFairy::InitializeData(UIInitData* initParam)
{
	for (auto call : _calls)
	{
		if (call.Value) call.Value->InitializeTab(call.Key);
	}

	ChangeInitTab(initParam);
	OnUpdateRedDot();
}

void UGsUIWindowFairy::ReleaseData()
{
	for (auto call : _calls)
	{
		if (call.Value) call.Value->ReleasedTab();
	}
}

void UGsUIWindowFairy::ChangeInitTab(UIInitData* initParam)
{
	_currentFunction = -1;
	UIInitData data = UIInitData{ 0, 0 };
	if (initParam != nullptr)
	{
		data._tabType = initParam->_tabType;
		data._initData = initParam->_initData;
	}
		
	if (auto iter = _calls.Find(data._tabType))
	{
		(*iter)->OnInitData(data._initData);
	}

	_initTab = data._tabType;
}

void UGsUIWindowFairy::OnUpdateRedDot()
{
	_redDotFairy->SetVisibility(GSFairy()->IsFairyRedDot() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_redDotCollection->SetVisibility(GSFairy()->IsFairyCollectionRedDot() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_redDotCompose->SetVisibility(ESlateVisibility::Hidden);

	_redDotSummon->SetVisibility(GSFairy()->IsFairySummonItemRedDot() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowFairy::OnUpdateFairy(TArray<FairyId>& dataList)
{
	_contentsFairy->OnUpdateItemList(dataList);
	_contentFairyCompose->OnUpdateItemList(dataList);
}

void UGsUIWindowFairy::OnUpdateFairyCollection(TArray<CollectionId>& dataList)
{
	_contentFairyCollection->OnUpdateItemList(dataList);
}

void UGsUIWindowFairy::OnSummonComplete()
{
	_contentFairyCompose->OnSummonComplete();
}

void UGsUIWindowFairy::OnChangedSummonFairy()
{
	if (_contentsFairy)
	{
		_contentsFairy->OnChangeSummonFairy();
		OnClickCloseAllStack();
	}
}

void UGsUIWindowFairy::OnObtainFairyResult(FairyObtainReason reason, TArray<FairyId>& dataList, bool isHideWindow)
{
	if (isHideWindow)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	if (reason == FairyObtainReason::COMPOSE && _contentFairyCompose)
	{
		_contentFairyCompose->OnComposeFairyResult(dataList);
	}

	if (_contentFairySummonComfirm)
	{
		_contentFairySummonComfirm->OnNotiComfirmChanged();
	}

	//if (reason == FairyObtainReason::CEILING && _contentFairyCeiling)
	//{
	//	_contentFairyCeiling->OnCeilingResult(TODO);
	//}
}

void UGsUIWindowFairy::OnEnchantFairyResult(FairyId id, FairyEnchantResult result)
{
	if (_contentsFairy)
	{
		_contentsFairy->OnEnchantFairyResult(id, result);
	}
}

void UGsUIWindowFairy::OnCeilingFairyResult(FairyEnchantResult result)
{
	_contentFairyCompose->OnCeilingFairyResult(result != FairyEnchantResult::DECREASE);
}

void UGsUIWindowFairy::OnUpdateSummonWaitingFairy(FairyConfirmId summonWaitDBId)
{
	if (_contentFairySummonComfirm)
	{
		_contentFairySummonComfirm->OnUpdateSummonWaitingFairy(summonWaitDBId);
	}

	OnUpdateRedDot();
}

void UGsUIWindowFairy::OnConfirmSummonWaitingFairy(FairyConfirmId summonWaitDBId)
{
	if (_contentFairySummonComfirm)
	{
		_contentFairySummonComfirm->OnConfirmSummonWaitingFairy(summonWaitDBId);
	}

	OnUpdateRedDot();
}

void UGsUIWindowFairy::OnFindFairy(FairyId id)
{
	if (_contentsFairy)
	{	
		_contentsFairy->OnSetScrollToSelectedFairyItem(id);
		_toggleGroupFunction.SetSelectedIndex(0, _currentFunction != 0);
	}
}

void UGsUIWindowFairy::SetTutorial(int32 InTargetIndex)
{
	if (-1 == InTargetIndex)
	{
		OnTutorialListEvent.Unbind();
		_contentsFairy->SetTutorialTarget(-1);
	}
	else
	{
		_contentsFairy->OnTutorialSettingComplete.BindUObject(this, &UGsUIWindowFairy::OnTutorialSettingComplete);
		_contentsFairy->SetTutorialTarget(InTargetIndex);
	}
}

void UGsUIWindowFairy::OnTutorialSettingComplete(UWidget* InTarget)
{
	_contentsFairy->OnTutorialSettingComplete.Unbind();

	OnTutorialListEvent.ExecuteIfBound(InTarget);
}
