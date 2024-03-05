// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsSchemaEnums.h"
#include "DataSchema/Option/GsSchemaOptionItemSet.h"
#include "Option/GsOptionEnumType.h"
#include "GsUIOptionItemSlot.generated.h"

class UGsUITooltipDesc;
class UScaleBox;
class UWidgetSwitcher;
class UTextBlock;
class UHorizontalBox;
class USlider;
class UGsButton;
class UGsUIOptionCheckButtonGroup;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textNameTag;

	// 아이템 타입 ( ID )
	EGsOptionItem _itemType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherItemType;

	////////////////////////////////////////////////////////////////////////////
	//	버튼그룹
	////////////////////////////////////////////////////////////////////////////
	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _slotEntryClass;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _mainText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _subText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScaleBox* _subTextScaleBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionRadioButtonGroup* _optionRadioButtonGroup;

	////////////////////////////////////////////////////////////////////////////
	//	버튼
	////////////////////////////////////////////////////////////////////////////	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionButton* _button;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _HorizontalBoxButtonList;

	////////////////////////////////////////////////////////////////////////////
	//	토글 on/off
	////////////////////////////////////////////////////////////////////////////
	// 아이템리스트는 아래로 차례차례 추가 (토글)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionToggle* _onOffToggle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _onOffTooltipButton;

	////////////////////////////////////////////////////////////////////////////
	//	슬라이더 on/off
	////////////////////////////////////////////////////////////////////////////
	// 슬라이더
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USlider* _slider; 

	// 토글1
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionToggle* _sliderToggle1;

	// 토글2
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionToggle* _sliderToggle2;

	// 아이템 버튼그룹 정보
	const struct FGsSchemaOptionButtonGroup* _buttonGroup = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _sliderButtonHorizontalBox;

	//////////////////////////////////////////////////////////////////////////////
	// 퍼센트 on/off
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textPercent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockPercent;
	// 퍼센트 텍스트 세팅 함수
	TFunction<void(float In_val)> _percentTextFunc;
	EGsSliderTextType _sliderTextType = EGsSliderTextType::NONE;

	float _minValue = 0;
	float _maxValue = 1;
	////////////////////////////////////////////////////////////////////////////
	//	Text
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textMainText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSubText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScaleBox* _textSubTextScaleBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _tooltipButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UGsButton* _tooltipCommonButton;

	////////////////////////////////////////////////////////////////////////////
	//	ToggleGroup
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionToggleGroup* _optionToggleGroup;

	////////////////////////////////////////////////////////////////////////////
	//	CheckButtonGroup
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionCheckButtonGroup* _optionCheckButtonGroup;

	// 컬러
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIOption")
	FLinearColor _colorNormal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIOption")
	FLinearColor _colorMain;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIOption")
	FLinearColor _colorSub;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIOption")
	FLinearColor _colorRed;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;	
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 버튼이 클릭 됐을 때
	UFUNCTION()
	void OnClickButton(EGsOptionItem InType);

	// 토글이 클릭 됐을 때
	UFUNCTION()
	void OnClickToggle(EGsOptionItem InType, bool InIsSelect);

	UFUNCTION()
	void OnClickToggleGroup(EGsOptionItem InType, bool InIsSelected);
	
	UFUNCTION()
	void OnClickRadioButtonGroup(EGsOptionItem InType, int32 InIndex);

	UFUNCTION()
	void OnClickedTooltip();
	
	// 슬라이더 이동이 있을 때
	UFUNCTION()
	void OnSlider(float InValue);

public:
	void Update(const EGsOptionSelectType &InOptionSelectType, EGsOptionItem InType, 
		const TArray<EGsOptionPlatformType>& InOptionDisableList);
	/*void UpdateItem(const EGsOptionSelectType& InOptionSelectType, EGsOptionItem InType, 
			EGsOptionPlatformType InOptionDisable = EGsOptionPlatformType::OPTION_ALL);*/

protected:
	// 키값 변환
	UFUNCTION()
	FString GetEGsOptionTypeAsString(EGsOptionItem EnumValue);

	void SetSliderText(EGsSliderTextType In_SliderTextType, float In_val);

	bool IsConvertSliderValue();
	void RatioToValue(float In_Ratio, OUT float& Out_Value);
	void ValueToRatio(float In_Value, OUT float& Out_Ratio);

	void InitializeToolTipDescPost();
	void SetToolTipDecPos();

	// 소수 3자리에서 2자리로 반올림해줌
	float RoundRatio(float InRatio);
};
