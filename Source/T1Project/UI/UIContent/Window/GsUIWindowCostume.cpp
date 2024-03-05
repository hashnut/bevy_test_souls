#include "GsUIWindowCostume.h"
#include "Management/ScopeGame/GsCostumeManager.h"

#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/Window/Fairy/GsSelectedPanelInterface.h"
#include "UI/UIContent/Window/Costume/GsUIPageCostume.h"
#include "UI/UIContent/Window/Costume/GsUIPageCostumeCollection.h"
#include "UI/UIContent/Window/Costume/GsUIPageCostumeCompose.h"
#include "UI/UIContent/Window/Costume/GsUIPageCostumeSummonConfirm.h"
#include "UI/UIContent/Window/Costume/GsUIPageCostumeAppearance.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

void UGsUIWindowCostume::NativeOnInitialized()
{	
	_helpContentsKey = EGsHelpContents::Costume;
	TArray<UGsSwitcherButton*> btnTabs =
	{
		_btnCostume, _btnCompose, _btnCollection, _btnSummonWait
	};

	TArray<IGsSelectedPanelInterface*> interfaces =
	{
		_contentsCostume, _contentsCompose, _contentsCollection, _contentsSummonWait
	};

	for (auto page : {	Cast<UUserWidget>(_contentsCostume), Cast<UUserWidget>(_contentsCompose),
						Cast<UUserWidget>(_contentsCollection), Cast<UUserWidget>(_contentsSummonWait) })
	{
		auto index = _tabSwitcher->GetChildIndex(page);

		_calls.Emplace(index, interfaces[index]);
		_toggleGroupFunction.AddToggleWidget(btnTabs[index]);
	}
	_currentFunction = -1;
	_toggleGroupFunction.OnSelectChanged.BindUObject(this, &UGsUIWindowCostume::OnFunctionTabChanged);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowCostume::OnClickCloseAllStack);

	Super::NativeOnInitialized();
}

void UGsUIWindowCostume::NativeConstruct()
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

void UGsUIWindowCostume::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIWindowCostume::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_currentFunction < 0)
	{
		_toggleGroupFunction.SetSelectedIndex(_initTab, true);//OnTabChanged 불리기 전은, 강제로 toggleGroup 처리를 지시해야 한다
		_initTab = 0;
	}
}

FReply UGsUIWindowCostume::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

FReply UGsUIWindowCostume::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIWindowCostume::OnFunctionTabChanged(int32 index)
{
	if (_currentFunction == index && index < 0) return;

	static TArray<int32> enableViewTabs = {
		_tabSwitcher->GetChildIndex(_contentsCostume),
		_tabSwitcher->GetChildIndex(_contentsAppearance)
	};

	_tabSwitcher->SetActiveWidgetIndex(index);
	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetUnselect();
	}

	_currentFunction = index;
	_costumeBg->SetVisibility(enableViewTabs.Find(_currentFunction) != INDEX_NONE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetSelect();
	}

	OnUpdateRedDot();
}

bool UGsUIWindowCostume::OnBack()
{
	if (auto costumeManager = GSCostume())
	{
		if (costumeManager->IsEffectPlaying())
		{
			return false;
		}
	}
	if (_contentsCollection && _contentsCollection->CloseChildWidget())
	{
		return true;
	}
	_isOpend = false;
	return Super::OnBack();
}

void UGsUIWindowCostume::OnClickCloseAllStack()
{
	if (auto costumeManager = GSCostume())
	{
		if (costumeManager->IsEffectPlaying())
		{
			return;
		}
	}

	_isOpend = false;
	CloseProcess();

	Super::OnClickCloseAllStack();
}

//close() 를 에디터를 끄거나 play를 종료할때도 부름(왜인지는 모름) 버튼 누르는 곳으로 바꿔줌
void UGsUIWindowCostume::CloseProcess()
{
	if (auto iter = _calls.Find(_currentFunction))
	{
		(*iter)->SetUnselect();
	}

	_currentFunction = -1;
}

bool UGsUIWindowCostume::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsCostume))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COSTUME);
		return true;
	}

	return false;
}

void UGsUIWindowCostume::InitializeData(UIInitData* initParam)
{
	for (auto call : _calls)
	{
		if (call.Value) call.Value->InitializeTab(call.Key);
	}

	ChangeInitTab(initParam);
	OnUpdateRedDot();
}

void UGsUIWindowCostume::ReleaseData()
{
	for (auto call : _calls)
	{
		if (call.Value) call.Value->ReleasedTab();
	}
}

void UGsUIWindowCostume::ChangeInitTab(UIInitData* initParam)
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

void UGsUIWindowCostume::OnUpdateRedDot()
{
	_redDotCostume->SetVisibility(GSCostume()->IsCostumeRedDot() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_redDotCollection->SetVisibility(GSCostume()->IsCostumeCollectionRedDot() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_redDotCompose->SetVisibility(ESlateVisibility::Hidden);
	_redDotDress->SetVisibility(ESlateVisibility::Hidden);

	_redDotDress->SetVisibility(GSCostume()->IsCostumeSummonItemRedDot() ?
		ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowCostume::OnUpdateCostume(TArray<CostumeId>& dataList)
{
	_contentsCostume->OnUpdateItemList(dataList);
}

void UGsUIWindowCostume::OnUpdateCostumeCollection(TArray<CollectionId>& dataList)
{
	_contentsCollection->OnUpdateItemList(dataList);
}

void UGsUIWindowCostume::OnSummonComplete()
{
	_contentsCompose->OnSummonComplete();
}

void UGsUIWindowCostume::OnChangeDressUp()
{
	if (_contentsCostume)
	{
		_contentsCostume->OnChangeDressUp();

		OnClickCloseAllStack();
	}
}

void UGsUIWindowCostume::OnObtainCostumeResult(CostumeObtainReason reason, TArray<CostumeId>& dataList, bool isHideWindow)
{
	if (isHideWindow)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	if (reason == CostumeObtainReason::COMPOSE && _contentsCompose)
	{
		_contentsCompose->OnComposeResult(dataList);
	}

	if (reason == CostumeObtainReason::NONE && _contentsSummonWait)
	{
		_contentsSummonWait->OnNotiComfirmChanged();
	}

	//if (reason == CostumeObtainReason::CEILING && _contentsCeiling)
	//{
	//	_contentsCeiling->OnCeilingResult();
	//}

}

void UGsUIWindowCostume::OnCeilingCostumeResult(bool success)
{
	if (_contentsCompose)
	{
		_contentsCompose->OnCeilingResult(success);
	}
}

void UGsUIWindowCostume::OnUpdateSummonWaiting(CostumeConfirmId summonWaitDBId)
{
	if (_contentsSummonWait)
	{
		_contentsSummonWait->OnUpdateSummonWaiting(summonWaitDBId);
	}

	OnUpdateRedDot();
}

void UGsUIWindowCostume::OnConfirmSummonWaiting(CostumeConfirmId summonWaitDBId)
{
	if (_contentsSummonWait)
	{
		_contentsSummonWait->OnConfirmSummonWaiting(summonWaitDBId);
	}

	OnUpdateRedDot();
}

void UGsUIWindowCostume::OnFindCostume(CostumeId id)
{
	if (_contentsCostume)
	{
		_contentsCostume->OnSetScrollToSelectedCostumeItem(id);
		_toggleGroupFunction.SetSelectedIndex(0, _currentFunction != 0);
	}
}

void UGsUIWindowCostume::SetTutorial(int32 InTargetIndex)
{
	if (-1 == InTargetIndex)
	{
		OnTutorialListEvent.Unbind();
		_contentsCostume->SetTutorialTarget(-1);
	}
	else
	{
		_contentsCostume->OnTutorialSettingComplete.BindUObject(this, &UGsUIWindowCostume::OnTutorialSettingComplete);
		_contentsCostume->SetTutorialTarget(InTargetIndex);
	}
}

void UGsUIWindowCostume::OnTutorialSettingComplete(UWidget* InTarget)
{
	_contentsCostume->OnTutorialSettingComplete.Unbind();

	OnTutorialListEvent.ExecuteIfBound(InTarget);
}
