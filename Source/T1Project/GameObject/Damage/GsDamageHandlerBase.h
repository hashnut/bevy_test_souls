// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GsGameObjectHandler.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Classes/GsPool.h"
/**
 * 선판정 기반 대미지 (Hit) 처리 클래스
 * FGsSchemaSkillSet을 기준으로 해당 Notify 타이밍(notifyInfoList.offset)에 맞춰 대미지 및 히트 연출을 처리
 */

namespace PD
{
	namespace SC
	{
		struct PKT_SC_SKILL_DAMAGED_READ;
		struct PKT_SC_SKILL_HEALED_READ;
	}
}

//-------------------------------------------------------------------------------------------------
// FGsDamageNotifyInfo
//-------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsDamageNotifyInfo
{
	static int AllocCount;

	float _offset = 0.f;									// 출력 시간
	FVector _stiffenPos = FVector::ZeroVector;				// 경직 발동 동기화 위치 (없을 경우 ZeroVector)
	float _stiffenDuration = 0.0f;							// 경직 유지시간

private:
	uint8*					_packetBuffer;					// 패킷 버퍼
	
	FGsDamageNotifyInfo*	_pNext;
	FGsDamageNotifyInfo*	_pPrevious;

public:
	FGsDamageNotifyInfo();
	~FGsDamageNotifyInfo();

public:
	void SetNext(FGsDamageNotifyInfo* pack) { _pNext = pack; };
	void SetPrevious(FGsDamageNotifyInfo* pack) { _pPrevious = pack; };

	FGsDamageNotifyInfo* GetNext(void)     const { return _pNext; };
	FGsDamageNotifyInfo* GetPrevious(void) const { return _pPrevious; };

public:
	inline void* GetData(void) const
	{
		return _packetBuffer;
	}

public:
	bool operator==(const FGsDamageNotifyInfo& __Other) const;
};

//-------------------------------------------------------------------------------------------------
// FGsHealNotifyInfo Pool기능을 사용하므로 따로 구조체 선언하여 구현
//-------------------------------------------------------------------------------------------------
struct T1PROJECT_API FGsHealNotifyInfo
{
	static int AllocCount;

	float _offset = 0.f;					// 출력 시간
private:
	uint8* _packetBuffer;					// 패킷 버퍼

	FGsHealNotifyInfo* _pNext;
	FGsHealNotifyInfo* _pPrevious;

public:
	FGsHealNotifyInfo();
	~FGsHealNotifyInfo();

public:
	void SetNext(FGsHealNotifyInfo* pack) { _pNext = pack; };
	void SetPrevious(FGsHealNotifyInfo* pack) { _pPrevious = pack; };

	FGsHealNotifyInfo* GetNext(void)     const { return _pNext; };
	FGsHealNotifyInfo* GetPrevious(void) const { return _pPrevious; };

public:
	inline void* GetData(void) const
	{
		return _packetBuffer;
	}

public:
	bool operator==(const FGsHealNotifyInfo& __Other) const;
};


//-------------------------------------------------------------------------------------------------
// FGsDamageRunner
//-------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsDamageRunner
{
protected:
	TArray<FGsDamageNotifyInfo*> _listDamageInfo;

public:
	static TGsPoolPreSizeAlloc<FGsDamageNotifyInfo>* PoolNotify;

public:
	FGsDamageRunner();

	void Clear();

	void Pop(float inDeltaTime, OUT TArray<FGsDamageNotifyInfo*>& OutList);
	void Pop(uint32 NotifyId, OUT TArray<FGsDamageNotifyInfo*>& OutList);
	
	void Push(float Offset, PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet);	
	bool SetStiffen(uint32 NotifyId, const FVector& StiffenPos, float fDuration);

	bool IsEmpty() const;
	TArray<FGsDamageNotifyInfo*>& GetDamageNotifyList();
};

//-------------------------------------------------------------------------------------------------
// FGsHealRunner
//-------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsHealRunner
{
protected:
	TArray<FGsHealNotifyInfo*> _listHealInfo;

public:
	static TGsPoolPreSizeAlloc<FGsHealNotifyInfo>* PoolHealNotify;

public:
	FGsHealRunner();

	void Clear();

	void Pop(float inDeltaTime, OUT TArray<FGsHealNotifyInfo*>& OutList);
	void Pop(uint32 NotifyId, OUT TArray<FGsHealNotifyInfo*>& OutList);
	void Push(float Offset, PD::SC::PKT_SC_SKILL_HEALED_READ* Packet);

	bool IsEmpty() const;
	TArray<FGsHealNotifyInfo*>& GetHealNotifyList();
};

//-------------------------------------------------------------------------------------------------
// FGsDamageHandlerBase
//-------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsDamageHandlerBase : public IGsGameObjectHandler
{
protected:
	class UGsGameObjectBase* _owner = nullptr;

	// 타격자 기준 DamageRunner 관리 : 타격자가 동시에 다른 스킬을 쓸수는 없다.
	/*TMap<uint32, FGsDamageRunner> _damageRunner;*/
	FGsDamageRunner _damageRunner;
	FGsHealRunner _healRunner;

public:
	FGsDamageHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::DAMAGE) {};
	virtual ~FGsDamageHandlerBase() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Update(float Delta) override;
	virtual void Finalize() override;

public:
	// 죽음에 이르거나 특정 상황에 한번에 처리할 경우 호출
	virtual void ExcuteDamage(TArray<FGsDamageNotifyInfo*>& ListInfo);
	virtual void ExcuteHeal(TArray<FGsHealNotifyInfo*>& ListInfo);
	// 특정 정보로 강제 호출
	virtual void FindExcuteDamage(uint32 NotifyId);
	virtual void Clear();

	// 패킷 등록
	virtual void Add(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet);
	virtual void Add(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet);
	// 경직 지연 처리
	bool AddStiffen(uint32 NotifyId, const FVector& StiffenPos, float Duration);

protected:
	// 히트 효과 처리
	virtual void HitEffect(const struct FGsSchemaSkillSet* SkillData, int64 Caster, int64 Target, HitResultType ResultType);

public:
	bool IsEmpty() const;
};
