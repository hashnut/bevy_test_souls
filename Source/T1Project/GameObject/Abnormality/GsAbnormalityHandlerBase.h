// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/GsGameObjectHandler.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

#include "Util/GsEnum.h"

namespace PD
{
	namespace SC
	{
		struct PKT_SC_PASSIVITY_ADDED_READ;
		struct PKT_SC_PASSIVITY_REMOVED_READ;
		struct PKT_SC_ABNORMALITY_READ;
	}
}

/**
 * 상태 관련 처리 클래스
 * Buff System
 * 아직 구체화 되진 않았지만 상태관련 패킷 정보를 이곳에서 받아 확장하여 사용
 */
class T1PROJECT_API FGsAbnormalityHandlerBase : public IGsGameObjectHandler, public TSharedFromThis<FGsAbnormalityHandlerBase>
{
protected:
	class UGsGameObjectBase* _owner{ nullptr };
	class FGsCrowdControlBase* _crowdControl{ nullptr };

	TMap<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>> _mapAbnormality ;
	TMap<AbnormalityInstanceId, uint64> _mapSound;							// 사운드 처리용 버퍼
	TArray<AbnormalityInstanceId> _abnormalityDebuffList;
	
	TArray<TWeakPtr<FGsAbnormalityData>> _listMaterialChange;				// 추후 적용때 
	TWeakPtr<FGsAbnormalityData> _materialAppliedAbnormality{ nullptr };
	//AbnormalityInstanceId _currentMaterialId = 0;				// 추후 적용때 제거

	// 상태이상 이펙트 스케일 정보
	FVector _abnormalityEffectScale{ FVector::ZeroVector };

	// 상태 타입 파악을 위한 캐싱 정보
	intEnumArray<AbnormalityEffectType> _typeCount{ AbnormalityEffectType::MAX };

public:
	FGsAbnormalityHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::ABNORMALITY) //내부 변수가 초기화 되지 않고 사용된다.
		, _owner(nullptr)
		, _crowdControl(nullptr)
		, _mapAbnormality()
		, _mapSound()
		, _abnormalityDebuffList()
		, _listMaterialChange()
		, _materialAppliedAbnormality(nullptr)
		, _typeCount(AbnormalityEffectType::MAX) 
	{}; 

	virtual ~FGsAbnormalityHandlerBase() = default;

public:
	// UTIL : FGsSchemaAbnormalitySet 데이터의 effectIdList에서 요청한 AbnormalityEffectType이 존재하는가
	static bool IsAbnormalityEffectType(AbnormalityEffectType FindType,
		const FGsSchemaAbnormalitySet* AbnormalitySetTable);

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;
	virtual void InitializeActor(class AActor* Owner) override;

	virtual void Update(float Delta) override;

protected:
	virtual class FGsCrowdControlBase* CreateCrowdControl();

public:
	// Passivity
	virtual void Set(PD::SC::PKT_SC_PASSIVITY_ADDED_READ* InPacket) {}
	virtual void Set(PD::SC::PKT_SC_PASSIVITY_REMOVED_READ* InPacket) {}
	virtual bool GetPassiveDataList(OUT TArray<FGsPassivityData*>& OutList) { return false; }
	virtual bool GetPassivityDebuffList(OUT TArray<FGsPassivityData*>& OutList, bool bUseSort) { return false; }
	virtual int32 GetBuffCount() const;
	virtual void OnUpdateCollectionPassivity(int32 InPassivityOwnerType) {}

protected:
	virtual void OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewAdded, bool isSpawnEffect = true);
	virtual void OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData);
	//virtual void OnLoopEffectActive(TWeakPtr<FGsAbnormalityData> inData);
	// 새로 추가되거나, 완전 삭제되는 AbnormalityEffectType 처리(OnActive, OnDeActive 에서는 처음 접속했을때 처리를 할수 없다.)
	// @see : void Set(PD::SC::PKT_SC_ABNORMALITY_READ* Packet);
	virtual void UpdateAbnormalityEffectType(AbnormalityEffectType EffectType, bool IsAdd);

	bool IsMaterialEffectApplied(AbnormalityEffectType effectType);
	virtual void UpdateMaterial(TWeakPtr<FGsAbnormalityData> updateData = nullptr);
	void UpdateEffectScale();

	virtual void PlaySound(const FGsSchemaSoundResData* InSoundData, AbnormalityInstanceId InKey);
	// InKey == 0 이면 등록되있는 모든것 처리
	virtual void StopSound(AbnormalityInstanceId InKey);

	void SetVisibleOwner(class UGsGameObjectCreature* castObject, bool set, bool effectApply = true);
	void CheckTeamChangedInHidden();

public:
	// Abnormality
	void Set(PD::SC::PKT_SC_ABNORMALITY_READ* Packet);
	virtual void Add(const AbnormalityData& NetData, bool isRefresh = true);
	virtual void Remove(AbnormalityInstanceId InstanceId);

protected:
	// 현재 걸려있는 타입을 재갱신
	void UpdateTypeCount();
	void ClearTypeCount();

	bool EmplaceExecuteData(IN const AbnormalityData& NetData);
	bool EmplaceMaterialChangeData(TSharedPtr<FGsAbnormalityData> executeData);

public:
	bool GetAbnormalityDataList(OUT TArray<FGsAbnormalityData*>& OutList, bool bUseSort = true);
	bool GetAbnormalityDebuffList(OUT TArray<FGsAbnormalityData*>& OutList, bool bUseSort);
	// 여러개의 같은 타입의 Abnormality중 가장 지속 시간이 긴 데이터 얻기
	TWeakPtr<FGsAbnormalityData> GetLongerstAbnormalityData(AbnormalityEffectType Type);
	TWeakPtr<FGsAbnormalityData> GetAbnormalityData(AbnormalityInstanceId InKey) const;

	// 핸재 걸려있는 상태 타입 파악
	int GetAbnormalityEffectType(AbnormalityEffectType Type) const;
	bool IsAbnormalityEffectType(AbnormalityEffectType Type) const;
	bool IsAbnormalityMaterialChangeType(AbnormalityEffectType Type) const;
	bool IsCrowdControlMaterialChangeType(AbnormalityEffectType Type) const;

	bool IsAbnormalityId(AbnormalityId InAbnormalityId) const;
	bool IsAbnormalityGroupId(int32 InAbnormalityGroupID) const;
	virtual bool IsTeamMate() { return false; }

	class FGsCrowdControlBase* GetCrowdControl() const { return _crowdControl; }

public:
	bool IsStateChangedByAbnormality();

	bool IsDamagedByAbnormality();

public:
	virtual bool IsTargetedAvailableState();
};
