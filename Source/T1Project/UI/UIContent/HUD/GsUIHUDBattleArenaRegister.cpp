#include "GsUIHUDBattleArenaRegister.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Helper/GsTimeStringHelper.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UGsUIHUDBattleArenaRegister::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBattleArena->OnClicked.AddDynamic(this, &UGsUIHUDBattleArenaRegister::OnClicked);
}

void UGsUIHUDBattleArenaRegister::NativeConstruct()
{
	Super::NativeConstruct();

	ClearTimer();

	_vfxActiveEffect->HideVFX();
}

void UGsUIHUDBattleArenaRegister::NativeDestruct()
{
	ClearTimer();

	_vfxActiveEffect->HideVFX();

	Super::NativeDestruct();
}

void UGsUIHUDBattleArenaRegister::OnClicked()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		battleArenaHandler->TryToRegisterOrCancel();
	}
}

void UGsUIHUDBattleArenaRegister::OnTimer()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		_textRemainingTime->SetText(battleArenaHandler->TextStateRemainingTime());
	}
}

void UGsUIHUDBattleArenaRegister::ClearTimer()
{
	UWorld* world = GetWorld();
	if (world && _timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsUIHUDBattleArenaRegister::SetData(EBattleArenaGuildWarState InState, uint64 InEndTime)
{
	if (MapType::GUILD_WAR == GLevel()->GetCurrentMapType() || EBattleArenaGuildWarState::ERegisterPeriod != InState && EBattleArenaGuildWarState::ERegisterCompleted != InState)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);	
		ClearTimer();
		return;
	}
	
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		GSLOG(Error, TEXT("nullptr == hud"));
		return;
	}

	if (FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler())
	{
		_textRemainingTime->SetText(battleArenaHandler->TextStateRemainingTime());

		if (false == _timerHandle.IsValid())
		{
			if (UWorld* world = GetWorld())
			{
				// 2023/12/8 PKT - Timer를 사용하면 Text가 잠깐 튀는 현상이 발생. 미리 한번 불러 준다.
				OnTimer();
				world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIHUDBattleArenaRegister::OnTimer, 0.5f, true);
			}			
		}				
	}

	if (EBattleArenaGuildWarState::ERegisterPeriod == InState)
	{
		if (false == _vfxActiveEffect->IsPlayingAnimation())
		{
			_vfxActiveEffect->ShowVFX();
		}
	}
	else
	{
		_vfxActiveEffect->HideVFX();
	}	
}