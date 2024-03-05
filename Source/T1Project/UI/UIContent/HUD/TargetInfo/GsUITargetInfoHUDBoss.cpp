// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITargetInfoHUDBoss.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "Guild/GsGuildPlayerData.h"


void UGsUITargetInfoHUDBoss::SetTarget(UGsGameObjectCreature* InTarget, const FString& InName)
{
	Super::SetTarget(InTarget, InName);

	_targetGameId = InTarget->GetGameId();
}

void UGsUITargetInfoHUDBoss::ShowAggroTarget(bool InIsBoss)
{
	_isShowAggroTarget = InIsBoss;

	InvalidateAggroTarget();

	if (_panelAggro)
	{
		_panelAggro->SetVisibility(_isExistAggroTarget ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsUITargetInfoHUDBoss::InvalidateAggroTarget()
{
	_isExistAggroTarget = false;
	if (false == _isShowAggroTarget)
		return;

	FGsBossHandler* handler = FGsBossHandler::GetInstance();
	if (nullptr == handler)
		return;

	int64 aggroTargetGameId = handler->GetFieldBossHandler()->GetAggroTarget(_targetGameId);
	UGsGameObjectBase* playerBase = GSGameObject()->FindObject(EGsGameObjectType::Player, aggroTargetGameId);
	if (nullptr == playerBase)
		return;

	const FGsGameObjectDataPlayer* playerData = playerBase->GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == playerData)
		return;

	_isExistAggroTarget = true;
	_textBlockUserNameAggro->SetText(FText::FromString(playerData->GetName()));

	SetAggroTargetGuild(playerData);
}

void UGsUITargetInfoHUDBoss::SetAggroTargetGuild(const FGsGameObjectDataPlayer* InPlayerData)
{
	ESlateVisibility guildVisibility = ESlateVisibility::Collapsed;
	if (InPlayerData)
	{
		if (const FGsGuildPlayerData* guildData = InPlayerData->GetGuildData())
		{
			if (INVALID_GUILD_DB_ID != guildData->_guildDBId)
			{
				guildVisibility = ESlateVisibility::SelfHitTestInvisible;

				_iconGuildAggro->SetEmblemId(guildData->_emblemId, false);

				FGsGuildPlayerData* guildDataNoConst = const_cast<FGsGuildPlayerData*>(guildData);
				_textBlockGuildNameAggro->SetText(FText::FromString(guildDataNoConst->GetGuildName()));
			}
		}
	}

	_panelGuildRootAggro->SetVisibility(guildVisibility);
}
