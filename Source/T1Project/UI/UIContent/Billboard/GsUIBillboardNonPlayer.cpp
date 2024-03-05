// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardNonPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Boss/GsFieldBossHandler.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Util/GsTimeSyncUtil.h"
#include "UTIL/GsUIUtil.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"

#include "Option/GsGameUserSettings.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "T1Project.h"


void UGsUIBillboardNonPlayer::NativeConstruct()
{
	Super::NativeConstruct();	
	
	_panelTextScanOrder->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBillboardNonPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// 피격 후 일정시간만 보여준다
	if (0 < _showRemainTime)
	{
		_showRemainTime -= InDeltaTime;

		if (0 >= _showRemainTime)
		{
			_showRemainTime = 0.0f;
			
			OnEndShowTime();
		}
	}

	if (0 < _hideRemainTime)
	{
		_hideRemainTime -= InDeltaTime;
		
		if (0 >= _hideRemainTime)
		{
			_hideRemainTime = 0.0f;

			OnEndHideTime();
		}
		else
		{
			SetBillboardOpacity(FMath::Clamp(_hideRemainTime / _hideTime, 0.0f, 1.0f));
		}
	}

	// 죽은 캐릭터의 HP바 가림
	if (0.0f >= _hpBarValue)
	{
		ResetTime();
	}

	if (0.0f < _fdelayInvalidateTime)
	{
		_fdelayInvalidateTime -= InDeltaTime;

		if (0 >= _fdelayInvalidateTime)
		{
			_fdelayInvalidateTime = 0.f;
			UpdateHpAndShieldBar();
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBillboardNonPlayer::Reset()
{
	Super::Reset();

	_target = nullptr;
	_bWasTargetRendered = false;
	_currHp = 0;
	_maxHp = 0;
	_currShield = 0;
	_fdelayInvalidateTime = 0.f;
	_isScanMember = false;

	ResetTime(true);

	_showTime = 2.5f;
}

void UGsUIBillboardNonPlayer::OnEndShowTime()
{
	// 보일때만 시작함
	if (0 < GetRenderOpacity())
	{
		if (HasHideAnimation())
		{
			// 갱신이 잦으므로 따로 타이머를 돌리지 않는다
			_hideRemainTime = _hideTime;
			return;
		}
	}

	ResetTime();
}

void UGsUIBillboardNonPlayer::OnEndHideTime()
{
	ResetTime();
}

void UGsUIBillboardNonPlayer::ResetTime(bool In_isForced)
{
	_showRemainTime = 0.0f;
	_hideRemainTime = 0.0f;

	SetBillboardOpacity(0.0f, In_isForced);
}

void UGsUIBillboardNonPlayer::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	if (false == _target.IsValid())
	{
		return;
	}

	bool bIsMonster = IsMonsterLike();
	if (UGsGameObjectNonPlayerBase* nonPlayer = _target->CastGameObject<UGsGameObjectNonPlayerBase>())
	{
		if (const FGsSchemaNpcData* npcData = nonPlayer->GetNpcData())
		{
			SetNameText(npcData->nameText);
		}
	}
	
	_panelBarRoot->SetVisibility((bIsMonster) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	// 몬스터말고 npc만 옵션 체크 처리
	if (IsMonsterLike() == false)
	{
		UpdateShowNameOption();
		//UpdateServer();
	}

	UpdateShowHPBar();
}

void UGsUIBillboardNonPlayer::UpdateStat()
{
	if (false == _target.IsValid())
	{
		return;
	}

	if (false == IsMonsterLike())
	{
		return;
	}

	if (UGsGameObjectCreature* creature = _target->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsStatInfo* hpStat = creature->GetCreatureStatInfoByType(StatType::HP))
		{
			SetHp(hpStat->GetStatValue(), hpStat->GetMaxStatValue(), false);
		}

		if (const FGsStatInfo* shieldStat = creature->GetCreatureStatInfoByType(StatType::HP_SHIELD))
		{
			SetShield(shieldStat->GetStatValue(), false);
		}
		else
		{
			SetShield(0, false);
		}

		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardNonPlayer::SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		if (_fdelayInvalidateTime != 0.0f)
		{
			UpdateHpAndShieldBar();
		}
		_fdelayInvalidateTime = DELAY_HP_DISPLAY_TIME;
	}
}

void UGsUIBillboardNonPlayer::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (bInUpdateBar)
	{
		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardNonPlayer::UpdateHpAndShieldBar()
{
	SetHPShieldProgressBar(_barShieldFront, _barShieldBack, _currHp, _maxHp, _currShield);
}

void UGsUIBillboardNonPlayer::OnChangeRenderState(bool bIsRendered)
{
	_bWasTargetRendered = bIsRendered;
	
	if (IsMonsterLike())
	{
		// 몬스터는 피격시에만 네임플레이트를 켜주므로 컬링여부에 따라 끄는 코드만 존재
		if (false == bIsRendered)
		{
			ResetTime();
		}
		else if (IsRenderShowStateUpdateMonster())
		{
			StartShow();
		}
	}
	else
	{
		// 주의: SetTarget 전엔 모두 false 여서 정보가 보일 수 있음
		if (_target.IsValid())
		{
			if (bIsRendered)
			{
				StartShow();
			}
			else
			{
				ResetTime();
			}
		}	
	}
}

void UGsUIBillboardNonPlayer::OnHit(class UGsGameObjectBase* InAttacker)
{
	Super::OnHit(InAttacker);
	
	// 로컬 플레이어가 때렸을때만 보여준다
	if (InAttacker &&
		InAttacker->IsA<UGsGameObjectLocalPlayer>())
	{
		StartShow();
	}
}

void UGsUIBillboardNonPlayer::OnTargeting(bool InIsTarget, UGsGameObjectBase* InOwner, bool InIsFirstTarget, bool InIsEnemy)
{
	Super::OnTargeting(InIsTarget, InOwner, InIsFirstTarget, InIsEnemy);

	bool activeHPBarRoot = IsShowHPBar();
	if (activeHPBarRoot == true)
	{
		// 이미 피가 닳아있는 경우를 위함
		UpdateStat();
	}

	if (InIsTarget)
	{
		StartShow();
	}

	UpdateBillboardOpacity();
}

void UGsUIBillboardNonPlayer::OnChangedStat(const FGsStatInfo& InStatInfo)
{
	Super::OnChangedStat(InStatInfo);

	bool activeHPBarRoot = IsShowHPBar();
	if (activeHPBarRoot == false)
	{
		return;
	}	

	if (StatType::HP == InStatInfo.GetStatType())
	{
		SetHp(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue(), true);
	}
	else if (StatType::HP_SHIELD == InStatInfo.GetStatType())
	{
		SetShield(InStatInfo.GetStatValue(), true);
	}
}

void UGsUIBillboardNonPlayer::StartShow()
{
	if (_bWasTargetRendered)
	{
		if (IsMonsterLike())
		{
			if (false == IsAlwaysShowHPBar())
			{
				_showRemainTime = _showTime;
				_hideRemainTime = 0.0f;
			}
		}
		else
		{
			// 몬스터가 아닐 경우, 항상 보여주기 위해 타이머를 돌리지 않음
			_showRemainTime = 0.0f;
			_hideRemainTime = 0.0f;
		}

		SetBillboardOpacity(1.0f);
	}
}

bool UGsUIBillboardNonPlayer::IsMonsterLike() const
{
	if (_target.IsValid())
	{
		if (UGsGameObjectNonPlayerBase* nonPlayer = _target->CastGameObject<UGsGameObjectNonPlayerBase>())
		{
			if (const FGsSchemaNpcData* npcData = nonPlayer->GetNpcData())
			{
				bool isMonsterLike = false;
				switch (npcData->iffFactionType)
				{
				case IffFactionType::MONSTER:
				case IffFactionType::MONSTER_SPACECRACK:
				case IffFactionType::NPC_GUARD:
				case IffFactionType::NPC_INVASIONGUARDIAN:
				case IffFactionType::NPC_WAGON:
				case IffFactionType::NPC_WAGONGUARD:
				case IffFactionType::NPC_WAGONPEDDLER:
				case IffFactionType::NPC_WAGONTHIEF:
					isMonsterLike = true;
					break;

				}

				return isMonsterLike;
			}
		}
	}

	return false;
}
// 스캔 순서 텍스트 입력
void UGsUIBillboardNonPlayer::SetScanOrderText(const FText& In_name)
{
	_textScanOrder->SetText(In_name);
}
// 스캔 멤버인지 변경
void UGsUIBillboardNonPlayer::SetScanMember(bool In_isMember)
{
	// 스캔 멤버의 상태 변경
	_isScanMember = In_isMember;

	ESlateVisibility visibility = (In_isMember == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_panelTextScanOrder->SetVisibility(visibility);
	
	UpdateBillboardOpacity();
}

// 불투명도 조절
void UGsUIBillboardNonPlayer::SetBillboardOpacity(float In_opacity, bool In_isForced)
{
	_billboardOpacity = In_opacity;

	UpdateBillboardOpacity(In_isForced);
}

// 불투명도 갱신
void UGsUIBillboardNonPlayer::UpdateBillboardOpacity(bool In_isForced)
{
	if (false == In_isForced &&
		(IsScanNumber() || IsTargeting()))
	{
		// 스캔 멤버나 타게팅 대상이면 hp 패널투명도만 조절		
		// 전체 투명도 복구
		SetRenderOpacity(1.0f);
		_panelBarRoot->SetRenderOpacity(_billboardOpacity);
	}	
	else
	{
		// 스캔 멤버가 아니면 전체 투명도 조절
		// hp 패널은 복구
		_panelBarRoot->SetRenderOpacity(1.0f);
		SetRenderOpacity(_billboardOpacity);
	}
}

// 이름 출력 옵션 갱신
void UGsUIBillboardNonPlayer::UpdateShowNameOption()
{
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return;
	}
	// 옵션 값 확인
	bool isShowDropItemName =
		(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::NPC_NAME);

	ESlateVisibility visibility =
		(isShowDropItemName == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	
	_textBlockName->SetVisibility(visibility);
}

//void UGsUIBillboardNonPlayer::UpdateServer()
//{
//	if(_target.IsValid())
//	{
//		if (UGsGameObjectNonPlayerBase* nonPlayer = _target->CastGameObject<UGsGameObjectNonPlayerBase>())
//		{
//			if (const FGsSchemaNpcData* npcData = nonPlayer->GetNpcData())
//			{
//				auto myHomeWorldId	= GGameData()->GetUserData()->mHomeWorldId;
//				auto nowMapWorldId	= GGameData()->GetLoadWorldId();
//				auto isInterServerMap = GLevel()->IsInterServerMap();
//
//				if (SharedFunc::IsNpcNameRed(isInterServerMap, npcData->npcFunctionType, nowMapWorldId, myHomeWorldId))
//				{
//					//auto OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
//					//SetNameColor(OutColor);
//				}
//			}
//		}
//	}
//}

// hp bar cheat show/hide
void UGsUIBillboardNonPlayer::UpdateShowHPBar()
{
	bool activeHPBarRoot = IsShowHPBar();
	if (activeHPBarRoot == true)
	{
		UpdateStat();
	}
	ShowHPBar(activeHPBarRoot);
}
void UGsUIBillboardNonPlayer::ShowHPBar(bool In_isShow)
{
	ESlateVisibility hpBarVisibility =
		(In_isShow == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_panelBarRoot->SetVisibility(hpBarVisibility);
}
bool UGsUIBillboardNonPlayer::IsShowHPBar()
{
	// 1. cheat option on check
	bool isShowHPBarOption = GOption()->GetIsShowHPbar();

	// 2. not monster-> not show
	bool bIsMonster = false;

	// 3. check world boss(always show)
	//bool isWorldMonster = false;
	// 3. check show monster
	bool isShowMonster = false;
	// 4. check player team treasure monster(always show)
	bool isPlayerTeamTreasureMonster = false;

	if (_target.IsValid() == true)
	{
		if (UGsGameObjectNonPlayerBase* nonPlayer = _target->CastGameObject<UGsGameObjectNonPlayerBase>())
		{
			isPlayerTeamTreasureMonster =  nonPlayer->IsPlayerTeamTreasureMonster();

			if (const FGsSchemaNpcData* npcData = nonPlayer->GetNpcData())
			{
				bIsMonster = IsMonsterLike();
				if (bIsMonster == true)
				{
					// https://jira.com2us.com/jira/browse/CHR-17595
					// NPC가 Mapdata.maptype = Instance Single, Single Filed 내부에 있는 경우
					// NPCdata.grade = NAMED, BOSS, RAID인 경우 HUD 상 HP를 표기
					// NPCdata.grade = NORMAL, ELITE인 경우 HUD 상 HP 미표기

					if (npcData->npcFunctionType == NpcFunctionType::FIELD_BOSS)
					{
						isShowMonster = false;
// 						int32 currentMapId = GLevel()->GetCurrentLevelId();
// 						const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::GetFieldBossData(currentMapId, npcData->id);
// 						if (bossData)
// 						{
// 							isWorldMonster = (bossData->bossType == BossType::COOPERATION ||
// 								bossData->bossType == BossType::COOPERATION_MUTANT) ? true : false;
// 						}
					}
					else
					{
						isShowMonster = UGsUIUtil::IsShowMonsterHP(npcData->grade, npcData->npcFunctionType);
					}
				}
			}
		}
	}

	bool isShowHPBar = false;
	// world monster -> check show monster always show
	if (
		//isWorldMonster == true || 
		isShowMonster == true ||
		isPlayerTeamTreasureMonster == true)
	{
		isShowHPBar = true;
	}
	// not monster(other npc) always not show
	else if(bIsMonster == false)
	{
		isShowHPBar = false;
	}
	// cheat show optoin
	else
	{
		isShowHPBar = isShowHPBarOption;
	}
	return isShowHPBar;
}