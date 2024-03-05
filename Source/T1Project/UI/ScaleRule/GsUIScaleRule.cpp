// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIScaleRule.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Significance/GsGameViewPortClient.h"
#include "Engine/Engine.h"


static float GUIScaleValue = 1.f;

float UGsUIScaleRule::GetDPIScaleBasedOnSize(FIntPoint Size) const
{
	// ScaleToFit 룰에 옵션 비율 곱해서 적용
	float tempValue = (DesignScreenSize.X > 0 && DesignScreenSize.Y > 0) ? 
		FMath::Min((float)(Size.X) / DesignScreenSize.X, (float)(Size.Y) / DesignScreenSize.Y) : 1.f;

	return tempValue * GUIScaleValue;

	/*
	// 1이면 기본 스케일 적용
	int32 EvalPoint = 0;
	if (1.f <= GUIScaleValue)
	{
		EvalPoint = FMath::Min(Size.X, Size.Y);

		const FRichCurve* DPICurve = GetDefault<UUserInterfaceSettings>()->UIScaleCurve.GetRichCurveConst();
		return DPICurve->Eval((float)EvalPoint, 1.0f);
	}
	
	return GUIScaleValue;
	*/
}

float UGsUIScaleRule::GetUIScaleValue()
{
	return GUIScaleValue;
}

void UGsUIScaleRule::SetUIScaleValue(float InValue)
{
	float prevValue = GUIScaleValue;
	GUIScaleValue = FMath::Clamp(InValue, 0.6f, 1.f);

	if (prevValue != GUIScaleValue)
	{
		// 강제 갱신 시킨다
		const auto UserInterfaceSettings = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
		if (UserInterfaceSettings)
		{
			UserInterfaceSettings->GetDPIScaleBasedOnSize(FIntPoint::ZeroValue);
		}
	}

	// 드래그 민감도 갱신
	if (UGsGameViewPortClient* viewportClient = Cast<UGsGameViewPortClient>(GEngine->GameViewport))
	{
		viewportClient->UpdateDragTriggerDistance();
	}
}
