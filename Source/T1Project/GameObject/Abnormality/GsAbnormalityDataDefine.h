// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Classes/Collection/GsCollectionData.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Collection/GsSchemaCollectionData.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexLevelInfo.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"
#include "../../UTIL/GsTableUtil.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Algo/Transform.h"
#include "Algo/ForEach.h"

// 클라이언트에서 사용할 Abnormality 구조체 정의
struct T1PROJECT_API FGsAbnormalityData
{
	double _pktReceiveAppTime = 0.f;

protected:
	AbnormalityData _netData;
	const FGsSchemaAbnormalitySet* _tableData = nullptr;
	TArray<const FGsSchemaAbnormalityIndexSet*> _abnormalityEffectSet;
	TArray<const FGsSchemaAbnormalityCrowdControlSet*> _animControls;

	bool _isHidingType{ false };
public:
	FGsAbnormalityData(const FGsSchemaAbnormalitySet* InAbnormalityData, const AbnormalityData& item, float pktTime = 0.f)
	{
		_tableData = InAbnormalityData;
		_animControls.Reset();

		UpdateAbnormalityInfo(item, pktTime);

		if (InAbnormalityData)
		{
			auto level = _netData.GetLevel();
			Algo::TransformIf(InAbnormalityData->effectIdList, _abnormalityEffectSet, [](auto index) {
				return UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(index) != nullptr;
				}, [](auto index) { return UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(index); });

			Algo::ForEachIf(_abnormalityEffectSet, [this, level](auto effectSet) {
					if (effectSet->levelList.IsValidIndex(level) &&
						effectSet->levelList[level].effectRow.template GetRowByType<FGsSchemaAbnormalityCrowdControlSet>() != nullptr)
					{
						if (auto crowdControlSet = effectSet->levelList[level].effectRow.template GetRowByType<FGsSchemaAbnormalityCrowdControlSet>())
						{
							return crowdControlSet->param.animationTypeInfo != EGsAbnormalityAnimProcessType::NONE &&
								   crowdControlSet->param.animationTypeInfo != EGsAbnormalityAnimProcessType::Max;
						}
					}
					return false; 
				}, [this, level](auto effectSet) {
					_animControls.Emplace(effectSet->levelList[level].effectRow.template GetRowByType<FGsSchemaAbnormalityCrowdControlSet>()); // crowdControlSet->param.animationTypeInfo);
				});

			_isHidingType = IsMaterialChangedType() && IsEnableAbnormalityEffectType(AbnormalityEffectType::UTIL_HIDING);
		}
	}

	~FGsAbnormalityData()
	{
		_tableData = nullptr;
	}

	void UpdateAbnormalityInfo(const AbnormalityData& item, float pktTime = 0.f)
	{
		//_netData = item;
		_netData.mId = item.mId;
		_netData.mLevel = item.mLevel;
		_netData.mInstanceId = item.mInstanceId;
		_netData.mRemainTime = item.mRemainTime;
		_netData.mStackCount = item.mStackCount;
		_netData.mCasterId = item.mCasterId;

		_pktReceiveAppTime = (pktTime  == 0.f) ? FApp::GetCurrentTime() : static_cast<double>(pktTime);
	}

	void UpdateRemainTime(float pktTime = 0.f) //preview에서 사용
	{
		_netData.SetRemainTime(_netData.GetRemainTime() - pktTime);
	}

	AbnormalityEffectType GetEffectType() const
	{
		if (_isHidingType) return AbnormalityEffectType::UTIL_HIDING; //가장 높은 순위

		if (auto iter = _abnormalityEffectSet.FindByPredicate([](const auto e) { return e->effectType != AbnormalityEffectType::NONE; }))
		{
			return (*iter)->effectType;
		}
		return AbnormalityEffectType::NONE;
	}

	int GetAllEffectType(TArray<AbnormalityEffectType>& effects) const
	{
		Algo::Transform(_abnormalityEffectSet, effects, [](const auto e) { return e->effectType; });
		return effects.Num();
	}

	const FGsSchemaAbnormalitySet* GetTable() const { return _tableData; }

	bool IsValidAbnormality() { return GetTable() && GetCurrRemainTime() > 0.f; }

	bool IsHidingType() { return _isHidingType;}
	bool IsMaterialChangedType() { return _tableData && _tableData->marterialInfo.objectData != nullptr; };
	bool IsAnimtionControlType() { return _animControls.Num() > 0; }

	UGsGameObjectMaterialData* GetMaterialData() { return IsMaterialChangedType() ? _tableData->marterialInfo.objectData : nullptr;}
	int32 GetMaterialPriority() { return IsMaterialChangedType() ? _tableData->marterialInfo.priority : -1;}

	uint32 GetTableId() const
	{
		return (_tableData) ? _tableData->id : INVALID_ABNORMALITY_ID;
	}

	int32 GetTableGroupId() const
	{
		return (_tableData) ? _tableData->stackGroupId : INVALID_ABNORMALITY_ID;
	}

	int GetAnimationControlTypeList(TArray<EGsAbnormalityAnimProcessType>& animList) {

		Algo::Transform(_animControls, animList, [](const auto e) { return e->param.animationTypeInfo; });
		return animList.Num();
	}

	const FGsSchemaAbnormalityCrowdControlSet* GetAnimationControlByAnimType(EGsAbnormalityAnimProcessType type)
	{
		if (auto iter = _animControls.FindByPredicate([type](auto e) { return e->param.animationTypeInfo == type; }))
			return *iter;
		return nullptr;
	}

	bool IsEnableAnimationProcessType(EGsAbnormalityAnimProcessType animType) {
		return IsValidAbnormality() && 
			_animControls.FindByPredicate([animType](auto e) { return e->param.animationTypeInfo == animType; }) != nullptr;
	}

	bool IsEnableAbnormalityEffectType(AbnormalityEffectType type) {
		return IsValidAbnormality() &&
			_abnormalityEffectSet.FindByPredicate([type](auto e) { return e->effectType == type; }) != nullptr;
	}

	const FGsSchemaAbnormalityIndexSet* GetEnableAbnormalityEffectByEffectType(AbnormalityEffectType type) {
		if (auto iter = _abnormalityEffectSet.FindByPredicate([type](auto e) { return e->effectType == type; }))
			return *iter;
		return nullptr;
	}

	uint32 GetInstanceId() const
	{
		return _netData.GetInstanceId();
	}

	uint8 GetStackCount() const
	{
		return _netData.GetStackCount();
	}

	uint8 GetLevel() const
	{
		return _netData.GetLevel();
	}

	int64 GetCasterGuid() const
	{
		return _netData.GetCasterId();
	}

	float GetRemainTime() const
	{
		float time = _netData.GetRemainTime();
		if (FLT_MIN == time || FLT_MAX == time)
		{
			return 0.f;
		}

		return time;
	}

	float GetCurrRemainTime() const
	{
		if (FLT_MIN == GetRemainTime() || FLT_MAX == GetRemainTime())
		{
			return 0.f;
		}

		// 남은시간에서 흐른시간을 빼서 준다
		double currTime = FApp::GetCurrentTime();
		double deltaTime = currTime - _pktReceiveAppTime;
		float time = GetRemainTime() - static_cast<float>(deltaTime);

		return (0 < time) ? time : 0.f;
	}

	double GetEndAppTime() const
	{
		if (FLT_MIN == GetRemainTime() || FLT_MAX == GetRemainTime())
		{
			return 0.f;
		}

		return static_cast<double>(GetRemainTime()) + _pktReceiveAppTime;
	}
};

//---------------------------------------------------------------------------------------------------------------------

// 클라이언트에서 사용할 Passivity 구조체 정의
struct T1PROJECT_API FGsPassivityData
{
	PassivityId _passivityId = INVALID_PASSIVITY_ID;
	PassivityOwnerType _ownerType = PassivityOwnerType::MAX;
	PassivityOwnerId _ownerId = INVALID_PASSIVITY_OWNER_ID;
	const FGsSchemaPassivitySet* _data = nullptr; 
	
	const IGsCollectionData* _collectionData = nullptr; // 주의: 이 값은 null일 수 있음
	const FGsSchemaCollectionData* _collectionTable = nullptr; // 주의: 이 값은 null일 수 있음

public:
	FGsPassivityData() = default;
	FGsPassivityData(PassivityId InPassivityId, PassivityOwnerType InOwnerType, PassivityOwnerId InOwnerId) :
		_passivityId(InPassivityId), _ownerType(InOwnerType), _ownerId(InOwnerId),
		_data(nullptr), _collectionData(nullptr), _collectionTable(nullptr)
	{
	}

	~FGsPassivityData()
	{
		_data = nullptr;
		_collectionData = nullptr;
		_collectionTable = nullptr;
	}

	bool IsSameData(PassivityId InId, PassivityOwnerType InOwnerType, PassivityOwnerId InOwnerId)
	{
		return (_passivityId == InId &&
			_ownerType == InOwnerType &&
			_ownerId == InOwnerId) ? true : false;
	}
	
	bool IsEnvironment() const
	{
		if (_data)
		{
			return (PassivityOwnerType::MAP == _ownerType) ? true : false;
		}

		return false;
	}

	FText GetNameText() const
	{
		if (_collectionTable)
		{
			return _collectionTable->nameText;
		}

		if (_data)
		{
			return _data->nameText;
		}

		return FText::GetEmpty();
	}

	FText GetDescText() const
	{
		if (_collectionTable)
		{
			return _collectionTable->descText;
		}

		if (_data)
		{
			return _data->descText;
		}

		return FText::GetEmpty();
	}

	FSoftObjectPath GetIconPath() const
	{
		if (_collectionTable)
		{
			return _collectionTable->iconPath;
		}

		if (_data)
		{
			return _data->iconPath;
		}

		return FSoftObjectPath();
	}

	FSoftObjectPath GetSmallIconPath() const
	{
		if (_collectionTable)
		{
			return _collectionTable->smallIconPath;
		}

		if (_data)
		{
			return _data->smallIconPath;
		}

		return FSoftObjectPath();
	}
};
