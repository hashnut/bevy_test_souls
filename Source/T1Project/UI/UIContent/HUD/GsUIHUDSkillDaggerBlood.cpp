#include "GsUIHUDSkillDaggerBlood.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillSlotData.h"

#include "WidgetSwitcher.h"
#include "ProgressBar.h"
#include "DaggerBlood/GsUIDaggerBlood.h"
#include "Message/MessageParam/GsMessageParam.h"


void UGsUIHUDSkillDaggerBlood::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_bloodProgress = { _blood1, _blood2, _blood3, _blood4 };

	_blood4->OnUsedDaggerBlood.AddLambda([this]() { SetUseDaggerBloodSkill(true); }); //skill 사용 연출이 끝나고 progressbar로 전환
}

void UGsUIHUDSkillDaggerBlood::NativeConstruct()
{
	Super::NativeConstruct();

	_curBloodCount = 0;

	BindMessages();

	OnChangeSkillSet(nullptr);
}

void UGsUIHUDSkillDaggerBlood::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIHUDSkillDaggerBlood::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_runStatConsumSkill)
	{
		auto remainTime = _endTime - FApp::GetCurrentTime();

		if (remainTime >= 0.f)
		{
			auto alpha = remainTime / _coolTime;
			_daggerBloodSkillProgress->SetPercent(FMath::Lerp(0.f, 1.f, alpha));
		}
		else
		{
			SetUseDaggerBloodSkill(false);
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIHUDSkillDaggerBlood::BindMessages()
{
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SET,
		this, &UGsUIHUDSkillDaggerBlood::OnChangeSkillSet));
	//_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT,
	//	this, &UGsUIHUDSkillDaggerBlood::OnChangeSkillSlot));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::PLAYER_STAT_UPDATE,
		this, &UGsUIHUDSkillDaggerBlood::OnUpdatePlayerStatList));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_ADD,
		this, &UGsUIHUDSkillDaggerBlood::OnAddAbnormality));
	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
		this, &UGsUIHUDSkillDaggerBlood::OnRemoveAbnormality));
	//_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_CHANGE_BUFFSKILL_SLOT,
	//	this, &UGsUIHUDSkillDaggerBlood::OnStartBuffSkill));	
}

void UGsUIHUDSkillDaggerBlood::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& msgHandle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(msgHandle);
	}
	_msgGameObjectHandleList.Empty();
}

void UGsUIHUDSkillDaggerBlood::OnChangeSkillSet(const IGsMessageParam* InParam) //스킬셋 바뀔때
{		
	bool useSensitiveConsumSkill = IsEnableSensitiveSkill();

	_daggerBloodSwitcher->SetVisibility(useSensitiveConsumSkill ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (useSensitiveConsumSkill)
	{
		SetUseDaggerBloodSkill(false); //스킬 셋 바뀔 때는 abnormality 무효
	}
}

void UGsUIHUDSkillDaggerBlood::OnChangeSkillSlot(const IGsMessageParam* InParam)
{
	bool useSensitiveConsumSkill = IsConsumableSkillSlot(StatType::SENSITIVE);
	_daggerBloodSwitcher->SetVisibility(useSensitiveConsumSkill ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if(const auto param = InParam == nullptr ? nullptr : InParam->Cast<const FGsGameObjectMessageParamSkillSlot>())
	{
		if (param->_skill &&
			GSkill()->GetCurrentWeaponType() == param->_weaponType)
		{
			if (false == IsStatConsumableActiveSkill(param->_skill))
			{
				SetUseDaggerBloodSkill(false); //스킬이 바뀌어 살기필요 스킬이 set 되었을 때
			}
		}
	}
}

void UGsUIHUDSkillDaggerBlood::OnUpdatePlayerStatList(const IGsMessageParam* InParam)
{
	if (false == _daggerBloodSwitcher->IsVisible())
	{
		return;
	}

	const FGsGameObjectMessageParamPlayerStat* param = InParam->Cast<const FGsGameObjectMessageParamPlayerStat>();
	if (nullptr == param)
	{
		return;
	}

	if (false == IsEnableSensitiveSkill())
	{
		return;
	}

	const bool sensitive = param->FindStatType(StatType::SENSITIVE);
	const bool max_sensitive = param->FindStatType(StatType::MAX_SENSITIVE);

	const auto sensitiveStat = param->StatMap.FindByPredicate([](TPair<StatType, int32>& elem)->bool { return StatType::SENSITIVE == elem.Key; });
	const auto maxSensitiveStat = param->StatMap.FindByPredicate([](TPair<StatType, int32>& elem)->bool { return StatType::MAX_SENSITIVE == elem.Key; });

	if (sensitive) // && (*sensitiveStat).Value > 0) //maxSensitiveStat && (*maxSensitiveStat).Value > 0)
	{
	/*	if (const FGsStatInfo* statInfo = GetSensitiveStatInfo())
		{
			int bloodCount = statInfo->GetStatValue();
			GSLOG(Log, TEXT("******OnUpdatePlayerStatList StatType::SENSITIVE [% d -> % d]"), _curBloodCount, bloodCount);
		}*/

		UpdateBloodDotInfo(false);
	}
}

void UGsUIHUDSkillDaggerBlood::OnAddAbnormality(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>();
	if (nullptr == param)
	{
		return;
	}

	AbnormalityInstanceId instanceId = static_cast<AbnormalityInstanceId>(param->_first);

	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler())
		{
			if (auto abnormality = abnormalityHandler->GetAbnormalityData(instanceId).Pin())
			{
				if (abnormality->IsEnableAbnormalityEffectType(AbnormalityEffectType::STAT_VENT))
				{
					if (!_runStatConsumSkill)
					{
						UpdateBloodDotInfo(true);
					}

					if (_runStatConsumSkill || DotState::UsePlay != _bloodProgress[0]->GetDaggerBloodStat())
					{
						SetUseDaggerBloodSkill(true);  //시간 갱신
					}
				}
			}
		}
	}
}

void UGsUIHUDSkillDaggerBlood::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler())
		{
			if (_runStatConsumSkill && !abnormalityHandler->GetLongerstAbnormalityData(AbnormalityEffectType::STAT_VENT).IsValid())
			{
				SetUseDaggerBloodSkill(false);
			}
		}
	}
}

void UGsUIHUDSkillDaggerBlood::UpdateBloodDotInfo(bool abnormalityAdded)
{
	const FGsStatInfo* statInfo = GetSensitiveStatInfo();
	if (nullptr == statInfo)
	{
		return;
	}

	int maxCount = statInfo->GetMaxStatValue();
	int bloodCount = statInfo->GetStatValue();

	TArray<DotState> dotUse = { DotState::None, DotState::None, DotState::None, DotState::None };
	
	if (abnormalityAdded) //무조건 실행한다.
	{
		dotUse.Empty();
		dotUse = { DotState::UsePlay, DotState::UsePlay, DotState::UsePlay, DotState::UsePlay };
	}
	else
	{
		if (bloodCount == maxCount)
		{
			// 꽉차있음
			dotUse.Empty();
			dotUse = { DotState::CompleteLoop, DotState::CompleteLoop, DotState::CompleteLoop, DotState::CompleteLoop };
		}
		else if (_curBloodCount != bloodCount)
		{
			// 살기 획득
			if (_curBloodCount < bloodCount) //get, disable
			{
				for (int i = 0; i < dotUse.Num(); i++)
				{
					if (i < _curBloodCount)  dotUse[i] = DotState::GetLoop;
					else if (i < bloodCount) dotUse[i] = DotState::Get;
					else break;
				}
			}
			else  //감소
			{
				for (int i = 0; i < dotUse.Num(); i++)
				{
					if (i < bloodCount)  dotUse[i] = DotState::GetLoop;
					else break;
				}
			}
			//else if (_curBloodCount == maxCount && bloodCount == 0)
			//{
			//	// 살기 사용
			//	dotUse.Empty();
			//	dotUse = { DotState::UsePlay, DotState::UsePlay, DotState::UsePlay, DotState::UsePlay };
			//}
		}
		else //루프
		{
			for (int i = 0; i < dotUse.Num(); i++)
			{
				if (i < bloodCount)  dotUse[i] = DotState::GetLoop;
				else break;
			}
		}
	}

	for (auto i = 0; i < _bloodProgress.Num(); i++)
	{
		_bloodProgress[i]->SetDaggerBloodStat(dotUse[i]);
	}

	//GSLOG(Log, TEXT("UpdateBloodDotInfo % d count[% d -> % d]"), abnormalityAdded, _curBloodCount, bloodCount);

	_curBloodCount = bloodCount;
}

const FGsStatInfo* UGsUIHUDSkillDaggerBlood::GetSensitiveStatInfo() const
{
	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsGameObjectDataCreature* creatureData = localPlayer->GetCastData<FGsGameObjectDataCreature>())
		{
			if (TSharedPtr<FGsStatBase> base = creatureData->GetStatBase())
			{
				return base->GetStatInfo(StatType::SENSITIVE);
			}
		}
	}

	return nullptr;
}

int32 UGsUIHUDSkillDaggerBlood::GetStatConsumableSkillSlot(StatType statType)
{
	if (UGsSkillManager* skillMgr = GSkill())
	{
		for (auto i = MAX_ACTIVE_SKILL_SLOT_ID + 1; i <= MAX_BUFF_SKILL_SLOT_ID; i++)
		{
			if (auto slotSkill = skillMgr->FindSkillSlot(i))
			{
				if (slotSkill->GetSkillRequireWeaponType() == skillMgr->GetCurrentWeaponType())
				{
					if (const FGsSchemaSkillCostConsumeInfo* consumeInfoData =
						slotSkill->GetCurrLevelCostConsumeInfoData(slotSkill->GetTotalLevel()))
					{
						if (consumeInfoData->statInfoList.FindByPredicate([statType](auto& stat) { return stat.type == statType;}))
						{
							return i;
						}
					}
				}
			}
		}
	}
	return -1;
}

bool UGsUIHUDSkillDaggerBlood::IsConsumableSkillSlot(StatType statType)
{
	return (MAX_ACTIVE_SKILL_SLOT_ID < GetStatConsumableSkillSlot(StatType::SENSITIVE));
}

bool UGsUIHUDSkillDaggerBlood::IsStatConsumableActiveSkill(const FGsSkill* skill, StatType statType)
{
	if (const FGsSchemaSkillCostConsumeInfo* consumeInfoData = skill == nullptr ? nullptr :
		skill->GetCurrLevelCostConsumeInfoData(skill->GetTotalLevel()))
	{
		return consumeInfoData->statInfoList.FindByPredicate([statType](auto& stat) { return stat.type == statType;}) != nullptr;
	}

	return false;
}

bool UGsUIHUDSkillDaggerBlood::IsEnableSensitiveSkill()
{
	return GSkill() && GSkill()->GetCurrentWeaponType() == CreatureWeaponType::DAGGER;
}

void UGsUIHUDSkillDaggerBlood::SetUseDaggerBloodSkill(bool set)
{	
	_daggerBloodSwitcher->SetActiveWidgetIndex(set ? 0 : 1);
	if (set)
	{
		if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler())
			{
				// 살기 폭발에 의해 걸린 버프 체크
				if (auto abnormality = abnormalityHandler->GetLongerstAbnormalityData(AbnormalityEffectType::STAT_VENT).Pin())
				{
					if (abnormality->GetCurrRemainTime() > 0.f)
					{
						_coolTime = abnormality->GetCurrRemainTime();
						_endTime = FApp::GetCurrentTime() + _coolTime;

						if(!_runStatConsumSkill) _daggerBloodSkillProgress->SetPercent(1.f);
					}
					
				}
			}
			_runStatConsumSkill = true; //열린 switch를 닫도록
		}		
	}
	else
	{
		_runStatConsumSkill = false;
		_coolTime = 0.f;
		_endTime = 0.f;
	}

	UpdateBloodDotInfo();
}
