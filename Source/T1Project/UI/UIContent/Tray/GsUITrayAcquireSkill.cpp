// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayAcquireSkill.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/Skill/GsUISkillIcon.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Skill/GsSkill.h"


void UGsUITrayAcquireSkill::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			_remainTime = 0.f;
			Close();
		}
	}
}

void UGsUITrayAcquireSkill::SetSkill(SkillId InSkillId)
{
	if (const FGsSkill* skill = GSkill()->FindSkill(InSkillId, CreatureWeaponType::MAX, true))
	{
		_textBlockName->SetText(skill->_userSkillSetData->nameText);
		_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(skill->_tableData->grade));

		_uiIcon->SetData(skill);

		// 캐릭터 연출
		if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>())
			{
				// 주의: SkillGrade의 0은 에러값이므로 -1 하여 사용한다.
				int32 effectIndex = static_cast<int32>(skill->_tableData->grade) - 1;
				if (AGsCharacterLocalPlayer* character = castLocal->GetLocalCharacter())
				{
					character->PlaySkillLearnEffect(effectIndex);
				}
			}
		}
	}
	else
	{
		_textBlockName->SetText(FText::GetEmpty());
		_uiIcon->SetData(nullptr);
	}

	_remainTime = _lifeTimeS;

	StopAllAnimationsEx();
	PlayAcquireAnimation();
}
