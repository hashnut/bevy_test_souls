// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Option/GsGameUserSettings.h"

/*
	옵션 - 그래픽 열거형
*/
UENUM()
enum class EGsDevelopOptionGraphic : uint8
{	
	EFFECT,				// 비주얼효과 품질 (0~3)
	TEXTURE,			// 텍스쳐 품질 (0~3)
	FOLIAGE,			// 수풀밀도 품질 (0~3)
	
	OUTLINE,			// 특수효과 - 캐릭터 아웃라인 (0~2)
	DISABLE_DISTORTION,	// 특수효과 - 굴절 ON/OFF

	TERRAIN_NORMAL_DISTANCE, // 지형 디테일 거리 LOD
	
	MAX
};
/**
 *  개발 옵션 적용 테스트
 */
class T1PROJECT_API FGsDevelopmentOption
{
private:
	static const int8 MAX_OPT_OUTLINE_LEVELS = 2;
	static const int32	MAX_OPT_LODDIST = 10;

	class UGsGameUserSettings* _userSettings;
	// userInfo 옵션
	TMap<EGsDevelopOptionGraphic, int32>	_graphicOptions;

public:
	void Initialize(UGsGameUserSettings* In_userSettings);
	void Finalize();

	void SetVisualEffectQuality(int32 Value);
	void SetTextureQuality(int32 Value);
	void SetFoliageQuality(int32 Value);
	void SetDisableDistortion(bool Value);
	void SetOutlineLevel(int32 Value);
	void SetOutlineDistCVar(int32 Value);
	void SetLODDistanceScale(int32 Value);

	void SetGraphicOpt(EGsDevelopOptionGraphic In_optionType, int32 In_val);

	const TMap<EGsDevelopOptionGraphic, int32>& GetGraphicOptions() { return _graphicOptions; }
};
