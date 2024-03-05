#pragma once

#include "GameObject/GsGameObjectHandler.h"

/**
 * Object 타겟 Base클래스
 * 현재는 가장 심플하게 구현해둔다.
 */
class T1PROJECT_API FGsTargetHandlerBase : public IGsGameObjectHandler
{
public:
	// 어떤 경로로 타겟 설정은 하는가에 대한 타입
	enum SelectTargetFilter
	{
		None,
		LockOn,
		ScanList,
		SelectTarget,
		Auto,
		AutoSkill,
		Abnormality_Taunt,
		TargetChange,
	};

protected:
	TWeakObjectPtr<UGsGameObjectBase> _target;

protected:
	UGsGameObjectBase* _owner = nullptr;
	// 오토가 세팅한 타겟인가
	SelectTargetFilter _selectFilter = SelectTargetFilter::None;

public:
	FGsTargetHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::TARGET) {};
	virtual ~FGsTargetHandlerBase()	= default;

public:
	virtual void Initialize(UGsGameObjectBase* owner) override;
	virtual void Finalize()  override {};

public:
	virtual void ClearTarget(bool isAllClear = false);
	virtual void ClearTarget(UGsGameObjectBase* target);
	virtual void ClearSkillSelectTarget(int64 InGameId = 0);

	virtual bool SetTarget(UGsGameObjectBase* target, bool isRotate = false, SelectTargetFilter selectFilter = SelectTargetFilter::None);

public:
	UGsGameObjectBase* GetTarget() const;
	virtual UGsGameObjectBase* GetSecondTarget() const { return nullptr; }
	bool			   GetIsAutoTarget() const;

	bool IsTargetByAuto()
	{
		return (_selectFilter == SelectTargetFilter::Auto) ? true : false;
	}

private:
	void CallbackClearTarget(UGsGameObjectBase* target);
};
