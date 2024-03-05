// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeBody.h"
#include "UI/UIControlLib/Control/GsUIStepSlider.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"


void UGsUICustomizeBody::NativeOnInitialized()
{
	// 슬라이더 바인딩
	_sliderHeight->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderHeight);
	_sliderHead->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderHead);
	_sliderChest->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderChest);
	_sliderWaist->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderWaist);
	_sliderPelvis->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderPelvis);
	_sliderLegs->OnValueChanged.BindUObject(this, &UGsUICustomizeBody::OnSliderLegs);

	Super::NativeOnInitialized();
}

void UGsUICustomizeBody::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	_sliderHeight->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::HEIGHT)));
	_sliderHead->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::HEAD_SIZE)));
	_sliderChest->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::CHEST_SIZE)));
	_sliderWaist->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::WAIST_SIZE)));
	_sliderPelvis->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::PELVIS_SIZE)));
	_sliderLegs->SetValue(ConvertIntToControlValue(_selectedData->GetValue(EGsUICustomizeType::LEG_SIZE)));
}

void UGsUICustomizeBody::OnSliderHeight(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 키
	_selectedData->SetValue(EGsUICustomizeType::HEIGHT, ConvertFloatToDataValue(InValue));


	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::HEIGHT,
		_selectedData->GetValue(EGsUICustomizeType::HEIGHT));

	GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::HEIGHT_CHANGE);
}

void UGsUICustomizeBody::OnSliderHead(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 머리크기
	_selectedData->SetValue(EGsUICustomizeType::HEAD_SIZE, ConvertFloatToDataValue(InValue));

	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::HEAD_SIZE,
		_selectedData->GetValue(EGsUICustomizeType::HEAD_SIZE));
}

void UGsUICustomizeBody::OnSliderChest(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 가슴 크기
	_selectedData->SetValue(EGsUICustomizeType::CHEST_SIZE, ConvertFloatToDataValue(InValue));

	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::CHEST_SIZE,
		_selectedData->GetValue(EGsUICustomizeType::CHEST_SIZE));
}

void UGsUICustomizeBody::OnSliderWaist(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 허리 크기
	_selectedData->SetValue(EGsUICustomizeType::WAIST_SIZE, ConvertFloatToDataValue(InValue));
	int32 waistSize = _selectedData->GetValue(EGsUICustomizeType::WAIST_SIZE);

	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::WAIST_SIZE, waistSize);
	//_customizeHandler->SetBodyChange(FGsCustomizeHandler::EGsTargetNames::WaistL, waistSize);
}

void UGsUICustomizeBody::OnSliderPelvis(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 골반 크기
	_selectedData->SetValue(EGsUICustomizeType::PELVIS_SIZE, ConvertFloatToDataValue(InValue));

	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::PELVIS_SIZE,
		_selectedData->GetValue(EGsUICustomizeType::PELVIS_SIZE));
}

void UGsUICustomizeBody::OnSliderLegs(float InValue)
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 다리 두께
	_selectedData->SetValue(EGsUICustomizeType::LEG_SIZE, ConvertFloatToDataValue(InValue));

	int32 legSize = _selectedData->GetValue(EGsUICustomizeType::LEG_SIZE);

	_customizeHandler->UpdateCustomizeUIData(_selectedData);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::LEG_SIZE, legSize);
	//_customizeHandler->SetBodyChange(FGsCustomizeHandler::EGsTargetNames::LegsL, legSize);
}
