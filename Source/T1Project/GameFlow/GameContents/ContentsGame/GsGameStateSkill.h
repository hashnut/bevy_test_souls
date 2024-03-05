// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageUI.h"



class UGsUIWindowSkill;
class UGsUITraySkillEnchantWait;
class FGsSkill;
struct IGsMessageParam;

/**
 * 스킬창 들어갔을 때 로직
 */
class T1PROJECT_API FGsGameStateSkill : public FGsContentsGameBase
{

protected:
	MsgUIHandleArray _msgUIHandleList;

	TWeakObjectPtr<UGsUITraySkillEnchantWait> _trayEnchantWait;

	SkillId _enchantSkillId = INVALID_SKILL_ID;
	SkillId _resultSkillId = INVALID_SKILL_ID;	
	SkillEnchantResult _enchantResult = SkillEnchantResult::NONE;
	bool _bIsEnchantProgressEnd = false;

public:
	FGsGameStateSkill();
	virtual ~FGsGameStateSkill();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void OnReconectionEnd() override;

protected:
	void RegisterMessages();
	void DeregisterMessages();

	// 메시지 핸들
	void OnSkillEnchantEffectStart(const IGsMessageParam* InParam);
	void OnSkillEnchantEffectResult(const IGsMessageParam* InParam);
	void OnSkillEnchantEffectEndProgress(const IGsMessageParam*);

	// 레드닷 리스트 채우기
	void SetRedDotSkillList(OUT TArray<const FGsSkill*>& OutList);
	void OpenResult();
	const FGsSkill* GetResultSkill() const;
};
