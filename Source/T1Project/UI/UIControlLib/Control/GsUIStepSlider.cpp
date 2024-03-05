// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIStepSlider.h"
#include "Components/Slider.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIStepSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slider->OnValueChanged.AddDynamic(this, &UGsUIStepSlider::OnSliderValueChanged);
	_btnIncrease->OnClicked.AddDynamic(this, &UGsUIStepSlider::OnClickIncrease);
	_btnDecrease->OnClicked.AddDynamic(this, &UGsUIStepSlider::OnClickDecrease);
}

void UGsUIStepSlider::SetValue(float InValue)
{
	_slider->SetValue(InValue);
}

float UGsUIStepSlider::GetValue() const
{
	return _slider->GetValue();
}

void UGsUIStepSlider::OnClickIncrease()
{
	float sliderValue = FMath::Clamp(_slider->GetValue() + _slider->StepSize, 0.f, 1.f);
	_slider->SetValue(sliderValue);

	OnValueChanged.ExecuteIfBound(sliderValue);
}

void UGsUIStepSlider::OnClickDecrease()
{
	float sliderValue = FMath::Clamp(_slider->GetValue() - _slider->StepSize, 0.f, 1.f);
	_slider->SetValue(sliderValue);

	OnValueChanged.ExecuteIfBound(sliderValue);
}

void UGsUIStepSlider::OnSliderValueChanged(float InValue)
{
	OnValueChanged.ExecuteIfBound(InValue);
}
