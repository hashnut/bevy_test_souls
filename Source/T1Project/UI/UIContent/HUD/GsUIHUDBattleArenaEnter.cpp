#include "GsUIHUDBattleArenaEnter.h"
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
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"



void UGsUIHUDBattleArenaEnter::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBattleArena->OnClicked.AddDynamic(this, &UGsUIHUDBattleArenaEnter::OnClicked);
}

void UGsUIHUDBattleArenaEnter::NativeConstruct()
{
	Super::NativeConstruct();

	ClearTimer();

	_vfxActiveEffect->HideVFX();
}

void UGsUIHUDBattleArenaEnter::NativeDestruct()
{
	ClearTimer();

	_vfxActiveEffect->HideVFX();

	Super::NativeDestruct();
}

void UGsUIHUDBattleArenaEnter::OnClicked()
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
	{	// 2023/11/21 PKT - Arena 입장을 보낸다.
		battleArenaHandler->TryToEnterBattleArenaGuildWar();		
	}
}

void UGsUIHUDBattleArenaEnter::OnTimer()
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

		if (EBattleArenaGuildWarState::EMatchingWait == battleArenaHandler->GetState())
		{
			FText format;
			FGsBattleArenaHelper::FindText(TEXT("UI_GuildArena_Entrance_Wait"), format);
		}
	}
}

void UGsUIHUDBattleArenaEnter::ClearTimer()
{
	UWorld* world = GetWorld();
	if (world && _timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsUIHUDBattleArenaEnter::SetData(EBattleArenaGuildWarState InState, uint64 InEndTime)
{
	if (MapType::GUILD_WAR == GLevel()->GetCurrentMapType() 
		|| (EBattleArenaGuildWarState::EMatchingWait != InState && EBattleArenaGuildWarState::EAdmissionGameAvailable != InState) )
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		
		_vfxActiveEffect->HideVFX();
		
		ClearTimer();		
		return;
	}

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	if (EBattleArenaGuildWarState::EMatchingWait == InState)
	{
		_textRemainingTimeDesc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (_vfxActiveEffect->IsPlayingAnimation())
		{
			_vfxActiveEffect->HideVFX();
		}
	}
	else if (EBattleArenaGuildWarState::EAdmissionGameAvailable == InState)
	{
		_textRemainingTimeDesc->SetVisibility(ESlateVisibility::Collapsed);

		if (false == _vfxActiveEffect->IsPlayingAnimation())
		{
			_vfxActiveEffect->ShowVFX();
		}
	}
	
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
		_textRemainingTime->SetText(FText::GetEmpty());

		if (false == _timerHandle.IsValid())
		{
			if (UWorld* world = GetWorld())
			{	// 2023/12/8 PKT - Timer를 사용하면 Text가 잠깐 튀는 현상이 발생. 미리 한번 불러 준다.
				OnTimer();
				world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIHUDBattleArenaEnter::OnTimer, 0.5f, true);
			}
		}
	}
}