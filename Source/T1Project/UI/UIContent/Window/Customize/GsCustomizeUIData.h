// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGsSchemaCustomizeMenu;

enum class EGsUICustomizeType : uint8
{
	PRESET_INDEX = 0,
	FACE_INDEX,
	HAIR_INDEX,
	TATTOO_INDEX,
	HAIR_COLOR_INDEX,
	SKIN_COLOR_INDEX,
	PUPIL_COLOR_INDEX,
	BROW_COLOR_INDEX,
	HEIGHT,
	HEAD_SIZE,
	CHEST_SIZE,
	WAIST_SIZE,
	PELVIS_SIZE,
	LEG_SIZE,

	MAX_CUSTOMIZE_TYPE
};

/**
 * 커스터마이징 UI에서 유저가 커스터마이징 한 정보.
 * 서버에 보낼 정보를 구성할 때 이 정보를 이용.
 */
struct FGsCustomizeUIData
{
public:
	int32 _dataList[static_cast<uint8>(EGsUICustomizeType::MAX_CUSTOMIZE_TYPE)];

	// 최대값~ 랜덤할때 사용
	int32 _maxValueList[static_cast<uint8>(EGsUICustomizeType::MAX_CUSTOMIZE_TYPE)];

protected:
	bool _bIsChanged = false;

public:
	const int32 INVALID_ID = -1;
	const int32 INVALID_INDEX = -1;
	const int32 INVALID_VALUE = -1;
	const struct FGsSchemaCustomizeMenu* _menu;

public:
	void SetPresetData(int32 InPresetIndex);	
	void SetRandom();

protected:
	void SetMaxValueData(const FGsSchemaCustomizeMenu* InMenu);

public:
	void SetValue(EGsUICustomizeType InType, int32 InValue);
	int32 GetValue(EGsUICustomizeType InType) const;

	void SetMenu(const FGsSchemaCustomizeMenu* InMenu) { _menu = InMenu; }
	const FGsSchemaCustomizeMenu* GetMenu() const { return _menu; }

	// 주의: Index 종류는 저장된 Id값을 얻어야 한다.
	int32 GetBodyId() const; // Preset의 PartsId를 얻어옴
	int32 GetFaceId() const; 
	int32 GetHairId() const;
	int32 GetTattooId() const;

	// 주의: _dataList의 컬러 Index는 ColorGroup의 인덱스로, Palette의 Index로 변화해서 얻어야 한다.
	int32 GetHairColorIndex() const;
	int32 GetSkinColorIndex() const;
	int32 GetPupilColorIndex() const;
	int32 GetBrowColorIndex() const;

	// 프리셋 변경 시 체크용
	bool IsChanged() const { return _bIsChanged; }
};
