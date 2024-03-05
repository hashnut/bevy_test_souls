#include "GsUIPopupSealNodeGainPool.h"

#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Seal/GsSealNode.h"
#include "Seal/GsUISealNodeGainPoolListEntry.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

void UGsUIPopupSealNodeGainPool::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnClose->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSealNodeGainPool::OnClickCloseBtn);

	if (nullptr == _colorListSlotHelper)
	{
		_colorListSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_colorListSlotHelper->Initialize(_subClassOfList, _colorListPanel);
		_colorListSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIPopupSealNodeGainPool::OnRefreshEntryColorList);
	}

	if (nullptr == _statListSlotHelper)
	{
		_statListSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_statListSlotHelper->Initialize(_subClassOfList, _statListPanel);
		_statListSlotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIPopupSealNodeGainPool::OnRefreshEntryStatList);
	}

	if (nullptr != _prevSwitcherBtn)
	{
		_prevSwitcherBtn->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSealNodeGainPool::OnClickPrevBtn);
	}

	if (nullptr != _nextSwitcherBtn)
	{
		_nextSwitcherBtn->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSealNodeGainPool::OnClickNextBtn);
	}
}

void UGsUIPopupSealNodeGainPool::NativeDestruct()
{
	_colorListItems.Empty();
	_statListItems.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupSealNodeGainPool::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	{
		FGsSealManager* SealManager = GSSeal();
		if (nullptr == SealManager)
		{
			GSLOG(Error, TEXT("nullptr == SealManager"));
			return;
		}
		_sealRerollGainsSharePerLevel = SealManager->CollectSealRerollGainsSharePerLevel(_parameters._sealNodeId);

		// fix level
		TSet<Level> DisplayLevelSet;
		_sealRerollGainsSharePerLevel.GetKeys(DisplayLevelSet);
		TArray<Level> DisplayLevels = DisplayLevelSet.Array();

		if (2 > DisplayLevels.Num())
		{
			GSLOG(Error, TEXT("2 > DisplayLevels.Num(), _parameters._sealNodeId:%d"), _parameters._sealNodeId);
			return;
		}

		DisplayLevels.Sort();
		_displayLevelRange = TRange<Level>(DisplayLevels[0], DisplayLevels.Last());
		_parameters._displayLevel = FMath::Clamp(_parameters._displayLevel, _displayLevelRange.GetLowerBoundValue(), _displayLevelRange.GetUpperBoundValue());
	}

	ResetUI();

	Invalidate();
}

void UGsUIPopupSealNodeGainPool::ResetUI()
{
	_scrollBox->ScrollToStart();
}

bool SortGsUISealNodeGainPoolListEntryParameters(const UGsUISealNodeGainPoolListEntry::Parameters& InA, const UGsUISealNodeGainPoolListEntry::Parameters& InB)
{
	return InA._sortOrder < InB._sortOrder;
}

void UGsUIPopupSealNodeGainPool::Invalidate()
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	TWeakPtr<const FGsSealNode> SealNodeWeak = SealManager->GetSealNode(_parameters._sealNodeId);
	if (!SealNodeWeak.IsValid())
	{
		GSLOG(Error, TEXT("!SealNodeWeak.IsValid(), _parameters._sealNodeId:%d"), _parameters._sealNodeId);
		return;
	}

	TSharedPtr<const FGsSealNode> SealNode = SealNodeWeak.Pin();

	// 레이블
	_titleTxt->SetText(SealNode->GetTitle());
	_titleSubTxt->SetText(FGsUIStringHelper::MakeFormatedText(TEXT("Lv.{0}"), SealNode->GetLevel()));

	const FText DisplayLevelTxt = FGsUIStringHelper::MakeFormatedText(TEXT("Lv.{0}"), _parameters._displayLevel);
	if (_parameters._sealNodeLevel == _parameters._displayLevel)
	{
		// the same as my level
		_currentLevelSwitcher->SetActiveWidgetIndex(1);
		_currentLevelSelectedTxt->SetText(DisplayLevelTxt);
	}
	else
	{
		_currentLevelSwitcher->SetActiveWidgetIndex(0);
		_currentLevelTxt->SetText(DisplayLevelTxt);
	}
	
	// 버튼
	if (nullptr != _prevSwitcherBtn)
	{
		_prevSwitcherBtn->SetVisibility(_parameters._displayLevel <= _displayLevelRange.GetLowerBoundValue() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		_prevSwitcherBtn->SetSwitcherIndex(_parameters._sealNodeLevel < _parameters._displayLevel ? 1 : 0);
	}

	if (nullptr != _nextSwitcherBtn)
	{
		_nextSwitcherBtn->SetVisibility(_parameters._displayLevel >= _displayLevelRange.GetUpperBoundValue() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		_nextSwitcherBtn->SetSwitcherIndex(_parameters._sealNodeLevel > _parameters._displayLevel ? 1 : 0);
	}

	// 리스트
	const TPair<TArray<TPair<SealSlotColor, int32>>, TArray<TTuple<StatType, StatValue, int32>>>* SealRerollGainsShare = _sealRerollGainsSharePerLevel.Find(_parameters._displayLevel);
	if (nullptr == SealRerollGainsShare)
	{
		GSLOG(Error, TEXT("nullptr == SealRerollGainsShare"));
	}
	else
	{
		const TArray<TPair<SealSlotColor, int32>>& SealSlotColorShares = SealRerollGainsShare->Key;
		float ColorShareTotal = 0.f;
		for (const TPair<SealSlotColor, int32>& SealSlotColorShare : SealSlotColorShares)
		{
			ColorShareTotal += SealSlotColorShare.Value;
		}

		// TODO: 정렬
		_colorListItems.Empty(SealSlotColorShares.Num());
		for (const TPair<SealSlotColor, int32>& SealSlotColorShare : SealSlotColorShares)
		{
			const float SharePercent = (0.f != ColorShareTotal) ? (SealSlotColorShare.Value / ColorShareTotal) : 0.f;
			FText SharePercentText;
			FGsUIStringHelper::GetFormatedFractionalDigitsText(SharePercent * 100, 3, 3, SharePercentText);
			SharePercentText = FGsUIStringHelper::MakeFormatedText(TEXT("{0} %"), SharePercentText);
			_colorListItems.Add({
				FGsSealManager::MakeSealGradeText(SealSlotColorShare.Key),
				SharePercentText,
				SealSlotColorShare.Key
			});
		}
		_colorListSlotHelper->RefreshAll(_colorListItems.Num());

		const TArray<TTuple<StatType, StatValue, int32>>& SealSlotStatShares = SealRerollGainsShare->Value;
		float StatShareTotal = 0.f;
		for (const TTuple<StatType, StatValue, int32>& SealSlotStatShare : SealSlotStatShares)
		{
			StatShareTotal += SealSlotStatShare.Get<2>();
		}

		// TODO: 정렬
		_statListItems.Empty(SealSlotStatShares.Num());
		for (const TTuple<StatType, StatValue, int32>& SealSlotStatShare : SealSlotStatShares)
		{
			const StatType CurrentStatType = SealSlotStatShare.Get<0>();
			const int32 CurrentShare = SealSlotStatShare.Get<2>();
			FString StatAndValueStr;
			if (!FGsStatHelper::GetDesplayStatNameAndValue(CurrentStatType, SealSlotStatShare.Get<1>(), StatAndValueStr))
			{
				GSLOG(Error, TEXT("!FGsStatHelper::GetDesplayStatNameAndValue(%s, %d, StatAndValueStr)"), *StatTypeEnumToString(CurrentStatType), SealSlotStatShare.Get<1>());
				continue;
			}

			const float SharePercent = (0.f != StatShareTotal) ? (CurrentShare / StatShareTotal) : 0.f;
			FText SharePercentText;
			FGsUIStringHelper::GetFormatedFractionalDigitsText(SharePercent * 100, 3, 3, SharePercentText);
			SharePercentText = FGsUIStringHelper::MakeFormatedText(TEXT("{0} %"), SharePercentText);
			_statListItems.Add({
				FText::FromString(StatAndValueStr),
				SharePercentText,
				SealSlotColor::NONE,
				// 오름차순 > 확률 낮은것 부터, 그다음엔 스탯 Enum 순서대로
				((CurrentShare & 0xffff) << 0x10) | (StaticCast<uint32>(CurrentStatType) & 0xffff)
			});
		}

		_statListItems.Sort(&SortGsUISealNodeGainPoolListEntryParameters);

		// for (const UGsUISealNodeGainPoolListEntry::Parameters& pp : _statListItems)
		// {
		//	GSLOG(Log, TEXT("%u, %u, %u"), (pp._sortOrder >> 0x10) & 0xffff, StaticCast<uint32>(pp._sortOrder) & 0xffff, pp._sortOrder);
		// }

		_statListSlotHelper->RefreshAll(_statListItems.Num());
	}
}

void UGsUIPopupSealNodeGainPool::OnRefreshEntryColorList(int32 InIndex, UWidget* InEntry)
{
	UGsUISealNodeGainPoolListEntry* ListEntry = Cast<UGsUISealNodeGainPoolListEntry>(InEntry);
	if (nullptr == ListEntry)
	{
		GSLOG(Error, TEXT("nullptr == ListEntry, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	if (!_colorListItems.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_colorListSlotHelper.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	ListEntry->SetParameters(_colorListItems[InIndex]);
}

void UGsUIPopupSealNodeGainPool::OnRefreshEntryStatList(int32 InIndex, UWidget* InEntry)
{
	UGsUISealNodeGainPoolListEntry* ListEntry = Cast<UGsUISealNodeGainPoolListEntry>(InEntry);
	if (nullptr == ListEntry)
	{
		GSLOG(Error, TEXT("nullptr == ListEntry, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	if (!_statListItems.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_statListItems.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	ListEntry->SetParameters(_statListItems[InIndex]);
}

void UGsUIPopupSealNodeGainPool::OnClickCloseBtn()
{
	Close();
}

void UGsUIPopupSealNodeGainPool::OnClickPrevBtn()
{
	const Level offsetedLevel = FMath::Clamp<Level>(_parameters._displayLevel - 1, _displayLevelRange.GetLowerBoundValue(), _displayLevelRange.GetUpperBoundValue());
	if (_parameters._displayLevel == offsetedLevel)
	{
		return;
	}

	_parameters._displayLevel = offsetedLevel;
	ResetUI();
	Invalidate();
}

void UGsUIPopupSealNodeGainPool::OnClickNextBtn()
{
	const Level offsetedLevel = FMath::Clamp<Level>(_parameters._displayLevel + 1, _displayLevelRange.GetLowerBoundValue(), _displayLevelRange.GetUpperBoundValue());
	if (_parameters._displayLevel == offsetedLevel)
	{
		return;
	}

	_parameters._displayLevel = offsetedLevel;
	ResetUI();
	Invalidate();
}

UGsUIPopupSealNodeGainPool* UGsUIPopupSealNodeGainPool::OpenThis(Parameters InParameters)
{
	UGsUIManager* UIManager = GUI();
	if (nullptr == UIManager)
	{
		GSLOG(Error, TEXT("nullptr == UIManager"));
		return nullptr;
	}

	TWeakObjectPtr<UGsUIWidgetBase> WidgetBase = UIManager->OpenAndGetWidget(TEXT("PopupSealNodeGainPool"));
	if (!WidgetBase.IsValid())
	{
		GSLOG(Error, TEXT("WidgetBase.IsValid()"));
		return nullptr;
	}

	UGsUIPopupSealNodeGainPool* Popup = Cast<UGsUIPopupSealNodeGainPool>(WidgetBase.Get());
	if (nullptr == Popup)
	{
		GSLOG(Error, TEXT("nullptr == Popup"));
		return nullptr;
	}

	Popup->SetParameters(MoveTemp(InParameters));
	return Popup;
}

void UGsUIPopupSealNodeGainPool::OnInputCancel()
{
	Close();
}
