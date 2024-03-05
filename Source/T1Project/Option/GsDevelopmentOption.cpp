// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDevelopmentOption.h"
#include "Option/GsGameUserSettings.h"
#include "IConsoleManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

void FGsDevelopmentOption::Initialize(UGsGameUserSettings* In_userSettings)
{
	_userSettings = In_userSettings;

	_graphicOptions.Empty();
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::EFFECT, 3);
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::TEXTURE, 3);
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::FOLIAGE, 3);
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::OUTLINE, 1);
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::DISABLE_DISTORTION, 1);	
	_graphicOptions.Emplace(EGsDevelopOptionGraphic::TERRAIN_NORMAL_DISTANCE, 1);

	SetOutlineLevel(true);
	SetDisableDistortion(true);
	SetLODDistanceScale(1);
}

void FGsDevelopmentOption::Finalize()
{
	_graphicOptions.Empty();
}

void FGsDevelopmentOption::SetGraphicOpt(EGsDevelopOptionGraphic In_optionType, int32 In_val)
{
	_graphicOptions[In_optionType] = In_val;

	if (false == GOption()->GetIsDevelopmentMode())
	{
		return;
	}

	switch (In_optionType)
	{
	case EGsDevelopOptionGraphic::EFFECT:
		SetVisualEffectQuality(In_val);
		break;
	case EGsDevelopOptionGraphic::TEXTURE:
		SetTextureQuality(In_val);
		break;
	case EGsDevelopOptionGraphic::FOLIAGE:
		SetFoliageQuality(In_val);
		break;
	case EGsDevelopOptionGraphic::OUTLINE:
		SetOutlineLevel(In_val);
		break;
	case EGsDevelopOptionGraphic::DISABLE_DISTORTION:
		SetDisableDistortion(static_cast<bool>(In_val));
		break;
	case EGsDevelopOptionGraphic::TERRAIN_NORMAL_DISTANCE:
		SetLODDistanceScale(In_val);
		break;
	case EGsDevelopOptionGraphic::MAX:
		break;
	default:
		break;
	}
}

void FGsDevelopmentOption::SetVisualEffectQuality(int32 Value)
{
	if (_userSettings)
	{
		_userSettings->SetVisualEffectQuality(Value);
	}
}

void FGsDevelopmentOption::SetTextureQuality(int32 Value)
{
	if (_userSettings)
	{
		_userSettings->SetTextureQuality(Value);
	}
}

void FGsDevelopmentOption::SetFoliageQuality(int32 Value)
{
	if (_userSettings)
	{
		_userSettings->SetFoliageQuality(Value);
	}
}

void FGsDevelopmentOption::SetDisableDistortion(bool Value)
{
	IConsoleVariable* disableDistortionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DisableDistortion"));
	if (ensure(disableDistortionCVar))
	{
		disableDistortionCVar->Set(Value, ECVF_SetByGameSetting);
	}
}

void FGsDevelopmentOption::SetOutlineLevel(int32 Value)
{
#if BSAM_TEMP
#else
	IConsoleVariable* outlineCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("z.CartoonOutline"));
	if (ensure(outlineCVar))
	{
		outlineCVar->Set((Value == 0) ? 0 : MAX_OPT_OUTLINE_LEVELS, ECVF_SetByGameSetting);
	}
#endif
}

void FGsDevelopmentOption::SetOutlineDistCVar(int32 Value)
{
	IConsoleVariable* outlineDistCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("z.CartoonOutlineDist"));
	if (ensure(outlineDistCVar))
	{
		outlineDistCVar->Set(Value, ECVF_SetByGameSetting);
	}
}

void FGsDevelopmentOption::SetLODDistanceScale(int32 Value)
{
	IConsoleVariable* terrainNormalDistanceCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.StaticMeshLODDistanceScale"));
	if (ensure(terrainNormalDistanceCVar))	{
		
		terrainNormalDistanceCVar->Set(Value, ECVF_SetByGameSetting);
	}
}
