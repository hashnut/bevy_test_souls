// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionItemSlot.h"

#include "Management/ScopeGlobal/EGsOptionTextColorType.h"

#include "GsSchemaEnums.h"
#include "Option/GsOptionEnumType.h"

#include "Option/GsSchemaOptionButton.h"
#include "Option/GsSchemaOptionSlider.h"
#include "Option/GsSchemaOptionOnOff.h"
#include "Option/GsSchemaOptionText.h"
#include "Option/GsSchemaOptionButtonGroup.h"
#include "Option/GsSchemaOptionToggleGroup.h"
#include "Option/GsSchemaOptionCheckGroup.h"

#include "GsUIOptionButtonGroup.h"
#include "GsUIOptionButton.h"
#include "GsUIOptionToggle.h"
#include "GsUIOptionToggleGroup.h"
#include "GsUIOptionRadioButtonGroup.h"
#include "GsUIOptionCheckButtonGroup.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "HorizontalBox.h"
#include "Slider.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"

#include "WidgetSwitcher.h"
#include "SizeBox.h"
#include "Color.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Engine/Engine.h"


void UGsUIOptionItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 토글 이벤트
	_onOffToggle->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickToggle);

	// 버튼 이벤트
	_button->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickButton);

	// 슬라이더 이벤트
	_slider->OnValueChanged.AddDynamic(this, &UGsUIOptionItemSlot::OnSlider);
	_sliderToggle1->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickToggle);
	_sliderToggle2->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickToggle);

	_optionRadioButtonGroup->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickRadioButtonGroup);
	_optionToggleGroup->OnClickSlot.BindUObject(this, &UGsUIOptionItemSlot::OnClickToggleGroup);
	
	_tooltipButton->OnPressed.AddDynamic(this, &UGsUIOptionItemSlot::OnClickedTooltip);
	_onOffTooltipButton->OnPressed.AddDynamic(this, &UGsUIOptionItemSlot::OnClickedTooltip);
}

void UGsUIOptionItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIOptionItemSlot::NativeDestruct()
{
	if (_tooltipDesc->OnInvalidatePosition.IsBound())
	{
		_tooltipDesc->OnInvalidatePosition.Unbind();
	}

	Super::NativeDestruct();
}

void UGsUIOptionItemSlot::BeginDestroy()
{
	if (nullptr != _onOffToggle)
	{
		_onOffToggle = nullptr;
	}

	if (nullptr != _slider)
	{
		_slider->OnValueChanged.RemoveDynamic(this, &UGsUIOptionItemSlot::OnSlider);
		_slider = nullptr;

		if (nullptr != _sliderToggle1)
		{
			_sliderToggle1->OnClickSlot.Unbind();
			_sliderToggle1 = nullptr;
		}

		if (nullptr != _sliderToggle2)
		{
			_sliderToggle2->OnClickSlot.Unbind();
			_sliderToggle2 = nullptr;
		}
	}

	Super::BeginDestroy(); 
}

void UGsUIOptionItemSlot::OnClickButton(EGsOptionItem InType)
{
	GOption()->SetValue(InType, 0.0f);
}

void UGsUIOptionItemSlot::OnClickToggle(EGsOptionItem InType, bool InIsSelect)
{
	GOption()->SetValue(InType, (true == InIsSelect)? 1 : 0);
}

void UGsUIOptionItemSlot::OnClickToggleGroup(EGsOptionItem InType, bool InIsSelected)
{
	GOption()->SetValue(InType, (true == InIsSelected) ? 1 : 0);
}

void UGsUIOptionItemSlot::OnClickRadioButtonGroup(EGsOptionItem InType, int32 InIndex)
{
	GOption()->SetValue(InType, InIndex);
}

void UGsUIOptionItemSlot::OnSlider(float InValue)
{
	float value = 0;
	RatioToValue(InValue, value);

	GOption()->SetValue(_itemType, value);
	SetSliderText(_sliderTextType, value);
}

void UGsUIOptionItemSlot::Update(const EGsOptionSelectType & InOptionSelectType, EGsOptionItem InType,
	const TArray<EGsOptionPlatformType>& InOptionDisableList)
{
	UGsOptionManager* optManager = GOption();
	if (nullptr == optManager)
		return;

	_itemType = InType;

	auto Func = [&](const TArray<EGsOptionPlatformType>& optionDisableList)
	{
		bool isEnabled = true;
		for (auto iter : optionDisableList)
		{
			if (iter == EGsOptionPlatformType::OPTION_WINDOW)
			{
#if WITH_EDITOR || PLATFORM_WINDOWS
				isEnabled = false;
				continue;
#endif
			}
			else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
			{
#if PLATFORM_ANDROID
				isEnabled = false;
				continue;
#endif
			}
			else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
			{
#if PLATFORM_IOS
				isEnabled = false;
				continue;
#endif
			}
		}

		return isEnabled;
	};

	bool isEnabled = Func(InOptionDisableList);

	float val = 0.0f;
	bool isDynamicEnable = true;
	optManager->GetValue(_itemType, val, isDynamicEnable);

	bool isCustomValue = false;
	FText outCustomTitleValue;
	isCustomValue = optManager->GetCustomTitleValue(InType, outCustomTitleValue);

	if (_tooltipButton)
		_tooltipButton->SetVisibility(ESlateVisibility::Collapsed);

	if (_onOffTooltipButton)
		_onOffTooltipButton->SetVisibility(ESlateVisibility::Collapsed);

	if (_tooltipDesc)
		_tooltipDesc->Close();

	if (isEnabled)
	{
		isEnabled = isDynamicEnable;
	}

	FString dataKey = GetEGsOptionTypeAsString(_itemType);

	switch (InOptionSelectType)
	{
		case EGsOptionSelectType::Button:
		{
			const FGsSchemaOptionButton*  schemaButton = optManager->GetGsOptionButtonData(dataKey);
			if (nullptr != schemaButton)
			{				
				_button->SetData(_itemType, schemaButton->buttonText);	
				_button->SetIsEnabled((isEnabled) ? true : false);	

				_textNameTag = (isCustomValue) ? outCustomTitleValue : schemaButton->nameText;
				_textButton->SetText(schemaButton->buttonText);
			}
		}
		break;
		case  EGsOptionSelectType::ButtonGroup:
		{
			const FGsSchemaOptionButtonGroup* buttonGroup = optManager->GetGsOptionButtonGroupData(dataKey);
			if (nullptr != buttonGroup)
			{
				_buttonGroup = buttonGroup;
				int8 buttonCount = buttonGroup->buttonList.Num();
				
				_optionRadioButtonGroup->SetData(_itemType, buttonGroup, (int32)val, isEnabled);
						
				FLinearColor color;
				switch (buttonGroup->nameTextColorType)
				{
				case EGsOptionTextColorType::NORMAL:
					color = _colorNormal;
					break;
				case EGsOptionTextColorType::MAIN:
					color = _colorMain;
					break;
				case EGsOptionTextColorType::SUB:
					color = _colorSub;
					break;
				case EGsOptionTextColorType::RED:
					color = _colorRed;
					break;
				default:
					break;
				}

				_mainText->SetText(buttonGroup->nameText);
				_mainText->SetColorAndOpacity(color);
		
				bool isEmpty = buttonGroup->subText.IsEmpty();
				if (false == isEmpty)
					_subText->SetText(buttonGroup->subText);
				_subTextScaleBox->SetVisibility(isEmpty ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
			}
		}
		break;
	case  EGsOptionSelectType::Slider:
		{
			const FGsSchemaOptionSlider* slider = optManager->GetGsOptionSliderData(dataKey);
			if (nullptr != slider)
			{
				_textNameTag = (isCustomValue) ? outCustomTitleValue : slider->nameText;
				
				_sliderToggle1->SetVisibility(ESlateVisibility::Collapsed);
				_sliderToggle2->SetVisibility(ESlateVisibility::Collapsed);

				int toggleNum = slider->toggleList.Num();
				if (0 < toggleNum)
				{
					for (int i = 0; i <toggleNum; ++i)
					{
						FGsSchemaOptionToggle schemaOptionToggle = slider->toggleList[i];
						if (schemaOptionToggle.optionItemType != EGsOptionItem::None)
						{
							float outValue;
							bool isVisible = true;
							optManager->GetValue(schemaOptionToggle.optionItemType, outValue, isVisible);

							if (true == isVisible)
								isVisible = Func(schemaOptionToggle.OptionExceptionPlatformTypeList);

							if (0 < i)
							{
								_sliderToggle2->SetIsEnabled(isEnabled ? true : false);
								_sliderToggle2->SetVisibility(isVisible? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
								_sliderToggle2->SetData(schemaOptionToggle.optionItemType, (0 == outValue) ? false : true);
								_sliderToggle2->SetButtonText(schemaOptionToggle.onText, schemaOptionToggle.offText);
							}
							else
							{
								_sliderToggle1->SetIsEnabled(isEnabled ? true : false);
								_sliderToggle1->SetVisibility(isVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
								_sliderToggle1->SetData(schemaOptionToggle.optionItemType, (0 == outValue) ? false : true);
								_sliderToggle1->SetButtonText(schemaOptionToggle.onText, schemaOptionToggle.offText);
							}
						}
					}
				}

				_sliderTextType = slider->textType;
				_minValue = slider->MinValue;
				_maxValue = slider->MaxValue;

				val = (val > _maxValue) ? _maxValue : val;
				val = (val < _minValue) ? _minValue : val;

				float sliderRatio = val;
				ValueToRatio(val, sliderRatio);

				if (_sliderTextType == EGsSliderTextType::NONE)
				{
					_textBlockPercent->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					_textBlockPercent->SetVisibility(ESlateVisibility::Visible);

					SetSliderText(_sliderTextType, val);
				}

				_slider->SetValue(sliderRatio);
				_slider->SetIsEnabled(isEnabled ? true : false);				
			}
		}
		break;
	case  EGsOptionSelectType::OnOff:
		{
			InitializeToolTipDescPost();
			const FGsSchemaOptionOnOff* onOff = optManager->GetGsOptionOnOffData(dataKey);
			if (nullptr != onOff)
			{
				_textNameTag = (isCustomValue) ? outCustomTitleValue : onOff->nameText;

				_onOffToggle->SetData(_itemType, (0 == val)? false : true, isEnabled);
				_onOffToggle->SetButtonText(onOff->buttonOnText, onOff->buttonOffText);

				if (onOff->isTooltip)
				{
					_tooltipCommonButton = _onOffTooltipButton;
					if (_tooltipCommonButton)
					{
						_tooltipCommonButton->SetVisibility(ESlateVisibility::Visible);
						_tooltipDesc->SetDescText(onOff->tooltipText);
					}
				}
			}
		}
		break;
	case  EGsOptionSelectType::Text:
		{
			const FGsSchemaOptionText* schemaOptionText = optManager->GetGsOptionTextData(dataKey);
			if (nullptr != schemaOptionText)
			{
				FLinearColor color;
				switch (schemaOptionText->mainTextColorType)
				{
				case EGsOptionTextColorType::NORMAL:
					color = _colorNormal;
					break;
				case EGsOptionTextColorType::MAIN:
					color = _colorMain;
					break;
				case EGsOptionTextColorType::SUB:
					color = _colorSub;
					break;
				case EGsOptionTextColorType::RED:
					color = _colorRed;
					break;
				default:
					break;
				}

				_textMainText->SetText(schemaOptionText->mainTextContents);
				_textMainText->SetColorAndOpacity(color);

				bool isEmpty = schemaOptionText->subTextContents.IsEmpty();
				if (false == isEmpty)
					_textSubText->SetText(schemaOptionText->subTextContents);
				_textSubTextScaleBox->SetVisibility(isEmpty ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

				_tooltipButton->SetVisibility(schemaOptionText->isTooltip ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
				if (schemaOptionText->isTooltip)
				{
					_tooltipDesc->SetDescText(schemaOptionText->tooltipDesc);
				}
			}
		}
		break;
	case EGsOptionSelectType::OnOffGroup:
		{
			InitializeToolTipDescPost();
			const FGsSchemaOptionToggleGroup* onOffGroup = optManager->GetGsOptionOnOffGroupData(dataKey);
			if (nullptr != onOffGroup)
			{
				_textNameTag = (isCustomValue) ? outCustomTitleValue : onOffGroup->nameText;
				_optionToggleGroup->SetData(onOffGroup, isEnabled);		

				if (onOffGroup->isTooltip)
				{
					_tooltipCommonButton = _tooltipButton;
					if (_tooltipCommonButton)
					{
						_tooltipCommonButton->SetVisibility(ESlateVisibility::Visible);
						_tooltipDesc->SetDescText(onOffGroup->tooltipText);
					}
				}
			}
		}
		break;
	case  EGsOptionSelectType::CheckGroup:
		{
			InitializeToolTipDescPost();
			const FGsSchemaOptionCheckGroup* checkGroup = optManager->GetGsOptionCheckGroupData(dataKey);
			if (nullptr != checkGroup)
			{
				_textNameTag = (isCustomValue) ? outCustomTitleValue : checkGroup->nameText;
				_optionCheckButtonGroup->SetData(_itemType, checkGroup);
				_optionCheckButtonGroup->OnClickCheckBox.BindUObject(this, &UGsUIOptionItemSlot::OnClickToggle);

				if (checkGroup->isTooltip)
				{
					_tooltipCommonButton = _tooltipButton;
					if (_tooltipCommonButton)
					{
						_tooltipCommonButton->SetVisibility(ESlateVisibility::Visible);
						_tooltipDesc->SetDescText(checkGroup->tooltipText);
					}
				}
			}
		}
		break;
	default:
		break;
	}

	_swicherItemType->SetActiveWidgetIndex(static_cast<int>(InOptionSelectType));
}

FString UGsUIOptionItemSlot::GetEGsOptionTypeAsString(EGsOptionItem EnumValue)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGsOptionItem"), true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}

	return enumPtr->GetNameStringByIndex((int32)EnumValue);
}

void UGsUIOptionItemSlot::SetSliderText(EGsSliderTextType In_SliderTextType, float In_val)
{
	switch (In_SliderTextType)
	{
	case EGsSliderTextType::NONE:
		break;
	case EGsSliderTextType::PERCENT:
	{
		int32 percent = FMath::RoundToInt(In_val * 100);
		FString strPercent = FString::Format(TEXT("{0}%"), { percent });
		_textPercent = FText::FromString(strPercent);
	}
	break;
	case EGsSliderTextType::COUNT:
	{
		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("Count"), findText);

		_textPercent = FText::Format(findText, In_val);
	}
	break;
	case EGsSliderTextType::COUNT_LIMIT:
	{
		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("CountLimit"), findText);		
		_textPercent = FText::Format(findText, FMath::FloorToInt(In_val));
	}
	break;
	case EGsSliderTextType::COUNT_RANGE:
		break;
	case EGsSliderTextType::COUNT_RANGE_LITMIT:
		break;
	case EGsSliderTextType::TIME:
	{
		int sec = In_val, min = 0, hour = 0;
		min = sec / 60; // 입력받은 sec를 60으로 나누면 분(min)
		hour = min / 60; // min의 값을 60으로 나누면 시(hour)
		sec = sec % 60; // 시분초로 바꿔주는 것이므로, sec를 60으로 나눠 그 나머지가 남은 초
		min = min % 60; // 12줄과 마찬가지로, min을 60으로 나눠 그 나머지가 남은 분

		FString strSec, strMin, strHour;
		if (0 < hour)
			strHour = FGsTimeStringHelper::GetHourString(hour);
		if (0 < min)
			strMin = FGsTimeStringHelper::GetMinuteString(min);
		if (0 < sec)
			strSec = FGsTimeStringHelper::GetSecondString(sec);

		_textPercent = FText::FromString(FString::Printf(TEXT("%s%s%s"), *strHour, *strMin, *strSec));
	}
		break;
	case EGsSliderTextType::DISTANCE_M:
	{	// 2022/10/05 PKT - 단위가 미터인 거리 표현식
		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("Distance_M"), findText);

		_textPercent = FText::Format(findText, { StaticCast<int32>(In_val) });
	}
		break;
	default:
		break;
	}
}

bool UGsUIOptionItemSlot::IsConvertSliderValue()
{
	return true;
}

void UGsUIOptionItemSlot::RatioToValue(float In_Ratio, OUT float& Out_Value)
{
	float lerpedValue = FMath::Lerp<float>(_minValue, _maxValue, In_Ratio);	
	Out_Value = RoundRatio(lerpedValue);
}

void UGsUIOptionItemSlot::ValueToRatio(float In_Value, OUT float& Out_Ratio)
{
	if (_minValue > _maxValue)
	{
		Out_Ratio = 1.0f - ((In_Value - _maxValue) / (_minValue - _maxValue));
	}
	else
	{
		Out_Ratio = (In_Value - _minValue) / (_maxValue - _minValue);
	}

	Out_Ratio = RoundRatio(Out_Ratio);
}

void UGsUIOptionItemSlot::OnClickedTooltip()
{
	if (_tooltipDesc->IsOpen() && _tooltipDesc->GetRenderOpacity() == 1.0f)
	{
		_tooltipDesc->Close();
		return;
	}

	SetToolTipDecPos();
	_tooltipDesc->Open();
	_tooltipDesc->SetRenderOpacity(0.0f);
	_tooltipDesc->SetIsNeedRefresh(true);
}

void UGsUIOptionItemSlot::InitializeToolTipDescPost()
{	
	SetToolTipDecPos();
	_tooltipDesc->Open();
	_tooltipDesc->SetRenderOpacity(0.0f);
	_tooltipDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
	_tooltipDesc->OnInvalidatePosition.BindUObject(this, &UGsUIOptionItemSlot::SetToolTipDecPos);
}

void UGsUIOptionItemSlot::SetToolTipDecPos()
{
	_tooltipDesc->SetRenderOpacity(1.0f);
	UWidget* targetWidget = _tooltipCommonButton;

	FWidgetTransform panelTransform = _tooltipDesc->RenderTransform;
	if (targetWidget)
	{
		FVector2D targetTopLeftPixel;
		FVector2D targetTopLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), targetWidget->GetCachedGeometry(), FVector2D::ZeroVector, targetTopLeftPixel, targetTopLeftViewport);

		FVector2D targetTopRightViewport = targetTopLeftViewport;
		targetTopRightViewport.X += targetWidget->GetCachedGeometry().GetLocalSize().X;

		FVector2D topLeftPixel;
		FVector2D topLeftViewport;
		USlateBlueprintLibrary::LocalToViewport(GetParent(), _tooltipDesc->GetCachedGeometry(), FVector2D::ZeroVector, topLeftPixel, topLeftViewport);

		// 화면 크기
		FVector2D mainViewportSize;
		GEngine->GameViewport->GetViewportSize(mainViewportSize);

		float targetHeight = targetWidget->GetCachedGeometry().GetLocalSize().Y;
		float tooltipHeight = _tooltipDesc->GetCachedGeometry().GetLocalSize().Y + _tooltipDesc->GetTextBlockSize().Y;
		
		FVector2D localHeight(0.f, targetTopRightViewport.Y + targetHeight + tooltipHeight);
		FVector2D absHeight = GetCachedGeometry().GetAccumulatedRenderTransform().TransformVector(localHeight);

		// 툴팁 하단 위치 >  화면 영역 높이
		if (absHeight.Y > mainViewportSize.Y)
		{
			// 타겟 우측면 위로 늘어지게 출력
			panelTransform.Translation += -topLeftViewport + targetTopRightViewport;
			panelTransform.Translation.Y += targetHeight - tooltipHeight;
		}
		else
		{
			// 타겟 우측면 아래로 늘어지게 출력
			panelTransform.Translation += -topLeftViewport + targetTopRightViewport;
		}

		_tooltipDesc->SetRenderTransform(panelTransform);
	}
}

float UGsUIOptionItemSlot::RoundRatio(float InRatio)
{
	int32 value = FMath::RoundToInt(InRatio * 100);
	
	// 주의: 0.01을 곱하면 값이 다시 지저분해지므로 100으로 나누어서 리턴
	return static_cast<float>(value) / 100;
}
