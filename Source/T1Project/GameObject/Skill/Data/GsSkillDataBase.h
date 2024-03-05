#pragma once

#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotify.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Util/GsTableUtil.h"

// DC 구조체가 상속 지원이 어려워 중간 인터페이스 클래스를 통해 캐스팅 가능하게 처리
class T1PROJECT_API IGsSkillRunnerData
{
public:
	virtual void Init() {}
	virtual void Clear() {}

public:
	virtual ActionCategory GetType() const = 0;
	virtual int32 GetId() const = 0;
	virtual uint8 GetCategory() const = 0;
	virtual const TArray<FGsSchemaSkillNotify>* GetNotifyList() const = 0;
	virtual const TArray<FGsSchemaSkillNotify>* GetClientNotifyList() const = 0;
	virtual const FSoftObjectPath& GetPath() const = 0;
	virtual float GetTotalTime() const = 0;
	virtual bool CanMove() const = 0;
	virtual bool IsValid() const = 0;

	//virtual bool CanCancel(float ElapsedTime) const = 0;
};

class T1PROJECT_API FGsSkillRunnerSKillData : public IGsSkillRunnerData
{
protected:
	const FGsSchemaSkillSet* _skillData = nullptr;

public:
	virtual void Clear() override
	{
		_skillData = nullptr;
	}

public:
	virtual ActionCategory GetType() const override
	{
		return ActionCategory::SKILL;
	}
	virtual int32 GetId() const override
	{
		return _skillData->id;
	}
	virtual uint8 GetCategory() const override
	{
		return (uint8)_skillData->category;
	}
	virtual const TArray<FGsSchemaSkillNotify>* GetNotifyList() const override
	{
		return &_skillData->notifyInfoList;
	}
	virtual const TArray<FGsSchemaSkillNotify>* GetClientNotifyList() const override
	{
		return &_skillData->clientNotifyInfoList;
	}
	virtual const FSoftObjectPath& GetPath() const override
	{
		return _skillData->aniPath;
	}
	virtual float GetTotalTime() const override
	{
		return _skillData->totalRuntime;
	}
	virtual bool CanMove() const override
	{
		return _skillData->movable;
	}
	virtual bool IsValid() const override
	{
		return _skillData != nullptr;
	}

public:
	SkillAreaCenter GetCollisitonCenterType() const
	{
		return _skillData->collisionInfoList.Last().centerType;
	}
	
public:
	void SetData(const FGsSchemaSkillSet* Data)
	{ 
		_skillData = Data; 
	}
	const FGsSchemaSkillSet* GetTableData() const
	{ 
		return _skillData; 
	}
};

class T1PROJECT_API FGsSkillRunnerCommonData : public IGsSkillRunnerData
{
protected:
	const FGsSchemaSkillCommon* _commonData = nullptr;

public:
	virtual void Clear() override
	{
		_commonData = nullptr;
	}

public:
	virtual ActionCategory GetType() const override
	{
		return ActionCategory::COMMON;
	}
	virtual int32 GetId() const override
	{
		return _commonData->id;
	}
	virtual uint8 GetCategory() const override
	{
		return (uint8)_commonData->category;
	}
	virtual const TArray<FGsSchemaSkillNotify>* GetNotifyList() const override
	{
		return nullptr;
	}
	virtual const TArray<FGsSchemaSkillNotify>* GetClientNotifyList() const override
	{
		return nullptr;
	}
	virtual const FSoftObjectPath& GetPath() const override
	{
		return _commonData->aniPath;
	}
	virtual float GetTotalTime() const override
	{
		return _commonData->totalRuntime;
	}
	virtual bool CanMove() const override
	{
		return false;
	}
	virtual bool IsValid() const override
	{
		return _commonData != nullptr;
	}

public:
	void SetData(const FGsSchemaSkillCommon* Data)
	{ 
		_commonData = Data;
	}
	const FGsSchemaSkillCommon* GetTableData() const
	{ 
		return _commonData;
	}
};
