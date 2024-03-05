#include "GsUIHelper.h"
#include "GameFlow/GsGameFlow.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "UI/UIContent/Popup/GsUIPopupSystem.h"
#include "UI/UIContent/Popup/GsUIPopupOK.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupSystemYesNo.h"
#include "UI/UIContent/Tray/GsUITrayBlockUI.h"
#include "UI/UIContent/Tray/GsUITrayTickerSystem.h"
#include "UI/UIContent/Tray/GsUITrayTickerMessage.h"
#include "UI/UIContent/Tray/GsUITrayTickerBossDespawn.h"
#include "UI/UIContent/Tray/Invade/GsUITrayTickerInvadeOneText.h"
#include "UI/UIContent/Tray/Invade/GsUITrayTickerInvadeTwoText.h"
#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Significance/GsGameViewPortClient.h"
#include "Templates/Function.h"
#include "Message/GsMessageSystem.h"
#include "PaperSprite.h"
#include "Data/GsResourceManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "Classes/Engine/Texture2D.h"
#include "UI/UIContent/Tray/GsUITrayTickerClear.h"
#include "UI/UIContent/Tray/GsUITrayTickerFail.h"
#include "UI/UIContent/Tray/GsUITrayTickerBattle.h"
#include "UI/UIContent/Tray/GsUITrayTickerCount.h"
#include "UI/UIContent/Tray/GsUITrayTickerCommon.h"
#include "UI/UIContent/Tray/GsUITrayDimmed.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UIContent/Tray/GsUITrayTickerArenaCount.h"
#include "UI/UIContent/Tray/GsUITrayTickerBMPurchase.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "PopupSystem/GsPopupSystemEnum.h"
#include "Net/GsNetBase.h"
#include "Net/GSNet.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"
#include "UI/UIContent/Tray/GsUITrayTickerAlertSection.h"
#include "UI/UIContent/Tray/GsUITrayTickerLauncher.h"
#include "UI/UIContent/Tray/GSUITrayTickerCountEX.h"
#include "UI/UIContent/Window/SpiritShot/GsUITrayTickerSpiritShotEnchant.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "DataSchema/Help/GsSchemaHelpContents.h"
#include "DataSchema/GameMaster/GsSchemaGMConfig.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Async.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/GsScopeGlobal.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "Net/GsNetSendService.h"
#include "Engine/World.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "UI/UIContent/Tray/GsUITrayPGPayBlock.h"



void FGsUIHelper::PopupSystemMsg(const FText& InMessage, TFunction<void()> InCloseCallback)
{
#ifdef M5
	// UGameViewportClient 생성 전에 호출될 경우, AddToViewport가 실패함에 유의
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupSystem"));
		if (widget.IsValid())
		{
			UGsUIPopupSystem* popup = Cast<UGsUIPopupSystem>(widget.Get());
			if (popup)
			{
				GSLOG(Log, TEXT("==== PopupSystemMsg Text : %s===="), *InMessage.ToString());
				popup->SetData(InMessage, InCloseCallback);
			}
		}
	}
#else
	GPopupSystem()->PopupSystemMsg(InMessage, InCloseCallback);
#endif // M5
}

void FGsUIHelper::PopupOK(const FText& InMessage, TFunction<void()> InCloseCallback)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupOK"));
		if (widget.IsValid())
		{
			UGsUIPopupOK* popup = Cast<UGsUIPopupOK>(widget.Get());
			if (popup)
			{
				popup->SetData(InMessage, InCloseCallback);
			}
		}
	}
}

UGsUIPopupYesNo* FGsUIHelper::PopupYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback,
	TFunction<void(bool)> InDestructCallback)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
		if (widget.IsValid())
		{
			UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
			if (popup)
			{
				popup->SetData(InMessage, InCloseCallback, InDestructCallback);
				return popup;
			}
		}
	}

	return nullptr;
}

class UGsUIPopupYesNoTwoMessage* FGsUIHelper::PopupYesNoTwoMessage(const FText& InMessage1, const FText& InMessage2
	, TFunction<void(bool)> InCloseCallback /*= nullptr*/)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
		if (widget.IsValid())
		{
			UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get());
			if (popup)
			{
				popup->SetData(InMessage1, InMessage2, InCloseCallback);
				return popup;
			}
		}
	}

	return nullptr;
}

void FGsUIHelper::PopupNetError(const FText& InMessage)
{
	GSLOG(Error, TEXT("[NetError] %s"), *InMessage.ToString());

	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);
	PopupSystemMsg(InMessage);
}

void FGsUIHelper::PopupNetError(const FTextKey& InKey)
{
	GSLOG(Error, TEXT("[NetError] %s"), InKey.GetChars());

	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);

	FText errorText;
	FText::FindText(TEXT("NetText"), InKey, errorText);

	PopupSystemMsg(errorText);
}

void FGsUIHelper::PopupNetError(PD::Result InMainResult, PD::Result InDetailResult, bool bIsLauncher)
{
	if (PACKET_RESULT_SUCCESS == InMainResult)
	{
		return;
	}

	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);

	FTextKey textNamespace = bIsLauncher ? TEXT("LauncherNetText") : TEXT("NetText");

	FString mainKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InMainResult) });
	FText errorMainText;
	FText::FindText(textNamespace, mainKey, errorMainText);

	FString msgMain = (errorMainText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InMainResult, PD::ResultEnumToString(InMainResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InMainResult, *(errorMainText.ToString()) });

	FString DetailKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InDetailResult) });
	FText errorDetailText;
	FText::FindText(textNamespace, mainKey, errorDetailText);

	FString msgDetail = (errorDetailText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InDetailResult, PD::ResultEnumToString(InDetailResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InDetailResult, *(errorDetailText.ToString()) });

	FString msg = FString::Format(TEXT("{0}\n\n{1}"), { msgMain, msgDetail });

	GSLOG(Error, TEXT("[NetAC] %s"), *msg);

	PopupSystemMsg(FText::FromString(msg));
}

void FGsUIHelper::PopupNetError(PD::Result InMainResult, PD::Result InDetailResult, const FString& InHiveHostCategory, bool bIsLauncher)
{
	if (PACKET_RESULT_SUCCESS == InMainResult)
	{
		return;
	}

	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);

	FTextKey textNamespace = bIsLauncher ? TEXT("LauncherNetText") : TEXT("NetText");

	FString mainKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InMainResult) });
	FText errorMainText;
	FText::FindText(textNamespace, mainKey, errorMainText);

	FString msgMain = (errorMainText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InMainResult, PD::ResultEnumToString(InMainResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InMainResult, *(errorMainText.ToString()) });

	FString DetailKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InDetailResult) });
	FText errorDetailText;
	FText::FindText(textNamespace, DetailKey, errorDetailText);

	FString msgDetail = (errorDetailText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InDetailResult, PD::ResultEnumToString(InDetailResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InDetailResult, *(errorDetailText.ToString()) });

#if	!UE_BUILD_SHIPPING
	FString msg = FString::Format(TEXT("{0}\n\n{1}\n{2}"), { msgMain, msgDetail, InHiveHostCategory });
#else
	FString msg = FString::Format(TEXT("{0}\n\n{1}"), { msgMain, msgDetail });
#endif

	GSLOG(Error, TEXT("[NetAC] %s"), *msg);

	PopupSystemMsg(FText::FromString(msg));
}

void FGsUIHelper::PopupNetError(PD::Result InResult, const FString& InValue, bool bIsLauncher)
{
	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);

	if (PACKET_RESULT_SUCCESS == InResult)
	{
		return;
	}	

	FString displayKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InResult) });

	FTextKey textNamespace = bIsLauncher ? TEXT("LauncherNetText") : TEXT("NetText");

	FText errorText;
	FText::FindText(textNamespace, displayKey, errorText);

	FString msg = (errorText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InResult, PD::ResultEnumToString(InResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InResult, *(errorText.ToString()) });

#if	!UE_BUILD_SHIPPING
	FString msgError = FString::Format(TEXT("{0}\n{1}"), { msg, InValue });
#else
	FString msgError = FString::Format(TEXT("{0}"), { msg });
#endif

	GSLOG(Error, TEXT("[NetCS] %s"), *msgError);

	PopupSystemMsg(FText::FromString(msgError));
}

void FGsUIHelper::PopupNetError(PD::Result InResult, bool bIsLauncher)
{
	ForcedHideBlockUI(EGsHideBlockUIFactor::POPUP_NET_ERROR);

	if (PACKET_RESULT_SUCCESS == InResult)
	{
		return;
	}

	if (false == bIsLauncher)
	{
		ProcessNetError(0, static_cast<int32>(InResult));
		return;
		//switch (InResult)
		//{
		//case PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL:
		//{
		//	// 가방이 가득찼습니다.
		//	FText Msg;
		//	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL:
		//{
		//	// 소지 무게가 최대치입니다.
		//	FText Msg;
		//	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::BOOKMARK_ERROR_LIST_MAX:
		//{
		//	FText Msg;
		//	FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateInvalidArea"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::BOOKMARK_ERROR_INVALID_MAP:
		//{
		//	FText Msg;
		//	FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateInvalidArea"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::QUEST_ERROR_GIVE_UP_STATE:
		//{
		//	FText Msg;
		//	FText::FindText(TEXT("QuestBoardText"), TEXT("QuestBoardabandonDeny"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::QUEST_ERROR_INVALID_ACCEPT_STORY_ID:
		//{
		//	FText Msg;
		//	FText::FindText(TEXT("QuestBoardText"), TEXT("QuestBoardAcceptDeny"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//case PD::Result::ITEM_ERROR_ADD_STORE_FULL:
		//case PD::Result::ITEM_ERROR_ADD_STORAGE_FULL:
		//{
		//	FText Msg;
		//	FText::FindText(TEXT("DepotText"), TEXT("Alert_FullDepot"), Msg);
		//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
		//	return;
		//}
		//}
	}

	FTextKey textNamespace = bIsLauncher ? TEXT("LauncherNetText") : TEXT("NetText");

	FString displayKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InResult) });

	FText errorText;
	FText::FindText(textNamespace, displayKey, errorText);

#if UE_BUILD_SHIPPING
	FString msg = FString::Format(TEXT("{0}"), { *(errorText.ToString()) });
#else	// UE_BUILD_SHIPPING	
	FString msg = (errorText.IsEmpty()) ?
		FString::Format(TEXT("Code({0})\n{1}"), { InResult, PD::ResultEnumToString(InResult) }) :
		FString::Format(TEXT("Code({0})\n{1}"), { InResult, *(errorText.ToString()) });
#endif	// UE_BUILD_SHIPPING	

	GSLOG(Error, TEXT("[NetCS] %s"), *msg);

	PopupSystemMsg(FText::FromString(msg));
}

void FGsUIHelper::LogNetError(PD::Result InResult)
{
	FString msg = FString::Format(TEXT("Code({0})\n{1}"), { InResult, PD::ResultEnumToString(InResult) });
	GSLOG(Error, TEXT("[NetError] %s"), *msg);
}

bool FGsUIHelper::IsOpenBlockUI()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			return controller->IsOpened();
		}
	}

	return false;
}

void FGsUIHelper::ShowBlockUI()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->Show();
		}
	}
}

void FGsUIHelper::SetBlockUISetting(const FText& InMessage, bool bInHideUI, float InLifeSecond)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->SetSetting(InMessage, bInHideUI, InLifeSecond);
		}
	}
}

void FGsUIHelper::HideBlockUI()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->Hide();
		}
	}
}

void FGsUIHelper::ShowBlockUIExplicit(const FName& InKey)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->ShowExplicit(InKey);
		}
	}
}

void FGsUIHelper::HideBlockUIExplicit(const FName& InKey)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->HideExplicit(InKey);
		}
	}
}

void FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor InFactor)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsBlockUIController* controller = uiMgr->GetBlockUIController())
		{
			controller->ForcedHide(InFactor);
		}
	}
}

void FGsUIHelper::PopupQuitGame()
{
#if PLATFORM_IOS
	// IOS 에서는 이 창을 출력하지 않음
#else // PLATFORM_IOS

#if PLATFORM_ANDROID
	// 안드로이드에서는 하이브 종료 팝업을 출력
	GHive()->ShowExit();

#else // PLATFORM_ANDROID

	// TEXT: 게임을 종료하시겠습니까?
	FText msg;
	FText::FindText(TEXT("LauncherText"), TEXT("AskQuitGame"), msg);

	PopupSystemYesNo(msg, [](bool bYes)
		{
			if (bYes)
			{
				// 2023/7/31 PKT - 선호 차석님 요청으로 클라이언트 종료 전 서버에게 종료 하겠다고 알리고 종료.
				//				   OfflinePlay 관련 마지막 위치(Pos) 동기화를 위해
				if (FGsGameFlow::Mode::GAME == GMode()->GetCurrentFlowType())
				{
					FGsNetSendService::SendUserGameExit();
				}
								
				if (UWorld* world = GEngine->GameViewport->GetWorld())
				{
					// 2023/8/1 PKT - 오프라인 이슈로 클라 종료 직전 패킷을 보내는데 이때 클라가 바로 종료 하면 직전의 패킷을 서버에게 전달하지 않게 된다.
					//                때문에 종료를 1Tick 뒤로 밀어 냈음
					world->GetTimerManager().SetTimerForNextTick([world]()
						{
							UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Quit, false);
						}
					);					
				}
			}
		});

#endif // PLATFORM_ANDROID

#endif // PLATFORM_IOS
}

void FGsUIHelper::TraySystemTicker(const FText& InMessage, bool bInPriority)
{
	if (InMessage.IsEmpty())
	{
		return;
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerSystem"));
		if (widget.IsValid())
		{
			UGsUITrayTickerSystem* ticker = Cast<UGsUITrayTickerSystem>(widget.Get());
			if (ticker)
			{
				ticker->SetData(InMessage, bInPriority);
			}
		}
	}
}

void FGsUIHelper::TrayMessageTicker(const FText& InMessage, bool bInPriority)
{
	if (InMessage.IsEmpty())
	{
		return;
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMessage"));
		if (widget.IsValid())
		{
			UGsUITrayTickerMessage* ticker = Cast<UGsUITrayTickerMessage>(widget.Get());
			if (ticker)
			{
				ticker->SetData(InMessage, bInPriority);
			}
		}
	}
}

void FGsUIHelper::PopupSystemYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback)
{
#ifdef M5
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupSystemYesNo"));
	if (widget.IsValid())
	{
		UGsUIPopupSystemYesNo* popup = Cast<UGsUIPopupSystemYesNo>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}

		popup->SetData(InMessage, InCloseCallback);
	}
#else
	GPopupSystem()->PopupSystemYesNo(InMessage, InCloseCallback);
#endif // M5
}

UTexture2D* FGsUIHelper::LoadTextureFromPath(const FSoftObjectPath& InPath)
{
	if (false == InPath.IsValid())
	{
		return nullptr;
	}

	return GResource()->LoadSync<UTexture2D>(InPath);
}

void FGsUIHelper::AsyncLoadTextureFromPath(const FSoftObjectPath& InPath, FStreamableDelegateParam inRes)
{
	if (false == InPath.IsValid())
	{
		return;
	}

	GResource()->LoadAsync(InPath, inRes);
}

UPaperSprite* FGsUIHelper::LoadSpriteFromPath(const FSoftObjectPath& InPath)
{
	if (false == InPath.IsValid())
	{
		return nullptr;
	}
	
	return GResource()->LoadSync<UPaperSprite>(InPath);
}

void FGsUIHelper::AsyncLoadSpriteFromPath(const FSoftObjectPath& InPath, FStreamableDelegateParam inRes)
{
	if (false == InPath.IsValid())
	{
		return;
	}

	GResource()->LoadAsync(InPath, inRes);
}

void FGsUIHelper::AddChatMessageSystem(const FText& InMessage)
{
	// 월드 진입도중 채팅 시 문제될 수 있어 널체크
	if (UGsChatManager* chatMgr = GSChat())
	{
		chatMgr->OnSystemChat(InMessage.ToString());
	}
}

// 성공 공용티커
void FGsUIHelper::TrayTickerClear(TFunction<void()>&& InCallback)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerClear"));
	if (widget.IsValid())
	{
		UGsUITrayTickerClear* tray = Cast<UGsUITrayTickerClear>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(InCallback);
	}
}

// 실패 공용티커
void FGsUIHelper::TrayTickerFail(TFunction<void()>&& InCallback)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerFail"));
	if (widget.IsValid())
	{
		UGsUITrayTickerFail* tray = Cast<UGsUITrayTickerFail>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(InCallback);
	}
}

// 전투 공용티커
void FGsUIHelper::TrayTickerBattle(const FText& InMainText, const FText& InSubText, TFunction<void()>&& InCallback /* = nullptr */)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerBattle"));
	if (widget.IsValid())
	{
		UGsUITrayTickerBattle* tray = Cast<UGsUITrayTickerBattle>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(InMainText, InSubText, InCallback);
	}
}

// 카운트 공용티커
void FGsUIHelper::TrayTickerCount(TFunction<void()>&& InCallback)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerCount"));
	if (widget.IsValid())
	{
		UGsUITrayTickerCount* tray = Cast<UGsUITrayTickerCount>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(InCallback);
	}
}

void FGsUIHelper::TrayTickerCount(time_t InStartTime, int32 InCount, TFunction<void()>&& InCallback)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	time_t startTime = InStartTime;
	if (InStartTime == 0)
	{
		startTime = FGsTimeSyncUtil::GetServerNowSecond();
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerArenaCount"));
	if (widget.IsValid())
	{
		UGsUITrayTickerArenaCount* tray = Cast<UGsUITrayTickerArenaCount>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetCount(startTime, InCount);
		tray->SetData(InCallback);
	}
}

void FGsUIHelper::TrayTickerCountEx(int32 InCount, time_t InEndTime, TFunction<void()>&& InCallback /* = nullptr */)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerCountEx"));
	if (widget.IsValid())
	{
		UGSUITrayTickerCountEX* tray = Cast<UGSUITrayTickerCountEX>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetCountData(InCount, InEndTime, InCallback);
	}
}

// 범용 공용티커(소환 티커로 사용)
void FGsUIHelper::TrayTickerCommon(const FText& InMainText, const FText& InSubText, TFunction<void()>&& InCallback)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerCommon"));
	if (widget.IsValid())
	{
		UGsUITrayTickerCommon* tray = Cast<UGsUITrayTickerCommon>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(InMainText, InSubText, InCallback);
	}
}

bool FGsUIHelper::IsActiveTrayTickerCountEx()
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return false;
	}

	return uiMgr->IsActiveWidget(TEXT("TrayTickerCountEx"));
}

void FGsUIHelper::CloseTrayTickerCountEx()
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("TrayTickerCountEx"));
	if (widget.IsValid())
	{
		widget.Get()->Close();
	}
}

// 화면 딤드처리
void FGsUIHelper::TrayDimmed(bool isDimmedOn, const FLinearColor& InColor)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TPair<FName, FSoftObjectPath> pathInfo = uiMgr->GetPathTrayDimmed();

	if (isDimmedOn)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidgetByPath(pathInfo.Key, pathInfo.Value);
		if (widget.IsValid())
		{
			UGsUITrayDimmed* tray = Cast<UGsUITrayDimmed>(widget.Get());
			if (nullptr == tray)
			{
				return;
			}

			tray->SetDimmed(isDimmedOn, InColor);
		}
	}
	else
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(pathInfo.Key);
		if (widget.IsValid())
		{
			widget->Close();
		}
	}
}

void FGsUIHelper::TrayTickerBMShopPurchase(TPair<FGsBMShopProductData*, ItemAmount> InPairData, bool InIsBulkSale, bool InIsCash)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerBMPurchase"));
	if (widget.IsValid())
	{
		UGsUITrayTickerBMPurchase* tray = Cast<UGsUITrayTickerBMPurchase>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}
		tray->SetData(InPairData, InIsBulkSale, InIsCash);
	}
}

bool FGsUIHelper::TrayPGPayBlock()
{
	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> WidgetPtr = UiManager->OpenAndGetWidget(FName(UGsUITrayPGPayBlock::ASSET_NAME));
	if (!WidgetPtr.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return false;
	}

	return true;
}

void FGsUIHelper::TrayTickerBoss(EGsBossTickerStateType inState, CreatureId inNpcId)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerBoss"));
	if (widget.IsValid())
	{
		UGsUITrayTickerBoss* tray = Cast<UGsUITrayTickerBoss>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetData(inState, inNpcId);
	}
}
// 필드 보스 출현 타이밍 개선(고정 시간 스폰, 디스폰) 작업
// https://jira.com2us.com/jira/browse/CHR-23655
void FGsUIHelper::TrayTickerBossDespawn(CreatureId inNpcId)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerBossDespawn"));
	if (widget.IsValid())
	{
		UGsUITrayTickerBossDespawn* tray = Cast<UGsUITrayTickerBossDespawn>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetDespawnData(inNpcId);
	}
}

void FGsUIHelper::TrayTickerInvadeOneText(const FText& inContext, FSoftObjectPath inIconPath)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerInvadeOneText"));
	if (widget.IsValid())
	{
		UGsUITrayTickerInvadeOneText* tray = Cast<UGsUITrayTickerInvadeOneText>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetInvadeOneTextData(inContext, inIconPath);
	}
}

void FGsUIHelper::TrayTickerInvadeTwoText(const FText& inContext1, const FText& inContext2, FSoftObjectPath inIconPath)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerInvadeTwoText"));
	if (widget.IsValid())
	{
		UGsUITrayTickerInvadeTwoText* tray = Cast<UGsUITrayTickerInvadeTwoText>(widget.Get());
		if (nullptr == tray)
		{
			return;
		}

		tray->SetInvadeTwoText(inContext1, inContext2, inIconPath);
	}
}

void FGsUIHelper::TrayTickerSpiritShotEnchant(FText InText, bool IsSuccess /*= true*/)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget;
	if (IsSuccess)
	{
		widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerSpiritShotSuccess"));
	}
	else
	{
		widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerSpiritShotFail"));
	}

	if (widget.IsValid())
	{
		UGsUITrayTickerSpiritShotEnchant* tray = Cast<UGsUITrayTickerSpiritShotEnchant>(widget.Get());
		if (nullptr == tray)
		{
			GSLOG(Error, TEXT("nullptr == tray"));
			return;
		}
		tray->SetText(MoveTemp(InText));
	}
}

void FGsUIHelper::TraySectionMessageTicker(EGsNotiSection section, const FText& InTagMessage, TrayCondition condition, SectionPriority priority, float playTime)
{
	if (InTagMessage.IsEmpty())
	{
		return;
	}
	
	if (section == EGsNotiSection::ALERT)
	{
		//TrayOperatorMessageTicker(NoticeType::General, InTagMessage, priority, playTime);
		return;
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySectionMessage"));
		if (widget.IsValid())
		{
			UGsUITrayTickerSection* ticker = Cast<UGsUITrayTickerSection>(widget.Get());
			if (ticker)
			{
				ticker->SetData(section, InTagMessage, priority, playTime, condition);
			}
		}
	}
}

void FGsUIHelper::TraySectionMessageDelayTicker(EGsNotiSection section, const FText& InTagMessage, float delayTime, TrayCondition condition, SectionPriority priority, float playTime)
{
	if (section == EGsNotiSection::ALERT) return;
	if (InTagMessage.IsEmpty())
	{
		return;
	}

	if (UGsUIManager* uiMgr = GUI())
	{	
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySectionMessage"));
		if (widget.IsValid())
		{
			UGsUITrayTickerSection* ticker = Cast<UGsUITrayTickerSection>(widget.Get());
			if (ticker)
			{
				ticker->SetDelayData(section, InTagMessage, priority, playTime, delayTime, condition);
			}
		}
	}
}
void FGsUIHelper::TraySectionMessageTickerNetError(PD::Result InResult, EGsNotiSection section /*EGsNotiSection::EXTSYSTEM*/)
{
	if (PACKET_RESULT_SUCCESS == InResult)
	{
		return;
	}

	HideBlockUI();

	FString displayKey = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(InResult) });

	FText errorText;
	FText::FindText(TEXT("NetText"), displayKey, errorText);

	FString msg = (errorText.IsEmpty()) ? PD::ResultEnumToString(InResult) : *(errorText.ToString());

	GSLOG(Error, TEXT("[Net] %s"), *msg);

	if (section == EGsNotiSection::ALERT)
	{
		TrayOperatorMessageTicker(NoticeType::General, FText::FromString(msg));
	}
	else
	{
		TraySectionMessageTicker(section, FText::FromString(msg));
	}
	
}

void FGsUIHelper::TrayOperatorMessageTicker(NoticeType noticeType, const FText& InMessage, SectionPriority priority, float playTime)
{
	if (InMessage.IsEmpty())
	{
		return;
	}

	//나중에 처리에 대해 나오면...
	switch (noticeType)
	{
	case NoticeType::Event:
	case NoticeType::General:
	case NoticeType::Immediate:
	case NoticeType::Special:
	case NoticeType::Urgent:
		break;
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayAlertSectionMessage"));
		if (widget.IsValid())
		{
			if(auto ticker = Cast<UGsUITrayTickerAlertSection>(widget.Get()))
			{
				ticker->SetData(InMessage, noticeType, priority, playTime);
			}
		}
	}
}

void FGsUIHelper::OpenHelpPage(EGsHelpContents key)
{
	if (key == EGsHelpContents::None)
	{
		FText Msg;
		FText::FindText(TEXT("UICommonText"), TEXT("UI_HelpContents_CantFindEnum"), Msg);
		TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);

		return;
	}

	if (const FGsSchemaHelpContents* helpContent = UGsTableUtil::FindRowById<UGsTableHelpContents, FGsSchemaHelpContents>(key))
	{	
		FString customeViewId;
		auto zone = Configuration::getZone();
		for (auto& pair : helpContent->customeViewId)
		{
			FString strKey = pair.Key; strKey = strKey.TrimStart();
			if (zone == ZoneType::REAL)
			{
				if (strKey.StartsWith(TEXT("real"), ESearchCase::IgnoreCase))
				{
					customeViewId = FString::FromInt(pair.Value); break;
				}
			}
			else
			{
				if (strKey.StartsWith(TEXT("sandbox"), ESearchCase::IgnoreCase))
				{
					customeViewId = FString::FromInt(pair.Value); break;
				}
			}
		}
		
		GSLOG(Log, TEXT("Server[%s] OpenHelpPage %s[%d] %s"), UTF8_TO_TCHAR(Configuration::getServerId().c_str()), 
			zone == ZoneType::REAL ? TEXT("Real") : TEXT("SandBox"), static_cast<int>(zone), *customeViewId);

		if (!GHive()->IsSignIn())
		{
			GSLOG(Log, TEXT("FGsUIHelper::OpenHelpPage No HiveSignIn"));
			return;
		}

		static int GameResult = 0;
		Promotion::showCustomContents(PromotionCustomType::VIEW, TCHAR_TO_UTF8(*customeViewId), [](ResultAPI result, PromotionEventType promotionEventType) {
			if (result.isSuccess())
			{
				GameResult = 1;
				switch (promotionEventType)
				{
				case PromotionEventType::OPEN:   break; //block 필요하다면 처리
				case PromotionEventType::CLOSE:  break; //block 해지 필요하다면 처리
				}
			}
			else
			{
				GameResult = -1;
				AsyncTask(ENamedThreads::GameThread, []() {
					if (GameResult == -1)
					{
						FText Msg;
						FText::FindText(TEXT("UICommonText"), TEXT("UI_HelpContents_CantGetHiveKey"), Msg);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Msg);
						GameResult = 0;
					}
				});
				
				GSLOG(Log, TEXT("Promotion::showCustomContents Failure %d %s"), result.errorCode, UTF8_TO_TCHAR(result.toString().c_str()));
			}
		});
	}
}

void FGsUIHelper::TrayFadeIn(float InDuration, bool InStartNow)
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
		return;

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return;
		
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			// tray movie 파란 포털 동영상에서
			// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
			fadeEffect->SetRenderOpacity(1.0f);
			fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeIn, FLinearColor::Black, InDuration,
				InStartNow);
			fadeEffect->SetAnimationFinishedCallback([fadeEffect]()
				{
					fadeEffect->Close();
				});
		}
	}	
}

void FGsUIHelper::TrayLobbyFadeIn(float InDuration, bool InStartNow)
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
		return;

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return;

	UGsLobbyDataManager* lobbyDataManager = LSLobbyData();
	if (nullptr == lobbyDataManager)
		return;
	
	if (const UGsLobbyConstant* lobbyConstant = lobbyDataManager->GetLobbyConstantData())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(
			lobbyConstant->_pathTrayFadeEffectUI.Id, lobbyConstant->_pathTrayFadeEffectUI.Path);
		if (nullptr != widget && widget.IsValid())
		{
			if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
			{
				// tray movie 파란 포털 동영상에서
				// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
				fadeEffect->SetRenderOpacity(1.0f);
				fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeIn, FLinearColor::Black, InDuration,
					InStartNow);
				fadeEffect->SetAnimationFinishedCallback([fadeEffect]()
					{
						fadeEffect->Close();
					});
			}
		}
	}
}

void FGsUIHelper::StartTrayFadeInAnimaion()
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
		return;

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return;
			
	FName nameFadeEffect(TEXT("TrayFadeEffect"));
	if (true == uiManager->IsActiveWidget(nameFadeEffect))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayFadeEffect"));
		if (widget.IsValid())
		{
			TWeakObjectPtr<class UGsUITrayFadeEffect> fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get());
			if (fadeEffect.IsValid())
			{
				fadeEffect->StartAnimation();
			}
		}
	}	
}

void FGsUIHelper::CloseTrayFadeIn()
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
		return;

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return;
	
	FName nameFadeEffect(TEXT("TrayFadeEffect"));
	if (true == uiManager->IsActiveWidget(nameFadeEffect))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayFadeEffect"));
		if (widget.IsValid())
		{
			TWeakObjectPtr<class UGsUITrayFadeEffect> fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get());
			if (fadeEffect.IsValid())
			{
				fadeEffect->Close();
			}
		}
	}	
}

void FGsUIHelper::TrayTickerLauncher(const FText& InMessage, bool bInPriority)
{
	if (InMessage.IsEmpty())
		return;

	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
		return;

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return;

	TPair<FName, FSoftObjectPath> tickerPath = uiManager->GetPathTrayTickerLauncher();
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(tickerPath.Key, tickerPath.Value);
	if (widget.IsValid())
	{
		UGsUITrayTickerLauncher* ticker = Cast<UGsUITrayTickerLauncher>(widget.Get());
		if (ticker)
		{
			ticker->SetData(InMessage, bInPriority);
		}
	}
}

void FGsUIHelper::ProcessNetError(uint16 packetId,int32 result)
{
	uint8 typeErrorUI;
	uint8 typeErrorProc; 
	FText errorText;
	GPacketErrorHandler()->ProcessPacket(packetId, result, typeErrorUI, typeErrorProc, errorText);
}

const FSoftObjectPath* FGsUIHelper::GetBadgeIconPath(AccountGrade InAccountType)
{
	switch (InAccountType)
	{
	case AccountGrade::GM:
	{
		if (const auto table = Cast<UGsTableGMConfig>(FGsSchemaGMConfig::GetStaticTable()))
		{
			TArray<const FGsSchemaGMConfig*> gmConfigs;
			table->GetAllRows(gmConfigs);

			if (gmConfigs.IsValidIndex(0))
			{
				const FGsSchemaGMConfig* gmConfig = gmConfigs[0];
				return &gmConfig->gmBadgeIcon;
			}
		}
	}
	break;
	default:
		break;
	}
	return nullptr;
}

void FGsUIHelper::Play2DSound(const FString& InSoundName)
{
	UGsSoundPlayer* SoundPlayer = GSoundPlayer();
	if (nullptr != SoundPlayer)
	{
		SoundPlayer->PlaySound2D(InSoundName);
	}
}

void FGsUIHelper::Play2DSound(const FGsSchemaSoundResData* InSoundResData)
{
	if (nullptr == InSoundResData)
	{
		return;
	}

	UGsSoundPlayer* SoundPlayer = GSoundPlayer();
	if (nullptr != SoundPlayer)
	{
		SoundPlayer->PlaySound2D(InSoundResData);
	}
}
