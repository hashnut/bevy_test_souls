#include "GsUIHUDPlayableEventQuest.h"

#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "DataSchema/PlayableEvent/GsSchemaPlayableEventQuest.h"

#include "PlayableEvent/GsPlayableEventDefine.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeLobby.h"

#include "PlayableEvent/EGsPlayableEventQuestGoalType.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UI/UILib/Base/GsUIVFX.h"

void UGsUIHUDPlayableEventQuest::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _questCompleteEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_questCompleteEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIHUDPlayableEventQuest::FinishEffect);
	}
	
}
void UGsUIHUDPlayableEventQuest::NativeDestruct()
{
	if (true == _questCompleteEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_questCompleteEffect->OnVFXAnimationFinished.Unbind();
	}

	Super::NativeDestruct();
}
void UGsUIHUDPlayableEventQuest::InvalidateAllInternal()
{
	
	const FGsSchemaPlayableEventQuest* nowQuestTbl =
		LSLobbyPlayableEvent()->GetCurrentQuestTbl();
	if (nowQuestTbl == nullptr)
	{
		return;
	}
	// 1. quest id changed: complete effect show
	if (nowQuestTbl->id != _oldQuestId)
	{
		_oldQuestId = nowQuestTbl->id;
		StartAnimation();
	}
	// 2. quest id same: direct text setting
	else
	{
		SetText();
	}
}

void UGsUIHUDPlayableEventQuest::StartAnimation()
{
	_questCompleteEffect->ShowVFX();
}
void UGsUIHUDPlayableEventQuest::SetText()
{
	// 현재 목표 텍스트 가져온다
	const FGsSchemaPlayableEventQuest* nowQuestTbl =
		LSLobbyPlayableEvent()->GetCurrentQuestTbl();
	if (nowQuestTbl == nullptr)
	{
		return;
	}
	_textGoalContents = nowQuestTbl->goalText;

	// kill 이면 현재 /  목표 카운트
	if (nowQuestTbl->goalType == EGsPlayableEventQuestGoalType::Kill)
	{
		int currentCount =
			LSLobbyPlayableEvent()->GetCurrentProgressKillCount();
		FString formatString = FString::Printf(TEXT("%i / %i"), currentCount, nowQuestTbl->killCount);
		_textProgressCount = FText::FromString(formatString);
	}
	else
	{
		_textProgressCount = FText::FromString(TEXT(""));
	}
}

void UGsUIHUDPlayableEventQuest::FinishEffect()
{
	SetText();
	HideEffect();
}

void UGsUIHUDPlayableEventQuest::HideEffect()
{
	_questCompleteEffect->HideVFX();
}