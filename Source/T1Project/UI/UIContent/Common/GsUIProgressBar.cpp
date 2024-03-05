#include "GsUIProgressBar.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "Image.h"
#include "TextBlock.h"
#include "T1Project.h"

static FProgressBarStyle* DefaultProgressBarStyle = nullptr;

UGsUIProgressBar::UGsUIProgressBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultProgressBarStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultProgressBarStyle = new FProgressBarStyle(FCoreStyle::Get().GetWidgetStyle<FProgressBarStyle>("ProgressBar"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultProgressBarStyle->UnlinkColors();
	}

	BaseWidgetStyle = *DefaultProgressBarStyle;
	BaseWidgetStyle.FillImage.TintColor = FLinearColor::White;

	MultiWidgetStyle = *DefaultProgressBarStyle;
	MultiWidgetStyle.FillImage.TintColor = FLinearColor::White;

	BarFillType = EProgressBarFillType::LeftToRight;
	bIsMarquee = false;
	Percent = 0.f;
	BaseFillColorAndOpacity = FLinearColor::White;
	MultiFillColorAndOpacity = FLinearColor::White;
	BorderPadding = FVector2D(0, 0);
}

void UGsUIProgressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_decoSlot = GetDecoImageSlot(_panel);
	_effProgressBarRenewal->HideVFX();
}

void UGsUIProgressBar::SynchronizeProperties()
{
	_baseProgressBar->WidgetStyle = BaseWidgetStyle;
	_multiProgressBar->WidgetStyle = MultiWidgetStyle;

	_baseProgressBar->BorderPadding = BorderPadding;
	_multiProgressBar->BorderPadding = BorderPadding;

	_baseProgressBar->BarFillType = BarFillType;
	_multiProgressBar->BarFillType = BarFillType;

	_baseProgressBar->Percent = Percent;
	_multiProgressBar->Percent = SecontPercent > Percent ? SecontPercent : Percent;

	_baseProgressBar->bIsMarquee = bIsMarquee;
	//_multiProgressBar->bIsMarquee = bIsMarquee;  //multiProgressBar 처리 방식에 대해서는 아직 잘 모르겠으므로 일단 생략

	_baseProgressBar->SetFillColorAndOpacity(BaseFillColorAndOpacity);
	_multiProgressBar->SetFillColorAndOpacity(MultiFillColorAndOpacity);

	_baseProgressBar->SynchronizeProperties();
	_multiProgressBar->SynchronizeProperties();

	if(_decoSlot == nullptr) _decoSlot = GetDecoImageSlot(_panel);

	auto maxSize = _baseProgressBar->GetCachedGeometry().GetLocalSize() - (BorderPadding * 2.0f);
	UpdateDecoPosition(maxSize);

	Super::SynchronizeProperties();
}

void UGsUIProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	_destPercentTime = 0.f;
	_destPercent = 0.f;

	auto InsetPadding = _baseProgressBar->BorderPadding;
	_maxSize = _baseProgressBar->GetCachedGeometry().GetLocalSize() - (InsetPadding * 2.0f);
}

void UGsUIProgressBar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (ProgressSpeedPerRate > 0.f && _destPercentTime > 0.f)
	{
		UpdateProgress(InDeltaTime);
	}

	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (_maxSize.X <= 0 || _maxSize.Y <= 0)
	//{
	//	auto InsetPadding = _baseProgressBar->BorderPadding;
	//	_maxSize = _baseProgressBar->GetCachedGeometry().GetLocalSize() - (InsetPadding * 2.0f);
	//	UpdatePercent(Percent);
	//}
}

void UGsUIProgressBar::UpdatePercent(float InPercent, bool InIsFull /*= false*/)
{
	// 추가 로직 (1.0 일때 프로그레스바가 가득 차 있음)
	if (InIsFull)
	{
		_baseProgressBar->SetPercent(FMath::Clamp(InPercent, 0.f, 1.f));
	}
	// 기존 로직 (1.0 일때 프로그레스바가 비어 있게 된다)
	else
	{
		const bool IsProgressIng = (nullptr != _decoWidget && _decoWidget->GetVisibility() != ESlateVisibility::Collapsed) ? true : false;
		GSLOG(Log, TEXT("%d, %d, %d"), IsProgressIng ? 1 : 0, StaticCast<int32>(Percent), StaticCast<int32>(InPercent));
		if (IsProgressIng && ((StaticCast<int32>(Percent) < StaticCast<int32>(InPercent)) || FMath::IsNearlyEqual(InPercent, _maxPercent)))
		{
			_onArrivedFullProgressOnAnimation.ExecuteIfBound();
		}

		Percent = InPercent;

		const float RealPercent = (FMath::IsNearlyEqual(_maxPercent, Percent)) ? 1.f : InPercent - StaticCast<int32>(InPercent);

		_baseProgressBar->SetPercent(RealPercent);
		_multiProgressBar->SetPercent(RealPercent);

		//상위 component의 크기 변화로 _maxSize 변동이 생김 (destruct 되지 않고 재사용 될 때)
		_maxSize = _baseProgressBar->GetCachedGeometry().GetLocalSize() - (BorderPadding * 2.0f);

		UpdateDecoPosition(_maxSize);
	}
}

void UGsUIProgressBar::UpdateSecondPercent(float InPercent, bool InIsFull /*= false*/)
{
	// 추가 로직 (1.0 일때 프로그레스바가 가득 차 있음)
	if (InIsFull)
	{
		_multiProgressBar->SetPercent(FMath::Clamp(InPercent, 0.f, 1.f));
	}
	// 기존 로직 (1.0 일때 프로그레스바가 비어 있게 된다)
	else
	{
		SecontPercent = Percent < InPercent ? InPercent : Percent;
		_multiProgressBar->SetPercent(InPercent - StaticCast<int32>(InPercent));
	}
}

void UGsUIProgressBar::UpdateProgress(float InDeltaTime)
{
	_destPercentTime -= InDeltaTime;
	auto usePercent = _destPercent > 0.f ? _destPercent : _destSecondPercent;
	auto per = usePercent - ((_destPercentTime / ProgressSpeedPerRate) * SpeedCalculateRate);

	if (_destPercent > 0.f)
		UpdatePercent(_destPercentTime <= 0.f ? _destPercent : per);
	else if (_destSecondPercent > 0.f)
		UpdateSecondPercent(_destPercentTime <= 0.f ? _destSecondPercent : per);

	if (_destPercentTime <= 0.f)
	{
		_destPercent = 0.f;
		_destPercentTime = 0.f;

		_decoWidget->SetVisibility(ESlateVisibility::Collapsed);

		auto prePercent = _multiProgressBar->Percent;
		if (_destSecondPercent > 0.f && _destSecondPercent > prePercent)
		{
			if (_destSecondPercent <= _multiProgressBar->Percent)
				 _destSecondPercent = 0.f;
			else _destPercentTime = (_destSecondPercent - _baseProgressBar->Percent) / SpeedCalculateRate * ProgressSpeedPerRate;
		}
	}

	if (_destSecondPercent <= 0.f && _isShowEffect)
		SetProgressIncreaseEffect();
}

void UGsUIProgressBar::UpdateDecoPosition(const FVector2D& MaxSize)
{
	auto InsetPadding = BorderPadding;
	const float OnlyPercent = Percent - StaticCast<int32>(Percent);
	if (BarFillType == EProgressBarFillType::LeftToRight)
	{
		auto pos = InsetPadding + DecoOrigin + FVector2D(OnlyPercent, 0.f) * MaxSize;
		if (_decoSlot) _decoSlot->SetPosition(pos);
	}
	else if (BarFillType == EProgressBarFillType::RightToLeft)
	{
		auto pos = InsetPadding + (DecoOrigin + FVector2D(MaxSize.X, 0.f)) - FVector2D(OnlyPercent, 0.f) * MaxSize;
		if (_decoSlot) _decoSlot->SetPosition(pos);
	}
	else if (BarFillType == EProgressBarFillType::TopToBottom)
	{
		auto pos = InsetPadding + DecoOrigin + FVector2D(0.f, OnlyPercent) * MaxSize;
		if (_decoSlot) _decoSlot->SetPosition(pos);
	}
	else if (BarFillType == EProgressBarFillType::BottomToTop)
	{
		auto pos = InsetPadding + (DecoOrigin + FVector2D(0.f, MaxSize.Y)) - FVector2D(0.f, OnlyPercent) * MaxSize;
		if (_decoSlot) _decoSlot->SetPosition(pos);
	}
	else if (BarFillType == EProgressBarFillType::FillFromCenter)
	{
		//deco가 두개 이상이 되어야 하므로 처리하지 않는다.
	}

	if(_decoWidget) _decoWidget->SetVisibility(IsPlayProgress() ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

UCanvasPanelSlot* UGsUIProgressBar::GetDecoImageSlot(UCanvasPanel* panel)
{
	auto slots = panel->GetSlots();
	TArray<UWidget*> childList = panel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UUserWidget>())
		{
			auto index = panel->GetChildIndex(child);

			if (slots.IsValidIndex(index))
			{
				auto widget = Cast<UUserWidget>(child);
				if (_decoWidget == widget)
				{
					return Cast<UCanvasPanelSlot>(slots[index]);
				}
			}
		}
	}
	return nullptr;
}

void UGsUIProgressBar::SetPercent(float InPercent, bool InIsFull /*= false*/)
{
	UpdatePercent(InPercent, InIsFull);

	_isShowEffect = false;
	_destPercent = 0.f;
	_destSecondPercent = 0.f;
	_destPercentTime = 0.f;
}

void UGsUIProgressBar::SetMaxPercent(float InMaxPercent)
{
	_maxPercent = InMaxPercent;
}

void UGsUIProgressBar::SetSecondPercent(float InPercent, bool InIsFull /*= false*/)
{
	UpdateSecondPercent(InPercent, InIsFull);

	_isShowEffect = false;
	_destSecondPercent = 0.f;
	_destPercentTime = 0.f;
}

void UGsUIProgressBar::SetProgress(float InPercent, float extProgress, bool anim)
{
	if (ProgressSpeedPerRate > 0.f && anim)
	{
		auto prePercent = _destPercent > 0.f ? _destPercent : Percent;
		if (InPercent > prePercent || extProgress > InPercent)
		{
			auto usePercent = extProgress > InPercent ? extProgress : InPercent;
			_destPercentTime += ((usePercent - prePercent) / SpeedCalculateRate * ProgressSpeedPerRate);
			_destPercent = InPercent > prePercent ? InPercent : 0.f;
			_destSecondPercent = extProgress <= InPercent ? 0.f : extProgress;

			_decoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			return;
		}
	}

	SetPercent(InPercent);
}

void UGsUIProgressBar::SetProgressText(FText cur)
{
	_txtProgress->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_txtProgress->SetText(cur);
}

void UGsUIProgressBar::SetProgressIncreaseEffect()
{
	if (IsPlayProgress())
	{
		_isShowEffect = true;
		return;
	}

	_isShowEffect = false;
	_effProgressBarRenewal->ShowVFX();
}

bool UGsUIProgressBar::IsPlayProgress()
{
	return ProgressSpeedPerRate > 0.f && _destPercentTime > 0.f;
}

void UGsUIProgressBar::SetDecoWidgetVisibilityByAlpha(bool InVisible)
{
	if (nullptr == _decoWidget)
	{
		return;
	}

	if (InVisible)
	{
		_decoWidget->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		_decoWidget->SetColorAndOpacity(FLinearColor::Transparent);
	}
}
