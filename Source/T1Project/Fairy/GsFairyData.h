#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Skill/Passivity/GsSchemaPassivitySet.h"
#include "Abnormality/GsSchemaAbnormalitySet.h"
#include "Fairy/GsSchemaFairyCommon.h"
#include "Fairy/GsSchemaFairyEffect.h"
#include "Fairy/GsBasePortrait.h"

#include "ForEach.h"


#define USE_LOCALIZED_SOUND

class UParticleSystem;

/// <summary>
/// 2020.09.25 페어리의 경험치가 사라지고, 
/// 강화시 페어리를 이용하지 않고 특정 아이템을 이용하는 것으로 기획서 변경에 따라 경험치에 대한 부분 수정
/// 20201021 skillID 대신 PassivityID로 
/// </summary>
/// 
/// 
using FairyFeatureId = uint32;
using FairyGradeId = uint32; //임시
constexpr int64 INVALID_FAIRY_FEATURE_ID = 0;
constexpr int64 INVALID_FAIRY_GRADE_ID = 0;

class T1PROJECT_API FGsFairyItem
{
private:
	FairyId		_fairyID{ INVALID_FAIRY_ID };
	FairyLevelId _level{ INVALID_LEVEL };		// 페어리 레벨 정보
	FairyLevelId _highestLevel{ INVALID_LEVEL };	// 페어리 최대 달성 레벨 정보 ( passivity open)
	ItemAmount	_amount{ MIN_ITEM_AMOUNT };		// 페어리 보유 개수
	PresetValue _presetValue{ INVALID_PRESET_VALUE };	// 프리셋 정보
public:
	bool _isLevelNew{ false };

public:
	FGsFairyItem(FairyId fairyID, FairyData& itemData) 
		: _fairyID(fairyID), _level(itemData.mLevel), _highestLevel(itemData.mHighestLevel), _amount(itemData.mAmount), _presetValue(itemData.mPresetValue) {}

	virtual ~FGsFairyItem() = default;

	FairyId GetFaryID() { return _fairyID; }
	//FairyProbabilityId GetFairyProbabilityId() { return _fairyProbabilityId; }
	bool isCollected()  { return true; }
	ItemAmount GetAmount() { return _amount;}
	FairyLevelId GetCurrentLevel() { return _level; }
	FairyLevelId GetHighestLevel() { return _highestLevel;	}
	PresetValue GetPresetValue() { return _presetValue; }

	void UpdateFairyItem(FairyData& itemData)
	{
		_level = itemData.mLevel;
		_highestLevel = itemData.mHighestLevel;
		_amount = itemData.mAmount;
		_presetValue = itemData.mPresetValue;

		if (_highestLevel != itemData.mHighestLevel)
			_isLevelNew = true;
	}
};

struct T1PROJECT_API FGsFairyLevelInfo
{
	FairyLevelId _level{ INVALID_LEVEL };
	bool  _isOpenLevel{ false };
	const FGsSchemaPassivitySet* _statPassivity{ nullptr };
	const FGsSchemaPassivitySet* _byVehiclePassivity{ nullptr };
	const FGsSchemaAbnormalitySet* _byVehicleAbnormalitySet{ nullptr };
	TArray<const FGsSchemaPassivitySet*> _skillPassivities;
};

//fairy 도감용 데이터 - 모든 페어리와 수집된 페어리의 정보
struct T1PROJECT_API FGsFairyData : public IGsBasePortrait
{
	struct FairyLevelPassivity
	{
		const FGsSchemaPassivitySet* _baseStatPassivity{ nullptr };
		TArray<FGsFairyLevelInfo>    _levelPassivity;
	};

	FairyId _id{ INVALID_FAIRY_ID };
	const FGsSchemaFairyCommon*  _fairyCommon{ nullptr };
	FairyLevelPassivity _passivityInfo;

	TArray<FGsSchemaFairyEffect> _fairyBaseEffects;

	TWeakPtr<FGsFairyItem> _fairyItem; //invalidate 한 것은 보유중이 아님

	bool _isNew{ false };
	bool _isSummonNew{ false };
	bool _isEnableCompose{ true };

	FGsFairyData(const FGsSchemaFairyCommon* data) : _fairyCommon(data)
	{
		_id = _fairyCommon->id;
		if (auto levelData = _fairyCommon->levelId.GetRow())
		{
			_passivityInfo._baseStatPassivity = levelData->baseStatPassivityId.GetRow();

			for (const auto& levelInfo : levelData->levelInfoList)
			{
				FGsFairyLevelInfo levelPassivity;
				levelPassivity._level = levelInfo.level;
				levelPassivity._statPassivity = levelInfo.addStatPassivityId.GetRow();

				Algo::ForEachIf(levelInfo.passivityIdList, [](auto& e) { return e.GetRow() != nullptr; }, 
					[&levelPassivity](auto& e) { levelPassivity._skillPassivities.Add(e.GetRow());});

				levelPassivity._byVehiclePassivity = levelInfo.vehiclePassivityId.GetRow();
				levelPassivity._byVehicleAbnormalitySet = levelInfo.vehicleVisualAbnormality.GetRow();

				_passivityInfo._levelPassivity.Add(levelPassivity);
			}

			_passivityInfo._levelPassivity.StableSort([](auto& le, auto& re) { return le._level < re._level; });
		}
	}
	
	bool IsEnableUpgade() const { return GetCurrentLevel() < GetLimitLevelUp(); }
	bool IsCurrentLevelOpen() const { return GetCurrentLevel() <= GetOpenLevel(); }

	bool isCollectedFairy() { return _fairyItem.IsValid(); }
	ItemAmount GetAmount() override { return _fairyItem.IsValid() ? _fairyItem.Pin()->GetAmount() : 0; }
	FairyLevelId GetCurrentLevel() const { return _fairyItem.IsValid() ? _fairyItem.Pin()->GetCurrentLevel() : INVALID_LEVEL; }
	FairyLevelId GetOpenLevel()	const { return _fairyItem.IsValid() ? _fairyItem.Pin()->GetHighestLevel() : INVALID_LEVEL; }
	PresetValue GetPresetValue() const { return _fairyItem.IsValid() ? _fairyItem.Pin()->GetPresetValue() : INVALID_PRESET_VALUE; }
	FairyLevelId GetUpgradeLevel() const { return IsEnableUpgade() ? GetCurrentLevel() + 1 : INVALID_LEVEL; }

	bool isNewItem() override { return isCollectedFairy() ?  _isNew : false; }
	void removeNewMark() { _isNew = false;  }

	bool IsNewSummon() override { return _isSummonNew; }
	void removeNewSummonMark() { _isSummonNew = false;  }

	bool isLevelNewItem() { return isCollectedFairy() ? _fairyItem.Pin()->_isLevelNew : false; }
	void removeLevelNewMark() { if(_fairyItem.IsValid()) _fairyItem.Pin()->_isLevelNew = false; }
	bool isEnableComposingItem() override { return _isEnableCompose; }

	const FairyGrade GetFairyGrade() const { return _fairyCommon != nullptr ? _fairyCommon->grade : FairyGrade::NONE; }
	const int GetFairyTier() const { return _fairyCommon != nullptr ? _fairyCommon->tier : 0; }
	const FText GetFairyName() const { return _fairyCommon != nullptr ? _fairyCommon->name : FText::GetEmpty(); }
	
	bool isValidFairyLevelData() const { return _fairyCommon != nullptr && _fairyCommon->levelId.GetRow() != nullptr; }
	bool isValidFairyDetail() const { return _fairyCommon != nullptr && _fairyCommon->detailID.GetRow() != nullptr; }
	const FGsSchemaFairyDetail* GetFairyDetail() const { return _fairyCommon != nullptr ? _fairyCommon->detailID.GetRow() : nullptr; }

	const FairyFeatureId GetFairyFeatureID() const { return isValidFairyDetail() ? GetFairyDetail()->id : INVALID_FAIRY_FEATURE_ID; }
	const FString GetFairyExplanation() const { return isValidFairyDetail() ? GetFairyDetail()->explanation : FString(); }
	const FSoftObjectPath GetFairySpawnObjectPath() const { return isValidFairyDetail() ? GetFairyDetail()->pathPartFairy : FSoftObjectPath(); }
	const FSoftObjectPath GetFairyPortraitPath() const { return isValidFairyDetail() ? GetFairyDetail()->pathFairyPortrait : FSoftObjectPath(); }
	const FText GetFairyFlavorText() const { return isValidFairyDetail() ? GetFairyDetail()->flavorText : FText::GetEmpty(); }
#ifdef USE_LOCALIZED_SOUND
	const FString GetFairyVoiceFilePath() const { return GetFairyDetail()->voiceFilePath; }
	const FString GetSummonFairyVoiceFilePath() const { return  GetFairyDetail()->summonHighGradeVoicePath; }
#else
	const FGsSchemaSoundResData* GetFairyUISoundResData() const { return isValidFairyDetail() ? GetFairyDetail()->uiSelectedSound.GetRow() : nullptr; }
	const FSoftObjectPath GetFairUISoundPath() const {
		if (auto res = GetFairyUISoundResData()) return res->path;
		return FSoftObjectPath();
	}
#endif
	const FSoftObjectPath GetSummonFairyPath() const { return isValidFairyDetail() ? GetFairyDetail()->pathSummonFairy : FSoftObjectPath(); }
	const FText GetSummonFairyDialogue() const { return _fairyCommon != nullptr ? _fairyCommon->summonHighGradeComment : FText::GetEmpty(); }

	bool GetFairyLevelPassivityList(OUT const FGsSchemaPassivitySet*& baseStatPassivity, OUT TArray<FGsFairyLevelInfo>& list)
	{
		baseStatPassivity = _passivityInfo._baseStatPassivity;
		list.Append(_passivityInfo._levelPassivity);

		if (_fairyItem.IsValid())
		{
			auto openLevel = _fairyItem.Pin()->GetHighestLevel();
			Algo::ForEachIf(_passivityInfo._levelPassivity, [openLevel](auto& e) {
				return e._level <= openLevel; }, [](auto& e) {e._isOpenLevel = true; });

			return true;
		}
		return false;
	}

	const FGsSchemaPassivitySet* GetByVehiclePassivity(int32 level = 0)
	{
		if (const auto levelPassivity = _passivityInfo._levelPassivity.FindByPredicate([level](auto& e) { return e._level == level; }))
		{
			return levelPassivity->_byVehiclePassivity;
		}
		return nullptr;
	}

	bool GetFairyCurLevelPassivityList(OUT FGsFairyLevelInfo& info) //나중에는 필요없을 듯 일단 사용
	{
		Level openLevel = INVALID_LEVEL;
		if (_fairyItem.IsValid())
		{
			openLevel = _fairyItem.Pin()->GetHighestLevel();

			if (auto iter = _passivityInfo._levelPassivity.FindByPredicate([openLevel](auto& e) { return e._level == openLevel;}))
			{
				info = *iter;
			}

			Algo::ForEachIf(_passivityInfo._levelPassivity, [openLevel](auto& e) {
				return e._level <= openLevel; }, [](auto& e) {e._isOpenLevel = true; });

			return true;
		}

		if (_passivityInfo._levelPassivity.IsValidIndex(0))
		{
			info = _passivityInfo._levelPassivity[0];
		}
		return false;
	}

	bool GetFairyLevelSkillPassivityList(IN int32 level, OUT TArray<const FGsSchemaPassivitySet*>& list) const
	{
		if (const auto levelPassivity = _passivityInfo._levelPassivity.FindByPredicate([level](auto& e) { 
			return e._skillPassivities.IsValidIndex(0) &&  e._level == level; }))
		{
			list.Append(levelPassivity->_skillPassivities);
			return true;
		}
		return false;
	}

	bool GetCurFairyLevelSkillPassivityList(OUT TArray<const FGsSchemaPassivitySet*>& list) const
	{
		return  _fairyItem.IsValid() ? GetFairyLevelSkillPassivityList(_fairyItem.Pin()->GetCurrentLevel(), list) : GetFairyLevelSkillPassivityList(0, list);
	}

	bool GetNextFairyLevelSkillPassivityList(OUT TArray<const FGsSchemaPassivitySet*>& list) const
	{
		return _fairyItem.IsValid() && IsEnableUpgade() ? GetFairyLevelSkillPassivityList(_fairyItem.Pin()->GetCurrentLevel() + 1, list) : false;
	}

	const FGsSchemaPassivitySet* GetFairyBaseStatPassivity() const
	{
		return _passivityInfo._baseStatPassivity;
	}

	const FGsSchemaPassivitySet* GetFairyLevelStatPassivity(IN FairyLevelId level) {

		if (const auto levelPassivity = _passivityInfo._levelPassivity.FindByPredicate([level](auto& e) {
			return e._statPassivity && e._level == level; }))
		{
			return levelPassivity->_statPassivity;
		}
		return nullptr;
	}

	const FGsSchemaPassivitySet* GetCurFairyLevelStatPassivity() { return GetFairyLevelStatPassivity(GetCurrentLevel()); }

	const FairyLevelId GetLimitLevelUp() const { return isValidFairyLevelData() ? _fairyCommon->levelId.GetRow()->maxLevel : INVALID_FAIRY_LEVEL_ID; }

	const FGsSchemaFairyEffect* GetFairySkillEffect() { return isValidFairyDetail() ? &(GetFairyDetail()->skillEffect) : nullptr; }
	const FGsSchemaFairyEffect* GetFaityVehicleEffect() { return isValidFairyDetail() ? &(GetFairyDetail()->vehicleEffect) : nullptr; }
	const TArray<FGsSchemaFairyEffect>& GetFairyBaseEffect() const { return _fairyBaseEffects; }
	const TArray<UParticleSystem*> GetPcTeleportParticleEffectList() const { return isValidFairyDetail() ? (GetFairyDetail()->pcTeleportParticleEffectList) : TArray<UParticleSystem*>(); }
	const TArray<UParticleSystem*> GetFairyTeleportParticleEffectList() const { return isValidFairyDetail() ? (GetFairyDetail()->fairyTeleportParticleEffectList) : TArray<UParticleSystem*>(); }
	const FSoftObjectPath GetPathTeleportStartFairy() const { return isValidFairyDetail() ? (GetFairyDetail()->pathTeleportStartFairy) : FSoftObjectPath(); }
	const FSoftObjectPath GetPathTeleportArriveFairy() const { return isValidFairyDetail() ? (GetFairyDetail()->pathTeleportArriveFairy) : FSoftObjectPath(); }

	const FText GetOnSkillTalkMessage(EGsFairyTalkType talkType = EGsFairyTalkType::SkillOn) {

		if (isCollectedFairy())
		{
			auto curLevel = GetCurrentLevel();

			if (auto levelData = _fairyCommon->levelId.GetRow())
			{
				if (auto iter = levelData->levelInfoList.FindByPredicate([curLevel](auto& e) {return e.level == curLevel;}))
				{
					auto talkIndex = iter->fairyOnSkillTalk.IndexOfByPredicate([talkType](auto& talk) { return talk.talkOnType == talkType; });
					if(talkIndex != INDEX_NONE)
					{
						return iter->fairyOnSkillTalk[talkIndex].onSkillTalk;
					}
				}
			}
		}

		return FText::GetEmpty();
	}

	PortraitType GetType() override { return PortraitType::Fairy; }
	uint64 GetId() override { return _id; }
	bool IsCollected() override		{ return isCollectedFairy(); }
	const FText GetItemName() override { return GetFairyName(); }
	const FSoftObjectPath GetPortraitPath() override { return GetFairyPortraitPath(); }
	const FSoftObjectPath GetPortraitPath(CreatureGenderType gender = CreatureGenderType::ALL) override { return GetFairyPortraitPath();}
	ItemGrade GetGradeByItemBase() override { return static_cast<ItemGrade>(GetFairyGrade()); }
	const FText GetFlavorText() override	{ return GetFairyFlavorText(); }

	virtual ~FGsFairyData() {}
};

