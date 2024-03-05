


#include "GsEventActionImmediate.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "UTIL/GsTableUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIContent/Tray/GsUITrayTickerCommon.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageProgressBar.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageNotify.h"

#include "DataSchema/Map/EventAction/GsSchemaMapEventActionDialog.h"
#include "DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageProgressBarEX.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageGameObject.h"


void UGsEventActionImmediate::Initialize()
{
	ClearAll();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		_gameObjectMsg.Emplace(
			messageHolder->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&UGsEventActionImmediate::OnLocalPlayerSpawnComplete));
	}
}

void UGsEventActionImmediate::Finalize()
{
	ClearAll();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		for (auto& item : _gameObjectMsg)
		{
			messageHolder->GetGameObject().Remove(item);
		}
		_gameObjectMsg.Empty();

	}
}

void UGsEventActionImmediate::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	ClearAll();
}

void UGsEventActionImmediate::CombinText(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet, FText& OutMessage)
{
	TArray<int64> valueSet;
	// 2022/08/10 PKT - 넘어온 Param중 Value만 수집
	for (const auto& param : InParamSet)
	{
		valueSet.Emplace(param);
	}

	const static int32 NOT_FIND_INDEX = -1;
	FString combinMessage = InData->mainMessage.ToString();
	const FString replace_filed = FString(TEXT("{0}"));
	for (int32 idx = 0; idx < valueSet.Num(); ++idx)
	{
		FString findFiled = FString::Printf(TEXT("{%d}"), idx);
		if (NOT_FIND_INDEX >= combinMessage.Find(findFiled))
		{
			break;
		}

		FString replaceString = combinMessage.Replace(*findFiled, *replace_filed);

		combinMessage = FString::Format(*replaceString, { valueSet[idx] });
	}

	OutMessage = FText::FromString(combinMessage);
}

UGsUIHUDEventActionMessageProgressBar* UGsEventActionImmediate::GetWidgetMessageProgressBar()
{
	TWeakObjectPtr<UGsUIWidgetBase> HUD = GUI()->GetWidgetByKey(TEXT("HUDMain"));
	if (HUD.IsValid())
	{
		TWeakObjectPtr<UGsUIHUDFrameMain> HudFrame = Cast<UGsUIHUDFrameMain>(HUD.Get());
		if (HudFrame.IsValid())
		{
			return HudFrame->GetEventActionMessageProgressBar();
		}
	}
	return nullptr;
}

UGsUIHUDEventActionMessageProgressBarEX* UGsEventActionImmediate::GetWidgetMessageProgressBarEX()
{
	TWeakObjectPtr<UGsUIWidgetBase> HUD = GUI()->GetWidgetByKey(TEXT("HUDMain"));
	if (HUD.IsValid())
	{
		TWeakObjectPtr<UGsUIHUDFrameMain> HudFrame = Cast<UGsUIHUDFrameMain>(HUD.Get());
		if (HudFrame.IsValid())
		{
			return HudFrame->GetEventActionMessageProgressBarEX();
		}
	}
	return nullptr;
}

UGsUIHUDEventActionMessageNotify* UGsEventActionImmediate::GetWidgetMessageNotify()
{
	TWeakObjectPtr<UGsUIWidgetBase> HUD = GUI()->GetWidgetByKey(TEXT("HUDMain"), false);
	if (HUD.IsValid())
	{
		TWeakObjectPtr<UGsUIHUDFrameMain> HudFrame = Cast<UGsUIHUDFrameMain>(HUD.Get());
		if (HudFrame.IsValid())
		{
			return HudFrame->GetEventActionMessageNotify();
		}
	}
	return nullptr;
}

void UGsEventActionImmediate::OnEventStateBoard(const FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	UGsUIHUDEventActionMessageNotify* dialog = GetWidgetMessageNotify();
	if (dialog)
	{
		if (InIsActive)
		{
			dialog->SetVisibility(ESlateVisibility::HitTestInvisible);

			FText message;
			CombinText(InData, InParamSet, message);
			dialog->SetData(message);
		}
		else
		{
			dialog->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsEventActionImmediate::OnEventProgressBar(const FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	UGsUIHUDEventActionMessageProgressBar* dialog = GetWidgetMessageProgressBar();
	if (dialog)
	{
		if (InIsActive)
		{
			dialog->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			int64 elapsedTime = InParamSet.IsValidIndex(0) ? InParamSet[0] : 0;
			int64 totalTime = InParamSet.IsValidIndex(1) ? InParamSet[1] : 0;

			// 2023/2/27 PKT - Main Message가 있으면 함께 출력
			dialog->SetData(InData->mainMessage, elapsedTime, totalTime);
		}
		else
		{
			dialog->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsEventActionImmediate::OnEventProgressBarEX(const struct FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	UGsUIHUDEventActionMessageProgressBarEX* dialog = GetWidgetMessageProgressBarEX();
	if (dialog)
	{
		if (InIsActive)
		{
			dialog->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			int64 elapsedTime = InParamSet.IsValidIndex(0) ? InParamSet[0] : 0;
			int64 totalTime = InParamSet.IsValidIndex(1) ? InParamSet[1] : 0;

			// 2023/2/27 PKT - Main Message가 있으면 함께 출력
			dialog->SetData(InData->mainMessage, elapsedTime, totalTime);
		}
		else
		{
			dialog->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsEventActionImmediate::OnEventMessageTicker(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet)
{
	FText message;
	CombinText(InData, InParamSet, message);

	// 2022/08/10 PKT - 티커 출력
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
}

void UGsEventActionImmediate::OnEventPrologueTicker(const FGsSchemaMapEventActionDialog* InData)
{
	FGsUIHelper::TrayTickerCommon(InData->mainMessage, InData->subMessage);
}

void UGsEventActionImmediate::OnEventBattleTicker(const FGsSchemaMapEventActionDialog* InData)
{
	FGsUIHelper::TrayTickerBattle(InData->mainMessage, InData->subMessage);
}

void UGsEventActionImmediate::OnEventMessageChatTicker(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet)
{
	FText message;
	CombinText(InData, InParamSet, message);

	FGsUIHelper::AddChatMessageSystem(message);
}

void UGsEventActionImmediate::Play(EventSubDialogId InId, bool InIsActive, const TArray<EvenParameterValue>& InParamSet)
{
	const FGsSchemaMapEventActionImmediate* schemaData =
		UGsTableUtil::FindRowById<UGsTableMapEventActionImmediate, FGsSchemaMapEventActionImmediate>(InId);

	if (nullptr == schemaData)
	{
		GSLOG(Error, TEXT("nullptr == schemaData : [%d]"), InId);
		return;
	}

	/**
	 * 현재는 Dialog 타입만 있는데 추후 타입이 늘어나면 Type 정의와 Schema를 RowPoly로 변경 하면 됨.
	 */
	const FGsSchemaMapEventActionDialog* schemaDialogData = schemaData->data.GetRow();
	if (nullptr == schemaDialogData)
	{
		GSLOG(Error, TEXT("nullptr == schemaDialogData"));
		return;
	}

	switch (schemaDialogData->style)
	{
	case EventDialogSyle::TICKER:
		OnEventMessageTicker(schemaDialogData, InParamSet);
		break;
	case EventDialogSyle::STATEBOARD:
		OnEventStateBoard(schemaDialogData, InIsActive, InParamSet);
		break;
	case EventDialogSyle::PROLOGUE_TICKER:
		OnEventPrologueTicker(schemaDialogData);
		break;
	case EventDialogSyle::BATTLE_TICKER:
		OnEventBattleTicker(schemaDialogData);
		break;
	case EventDialogSyle::INVADE_TYPE_ONE_TEXT_TICKER:
		OnEventInvadeOneTextTicker(schemaDialogData);
		break;
	case EventDialogSyle::INVADE_TYPE_TWO_TEXT_TICKER:
		OnEventInvadeTwoTextTicker(schemaDialogData);
		break;
	case EventDialogSyle::TIMER_BOARD:
		OnEventProgressBar(schemaDialogData, InIsActive, InParamSet);
		break;
	case EventDialogSyle::TIMER_BOARD_EX:
		OnEventProgressBarEX(schemaDialogData, InIsActive, InParamSet);
		break;
	case EventDialogSyle::FIELD_BOSS_SPAWN_TICKER:
	case EventDialogSyle::FIELD_BOSS_DIE_TICKER:
		OnEventInvadeTwoTextTicker(schemaDialogData);
		break;
	case EventDialogSyle::INVADE_TYPE_SERVERNAME_TEXT_TICKER:
		OnEventMessageServerNameTicker(schemaDialogData, InParamSet);
		break;
	case EventDialogSyle::CHAT_TICKER:
		OnEventMessageChatTicker(schemaDialogData, InParamSet);
		break;
	default:
		GSLOG(Error, TEXT("Not Find Dialog Style : [%d]"), schemaDialogData->style);
		break;
	}
}

void UGsEventActionImmediate::ClearAll()
{
	if (UGsUIHUDEventActionMessageProgressBar* widget = GetWidgetMessageProgressBar())
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UGsUIHUDEventActionMessageProgressBarEX* widget = GetWidgetMessageProgressBarEX())
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UGsUIHUDEventActionMessageNotify* widget = GetWidgetMessageNotify())
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsEventActionImmediate::OnEventInvadeOneTextTicker(const FGsSchemaMapEventActionDialog* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	FGsUIHelper::TrayTickerInvadeOneText(InData->mainMessage, InData->iconPath);
	if (nullptr == InData->sound.GetRow())
	{
		return;
	}

	UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer();
	if (nullptr == soundPlayer)
	{
		return;
	}

	soundPlayer->PlaySound2D(InData->sound.GetRow());
}

void UGsEventActionImmediate::OnEventInvadeTwoTextTicker(const FGsSchemaMapEventActionDialog* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	FGsUIHelper::TrayTickerInvadeTwoText(InData->mainMessage, InData->subMessage, InData->iconPath);
	if (nullptr == InData->sound.GetRow())
	{
		return;
	}

	UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer();
	if (nullptr == soundPlayer)
	{
		return;
	}

	soundPlayer->PlaySound2D(InData->sound.GetRow());
}

void UGsEventActionImmediate::OnEventMessageServerNameTicker(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet)
{
	static auto GetWorldName = [](WorldId id)->FText {
		if (const auto worldTable = Cast<UGsTableWorldData>(FGsSchemaWorldData::GetStaticTable()))
		{
			const FGsSchemaWorldData* worldData = nullptr;
			if (worldTable->FindRowById(id, worldData))
			{
				return worldData->worldName;
			}
		}
		return FText::GetEmpty();
	};

	if (InData == nullptr) return;

	FText worldName;
	if (auto worldIdParam = InParamSet.FindByPredicate([](auto& e) { return e > 0; }))
	{
		worldName = GetWorldName(*worldIdParam);
	}
	FText message = FText::Format(InData->mainMessage, worldName);
	if (auto chatManager = GSChat())
	{
		chatManager->OnSystemChat(message.ToString());
	}

	FGsUIHelper::TrayTickerInvadeOneText(message, InData->iconPath);
	if (nullptr == InData->sound.GetRow())
	{
		return;
	}

	UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer();
	if (nullptr == soundPlayer)
	{
		return;
	}

	soundPlayer->PlaySound2D(InData->sound.GetRow());
}