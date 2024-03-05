// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * FSkillActionDataBase 데이터 기반으로 스킬 액션 처리 담당
 */
class T1PROJECT_API FGsSkillNotifyNodeBase
{
protected:
	const struct FGsSchemaSkillNotifySet* _currNotifyData = nullptr;
	const struct FGsSkillNetData* _netData = nullptr;

	float _scale = 1.f;
	float _offset = 0.f;
	// 삭제 대기중 상태
	bool _pendingKill = false;

public:
	FGsSkillNotifyNodeBase()			= default;
	virtual ~FGsSkillNotifyNodeBase()	= default;

public:
	// FGsSkillSetData 에서 찾아야 하는 정보가 있는 경우가 있음
	// 만약 필요없게 되면 FGsSkillSetData 참조는 삭제
	virtual void Initialize();
	virtual void Initialize(class UGsGameObjectBase* Owner, const class IGsSkillRunnerData* SkillRunnerData,
		const struct FGsSchemaSkillNotify* NotifyInfo, const struct FGsSkillNetData* NetData, float OffsetRate);
	
	virtual void Initialize(class UGsGameObjectBase* Owner, const struct FGsSchemaSkillNotifySet* NotifySetData,
		const struct FGsSkillNetData* NetData);

	virtual void Finalize();

public:
	// 현재는 노티파이의 Update를 제공 하지 않는다. (단발성)
	virtual bool Update(class UGsGameObjectBase* Owner, float CurrentTime);

	virtual void Action(class UGsGameObjectBase* Owner);
	virtual void Cancel(class UGsGameObjectBase* Owner);

public:
	int			GetSkillNotifyId() const;
	float		GetOffset() const				{ return _offset ;}
	bool		IsPendingKill() const			{ return _pendingKill; }
	void		SetPendingSkill(bool IsKill)	{ _pendingKill = IsKill; }

#if (WITH_EDITOR)
public:
	virtual FString GetDebugTypeName() { return TEXT("FGsSkillNotifyNodeBase"); }
#endif
};
