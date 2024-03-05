#pragma once
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Text.h"

class T1PROJECT_API FGsSkillCheckArray
{
private:
	TArray<class IGsSkillCheckBase*>	_checks;

	FText _lastErrMessage;

public:
	FGsSkillCheckArray() = default;
	virtual ~FGsSkillCheckArray() = default;

public:
	void Initialize();
	void Finalize();

public:
	EGsSkillCheckResultType ProcessCheck(const class FGsSkill* NextSkill);
	EGsSkillCheckResultType ProcessCheck(const class FGsSkill* NextSkill, int Index);

public:
	void Add(IGsSkillCheckBase* inChecker);
	void Remove(IGsSkillCheckBase* inChecker);
	void RemoveAll();

	void SetErrorMessage(const FText& ErrMessage);
	const FText& GetErrMessage() const;
	class IGsSkillCheckBase* GetCheckClass(int Index) const;
};

