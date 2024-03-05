#include "GsUIHUDBattleArenaScoreHud.h"
#include "../Guild/GsGuildHelper.h"
#include "../Helper/GsUIStringHelper.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Helper/GsUIHelper.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UGsUIHUDBattleArenaScoreHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDetailScore->OnClicked.AddDynamic(this, &UGsUIHUDBattleArenaScoreHud::OnClicked);
}

void UGsUIHUDBattleArenaScoreHud::NativeConstruct()
{
	Super::NativeConstruct();

	_btnDetailScore->SetVisibility(ESlateVisibility::Collapsed);
	_sizeBoxArenaCoin->SetVisibility(ESlateVisibility::Collapsed);

	ClearTimer();
}

void UGsUIHUDBattleArenaScoreHud::NativeDestruct()
{
	ClearTimer();

	_btnDetailScore->SetVisibility(ESlateVisibility::Collapsed);
	_sizeBoxArenaCoin->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}

void UGsUIHUDBattleArenaScoreHud::OnClicked()
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
		battleArenaHandler->SendReqGuildWarScoreInfo();
	}
}

void UGsUIHUDBattleArenaScoreHud::LoadTeamInfo(const FGsBattleArenaGuildWarHandler* InHandler)
{
	auto SetGuildEmblem = [](GuildEmblemId InGuildEmblemId, UImage* OutImage)
	{
		if (const FGsSchemaGuildEmblem* schemaGuildEmblem = FGsGuildHelper::GetGuildEmblem(InGuildEmblemId))
		{
			OutImage->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(schemaGuildEmblem->iconPathSmall));
			OutImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			OutImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	};

	SetGuildEmblem(InHandler->RedTeam()._guildEmblemId, _imgRedGuildEmblem);
	_textRedGuildName->SetText(InHandler->RedTeam()._guildName);

	SetGuildEmblem(InHandler->BlueTeam()._guildEmblemId, _imgBlueGuildEmblem);
	_textBlueGuildName->SetText(InHandler->BlueTeam()._guildName);
}

void UGsUIHUDBattleArenaScoreHud::LoadCoinInfo(const FGsBattleArenaGuildWarHandler* InHandler)
{
	MapId mapId = InHandler->GetMapId();
	const FGsSchemaGuildWarConfigData* ConfigData = InHandler->GetConfigData(mapId);
	if (nullptr == ConfigData || nullptr == ConfigData->coinId.GetRow())
	{
		return;
	}

	const FGsSchemaItemCommon* itemCommon = ConfigData->coinId.GetRow();
	if (nullptr == itemCommon)
	{
		return;
	}

	_imgCoin->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(itemCommon->icon));
}

void UGsUIHUDBattleArenaScoreHud::SetScoreInfo(int32 InRedTeamScore, int32 InBlueTeamScore)
{
	FText textRedScore;
	FGsUIStringHelper::GetAmountText(InRedTeamScore, MAX_uint64, textRedScore);
	_textRedScore->SetText(textRedScore);

	FText textBlueScore;
	FGsUIStringHelper::GetAmountText(InBlueTeamScore, MAX_uint64, textBlueScore);
	_textBlueScore->SetText(textBlueScore);
}

void UGsUIHUDBattleArenaScoreHud::SetCoinCount(ItemAmount InCount)
{
	FText textCoin;
	FGsUIStringHelper::GetAmountText(StaticCast<int32>(InCount), MAX_uint64, textCoin);

	_textCoinCount->SetText(textCoin);
}

void UGsUIHUDBattleArenaScoreHud::OnTimer()
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
			/*FText format;
			FGsBattleArenaHelper::FindText(TEXT("UI_GuildArena_Entrance_Wait"), format);*/
		}
	}
}

void UGsUIHUDBattleArenaScoreHud::ClearTimer()
{
	UWorld* world = GetWorld();
	if (world && _timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsUIHUDBattleArenaScoreHud::SetData(MapId InMapId, EBattleArenaGuildWarState InState, uint64 InEndTime, int32 InRedTeamScore, int32 InBlueTeamScore)
{
	if (const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo())
	{
		if (currentMapData->id != InMapId)
		{	// 2023/12/5 PKT - Arena Map이 아니면 빠져나감.
			this->SetVisibility(ESlateVisibility::Collapsed);
			ClearTimer();
			return;
		}
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

	const FGsBattleArenaGuildWarHandler* battleArenaHandler = hud->GetBattleArenaGuildWarHandler();
	if (nullptr == battleArenaHandler)
	{
		GSLOG(Error, TEXT("nullptr == battleArenaHandler"));
		return;
	}

	if (false == _timerHandle.IsValid())
	{
		_textRemainingTime->SetText(FText::GetEmpty());

		if (UWorld* world = GetWorld())
		{	// 2023/12/8 PKT - Timer를 사용하면 Text가 잠깐 튀는 현상이 발생. 미리 한번 불러 준다.
			OnTimer();
			world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIHUDBattleArenaScoreHud::OnTimer, 0.5f, true);
		}
	}

	if (EBattleArenaGuildWarState::EStandByStartGame == InState)
	{	// 2023/12/5 PKT - 게임이 시작 전일때는 보이지 않음. 빠져나감.
		_btnDetailScore->SetVisibility(ESlateVisibility::Collapsed);
		_sizeBoxArenaCoin->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (ESlateVisibility::Collapsed == _btnDetailScore->GetVisibility()
		|| ESlateVisibility::Collapsed == _sizeBoxArenaCoin->GetVisibility()
		)
	{
		// 2023/11/29 PKT - Image 셋팅
		LoadTeamInfo(battleArenaHandler);
		LoadCoinInfo(battleArenaHandler);

		_btnDetailScore->SetVisibility(ESlateVisibility::Visible);
		_sizeBoxArenaCoin->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	SetScoreInfo(InRedTeamScore, InBlueTeamScore);
	SetCoinCount(battleArenaHandler->GetCoinAmount());
}