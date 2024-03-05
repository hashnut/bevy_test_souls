#include "GsUIPopupBattleArenaGuildWarDetailScore.h"
#include "GsUIBattleArenaRankListEntry.h"
#include "../Guild/GsGuildHelper.h"
#include "../../Helper/GsUIStringHelper.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"




void UGsUIPopupBattleArenaGuildWarDetailScore::BeginDestroy()
{
	if (nullptr != _redPanelSlotHelper)
	{
		_redPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshRedTeamEntry);
		_redPanelSlotHelper = nullptr;
	}
	
	if (nullptr != _bluePanelSlotHelper)
	{
		_bluePanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshBlueTeamEntry);
		_bluePanelSlotHelper = nullptr;
	}
	Super::BeginDestroy();
}

void UGsUIPopupBattleArenaGuildWarDetailScore::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _redPanelSlotHelper)
	{
		_redPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_redPanelSlotHelper->Initialize(_redEntryWidgetClass, _redRootPanel);
		_redPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshRedTeamEntry);
	}

	if (nullptr == _bluePanelSlotHelper)
	{
		_bluePanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_bluePanelSlotHelper->Initialize(_blueEntryWidgetClass, _blueRootPanel);
		_bluePanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshBlueTeamEntry);
	}
	
	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnClickedOK);
	_btnRefresh->OnClicked.AddDynamic(this, &UGsUIPopupBattleArenaGuildWarDetailScore::OnClcikedRefresh);
}

void UGsUIPopupBattleArenaGuildWarDetailScore::NativeConstruct()
{
	Super::NativeConstruct();

	_redTeamInfo.Reset();
	_blueTeamInfo.Reset();

	_redMyStatusBoard->SetVisibility(ESlateVisibility::Hidden);
	_blueMyStatusBoard->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIPopupBattleArenaGuildWarDetailScore::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupBattleArenaGuildWarDetailScore::OnClickedOK()
{
	Close();
}

void UGsUIPopupBattleArenaGuildWarDetailScore::OnClcikedRefresh()
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

void UGsUIPopupBattleArenaGuildWarDetailScore::OnInputCancel()
{
	Close();
}

void UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshRedTeamEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _redTeamInfo._members.IsValidIndex(InIndex))
	{
		return;
	}

	int32 rank = InIndex + 1;
	const FGsBattleArenaGuildWarMemberInfo& memberInfo = _redTeamInfo._members[InIndex];
	if (memberInfo._userDBId == GGameData()->GetUserData()->mUserDBId)
	{
		/**
		 * 내 정보라면 RenTeam
		 */
		_redMyStatusBoard->SetData(rank, memberInfo);
		_redMyStatusBoard->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_blueMyStatusBoard->SetVisibility(ESlateVisibility::Collapsed);
	}

	TWeakObjectPtr<UGsUIBattleArenaRankListEntry> widget = Cast<UGsUIBattleArenaRankListEntry>(InEntry);
	if (widget.IsValid())
	{
		widget->SetData(rank, memberInfo);
	}
}

void UGsUIPopupBattleArenaGuildWarDetailScore::OnRefreshBlueTeamEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _blueTeamInfo._members.IsValidIndex(InIndex))
	{
		return;
	}

	int32 rank = InIndex + 1;
	const FGsBattleArenaGuildWarMemberInfo& memberInfo = _blueTeamInfo._members[InIndex];
	if (memberInfo._userDBId == GGameData()->GetUserData()->mUserDBId)
	{
		/**
		 * 내 정보라면 RenTeam
		 */
		_blueMyStatusBoard->SetData(rank, memberInfo);
		_blueMyStatusBoard->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_redMyStatusBoard->SetVisibility(ESlateVisibility::Collapsed);
	}

	TWeakObjectPtr<UGsUIBattleArenaRankListEntry> widget = Cast<UGsUIBattleArenaRankListEntry>(InEntry);
	if (widget.IsValid())
	{
		widget->SetData(rank, memberInfo);
	}
}

void UGsUIPopupBattleArenaGuildWarDetailScore::SetData(EBattleArenaGuildWarState InState, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam)
{
	auto SetGuildEmblem = [](GuildEmblemId InGuildEmblemId, UImage* OutImage)
	{
		if (const FGsSchemaGuildEmblem* schemaGuildEmblem = FGsGuildHelper::GetGuildEmblem(InGuildEmblemId))
		{
			OutImage->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(schemaGuildEmblem->iconPathBig));
			OutImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			OutImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	};
	// 2023/11/24 PKT - 길드 엠블럼
	SetGuildEmblem(InRedTeam._guildEmblemId, _imgRedGuildEmblem);
	SetGuildEmblem(InBlueTeam._guildEmblemId, _imgBlueGuildEmblem);

	auto SetPrefix = [](WorldId InWorldID, UTextBlock* OutTextPrefix)
	{
		FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(InWorldID);
		// 2023/11/24 PKT - 프리픽스 찾지 못했을때
		if (textPrefix.IsEmpty())
		{
			OutTextPrefix->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			OutTextPrefix->SetText(textPrefix);
			OutTextPrefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	};

	_textRedGuildName->SetText(InRedTeam._guildName);
	SetPrefix(InRedTeam._worldId, _textRedPrefix);
	
	_textBlueGuildName->SetText(InBlueTeam._guildName);
	SetPrefix(InBlueTeam._worldId, _textBluePrefix);

	if (EBattleArenaGuildWarState::EGameDone == InState)
	{
		_btnRefresh->SetIsEnabled(false);
		UWidgetBlueprintGeneratedClass* genClass = this->GetWidgetTreeOwningClass();
		if (genClass && 0 < genClass->Animations.Num())
		{
			const static int32 FIRST_ANIMATION_DATA = 0;
			if (UWidgetAnimation* animationData = genClass->Animations[FIRST_ANIMATION_DATA])
			{
				const static int32 NUM_LOOPS_TO_PLAY = 1;
				this->PlayAnimation(animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Type::Forward, 1.f);
			}
		}
	}
	else
	{
		_btnRefresh->SetIsEnabled(true);
		StopAllAnimations();
	}

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
			FText format;
			FGsBattleArenaHelper::FindText(TEXT("UI_BattleBoard_Reward_Condition"), format);

			FText message = FText::GetEmpty();
			if (const FGsSchemaGuildWarConfigData* configData = battleArenaHandler->GetConfigData(GLevel()->GetCurrentMapInfo()->id))
			{
				message = FText::Format(format, FText::AsNumber(configData->limitMinRewardPoint));
			}

			_textNoriceRewardCondition->SetText(message);
		}
	}
	

	Invalidate(InRedTeam, InBlueTeam);
}

void UGsUIPopupBattleArenaGuildWarDetailScore::Invalidate(const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam)
{
	_redTeamInfo = InRedTeam;
	_blueTeamInfo = InBlueTeam;

	FText textRedScore;
	FGsUIStringHelper::GetAmountText(_redTeamInfo._score, MAX_uint64, textRedScore);
	_textRedTotalScore->SetText(textRedScore);
	
	FText textBlueScore;
	FGsUIStringHelper::GetAmountText(_blueTeamInfo._score, MAX_uint64, textBlueScore);
	_textBlueTotalScore->SetText(textBlueScore);

	_redPanelSlotHelper->RefreshAll(_redTeamInfo._members.Num());
	_bluePanelSlotHelper->RefreshAll(_blueTeamInfo._members.Num());
}