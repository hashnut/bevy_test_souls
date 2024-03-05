

#include "GsTreeView.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIContent/Common/TreeView/GsUITreeViewMainElement.h"
#include "UI/UIContent/Common/TreeView/GsUITreeViewSubElement.h"

#include "T1Project/T1Project.h"



void UGsUITreeView::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();

	_slotHelperBtnSubElement = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperBtnSubElement->Initialize(_subClassOfBtnSubElement, _verticalSubElement);
	_slotHelperBtnSubElement->OnCreateEntry.AddDynamic(this, &UGsUITreeView::OnCreateSubElement);	
	_slotHelperBtnSubElement->OnRefreshEntry.AddDynamic(this, &UGsUITreeView::OnRefreshSubElement);
}

void UGsUITreeView::BeginDestroy()
{
	if (_slotHelperBtnSubElement)
	{
		_slotHelperBtnSubElement->OnCreateEntry.RemoveDynamic(this, &UGsUITreeView::OnCreateSubElement);
		_slotHelperBtnSubElement->OnRefreshEntry.RemoveDynamic(this, &UGsUITreeView::OnRefreshSubElement);
		_slotHelperBtnSubElement = nullptr;
	}

	UUserWidget::BeginDestroy();
}

void UGsUITreeView::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	// 2021/12/03 PKT - Animation 정보 초기화
	_animtionType = FoldAnimtion::Force;
	_animPlaySlotIndex = DONE_ANIMATION_FOLD_VALUE;

	if (false == _baseElement->OnClickedDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - Clicked Delegate
		_baseElement->OnClickedDelegate.BindUObject(this, &UGsUITreeView::OnClickedMainElement);
	}

	if (false == _baseElement->OnSelectedDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - State Changed Delegate
		_baseElement->OnSelectedDelegate.BindUObject(this, &UGsUITreeView::OnSelectedMainElement);
	}

	if (false == _baseElement->OnRedDotDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - Red Dot Delegate
		_baseElement->OnRedDotDelegate.BindUObject(this, &UGsUITreeView::OnRedDotMainElement);
	}

	//_slotHelperBtnSubElement->RefreshAll(ELEMENT_HIDE_ALL_INDEX);
}

void UGsUITreeView::NativeDestruct()
{
	// 2021/12/02 PKT - delegate nbind
	_baseElement->OnClickedDelegate.Unbind();
	_baseElement->OnSelectedDelegate.Unbind();
	_baseElement->OnRedDotDelegate.Unbind();

	UUserWidget::NativeDestruct();
}

void UGsUITreeView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	if (FoldAnimtion::Force != _animtionType)
	{	// 2021/12/05 PKT - Animation이 활성화 중이라면...
		const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
		
		const int32 DONE_ANIMATION_UNFOLD_VALUE = maxCount;
		const int32 done_index = (_animtionType == FoldAnimtion::Fold) ? DONE_ANIMATION_FOLD_VALUE : DONE_ANIMATION_UNFOLD_VALUE;

		if (done_index != _animPlaySlotIndex)
		{	// 2021/12/05 PKT - Animation이 끝나지 않았다면..
			const float MaxDeltaTime = float(GData()->GetGlobalData()->_treeViewFoldAnimationTime) / 1000.f;
			_deltaTime += InDeltaTime;
			if (MaxDeltaTime <= _deltaTime)
			{
				_animPlaySlotIndex = FMath::Clamp(_animPlaySlotIndex, 0, (maxCount - 1));
				
				TWeakObjectPtr<UGsUITreeViewBaseElement> elementEntry 
					= Cast<UGsUITreeViewBaseElement>(_slotHelperBtnSubElement->GetEntry(_animPlaySlotIndex));

				if (true == elementEntry.IsValid())
				{
					elementEntry->SetVisibility((_animtionType == FoldAnimtion::Fold) 
						? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
				}

				_animPlaySlotIndex = (_animtionType == FoldAnimtion::Fold) ? _animPlaySlotIndex - 1 : _animPlaySlotIndex + 1;
				_deltaTime = 0.f;
			}
		}
	}
}

void UGsUITreeView::SetIsSelected(bool bInIsSelected)
{
	_baseElement->SetSelected(bInIsSelected);

	if (_animtionType == FoldAnimtion::Force)
	{	// 2021/12/05 PKT - Animation이 활성화 되어 있지 않다면 즉각적으로 Sub Element를 활성화/비활성화
		ForceFold(false == bInIsSelected);
	}
	else
	{
		_animtionType = (true == bInIsSelected) ? FoldAnimtion::UnFold : FoldAnimtion::Fold;
	}

	_deltaTime = 0.f;
}

bool UGsUITreeView::GetIsSelected() const
{
	return _baseElement->GetIsSelected();
};

void UGsUITreeView::OnCreateSubElement(UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITreeViewSubElement> element = Cast<UGsUITreeViewSubElement>(InEntry);
	if (false == element.IsValid())
	{
		GSLOG(Error, TEXT("false == element.IsValid()"));
		return;
	}

	if (false == element->OnClickedDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - Clicked Delegate
		element->OnClickedDelegate.BindUObject(this, &UGsUITreeView::OnClickedSubElement);
	}

	if (false == element->OnSelectedDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - State Changed Delegate
		element->OnSelectedDelegate.BindUObject(this, &UGsUITreeView::OnSelectedSubElement);
	}

	if (false == element->OnRedDotDelegate.IsBoundToObject(this))
	{	// 2021/12/05 PKT - Red Dot Delegate
		element->OnRedDotDelegate.BindUObject(this, &UGsUITreeView::OnRedDotSubElement);
	}
}

void UGsUITreeView::OnClickedMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{	
	// 2021/12/05 PKT - On Clicked..
	if (_baseElement != InElement)
	{
		GSLOG(Error, TEXT("_baseElement != InElement"));
		return;
	}

	SetIsSelected(InIsSelected);
}

void UGsUITreeView::OnSelectedMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{
	if (_baseElement != InElement)
	{
		GSLOG(Error, TEXT("_baseElement != InElement"));
		return;
	}

	// 2021/12/03 PKT - Animation Type
	bool isFold = (false == InIsSelected);

	if (_animtionType == FoldAnimtion::Force)
	{// 2021/12/05 PKT - 일괄적으로 Sub Element 설정
		ForceFold(isFold);
	}
	else
	{
		_animtionType = isFold ? FoldAnimtion::Fold : FoldAnimtion::UnFold;
	}
	
	// 2021/12/03 PKT - 시간 초기화
	_deltaTime = 0.f;

	// 2021/12/05 PKT - On Delegate
	OnSelectedMainElementDelegate.ExecuteIfBound(this, InIsSelected);
}

void UGsUITreeView::OnClickedSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{
	const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITreeViewSubElement> subElement = Cast<UGsUITreeViewSubElement>(_slotHelperBtnSubElement->GetEntry(idx));
		if (subElement.IsValid() && subElement == InElement)
		{
			if (subElement->GetIsSelected() == false && subElement->GetIsSelected() != InIsSelected)
			{	// 2021/12/05 PKT - Sub Element는 비활성화 상태 일 경우 Clicked으로 상태를 변경 할 수 있음.
				subElement->SetSelected(InIsSelected);
			}

			break;
		}
	}
}

void UGsUITreeView::OnSelectedSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{
	const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITreeViewSubElement> subElement = Cast<UGsUITreeViewSubElement>(_slotHelperBtnSubElement->GetEntry(idx));
		if (false == subElement.IsValid())
		{
			continue;
		}

		if (subElement != InElement && true == InIsSelected )
		{
			subElement->SetSelected(false);
		}
		else if (subElement == InElement && true == InIsSelected)
		{	// 2021/12/05 PKT - 서브 Element 상태에 대해 알린다.
			OnSelectedSubElementDelegate.ExecuteIfBound(InElement, InIsSelected);

			if (false == _baseElement->GetIsSelected())
			{
				_baseElement->SetSelected(true);
			}
		}
	}
}

void UGsUITreeView::OnRedDotSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{
	bool isRedDot = false;
	const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITreeViewSubElement> subElement = Cast<UGsUITreeViewSubElement>(_slotHelperBtnSubElement->GetEntry(idx));
		if (subElement.IsValid())
		{
			isRedDot |= subElement->GetIsRedDot();
		}
	}

	// 2021/12/13 PKT - main RedDot 갱신
	_baseElement->SetIsRedDot(isRedDot);
}

void UGsUITreeView::ForceFold(bool InIsFold)
{
	const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITreeViewSubElement> subElement = Cast<UGsUITreeViewSubElement>(_slotHelperBtnSubElement->GetEntry(idx));
		if (subElement.IsValid())
		{
			subElement->SetVisibility(InIsFold ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}

	_animtionType = FoldAnimtion::Force;

	const int32 DONE_ANIMATION_UNFOLD_VALUE = maxCount;
	_animPlaySlotIndex = (true == InIsFold) ? DONE_ANIMATION_FOLD_VALUE : DONE_ANIMATION_UNFOLD_VALUE;

	_deltaTime = 0.f;
}

void UGsUITreeView::OnRefreshSubElement(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITreeViewSubElement> element = Cast<UGsUITreeViewSubElement>(InEntry);
	if (false == element.IsValid())
	{
		GSLOG(Error, TEXT("false == element.IsValid()"));
		return;
	}
	
	if (false == _data.IsValid())
	{
		GSLOG(Error, TEXT("false == _data.IsValid()"));
		return;
	}

	const FGsTreeViewData* treeViewData = _data.Get();

	if (false == treeViewData->childElements.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == IsValidIndex [%d]"), InIndex);
		return;
	}

	element->SetData(&(treeViewData->childElements[InIndex]));
}

void UGsUITreeView::SetData(TSharedPtr<const FGsTreeViewData> InData)
{
	if (false == InData.IsValid())
	{
		return;
	}

	_data = InData;

	_baseElement->SetData(&(_data.Get()->baseData));

	//_baseElement->SetData(MakeShared<const FGsTreeViewElementData>(_data.Get()->baseData));
	
	// 2021/11/24 PKT - Sub Element 생성
	int32 maxCount = _data.Get()->childElements.Num();
	_slotHelperBtnSubElement->RefreshAll(maxCount);

	// 2021/12/03 PKT - Default : sub Element hide 
	ForceFold(true);	
}

void UGsUITreeView::SetOnAnimation(bool InIsActivate)
{
	bool currentOnAnimtion = (_animtionType != FoldAnimtion::Force);

	if (InIsActivate == currentOnAnimtion)
	{	
		return;
	}
	
	bool isFold = (false == _baseElement->GetIsSelected());
	if (InIsActivate)
	{
		_animtionType = (isFold) ? FoldAnimtion::Fold : FoldAnimtion::UnFold;
	}
	else
	{
		ForceFold(isFold);
	}

	_deltaTime = 0.f;
}


void UGsUITreeView::SetSubElementReleaseSelected(const UWidget* InFocusOhterWidget)
{
	const int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUITreeViewSubElement> subElement = Cast<UGsUITreeViewSubElement>(_slotHelperBtnSubElement->GetEntry(idx));
		if (subElement.IsValid() && subElement != InFocusOhterWidget)
		{
			if (true == subElement->GetIsSelected())
			{
				subElement->SetSelected(false);
				break;
			}
		}
	}
}

void UGsUITreeView::OnRedDotMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected)
{
	if (_baseElement != InElement)
	{
		GSLOG(Error, TEXT("_baseElement != InElement"));
		return;
	}
	// 2021/12/13 PKT - 외부에 레드닷 변경을 알린다.
	OnRedDotDelegate.ExecuteIfBound(this, InIsSelected);
}


int32 UGsUITreeView::GetSubElementCount() const
{
	return _slotHelperBtnSubElement->GetItemCount();
}

bool UGsUITreeView::GetIsRedDot() const
{
	return _baseElement->GetIsRedDot();
}

UGsUITreeViewBaseElement* UGsUITreeView::GetSubElementByIndex(int32 InIndex) const
{
	int32 maxCount = _slotHelperBtnSubElement->GetItemCount();
	if (0 > InIndex || InIndex >= maxCount)
	{
		GSLOG(Error, TEXT("invalid InIndex index : [%d], maxCount : [%d]"), InIndex, maxCount);
		return nullptr;
	}
	return Cast<UGsUITreeViewBaseElement>(_slotHelperBtnSubElement->GetEntry(InIndex));
}