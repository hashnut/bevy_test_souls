#include "GsCustomizeUIData.h"
#include "DataSchema/Customize/GsSchemaCustomizePreset.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "DataSchema/Customize/GsSchemaCustomizePartsGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizeTattooGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizeSlotData.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorGroup.h"
#include "T1Project.h"


void FGsCustomizeUIData::SetPresetData(int32 InPresetIndex)
{
	if (nullptr == _menu)
	{
		return;
	}

	if (false == _menu->presetList.IsValidIndex(InPresetIndex))
	{
		GSLOG(Error, TEXT("[Customizing] Invalid presetIndex. index: %d, presetCount: %d."), 
			InPresetIndex, _menu->presetList.Num());
		return;
	}

	const FGsSchemaCustomizePreset* row = _menu->presetList[InPresetIndex].GetRow();
	if (nullptr == row)
	{
		GSLOG(Error, TEXT("[Customizing] Fail to find presetIndex: %d."), InPresetIndex);
		return;
	}

	// 값 배정
	SetValue(EGsUICustomizeType::PRESET_INDEX, InPresetIndex);

	SetValue(EGsUICustomizeType::FACE_INDEX, row->faceIndex);
	SetValue(EGsUICustomizeType::HAIR_INDEX, row->hairIndex);
	SetValue(EGsUICustomizeType::TATTOO_INDEX, row->tattooIndex);
	SetValue(EGsUICustomizeType::HAIR_COLOR_INDEX, row->hairColorIndex);
	SetValue(EGsUICustomizeType::SKIN_COLOR_INDEX, row->skinColorIndex);
	SetValue(EGsUICustomizeType::PUPIL_COLOR_INDEX, row->pupilColorIndex);
	SetValue(EGsUICustomizeType::BROW_COLOR_INDEX, row->browColorIndex);

	SetValue(EGsUICustomizeType::HEIGHT, row->height);
	SetValue(EGsUICustomizeType::HEAD_SIZE, row->headSize);
	SetValue(EGsUICustomizeType::CHEST_SIZE, row->chestSize);
	SetValue(EGsUICustomizeType::WAIST_SIZE, row->waistSize);
	SetValue(EGsUICustomizeType::PELVIS_SIZE, row->pelvisSize);
	SetValue(EGsUICustomizeType::LEG_SIZE, row->legSize);

	SetMaxValueData(_menu);

	_bIsChanged = false;
}

void FGsCustomizeUIData::SetRandom()
{
	// 프리셋 빼고 나머지 값을 섞어서 Set함. INVALID_VALUE 항목은 무시처리
	for (int32 i = 1; i <= static_cast<int32>(EGsUICustomizeType::MAX_CUSTOMIZE_TYPE); ++i)
	{
		if (INVALID_VALUE != _maxValueList[i])
		{
			int32 value = 0;
			if (0 < _maxValueList[i])
			{
				value = FMath::RandRange(0, _maxValueList[i] - 1);
			}

			SetValue(static_cast<EGsUICustomizeType>(i), value);
		}
	}
}

void FGsCustomizeUIData::SetMaxValueData(const FGsSchemaCustomizeMenu* InMenu)
{
	// 랜덤을 위한 max값 배정
	// 기획서에 체형, 눈썹색, 데칼(없음)은 기본 세팅으로 고정 이라고 되어있음.
	// 사용하지 않을 항목은 INVALID_VALUE 로 넣음

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::PRESET_INDEX)] = INVALID_VALUE;

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::FACE_INDEX)] = 0;
	if (const FGsSchemaCustomizePartsGroup* faceGroup = _menu->facePartsGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::FACE_INDEX)] = faceGroup->slotList.Num();
	}

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::HAIR_INDEX)] = 0;
	if (const FGsSchemaCustomizePartsGroup* hairGroup = _menu->hairPartsGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::HAIR_INDEX)] = hairGroup->slotList.Num();
	}

	// 랜덤 시 문신은 없음(0)값 고정
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::TATTOO_INDEX)] = 0;
	//if (const FGsSchemaCustomizeTattooGroup* tattooGroup = _menu->tattooGroup.GetRow())
	//{
	//	_maxValueList[EGsUICustomizeType::TATTOO_INDEX] = tattooGroup->slotList.Num();
	//}

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::HAIR_COLOR_INDEX)] = 0;
	if (const FGsSchemaCustomizeColorGroup* hairColorGroup = _menu->hairColorGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::HAIR_COLOR_INDEX)] = hairColorGroup->colorIndexList.Num();
	}

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::SKIN_COLOR_INDEX)] = 0;
	if (const FGsSchemaCustomizeColorGroup* skinColorGroup = _menu->skinColorGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::SKIN_COLOR_INDEX)] = skinColorGroup->colorIndexList.Num();
	}

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::PUPIL_COLOR_INDEX)] = 0;
	if (const FGsSchemaCustomizeColorGroup* pupilColorGroup = _menu->pupilColorGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::PUPIL_COLOR_INDEX)] = pupilColorGroup->colorIndexList.Num();
	}

	_maxValueList[static_cast<uint8>(EGsUICustomizeType::BROW_COLOR_INDEX)] = 0;
	if (const FGsSchemaCustomizeColorGroup* browColorGroup = _menu->browColorGroup.GetRow())
	{
		_maxValueList[static_cast<uint8>(EGsUICustomizeType::BROW_COLOR_INDEX)] = browColorGroup->colorIndexList.Num();
	}

	// 0~100사이값으로 사용하려면 100 넣을 것
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::HEIGHT)] = INVALID_VALUE; 
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::HEAD_SIZE)] = INVALID_VALUE;
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::CHEST_SIZE)] = INVALID_VALUE;
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::WAIST_SIZE)] = INVALID_VALUE;
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::PELVIS_SIZE)] = INVALID_VALUE;
	_maxValueList[static_cast<uint8>(EGsUICustomizeType::LEG_SIZE)] = INVALID_VALUE;
}

int32 FGsCustomizeUIData::GetBodyId() const
{
	if (_menu)
	{
		int32 presetIndex = GetValue(EGsUICustomizeType::PRESET_INDEX);
		if (_menu->presetList.IsValidIndex(presetIndex))
		{
			if (const FGsSchemaCustomizePreset* preset = _menu->presetList[presetIndex].GetRow())
			{
				return preset->partsId;
			}
		}
	}

	return INVALID_ID;
}

int32 FGsCustomizeUIData::GetFaceId() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizePartsGroup* faceGroup = _menu->facePartsGroup.GetRow())
		{
			int32 faceIndex = GetValue(EGsUICustomizeType::FACE_INDEX);
			if (faceGroup->slotList.IsValidIndex(faceIndex))
			{
				return faceGroup->slotList[faceIndex].targetId;
			}
		}
	}

	return INVALID_ID;
}

int32 FGsCustomizeUIData::GetHairId() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizePartsGroup* hairGroup = _menu->hairPartsGroup.GetRow())
		{
			int32 hairIndex = GetValue(EGsUICustomizeType::HAIR_INDEX);
			if (hairGroup->slotList.IsValidIndex(hairIndex))
			{
				return hairGroup->slotList[hairIndex].targetId;
			}
		}
	}

	return INVALID_ID;
}

int32 FGsCustomizeUIData::GetTattooId() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizeTattooGroup* tattooGroup = _menu->tattooGroup.GetRow())
		{
			int32 tattooIndex = GetValue(EGsUICustomizeType::TATTOO_INDEX);
			if (tattooGroup->slotList.IsValidIndex(tattooIndex))
			{
				return tattooGroup->slotList[tattooIndex].targetId;
			}
		}
	}

	return INVALID_ID;
}

int32 FGsCustomizeUIData::GetHairColorIndex() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizeColorGroup* group = _menu->hairColorGroup.GetRow())
		{
			int32 index = GetValue(EGsUICustomizeType::HAIR_COLOR_INDEX);
			if (group->colorIndexList.IsValidIndex(index))
			{
				return group->colorIndexList[index];
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Customize] Fail to get HairColorIndex."));
#endif

	return INVALID_INDEX;
}

int32 FGsCustomizeUIData::GetSkinColorIndex() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizeColorGroup* group = _menu->skinColorGroup.GetRow())
		{
			int32 index = GetValue(EGsUICustomizeType::SKIN_COLOR_INDEX);
			if (group->colorIndexList.IsValidIndex(index))
			{
				return group->colorIndexList[index];
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Customize] Fail to get SkinColorIndex."));
#endif

	return INVALID_INDEX;
}

int32 FGsCustomizeUIData::GetPupilColorIndex() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizeColorGroup* group = _menu->pupilColorGroup.GetRow())
		{
			int32 index = GetValue(EGsUICustomizeType::PUPIL_COLOR_INDEX);
			if (group->colorIndexList.IsValidIndex(index))
			{
				return group->colorIndexList[index];
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Customize] Fail to get PupilColorIndex."));
#endif

	return INVALID_INDEX;
}

int32 FGsCustomizeUIData::GetBrowColorIndex() const
{
	if (_menu)
	{
		if (const FGsSchemaCustomizeColorGroup* group = _menu->browColorGroup.GetRow())
		{
			int32 index = GetValue(EGsUICustomizeType::BROW_COLOR_INDEX);
			if (group->colorIndexList.IsValidIndex(index))
			{
				return group->colorIndexList[index];
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Customize] Fail to get BrowColorIndex."));
#endif

	return INVALID_INDEX;
}

void FGsCustomizeUIData::SetValue(EGsUICustomizeType InType, int32 InValue)
{
	uint8 index = static_cast<uint8>(InType);
	if (InValue != _dataList[index])
	{
		_bIsChanged = true;
	}

	_dataList[index] = InValue;
}

int32 FGsCustomizeUIData::GetValue(EGsUICustomizeType InType) const
{
	return _dataList[static_cast<uint8>(InType)];
}
