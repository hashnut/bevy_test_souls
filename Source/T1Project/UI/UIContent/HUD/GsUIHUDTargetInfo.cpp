// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDTargetInfo.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/NamedSlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/HUD/TargetInfo/GsUITargetInfoHUDBase.h"
#include "UI/UIContent/HUD/TargetInfo/GsUITooltipTargetInfo.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "UTIL/GsUIUtil.h"
#include "T1Project.h"


void UGsUIHUDTargetInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_normalInfo->OnClickTooltipButton.BindUObject(this, &UGsUIHUDTargetInfo::OnClickTooltip);
	_bossInfo->OnClickTooltipButton.BindUObject(this, &UGsUIHUDTargetInfo::OnClickTooltip);
}

void UGsUIHUDTargetInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetData(nullptr);

	_bossMsgHandlerList.Empty();
	_bossMsgHandlerList.Emplace(GMessage()->GetBoss().AddUObject(MessageBoss::CHANGE_AGGRO,
		this, &UGsUIHUDTargetInfo::OnChangeAggroTarget));
	_bossMsgHandlerList.Emplace(GMessage()->GetBoss().AddUObject(MessageBoss::HIDE_AGRRO,
		this, &UGsUIHUDTargetInfo::OnHideAggroTarget));
	_bossMsgHandlerList.Emplace(GMessage()->GetBoss().AddUObject(MessageBoss::HIDE_BOSS_BILLBOARD,
		this, &UGsUIHUDTargetInfo::OnHideAggroTarget));
}

void UGsUIHUDTargetInfo::NativeDestruct()
{
	_target = nullptr;

	Super::NativeDestruct();
}

void UGsUIHUDTargetInfo::SetData(const TargetHUD* InData)
{
	// 감추기
	if (nullptr == InData ||
		false == InData->_target.IsValid())
	{
		//_panelRoot->SetVisibility(ESlateVisibility::Hidden);
		_target = nullptr;
		return;
	}

	_target = InData->_target;
	//_panelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// HP바 타입 선택
	bool bIsBoss = false;
	switch (InData->_rankType)
	{
	case CreatureRankType::BOSS:
	case CreatureRankType::RAID:
		bIsBoss = true;
		break;
	}

	SwitchHPBar((bIsBoss) ? ETargetType::Boss : ETargetType::Normal);

	bool isShowHPBar = IsShowHPBar();
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->Reset();
		currInfo->SetTarget(InData->_target.Get(), InData->_name);
		currInfo->SetRemainLifeTime(InData->_lifeTime);

		currInfo->ShowHPBar(isShowHPBar);
		currInfo->ShowAggroTarget(bIsBoss);
	}

	// 툴팁 감추기
	SetUITooltipVisibility(false);

	if (isShowHPBar == true)
	{
		// 실드 리셋
		SetShield(0, false);

		UpdateStat();
	}
}

void UGsUIHUDTargetInfo::SetStat(const FGsStatInfo& InStatInfo, bool bInUpdateBar)
{
	if (StatType::HP == InStatInfo.GetStatType())
	{
		SetHP(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue(), bInUpdateBar);
	}
	else if(StatType::HP_SHIELD == InStatInfo.GetStatType())
	{ 
		SetShield(InStatInfo.GetStatValue(), bInUpdateBar);
	}
}

void UGsUIHUDTargetInfo::UpdateStat()
{
	if (false == _target.IsValid())
	{
		return;
	}

	if (const FGsStatInfo* statInfo = _target->GetCreatureStatInfoByType(StatType::HP))
	{
		SetHP(statInfo->GetStatValue(), statInfo->GetMaxStatValue(), false);
	}

	if (const FGsStatInfo* statInfo = _target->GetCreatureStatInfoByType(StatType::HP_SHIELD))
	{
		SetShield(statInfo->GetStatValue(), false);
	}

	UpdateHpShieldBar();
}

void UGsUIHUDTargetInfo::UpdateHpShieldBar()
{
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->UpdateHpShieldBar();
	}
}

void UGsUIHUDTargetInfo::SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->SetHP(InCurrent, InMax, bInUpdateBar);
	}
}

void UGsUIHUDTargetInfo::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->SetShield(InCurrent, bInUpdateBar);
	}
}

void UGsUIHUDTargetInfo::SwitchHPBar(ETargetType InType)
{
	_switchTarget->SetActiveWidgetIndex(static_cast<int32>(InType));
}

void UGsUIHUDTargetInfo::OnClickTooltip(UNamedSlot* InSlot)
{
	// 열려있으면 닫기
	if (_tooltipUI->IsOpen())
	{
		return;
	}

	// 툴팁 슬롯 위치로 조정
	InSlot->AddChild(_tooltipUI);

	SetUITooltipVisibility(true);
}

void UGsUIHUDTargetInfo::SetUITooltipVisibility(bool bInIsVisible)
{
	if (bInIsVisible)
	{
		if (_target.IsValid())
		{
			if (EGsGameObjectType::NonPlayer == _target->GetObjectType())
			{
				if (UGsGameObjectNonPlayer* npc = Cast<UGsGameObjectNonPlayer>(_target.Get()))
				{
					if (const FGsSchemaNpcData* npcData = npc->GetNpcData())
					{
						_tooltipUI->SetData(npcData);
						_tooltipUI->Open();

						return;
					}
				}
			}
		}
	}

	// 실패 시 bVisible 값이 true여도 감춤
	_tooltipUI->Close();
}

UGsUITargetInfoHUDBase* UGsUIHUDTargetInfo::GetCurrInfo() const
{
	int32 index = _switchTarget->GetActiveWidgetIndex();

	return (static_cast<int32>(ETargetType::Boss) == index) ? _bossInfo : _normalInfo;
}

bool UGsUIHUDTargetInfo::IsShowHPBar()
{
	// 1. cheat option on check
	bool isShowHPBarOption = GOption()->GetIsShowHPbar();
	if (false == _target.IsValid())
	{
		return isShowHPBarOption;
	}
	EGsGameObjectType objectType = _target->GetObjectType();

	bool isParty = false;
	bool isNpc = false;
	//bool isWorldBoss = false;
	bool isShowMonster = false;
	bool isMyGuildCamp = false;


	// 2. remote check(party always show)
	if (EGsGameObjectType::RemotePlayer == objectType)
	{
		int64 gameId = _target->GetGameId();
		if (FGsPartyManager* partyMgr = GSParty())
		{
			isParty = partyMgr->GetIsInParty() &&
				partyMgr->IsInPartyGameId(gameId);
		}
	}
	// 3. npc check(world boss always show, npc always not show)
	else if (EGsGameObjectType::NonPlayer == objectType)
	{
		if (UGsGameObjectNonPlayer* nonplayer = Cast<UGsGameObjectNonPlayer>(_target))
		{
			if (const FGsSchemaNpcData* npcTable = nonplayer->GetNpcData())
			{
				if (npcTable->iffFactionType != IffFactionType::MONSTER)
				{
					isNpc = true;
				}
				else
				{
					// https://jira.com2us.com/jira/browse/CHR-17595
					// NPC가 Mapdata.maptype = Instance Single, Single Filed 내부에 있는 경우
					// NPCdata.grade = NAMED, BOSS, RAID인 경우 HUD 상 HP를 표기
					// NPCdata.grade = NORMAL, ELITE인 경우 HUD 상 HP 미표기

					//if (npcTable->npcFunctionType == NpcFunctionType::FIELD_BOSS)
					//{
					//	int32 currentMapId = GLevel()->GetCurrentLevelId();
					//	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::GetFieldBossData(currentMapId, npcTable->id);
					//	if (bossData)
					//	{
					//		isWorldBoss = (bossData->bossType == BossType::COOPERATION ||
					//			bossData->bossType == BossType::COOPERATION_MUTANT) ? true : false;
					//	}
					//}
					isShowMonster = UGsUIUtil::IsShowMonsterHP(npcTable->grade, npcTable->npcFunctionType);
				}
			}
		}
	}
	// 4. camp check(our guild camp always show)
	else if (EGsGameObjectType::Camp == objectType)
	{
		if (UGsGameObjectCamp* camp = Cast<UGsGameObjectCamp>(_target))
		{
			uint64 campGuildDBId = camp->GetOwnerGuildDbid();
			uint64 localGuildDBId = GGuild()->GetGuildDBId();
			isMyGuildCamp = (campGuildDBId == localGuildDBId) ? true : false;
		}
	}
	else if (EGsGameObjectType::Sanctum == objectType)
	{
		if (UGsGameObjectSanctumBase* sanctumObj = Cast<UGsGameObjectSanctumBase>(_target))
		{
			isMyGuildCamp = false;

			GuildDBId myGuildDBId = GGuild()->GetGuildDBId();
			if (INVALID_GUILD_DB_ID != myGuildDBId)
			{
				GuildDBId sanctumGuildDBId = sanctumObj->GetGuildDBId();
				if (INVALID_GUILD_DB_ID != sanctumGuildDBId)
				{
					isMyGuildCamp = (myGuildDBId == sanctumGuildDBId) ? true : false;
				}
			}
		}
	}
	// 1. party, world boss -> check show monster , our guild camp -> always on
	// 2. npc -> always off
	// 3. cheat option
	bool isShowHPBar = (isParty == true || 
		//isWorldBoss == true || 
		isShowMonster == true ||
		isMyGuildCamp == true) ? true :
		(isNpc == true) ? false : isShowHPBarOption;

	return isShowHPBar;
}

void UGsUIHUDTargetInfo::UpdateShowHPBar()
{
	bool isShowHPBar = IsShowHPBar();
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->Reset();
		currInfo->ShowHPBar(isShowHPBar);
	}

	if (isShowHPBar == true)
	{
		// 실드 리셋
		SetShield(0, false);
		UpdateStat();
	}
}

void UGsUIHUDTargetInfo::UpdatePartyMember(const FGsUIMapPartyMemberParam* In_param)
{
	if (false == _target.IsValid())
	{
		return;
	}

	int64 gameId = _target->GetGameId();
	if (In_param->_gameId.Contains(gameId) == true)
	{
		UpdateShowHPBar();
	}
}

void UGsUIHUDTargetInfo::OnChangeAggroTarget(const IGsMessageParam* InParam)
{
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->ShowAggroTarget(true);
	}
}

void UGsUIHUDTargetInfo::OnHideAggroTarget(const IGsMessageParam* InParam)
{
	if (UGsUITargetInfoHUDBase* currInfo = GetCurrInfo())
	{
		currInfo->ShowAggroTarget(false);
	}
}
