#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Util/GsTableUtil.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Costume/GsSchemaCostumeAppearace.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "T1Project/GameObject/Define/GsGameObjectDefine.h"
#include "Fairy/GsBasePortrait.h"
#include "Algo/Find.h"

using CostumeFeatureId = uint32;
enum class CostumeParts : uint8 { NONE, BODY, HAIR, FULLSET };

class T1PROJECT_API FGsCostumeItem
{
private:
	CostumeId	_costumeId = INVALID_COSTUME_ID;	// 코스튬 템플릿 정보
	ItemAmount	_amount = MIN_ITEM_AMOUNT;			// 코스튭 보유 개수 : 0이면, 보유한 것 1이상은 합성의 재료로 소모	
	PresetValue	_presetValue = INVALID_PRESET_VALUE;	// 프리셋 정보
	CostumeParts _appliedParts{ CostumeParts::NONE };	// 현재 Appearance 일때만 의미 있는 값
	bool		_isLearnSkill = false;

public:
	FGsCostumeItem(const CostumeData& costumeData)
		: _costumeId(costumeData.mCostumeId)
		, _amount(costumeData.mAmount)
		, _presetValue(costumeData.mPresetValue)
		, _isLearnSkill(costumeData.mIsLearnSkill) {}
	virtual ~FGsCostumeItem() = default;

	CostumeId GetCostumeID() { return _costumeId; }
	bool isCollected() { return true; }

	ItemAmount GetAmount() { return _amount; }
	void SetAmount(ItemAmount amount) { _amount = amount; }	

	PresetValue GetPresetValue() { return _presetValue; }
	void SetPresetValue(PresetValue InPresetValue) { _presetValue = InPresetValue; }

	void SetEquipState(CostumeParts type)
	{
		_appliedParts = type;
	};

	void SetIsLearnSkill(bool IsLearnSkill) { _isLearnSkill = IsLearnSkill; }
};

//costume 도감용 데이터 
struct T1PROJECT_API FGsCostumeData : public IGsBasePortrait
{
	CostumeId _id{ 0 };

	const FGsSchemaCostumeCommon*  _costumeData{ nullptr };
	TWeakPtr<FGsCostumeItem> _costumeItem; //invalidate 한 것은 보유중이 아님

	struct FGsCostumeStatData
	{
		StatType type;
		int32 value;
	};
	FGsCostumeStatData _identityStat;
	TArray<FGsCostumeStatData> _mainStat;
	TArray<FGsCostumeStatData> _subStat;

	bool _isNew{ false };
	bool _isSummonNew{ false };
	bool _isEnableCompose{ false };

	FGsCostumeData() = default;
	FGsCostumeData(const FGsSchemaCostumeCommon* data) : _costumeData(data) { 
		if (data)
		{
			_id = data->id;

			const FGsSchemaPassivitySet* passivityData = data->statPassivityId.GetRow();
			if (passivityData && passivityData->type == PassivityType::STAT)
			{
				if (passivityData->effectInfoList.IsValidIndex(0))
				{
					int32 effectId = passivityData->effectInfoList[0].effectId;

					const FGsSchemaPassivityEffectStatSet* statData
						= UGsTableUtil::FindRowById<UGsTablePassivityEffectStatSet, FGsSchemaPassivityEffectStatSet>(effectId);

					for (auto mainStatIndex : { EGsCostumePassivityIndex::MainStat0, EGsCostumePassivityIndex::MainStat1,
												EGsCostumePassivityIndex::MainStat2, EGsCostumePassivityIndex::MainStat3,
												EGsCostumePassivityIndex::MainStat4, EGsCostumePassivityIndex::MainStat5 })
					{
						int index = static_cast<int>(mainStatIndex);
						if (statData->param.IsValidIndex(index))
						{
							_mainStat.Add({ statData->param[index].type, statData->param[index].value });
						}
					}

					_identityStat.type = StatType::NONE;
					_identityStat.value = 0;

					int identityStatIndex = static_cast<int>(EGsCostumePassivityIndex::IdentityStat);
					if (statData->param.IsValidIndex(identityStatIndex))
					{
						_identityStat.type = statData->param[identityStatIndex].type;
						_identityStat.value = statData->param[identityStatIndex].value;
					}

					int subStatIndex = static_cast<int>(EGsCostumePassivityIndex::SubStat);
					int count = statData->param.Num();
					for (; subStatIndex < count; subStatIndex++)
					{
						_subStat.Add({ statData->param[subStatIndex].type, statData->param[subStatIndex].value });
					}
				}
			}
		}
	}

	static TSharedPtr<FGsCostumeData> loadCostume(CostumeId id)
	{
		if (const FGsSchemaCostumeCommon* data = UGsTableUtil::FindRowById<UGsTableCostumeCommon, FGsSchemaCostumeCommon>(id))
		{
			return MakeShared<FGsCostumeData>(data);
		}
		return nullptr;
	}

	static TSharedPtr<FGsCostumeData> loadCostume(const FGsSchemaCostumeCommon* costume)
	{
		if (costume)	return MakeShared<FGsCostumeData>(costume);
		return nullptr;
	}

	bool isCollectedCostume() 	{ return _costumeItem.IsValid(); }
	bool isNewItem() override { return isCollectedCostume() ? _isNew : false; }

	bool IsNewSummon() override { return _isSummonNew; }
	void removeNewSummonMark() { _isSummonNew = false; }

	void removeNewMark()		{ _isNew = false; }
	bool isEnableComposingItem() override { return _isEnableCompose; }
	
	ItemAmount GetAmount()	override{ return _costumeItem.IsValid() ? _costumeItem.Pin()->GetAmount() : 0; }
	const CostumeGrade GetCostumeGrade() const { return _costumeData != nullptr ? static_cast<CostumeGrade>(_costumeData->grade) : CostumeGrade::NONE; }
	const int GetCostumeTier() const { return _costumeData != nullptr ? _costumeData->tier : 0; }
	const FText GetCostumeName() const { return _costumeData != nullptr ? _costumeData->name : FText::GetEmpty(); }
	PresetValue GetPresetValue() const { return _costumeItem.IsValid() ? _costumeItem.Pin()->GetPresetValue() : INVALID_PRESET_VALUE; }
	
	const FGsCostumeStatData& GetIdentityStat() { return _identityStat; };
	const TArray<FGsCostumeStatData>& GetMainStats() { return _mainStat; };
	const TArray<FGsCostumeStatData>& GetSubStats() { return _subStat; };

	const int GetCostumeRecommendedWeaponType(TArray<CreatureWeaponType>& weaponTypeList) { 
		if (_costumeData != nullptr)
		{
			weaponTypeList.Append(_costumeData->weaponType);
			return _costumeData->weaponType.Num();
		}
		return 0;
	}

	bool IsRecommendedWeaponType(CreatureWeaponType selectType) {
		int index = INDEX_NONE;
		return _costumeData != nullptr ? _costumeData->weaponType.Find(selectType, index) : false; }
	
	TArray<const FGsSchemaPassivitySet*> GetCostumePassivityList(CreatureWeaponType weaponType = CreatureWeaponType::ALL) const
	{
		TArray<const FGsSchemaPassivitySet*> passivityList;
		if (_costumeData != nullptr)
		{
			auto userRow = _costumeData->skillPassivityList.FilterByPredicate([weaponType](auto& e) {
				return e.weaponType == weaponType; });

			for (auto& e : userRow)
			{
				passivityList.Add(e.passivityId.GetRow());
			}
		}
		return passivityList;
	}

	const FGsSchemaCostumeSkill* GetCostumeActiveSkill() const
	{
		return (!_costumeData || _costumeData->skillActiveInfo.weaponType == CreatureWeaponType::NONE) ? 
			nullptr : &_costumeData->skillActiveInfo;
	}

	const FSoftObjectPath GetCostumeActiveSkillIconPath() const
	{
		if (const auto skillData = GetCostumeActiveSkill())
		{
			if (const auto useSkill = skillData->userSkillSetId.GetRow())
			{
				return useSkill->icon;
			}
		}
		return FSoftObjectPath();
	}

	const FGsSchemaSkillSet* GetCostumeUserSkillInfo(CreatureGenderType gender) const 
	{
		if (const auto skillData = GetCostumeActiveSkill())
		{
			if(const auto useSkill = skillData->userSkillSetId.GetRow())
				return (gender == CreatureGenderType::MALE) ? useSkill->maleSkillId.GetRow() : useSkill->femaleSkillId.GetRow();
		}

		return nullptr;
	}

	const uint32 GetCostumeClothID(CreatureGenderType gender) const {

		const FGsSchemaUserLookInfo* lookinfo = _costumeData == nullptr ? nullptr : _costumeData->detail.shapeInfo.GetRow();
		if (auto shape = lookinfo == nullptr ? nullptr : lookinfo->PartsBody.GetRow())
		{
			if (Algo::FindByPredicate(shape->shapeInfo, [gender](auto& pair) {
				return pair.Key == CreatureGenderType::ALL || pair.Key == gender; }))
				return shape->id;
		}
		return INVALID_CUSTOM_PART_ID;
	}

	const uint32 GetCostumeHairID(CreatureGenderType gender) const {

		const FGsSchemaUserLookInfo* lookinfo = _costumeData == nullptr ? nullptr : _costumeData->detail.shapeInfo.GetRow();
		if (auto shape = lookinfo == nullptr ? nullptr : lookinfo->PartsHair.GetRow())
		{
			if (Algo::FindByPredicate(shape->shapeInfo, [gender](auto& pair) { 
				return pair.Key == CreatureGenderType::ALL || pair.Key == gender; }))
				return shape->id;
		}
		return INVALID_CUSTOM_PART_ID;
	}

	const FSoftObjectPath GetCostumePortraitPath(CreatureGenderType gender = CreatureGenderType::ALL) const {
		if (_costumeData)
		{
			if (auto iter = _costumeData->detail.portrait.Find(gender))
				return *iter;
			if (auto iter = _costumeData->detail.portrait.Find(CreatureGenderType::ALL))
				return *iter;
			if (_costumeData->detail.portrait.Num() > 0)
				return _costumeData->detail.portrait.begin()->Value;
		}
		return FSoftObjectPath();
	}

	bool GetHiddenPartsForBodyCostume(CreatureGenderType gender, TArray<EGsPartsType>& hiddenParts) const {

		const FGsSchemaUserLookInfo* lookinfo = _costumeData == nullptr ? nullptr : _costumeData->detail.shapeInfo.GetRow();
		if (auto shape = lookinfo == nullptr ? nullptr : lookinfo->PartsBody.GetRow())
		{
			if (auto iter = Algo::FindByPredicate(shape->shapeInfo, [gender](auto& pair) {
				return pair.Key == CreatureGenderType::ALL || pair.Key == gender; }))
			{
				if (iter->Value.hiddenParts.Num() > 0)
				{
					hiddenParts.Append(iter->Value.hiddenParts);
					return true;
				}
			}	
		}
		return false;
	}

	bool GetHiddenPartsForHairCostume(CreatureGenderType gender, TArray<EGsPartsType>& hiddenParts) const {

		const FGsSchemaUserLookInfo* lookinfo = _costumeData == nullptr ? nullptr : _costumeData->detail.shapeInfo.GetRow();
		if (auto shape = lookinfo == nullptr ? nullptr : lookinfo->PartsHair.GetRow())
		{
			if (auto iter = Algo::FindByPredicate(shape->shapeInfo, [gender](auto& pair) {
				return pair.Key == CreatureGenderType::ALL || pair.Key == gender; }))
			{
				if (iter->Value.hiddenParts.Num() > 0)
				{
					hiddenParts.Append(iter->Value.hiddenParts);
					return true;
				}
			}
		}
		return false;
	}

	const FText GetSummonCostumeDialogue() const { return _costumeData != nullptr ? _costumeData->summonHighGradeComment : FText::GetEmpty(); }

	const bool IsCustomizedPelvisSizeApply() const { return _costumeData != nullptr ? _costumeData->detail.applyCustomizedPelvisSize : true; }

	const FText GetCostumeFlavorText() const { return _costumeData != nullptr ? _costumeData->detail.flavorText : FText::GetEmpty(); }	

	bool IsUsableAppearanceCostume() { return _costumeData != nullptr ? _costumeData->useAppearance : false; }
	bool IsAppearanceAppliableCostume() { return IsUsableAppearanceCostume() && isCollectedCostume(); }

	bool IsEnableAppearceParts(CostumeParts parts) {
		return IsUsableAppearanceCostume() && ((parts == CostumeParts::FULLSET && _costumeData->useWearParts.isSetAppearance)
			|| (parts == CostumeParts::HAIR && !_costumeData->useWearParts.isSetAppearance && _costumeData->useWearParts.hairPart)
			|| (parts == CostumeParts::BODY && !_costumeData->useWearParts.isSetAppearance && _costumeData->useWearParts.bodyPart));
	}

	PortraitType GetType() override { return PortraitType::Costume; }
	uint64 GetId() override { return _id; }
	bool IsCollected() override { return isCollectedCostume(); }
	const FText GetItemName() override { return GetCostumeName(); }
	const FSoftObjectPath GetPortraitPath(CreatureGenderType gender = CreatureGenderType::ALL) override { return GetCostumePortraitPath(gender); }
	const FSoftObjectPath GetPortraitPath() { return GetCostumePortraitPath(); }
	ItemGrade GetGradeByItemBase() override { return static_cast<ItemGrade>(GetCostumeGrade()); }
	const FText GetFlavorText() override { return GetCostumeFlavorText(); }

	virtual ~FGsCostumeData() {}
};


