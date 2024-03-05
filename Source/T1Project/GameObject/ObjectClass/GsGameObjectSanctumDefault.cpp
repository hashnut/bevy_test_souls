// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectSanctumDefault.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "ActorEx/GsCompleteCamp.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/Sanctum/GsSchemaSanctumConfig.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "T1Project.h"


void UGsGameObjectSanctumDefault::InitializeActor(AActor* Owner)
{
	Super::InitializeActor(Owner);

	// 액터 설정
	AGsCompleteCamp* actorCamp = Cast<AGsCompleteCamp>(GetActor());
	if (nullptr == actorCamp)
	{
		return;
	}

	actorCamp->SetIsSanctum(true);

	actorCamp->SetCampEmblemIcon(INVALID_GUILD_DB_ID);
	actorCamp->SetSpawnIndex(SPAWN_REASON::NORMAL == _spawnReasonType ? 0 : 1);
	actorCamp->ChangeAnimState(EGsStateCampComplete::Idle_CompleteCamp);
	actorCamp->SetHpBarAndNameWidgetVisibility(true);
	actorCamp->ShowHpBar(false);
	actorCamp->OnIdleEffect();

	const FGsSchemaCreatureCommonInfo* creatureCommonInfo = nullptr;
	if (_npcData)
	{
		creatureCommonInfo = _npcData->commonInfoId.GetRow();
	}	

	SetCapsuleAndTracePos(actorCamp, _spawnPos, false, creatureCommonInfo);

	// 성소 세팅
	if (const FGsSchemaSanctumConfig* configData = FGsSanctumManager::GetTableSanctumConfig())
	{
		actorCamp->SetMaterial(configData->sanctumMaterial);
	}	

	// 주의: _sanctumId가 아직 없을 수도 있다 OnSanctumSataus 에서 받으면 다시 갱신된다
	UpdateSanctumName(_sanctumId);

	// 인터렉션 버튼 세팅. 성소 점령전 중이면 활성
	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumDefault::SetInteractionActive(bool In_isActive, int In_index)
{
	AGsCompleteCamp* campActor = GetCampActor();
	if (nullptr == campActor)
	{
		return;
	}

	if (false == In_isActive)
	{
		campActor->SetInteractionClickCallback(nullptr);
		campActor->SetUseInteractionButton(false);

		campActor->SetMaterialColor(false);

		return;
	}

	TWeakObjectPtr<UGsGameObjectSanctumDefault> weakPtr(this);
	campActor->SetInteractionClickCallback([weakPtr](bool In_isByWidget)
		{
			if (weakPtr.IsValid())
			{
				weakPtr->OnClickInteract(In_isByWidget);
			}
		});

	campActor->SetUseInteractionButton(true);

	if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
	{
		interactionButton->SetData(this, NpcFunctionType::SANCTUM, In_index);
	}

	campActor->SetMaterialColor(true);
}

void UGsGameObjectSanctumDefault::OnClickInteract(bool In_isByWidget)
{
	if (GetIsActiveInteraction())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);
	}
}

void UGsGameObjectSanctumDefault::OnSanctumSataus(SanctumAreaId InSanctumId, bool bInCanInteract)
{
	// 주의: 액터 스폰 전에 들어올 수 있다
	_sanctumId = InSanctumId;
	_isInteractionActive = bInCanInteract;

	UpdateSanctumName(_sanctumId);

	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumDefault::UpdateSanctumName(SanctumAreaId InSanctumId)
{
	AGsCampBase* actorCamp = GetCampActor();
	if (nullptr == actorCamp)
	{
		_strFormattedName = TEXT("");
		return;
	}

	// 성소 이름만 표시할 것임
	actorCamp->SetShowNamePanel(false);

	if (const FGsSchemaSanctumData* sanctumData = FGsSanctumManager::GetTableSanctumData(InSanctumId))
	{
		_strFormattedName = sanctumData->nameText.ToString();
	}
	else
	{
		_strFormattedName = TEXT("");
	}

	actorCamp->SetSanctumName(FText::FromString(_strFormattedName));
}

void UGsGameObjectSanctumDefault::UpdateInteractionState()
{
	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumDefault::OnBattleEnd()
{
	_isInteractionActive = false;
}

AGsCompleteCamp* UGsGameObjectSanctumDefault::GetCampActor()
{
	if (AActor* actor = GetActor())
	{
		if (actor->IsValidLowLevel())
		{
			return Cast<AGsCompleteCamp>(actor);
		}
	}

	return nullptr;
}
