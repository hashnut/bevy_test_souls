// Fill out your copyright notice in the Description page of Project Settings.

#include "GsCheatManager.h"
#include "Engine.h"
#include "Internationalization/Internationalization.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"
#include "Trigger/CinemaTriggerActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "DrawDebugHelpers.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedInteractionEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"

#include "ActorEx/GsCharacterNonPlayer.h"
#include "ActorEx/GsActorFairyObject.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"
#include "ActorEx/GsCamp.h"

#include "Animation/GsCampAnimInstance.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceItem.h"
#include "Net/MessageHandler/GsNetMessageHandlerGameObject.h"

#include "GameMode/GsGameModeWorld.h"
#include "GameMode/GsGameModeTestClient.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsPhoneInfoManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsDropObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsStarLegacyManager.h"

#include "Input/GsInputBindingLocalPlayer.h"
#include "Input/GsInputEventMsgBase.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"

#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Environment/GsAreaEnvHandler.h"
#include "GameObject/GsSchemaShapeData.h"
#include "GameObject/Prop/GsSchemaPropData.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerBase.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GameObject/Boss/GsFieldBossHandler.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "AI/Function/GsAIFunc.h"
#include "AI/Data/GsAIDefine.h"
#include "AI/GsAIStateManager.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "AI/Quest/GsAIQuestHandler.h"

//RSQL
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/Cheat/GsSchemaCheatSet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyMoveLocation.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "DataSchema/Arena/GsSchemaArenaData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/ItemCollection/GsSchemaItemCollection.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsResourceManager.h"
#include "Quest/GsQuestData.h"
#include "Quest/Management/GsQuestActionManagement.h"

//SignificanceManager
#include "Significance/GsSignificanceManager.h"
#include "Camera/LookAt/GsCameraLookAtHandlerBase.h"
#include "Camera/Define/GsCameraGameDefine.h"
#include "Camera/Define/GsCameraGameDefine.h"

#include "GSGameInstance.h"
#include "T1Project.h"

#include "Trigger/BaseTriggerActor.h"
#include "Cinema/GsTargetRotateCamera.h"

#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsCameraMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "Util/GsItemUtil.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsDrawDebugHelpers.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsCronUtil.h"
#include "UTIL/GsDir.h"

#include "Sound/GsSoundPlayer.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDAuto.h"
#include "UI/UIContent/HUD/GsUIHUDScanButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "UI/UIContent/Tray/NotiMessage/GSUISectionMessageDefine.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupRevive.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"
#include "UI/ScaleRule/GsUIScaleRule.h"
#include "UI/UIContent/Popup/GsUIPopupAccountDelete.h"

#include "Option/GsGameUserSettings.h"
#include "Option/GsServerOption.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Blueprint/WidgetTree.h"

#include "ActorComponentEx/GsEffectComponent.h"
#include "Particles/ParticleSystem.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

#include "ActorEx/GsWeaponActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "Sound/SoundMix.h"
#include "AudioDevice.h"
#include "Sound/GsSoundMixerController.h"
#include "Sequence/GsSequencePlayer.h"
#include "Sequence/GsSequenceManager.h"
#include "UI/UIContent/Tray/GsUITrayTickerStatChange.h"
#include "UI/UIContent/Tray/StatChange/GsUIStatChangeData.h"
#include "Net/GsNetSendServiceBMShop.h"
#include "DataSchema/BMShop/GsSchemaBMShopProductData.h"
#include "HIVE_IAPV4.h"



bool UGsCheatManager::IsPlayerMighty = false;

bool UGsCheatManager::IsShowSpawnLocation = false;

bool UGsCheatManager::IsShowCreatureDestLocation = false;
const float PLAYER_WALK_SPEED = 600.0f;

bool UGsCheatManager::_isShowAnimURO = false;

bool UGsCheatManager::_IsShowNaviMeshInfo = false;
bool UGsCheatManager::_isShowZoneRange = false;
bool UGsCheatManager::_isShowNpcTargetSightRadius = false;
bool UGsCheatManager::_isShowNpcTargetInfo = false;

bool UGsCheatManager::_isShowEnterCommands = false;
TArray<FString> UGsCheatManager::_listCheatEnterCommands;

bool UGsCheatManager::IsEffectEmptyPlayer = true;

void UGsCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

	// 치트클래스 추가 시 UObject상속 받아 생성하고
	// 아래 list에 추가해줍니다.

	// 치트 클래스 추가 생성
	if (0 >= _listCheatGroup.Num())
	{
		//_listCheatGroup.Add(NewObject<UGsCheatNet>(this));
	}	
}

void UGsCheatManager::BeginDestroy()
{
	Super::BeginDestroy();

	// 치트클래스 삭제
	for (UObject* cheatObject : _listCheatGroup)
	{
		if (nullptr != cheatObject)
		{
			cheatObject->ConditionalBeginDestroy();
		}
	}
}

// 치트 메세지를 받아 추가 생성해 치트클래스로 넘겨줍니다.
bool UGsCheatManager::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool handled = Super::ProcessConsoleExec(Cmd, Ar, Executor);
	if (false == handled)
	{
		for (UObject* cheatObject : _listCheatGroup)
		{
			handled = cheatObject->ProcessConsoleExec(Cmd, Ar, Executor);
		}
	}

	if (handled == false)
	{
		// 엔진/클라에 존재하지 않는 치트키
		// 무조건 서버로 보낸다.
		FGsNetSendService::SendWorldAdminCommandQA(Cmd);
	}

	_listCheatEnterCommands.Emplace(Cmd);

	return handled;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////	 ↓ ↓ ↓ ↓		치트키는 아래로 추가		↓ ↓ ↓ ↓			//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//bool UGsCheatManager::SearchChildren(UWidget* InWidget, TArray<UTextBlock*>& InSearchTextBlocks)
//{
//	if (InWidget->GetParent()->GetChildrenCount() > 0)
//	{
//		for (int32 i = 0; InWidget->GetParent()->GetChildrenCount() > i; ++i)
//		{
//			SearchChildren(InWidget->GetParent()->GetChildAt(i), InSearchTextBlocks);
//		}
//
//		return true;
//	}
//	else
//	{
//		UTextBlock* TextBlock = Cast<UTextBlock>(InWidget);
//		if (TextBlock)
//		{
//			InSearchTextBlocks.Add(TextBlock);
//		}
//
//		return false;
//	}
//}

void UGsCheatManager::PrintUILocalization(FString InUIName)
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

	const FString rootPaths = FString(TEXT("/Game/Blueprint/UI/"));
	TArray<FString> outSubPaths;
	AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(rootPaths), outSubPaths, true);

	TArray<FAssetData> AllAssets;
	FARFilter Filter;
	for (int32 PathIdx = 0; PathIdx < outSubPaths.Num(); ++PathIdx)
	{
		Filter.PackagePaths.Add(*outSubPaths[PathIdx]);
	}

	UUserWidget* UserWidget = nullptr;
	AssetRegistryModule.Get().GetAssets(Filter, AllAssets);
	for (int32 AssetIdx = 0; AssetIdx < AllAssets.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AllAssets[AssetIdx];
		if (0 == AssetData.AssetName.ToString().Compare(InUIName))
		{
			FSoftObjectPath WidgetPath = FSoftObjectPath(AssetData.ObjectPath);
			UClass* uclass = GResource()->LoadSync<UClass>(WidgetPath, true);

			UserWidget = CreateWidget<UUserWidget>(GetWorld(), uclass);
			break;
		}
	}

	TArray<UWidget*> SearchAllWidgets;
	UserWidget->WidgetTree->GetAllWidgets(SearchAllWidgets);

	// 현재 사용중인 Text UI들(새로 추가될때마다 추가해줘야된다. 현재 알려진것만 추가)
	TArray<UTextBlock*> TextBlocks;
	TArray<URichTextBlock*> RichTextBlocks;
	TArray<UEditableTextBox*> EditableTextBoxs;
	TArray<UEditableText*> EditableTexts;
	for (int32 i = 0; i < SearchAllWidgets.Num(); ++i)
	{
		UTextBlock* TextBlock = Cast<UTextBlock>(SearchAllWidgets[i]);
		if (TextBlock)
		{
			TextBlocks.Add(TextBlock);
		}

		URichTextBlock* RichTextBlock = Cast<URichTextBlock>(SearchAllWidgets[i]);
		if (RichTextBlock)
		{
			RichTextBlocks.Add(RichTextBlock);
		}

		UEditableTextBox* EditableTextBox = Cast<UEditableTextBox>(SearchAllWidgets[i]);
		if (EditableTextBox)
		{
			EditableTextBoxs.Add(EditableTextBox);
		}

		UEditableText* EditableText = Cast<UEditableText>(SearchAllWidgets[i]);
		if (EditableText)
		{
			EditableTexts.Add(EditableText);
		}
	}

	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  Print UTextBlock "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));	
	GSALOG(Log, TEXT("\t UTextBlock Count : [%d]"), TextBlocks.Num());
	GSALOG(Log, TEXT("\t[Text::String] \t[Namespace] \t[Key] \t[FName]"));
	for (int i = 0; i < TextBlocks.Num(); ++i)
	{
		UTextBlock* TextBlock = TextBlocks[i];
		FText text = TextBlock->GetText();

		const FString SavedPackageNamespace = FTextInspector::GetNamespace(text).Get(FString());
		const FString CurrentKey = FTextInspector::GetKey(text).Get(FString());
		const FString fName = TextBlock->GetFName().ToString();

		GSALOG(Log, TEXT("\t[%s] \t[%s] \t[%s] \t[%s]"), *text.ToString(), *SavedPackageNamespace, *CurrentKey, *fName);
		
		GSALOG(Log, TEXT(""));
	}
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  End UTextBlock "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));

	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  Print URichTextBlock "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));
	GSALOG(Log, TEXT("\t URichTextBlock Count : [%d]"), RichTextBlocks.Num());
	GSALOG(Log, TEXT("\t[Text::String] \t[Namespace] \t[Key] \t[FName]"));
	for (int i = 0; i < RichTextBlocks.Num(); ++i)
	{
		URichTextBlock* RichTextBlock = RichTextBlocks[i];
		FText text = RichTextBlock->GetText();

		const FString SavedPackageNamespace = FTextInspector::GetNamespace(text).Get(FString());
		const FString CurrentKey = FTextInspector::GetKey(text).Get(FString());
		const FString fName = RichTextBlock->GetFName().ToString();

		GSALOG(Log, TEXT("\t[%s] \t[%s] \t[%s] \t[%s]"), *text.ToString(), *SavedPackageNamespace, *CurrentKey, *fName);

		GSALOG(Log, TEXT(""));
	}
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  End URichTextBlock "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));

	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  Print UEditableTextBox "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));
	GSALOG(Log, TEXT("\t UEditableTextBox Count : [%d]"), EditableTextBoxs.Num());
	GSALOG(Log, TEXT("\t[Text::String] \t[Namespace] \t[Key] \t[FName]"));
	for (int i = 0; i < EditableTextBoxs.Num(); ++i)
	{
		UEditableTextBox* EditableTextBox = EditableTextBoxs[i];
		FText text = EditableTextBox->GetText();

		const FString SavedPackageNamespace = FTextInspector::GetNamespace(text).Get(FString());
		const FString CurrentKey = FTextInspector::GetKey(text).Get(FString());
		const FString fName = EditableTextBox->GetFName().ToString();

		GSALOG(Log, TEXT("\t[%s] \t[%s] \t[%s] \t[%s]"), *text.ToString(), *SavedPackageNamespace, *CurrentKey, *fName);

		GSALOG(Log, TEXT(""));
	}
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  End UEditableTextBox "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));

	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  Print UEditableText "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));
	GSALOG(Log, TEXT("\t UEditableText Count : [%d]"), EditableTexts.Num());
	GSALOG(Log, TEXT("\t[Text::String] \t[Namespace] \t[Key] \t[FName]"));
	for (int i = 0; i < EditableTexts.Num(); ++i)
	{
		UEditableText* EditableText = EditableTexts[i];
		FText text = EditableText->GetText();

		const FString SavedPackageNamespace = FTextInspector::GetNamespace(text).Get(FString());
		const FString CurrentKey = FTextInspector::GetKey(text).Get(FString());
		const FString fName = EditableText->GetFName().ToString();

		GSALOG(Log, TEXT("\t[%s] \t[%s] \t[%s] \t[%s]"), *text.ToString(), *SavedPackageNamespace, *CurrentKey, *fName);

		GSALOG(Log, TEXT(""));
	}
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT("  End UEditableText "));
	GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
	GSALOG(Log, TEXT(""));

	if (UserWidget)
	{
		UserWidget->RemoveFromRoot();
		UserWidget = nullptr;
	}
}

void UGsCheatManager::ShowSpawnLocation(bool in_show)
{
	IsShowSpawnLocation = in_show;
}

void UGsCheatManager::CurrLoc()
{
	if (GEngine)
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			FVector currLoc = local->GetLocation();
			const FString locString = FString::Printf(
				TEXT("CurrentLocation [X:%.2f / Y:%.2f / Z:%.2f]"),
				currLoc.X, currLoc.Y, currLoc.Z
			);
			FVector2D textScale{ 2.f, 2.f };
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, locString, true, textScale);
		}
	}
}

void UGsCheatManager::LocalPlayerTargetClear()
{
	check(GSGameObject());

	if (UGsGameObjectBase * local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (auto Target = local->GetTargetHandler())
		{
			Target->ClearTarget(true);
		}
	}
}

void UGsCheatManager::PrintItemListLog()
{
	if (nullptr != GItem())
	{
		GItem()->PrintItemListLog();
	}
}

void UGsCheatManager::PrintEquipedItemListLog()
{
	if (nullptr != GItem())
	{
		GItem()->PrintEquipedItemListLog();
	}
}

void UGsCheatManager::SetUIVisible(bool InVal)
{
	FGsUIMsgParamBool param(InVal);
	GMessage()->GetUI().SendMessage(MessageUI::HIDE_UI_FOR_MOVIE, &param);
}

void UGsCheatManager::OpenUI(FString InUIPathTableKey)
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Open(FName(*InUIPathTableKey));
	}
}

void UGsCheatManager::CloseUI(FString InUIPathTableKey)
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(FName(*InUIPathTableKey));
	}
}

void UGsCheatManager::NpcDialogVelocity(uint32 inNpcDialogID, float inVelocity)
{
	if (FGsQuestHandler* handler = GSQuest()->GetQuestHandler())
	{
		if (FGsQuestActionManagement* action = handler->GetQuestActionManagement())
		{
			action->OpenNpcDialog(inNpcDialogID, inVelocity);
		}
	}
}

void UGsCheatManager::NpcDialogTarget(uint32 inNpcDialogID, int32 npcTblId)
{
	if (FGsQuestHandler* handler = GSQuest()->GetQuestHandler())
	{
		if (FGsQuestActionManagement* action = handler->GetQuestActionManagement())
		{
			action->OpenNpcDialog(inNpcDialogID, npcTblId);
		}
	}
}

void UGsCheatManager::GetRes()
{
	if (GEngine)
	{
		const FString resolutionStr = FString::Printf(
			TEXT("Resolution [X:%d / Y:%d]"), GSystemResolution.ResX, GSystemResolution.ResY
		);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, resolutionStr);
	}
}

void UGsCheatManager::SetSignificanceDistanceForVFX(float inDist)
{
}

#include "Net/GsNetSendServiceWorld.h"
void UGsCheatManager::PlayerRevive(int InType)
{
	FGsNetSendServiceWorld::SendResurrect((ResurrectType)InType);
}

#if WITH_EDITOR

void UGsCheatManager::StartSkill(int skillId)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
		FGsSkillHandlerLocalPlayer* skillHandler = castLocal->GetCastSkill<FGsSkillHandlerLocalPlayer>();
		skillHandler->OnSkill(skillId);
	}
}

void UGsCheatManager::ChangeBattleStatus(bool ApplyTarget)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectBase* targetObj = local;
		if (ApplyTarget)
		{
			FGsTargetHandlerBase* targetHandler = local->GetTargetHandler();
			targetObj = targetHandler->GetTarget();
		}

		if (nullptr == targetObj)
		{
			return;
		}

		FGsGameObjectDataCreature* creatureData = targetObj->GetCastData<FGsGameObjectDataCreature>();
		UserStatus currType = creatureData->GetUserStatus();
		creatureData->SetUserStatus((currType == UserStatus::USER_STATE_COMBAT) ?
			UserStatus::USER_STATE_NORMAL : UserStatus::USER_STATE_COMBAT);

		targetObj->CastGameObject<UGsGameObjectCreature>()->UpdateCombatStatus();
	}
}

void UGsCheatManager::ChangeWeaponType(CreatureWeaponType Type)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectPlayer* castObj = local->CastGameObject<UGsGameObjectPlayer>();
		// 현재 무기타입 얻기
		TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (item.IsValid())
		{
			FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			CreatureWeaponType weaponType = equipItem->GetCreatureWeaponType();

			// 무기 교체 로직 처리
			// 테스트용
			TArray<TWeakPtr<FGsItem>> items = GItem()->FindItem(ItemType::EQUIP);
			FGsItemEquipment* changeequipItem = nullptr;
			for (TWeakPtr<FGsItem> el : items)
			{
				if(false == el.IsValid())
					continue;

				if (el.Pin()->GetMainCategory() != ItemCategoryMain::WEAPON || equipItem->GetDBID() == el.Pin()->GetDBID())
				{
					continue;
				}

				FGsItemEquipment* castItem = el.Pin()->GetCastItem<FGsItemEquipment>();
				if (castItem->GetCreatureWeaponType() == Type && false == el.Pin()->IsEquipped())
				{
					changeequipItem = castItem;
					break;
				}
			}

			if (changeequipItem)
			{
				// 패킷 전송
				if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(local,
					CommonActionType::WEAPON_CHANGE))
				{
					FGsNetSendServiceItem::SendItemEquipChangeWeapon(changeequipItem->GetDBID(), commonSkill->totalRuntime);
				}
			}
		}
	}
}

void UGsCheatManager::SocialStateEvent(int TId, int SocialId)
{
	UGsGameObjectBase* gameObject = nullptr;
	if (TId == 0)
	{
		gameObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	}
	else
	{
		TArray<UGsGameObjectBase*> list = GSGameObject()->FindObjectNonPlayerByNPCId(TId);
		if (list.Num() > 0)
		{
			gameObject = list[0];
		}
	}

	if (gameObject)
	{
		FGsSkillHandlerBase* skillHandler = gameObject->GetSkillHandler();
		skillHandler->OnSocial(SocialId);
	}
}

void UGsCheatManager::StopSocialState(int TId)
{
	UGsGameObjectBase* gameObject = nullptr;
	if (TId == 0)
	{
		gameObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	}
	else
	{
		TArray<UGsGameObjectBase*> list = GSGameObject()->FindObjectNonPlayerByNPCId(TId);
		if (list.Num() > 0)
		{
			gameObject = list[0];
		}
	}

	if (gameObject)
	{
		FGsSkillHandlerBase* skillHandler = gameObject->GetSkillHandler();
		skillHandler->StopSocial();
	}
}

void UGsCheatManager::ObjectCollisionDebugShape(bool IsShow)
{
	GSGameObject()->_isCollisionDebugShape = IsShow;
}

void UGsCheatManager::ObjectRootmotionDebugShape(bool IsShow)
{
	GSGameObject()->_isRootmotionDebugShape = IsShow;
}
#endif

void UGsCheatManager::ShowNpcState(int InInfoType, int InNpcId)
{
#if	!UE_BUILD_SHIPPING

	bool isShow = (InInfoType == 0) ? false : true;

	GSGameObject()->_isShowNpcState = isShow;

	// Npc 의 디버그 정보창 Visible 여부를 변경해 준다
	TArray<class UGsGameObjectBase*> npcArray = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);
	
	for (UGsGameObjectBase* npc : npcArray)
	{
		if (UGsGameObjectNonPlayer* npcObj = Cast<UGsGameObjectNonPlayer>(npc))
		{
			if (0 != InNpcId && npcObj->GetNpcTblId() != InNpcId)
			{
				continue;
			}

			npcObj->ShowDebugTextInfo(isShow);
		}
	}

	FString command = FString::Printf(TEXT("showNpcState %d %d"), InInfoType, InNpcId);
	FGsNetSendService::SendWorldAdminCommandQA(command);

#endif
}

void UGsCheatManager::ShowIsPlayerDestLocationValid(bool InDraw)
{
	UGsQueryFloorHeightComponent::CanDrawCharacterNavmesh = InDraw;
}

void UGsCheatManager::SetPlayerOnLand()
{
	if (UGsGameObjectBase * local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer * player = Cast<UGsGameObjectLocalPlayer>(local))
		{
			if (AActor * actor = player->GetActor())
			{
				if (AGsCharacterLocalPlayer * character = Cast<AGsCharacterLocalPlayer>(actor))
				{
					FHitResult hit;

					if (UGsLevelUtil::TryLineTraceToLand(hit, actor->GetWorld(), actor->GetActorLocation()))
					{
						actor->SetActorLocation(hit.Location);
					}
				}
			}
		}
	}
}

void UGsCheatManager::ShowNpcDestLocation(bool inShow)
{
	IsShowCreatureDestLocation = inShow;
}

void UGsCheatManager::DrawDebugCreatureDestLocation(const UGsGameObjectBase* inNpc, const FVector& inDestLocation)
{
	UGsGameObjectBase* playerObj = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (nullptr == playerObj)
	{
		return;
	}

	if (playerObj == inNpc)
	{
		return;
	}

	AActor* player = playerObj->GetActor();

	if (nullptr == player)
	{
		return;
	}

	if (nullptr == inNpc)
	{
		return;
	}

	AActor* actor = inNpc->GetActor();

	if (nullptr == actor)
	{
		return;
	}

	UWorld* world = actor->GetWorld();

	if (nullptr == world)
	{
		return;
	}

	float dist = (player->GetActorLocation() - actor->GetActorLocation()).Size();
	float cullDistance = DEFAULT_CULLING_DISTANCE;

	if (GData())
	{
		if (GData()->GetGlobalData())
		{
			cullDistance = GData()->GetGlobalData()->ClientCullingDistance;
		}
	}

	if (dist < cullDistance)
	{
		FString gameId = FString::Printf(TEXT("%lld"), inNpc->GetGameId());
		FString destLocationText = "game id : " + gameId + "\npos : " + actor->GetActorLocation().ToString();
		FVector offset = inDestLocation - actor->GetActorLocation();
		DrawDebugString(world, offset, destLocationText, actor, FColor::White, 3.0, false, 1);

		DrawDebugLine(world, actor->GetActorLocation(), inDestLocation, FColor::Yellow, false, 3.0f, 0, 5.0);
	}
}

void UGsCheatManager::PlayMovie(FString inName)
{
	UGsUITrayMovie::PlayMovie(inName);
}

void UGsCheatManager::StopMovie()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("TrayMovie"));
	if (widget.IsValid())
	{
		if (UGsUITrayMovie * movie = Cast<UGsUITrayMovie>(widget.Get()))
		{
			movie->Stop();
			movie->Close();
		}
	}
}

void UGsCheatManager::CloseMovie()
{
	GUI()->CloseByKeyName(TEXT("TrayMovie"));
}

void UGsCheatManager::ChangeCulture(const FString& in_culture)
{
	FInternationalization::Get().SetCurrentCulture(in_culture);
}

void UGsCheatManager::GetHWInfo()
{
	// 1. 배터리 레벨
	int batteryLevel = 0;
#if PLATFORM_ANDROID
	batteryLevel = FAndroidMisc::GetBatteryLevel();
#elif PLATFORM_IOS
	batteryLevel = FIOSPlatformMisc::GetBatteryLevel();
#endif

	// 2. 네크워크 타입
	uint8 networkType = 0;
#if PLATFORM_ANDROID
	networkType = static_cast<uint8>(FAndroidMisc::GetNetworkConnectionType());
#elif PLATFORM_IOS
	networkType = static_cast<uint8>(FIOSPlatformMisc::GetNetworkConnectionType());
#endif

	// 3. 온도
	float temp = 0.f;
	uint8 currentTempLevel = 0;
#if PLATFORM_ANDROID
	temp = FAndroidMisc::GetDeviceTemperatureLevel();
	if (temp < 34.0f)		currentTempLevel = 0;
	else if (temp < 36.5f)	currentTempLevel = 1;
	else if (temp < 39.0f)	currentTempLevel = 2;
	else					currentTempLevel = 3;
#elif PLATFORM_IOS
	temp = FIOSPlatformMisc::GetDeviceTemperatureLevel();
	currentTempLevel = static_cast<int8>(temp);
#endif

	// 4. Vulkan Support
	bool vulkanSupport = false;
#if PLATFORM_ANDROID
	vulkanSupport = FAndroidMisc::IsVulkanAvailable();
#endif

	// 5. 현재 시간
	FDateTime now = FDateTime::Now();
	int32 hour = now.GetHour();
	int32 minute = now.GetMinute();
	int32 second = now.GetSecond();

	if (GEngine)
	{
		const FString hwStatusString = FString::Printf(
			TEXT("Battery[%d], NetworkType[%d], Temp[%f]/TempLevel[%d], Vulkan[%d], Time[%d:%d:%d]"),
			batteryLevel,
			networkType,
			temp, currentTempLevel,
			vulkanSupport,
			hour, minute, second
		);
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, hwStatusString);
	}
}

void UGsCheatManager::TestDespawnAll()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::GAMEOBJECT_REMOVE_ALL);
}
// npc 디스폰(tbl id로)
void UGsCheatManager::DespawnNpcByTblId(int In_tblId)
{
	GSClientSpawn()->DespawnNpcByTblId(In_tblId);
}

void UGsCheatManager::DespawnNpcByClient(float inRadius)
{
	UGsGameObjectBase* playerGameObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	TArray<class UGsGameObjectBase*> npcArray = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(playerGameObject);
	FVector playerLocation = localPlayer->GetActor()->GetActorLocation();
	for(UGsGameObjectBase* gameObject : npcArray)
	{
		if(AActor* actor = gameObject->GetActor())
		{
			if((actor->GetActorLocation() - playerLocation).Size() < inRadius)
			{
				GSAI()->RemoveMannerCheckTarget(gameObject->GetGameId());
				GSGameObject()->DespawnObject(gameObject);
			}
		}		
	}
}

void UGsCheatManager::TestDisconnect()
{
	TSharedPtr<FGsNetBase> active = GNet()->GetActive();
	if (active.IsValid())
	{
		active->Disconnect(true);
	}
}

#if WITH_EDITOR
void UGsCheatManager::SetNpcCapsule(FString inName, float inRadius, float inHalfHeight)
{
	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGsCharacterNonPlayer::StaticClass(), actors);

	for (AActor* temp : actors)
	{
		if (AGsCharacterNonPlayer* character = Cast<AGsCharacterNonPlayer>(temp))
		{
			if (character->GetActorLabel().Equals(inName))
			{
				character->GetCapsuleComponent()->SetCapsuleSize(inRadius, inHalfHeight);
			}
		}
	}
}
#endif

void UGsCheatManager::ShowAnimURO(bool inIsShow)
{
	_isShowAnimURO = inIsShow;
}

void UGsCheatManager::SetPhysicsAnimation(bool inOn)
{
	if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
		UGsAnimInstanceState* anim = castLocal->GetAnimInstance();
		anim->SetPhysicsAnimation(inOn);
	}
}


#if WITH_EDITOR
void UGsCheatManager::SetTriggerVisible(bool inVisible /*= true*/)
{
	TArray<AActor*> triggers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTriggerActor::StaticClass(), triggers);

	for (AActor* temp : triggers)
	{
		if (temp)
		{
			if (ABaseTriggerActor* trigger = Cast<ABaseTriggerActor>(temp))
			{
				trigger->SetVisible(inVisible);
			}
		}
	}
}

void UGsCheatManager::ActiveTargetRotateCamera(FString inCameraName, bool inInit /*= true*/)
{
	TArray<AActor*> cameraArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGsTargetRotateCamera::StaticClass(), cameraArray);

	for (AActor* actor : cameraArray)
	{
		if (actor)
		{
			if (actor->GetActorLabel().Equals(inCameraName))
			{
				if (AGsTargetRotateCamera* camera = Cast<AGsTargetRotateCamera>(actor))
				{
					camera->Active(inInit);
				}
			}
		}
	}
}

void UGsCheatManager::DeactiveTargetRotateCamera(FString inCameraName)
{
	TArray<AActor*> cameraArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGsTargetRotateCamera::StaticClass(), cameraArray);

	for (AActor* actor : cameraArray)
	{
		if (actor)
		{
			if (actor->GetActorLabel().Equals(inCameraName))
			{
				if (AGsTargetRotateCamera* camera = Cast<AGsTargetRotateCamera>(actor))
				{
					camera->Deactive();

					return;
				}
			}
		}
	}
}
#endif


// 테스트 hit 대사 그리기
void UGsCheatManager::DrawTestHitDialog(bool In_val)
{
	GSCameraMode()->DrawTestActive(In_val);	
}

void UGsCheatManager::CameraDebugLine()
{
	GMessage()->GetCamera().SendMessage(MessageContentCamera::DEBUG_DRAW_LOOK_AXIS_LINE);
}

// 대사 연출 카메라 시작
void UGsCheatManager::StartDialogCam(int In_viewDataId, int In_npcTblId, int In_addNpcTblId)
{
	UGsGameObjectBase* targetNpc =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(In_npcTblId);
	if (nullptr == targetNpc)
	{
		return;
	}

	TArray<UGsGameObjectBase*> arrTarget;
	arrTarget.Add(targetNpc);

	if (In_addNpcTblId != 0)
	{
		UGsGameObjectBase* targetNpc2 =
			GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId( In_addNpcTblId);

		if (targetNpc2 != nullptr)
		{
			arrTarget.Add(targetNpc2);
		}
	}
	const FGsSchemaCameraDialogViewData* viewData = UGsTableUtil::FindRow<FGsSchemaCameraDialogViewData>(In_viewDataId);
	FGsCameraDialogMessageParam cameraDialogParam(arrTarget, EGsCamearDialogEndCallBackType::None,
		viewData);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_START, &cameraDialogParam);
}
// 대사 연출 카메라 끝
void UGsCheatManager::EndDialogCam()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_END, nullptr);
}
// 대사 연출 카메라 변경
void UGsCheatManager::ChangeDialogModeView(int In_viewDataId, int In_npcTblId, int In_addNpcTblId)
{
	UGsGameObjectBase* targetNpc =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(In_npcTblId);
	if (nullptr == targetNpc)
	{
		return;
	}

	TArray<UGsGameObjectBase*> arrTarget;
	arrTarget.Add(targetNpc);

	if (In_addNpcTblId != 0)
	{
		UGsGameObjectBase* targetNpc2 =
			GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(In_addNpcTblId);

		if (targetNpc2 != nullptr)
		{
			arrTarget.Add(targetNpc2);
		}
	}
	const FGsSchemaCameraDialogViewData* viewData = UGsTableUtil::FindRow<FGsSchemaCameraDialogViewData>(In_viewDataId);
	FGsCameraDialogMessageParam cameraDialogParam(arrTarget, EGsCamearDialogEndCallBackType::None,
		viewData);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_MODE_VIEW_CHANGE, &cameraDialogParam);
}
// 대사 스폰 위치
void UGsCheatManager::ShowDialogSpawnPos(int In_targetNpcId, float In_rotate, float In_range)
{
	UGsGameObjectBase* targetNpc =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(In_targetNpcId);
	if (nullptr == targetNpc)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	FVector targetNpcPos = targetNpc->GetLocation();
	// 기준 라인
	FVector npcLookVec = FVector::ForwardVector;
	// 기본 방향 위치
	FVector baseDirPos = targetNpcPos - npcLookVec * 600.0f;
	DrawDebugLine(world, targetNpcPos, baseDirPos, FColor::Red, false, 10.0f);

	
	// 스폰 라인
	FVector npcDir =
		npcLookVec.RotateAngleAxis(In_rotate, FVector::UpVector);
	FVector newNpcPos = targetNpcPos - npcDir * In_range;

	// 스폰될 방향 그리기
	DrawDebugLine(world, targetNpcPos, newNpcPos, FColor::Green, false, 10.0f);

	// 스폰 위치 그리기
	DrawDebugCircle(world, newNpcPos,
		100, 100, FColor::Green, false, 10.0f,
		0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
}

// 컷 페이드 사용하는가
void UGsCheatManager::NpcDialogCutFadeOutUse(int In_use)
{
	GSCameraMode()->SetIsUseCutFadeOut((bool)In_use);
}

// 카메라 본 관련 출력할지
void UGsCheatManager::ShowCameraBoneLog(int In_use)
{
	if (FGsGameDataManager* dataManager = GGameData())
	{
		bool isShow = (In_use == 1) ? true : false;
		dataManager->SetIsShowTestCamBoneLog(isShow);
	}
}

// 인터랙션
// 범위 그릴지
void UGsCheatManager::ShowInteractionRange(bool In_isvisible)
{
	FGsPrimitiveInt32 param(static_cast<int32>(In_isvisible));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_DEBUG_DRAW_RANGE, &param);
}

void UGsCheatManager::ShowTestInteractionButtonPos(bool In_val)
{
	GSInteraction()->SetTestShowInteractionButtonPos(In_val);
}

void UGsCheatManager::ShowInteracionSphere(bool In_val)
{
	UGsGameObjectManager* manager = GSGameObject();
	if (manager == nullptr)
	{
		return;
	}
	UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer);
	if (findLocal == nullptr)
	{
		return;
	}
	UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsInteractionHandlerLocalPlayer* interationHandler = castLocal->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (interationHandler == nullptr)
	{
		return;
	}
	interationHandler->SetIsTestShowInteractionSphere(In_val);
	
}

void UGsCheatManager::PrintServerTime()
{
	GNet()->PrintServerTime();
}

// 노드 위치 그릴지
void UGsCheatManager::ShowWaypointNodePos(bool In_isDraw)
{
	if (UGsGameObjectBase* base = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* local = Cast<UGsGameObjectLocalPlayer>(base);
		if (nullptr != local)
		{
			FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			if (movement != nullptr)
			{
				movement->SetIsDrawWaypointDebugPos(In_isDraw);
			}
		}
	}
}
// 노드 중간 으로 갈지
void UGsCheatManager::WaypointIsCenterNode(bool In_isCenter)
{
	GSAI()->GetAIQuestHandler()->SetIsCenterNode(In_isCenter);
}
// 웨이포인트 테스트 위치로 이동
void UGsCheatManager::TestWaypointMove()
{
	if (UGsGameObjectBase* base = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* local = Cast<UGsGameObjectLocalPlayer>(base);
		if (nullptr != local)
		{
			FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			if (movement != nullptr)
			{
				int currMapId = GLevel()->GetCurrentLevelId();
				movement->ChangeAutoMoveState().StartMapIdPositions(local,
					currMapId,
					GData()->GetGlobalData()->_arrTestPos,
					nullptr,
					nullptr,
					GSAI()->GetAIQuestHandler()->GetIsCenterNode());
			}
		}
	}
}

void UGsCheatManager::WarpSpot(int32 inLevelID, int32 inSpotIndex)
{
	FString levelKeyString = FString::FromInt(inLevelID);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKeyString);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaSpotData* spotData;
	table->FindRow<FGsSchemaSpotData>(*levelKeyString, spotData);

	if (nullptr == spotData)
	{
		GSLOG(Error, TEXT("Missing level key : %d"), inLevelID);

		return;
	}

	const FGsSchemaSpotInfo* spot = spotData->SpotList.FindByPredicate([&](const FGsSchemaSpotInfo& inSpotInfo)->bool {
		return (inSpotInfo.id == inSpotIndex);
	});
	if (nullptr == spot)
	{
		GSLOG(Error, TEXT("Missing spot index : %d"), inSpotIndex);

		return;
	}

	FString command = FString::Printf(TEXT("warp %d %d %d %d"), inLevelID, (int32)spot->pos.X, (int32)spot->pos.Y, (int32)spot->pos.Z);
		
	FGsNetSendService::SendWorldAdminCommandQA(command);
}


void UGsCheatManager::WarpCurrentLevelSpot(int32 inSpotIndex)
{
	if (nullptr == GLevel())
	{
		return;
	}

	WarpSpot(GLevel()->GetCurrentLevelId(), inSpotIndex);
}

// 서브다이얼로그
void UGsCheatManager::OpenSubDialog(int32 inSubDialogId)
{
	if (FGsQuestHandler* handler = GSQuest()->GetQuestHandler())
	{
		if (FGsQuestActionManagement* action = handler->GetQuestActionManagement())
		{
			action->OpenSubDialog(inSubDialogId);
		}
	}
}

// 챕터 오픈 티커
void  UGsCheatManager::ShowCapterOpenTicker(int64 in_questId)
{
	if (FGsQuestHandler* handler = GSQuest()->GetQuestHandler())
	{
		if (FGsQuestActionManagement* action = handler->GetQuestActionManagement())
		{
			action->PalyChapterStartTicker(in_questId);
		}
	}
}

void UGsCheatManager::QuestClear()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->ClearQuest();
	questManager->AddNextStoryQuest(questManager->GetFirstQuestId());
	questManager->SetSubNpcTartget();

	FString command = FString::Printf(TEXT("questClear"));
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::CompleteQuest(int64 in_questId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(in_questId);
	if (nullptr == questData || !questData.IsValid())
	{
		FGsUIHelper::TrayMessageTicker(FText::FromString(
			FString::Format(TEXT("Error : Could not find questID ({%llu})"), { in_questId })));
		return;
	}
		
	QuestId questId = in_questId;
	const QuestType questType = questManager->GetQuestType(questId);
	if (questType == QuestType::MAIN)
	{
		questManager->ClearQuestMain();
		questManager->SetSubNpcTartget();
	}
	else if (questType == QuestType::SUB)
	{
		questManager->ClearQuestSub(questId);
		questManager->SetSubNpcTartget();
	}
	else if (questType == QuestType::MULTI_LEVEL_RANK)
	{
		questManager->ClearQuestRank(questId);
	}

	FString command = FString::Printf(TEXT("CompleteQuest %llu"), questId);
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::GotoQuestSubNpc(int64 in_questId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->CheatGotoQuestSubNpc(static_cast<QuestId>(in_questId));
}

void UGsCheatManager::GiveUpQuest(int32 inStoryId)
{
	FGsStoryParam param(inStoryId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_UIACTION_GIVEUP, &param);
}

void UGsCheatManager::DrawQuestGoalSpot(bool inIsDraw)
{
	GSQuest()->SetDrawQuestGoalSpot(inIsDraw);
}

void UGsCheatManager::PrintMainQuest(int inDepth)
{
	TArray<const FGsSchemaQuestMainChapter*> questChapterList;
	if (const UGsTable* table = FGsSchemaQuestMainChapter::GetStaticTable())
	{
		if (false == table->GetAllRows<FGsSchemaQuestMainChapter>(questChapterList))
			return;

		GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
		GSALOG(Log, TEXT("  Print MainQuest "));
		GSALOG(Log, TEXT("-------------------------------------------------------------------------------------------"));
		GSALOG(Log, TEXT(""));
		for (int i = 0; i< questChapterList.Num(); ++i)
		{
			auto chapter = questChapterList[i];
			GSALOG(Log, TEXT("%d-st Chapter %s[%d]"), i+1, *chapter->chapterTitleText.ToString(), chapter->id);
			
			int storyNum = chapter->questStoryList.Num();
			for (int j = 0; j < storyNum;  ++j)
			{
				if (2 > inDepth) break;
				auto story = chapter->questStoryList[j].GetRow();
				if (nullptr == story) continue;
				GSALOG(Log, TEXT("\t └ %d-st Story %s[%d], ReqLv (%d)"), j+1, *story->storyTitleText.ToString(), story->id, story->requireLevel);
				
				auto& storyQuest = story->questIdList;
				for (int k = 0; k < storyQuest.Num(); ++k)
				{
					if (3 > inDepth) break;
					auto quest = storyQuest[k].GetRow();
					if (nullptr == quest) continue;
					GSALOG(Log, TEXT("\t\t └ %d-st Quest [%llu] -> ObjectiveCount(%d)"), k+1, quest->id, quest->objectiveList.Num());
					
					auto& objectives = quest->objectiveList;
					for (int m = 0; m < objectives.Num(); ++m)
					{
						if (4 > inDepth) break;
						auto objective = objectives[m].GetRow();
						if (nullptr == objective) continue;
						GSALOG(Log, TEXT("\t\t\t └ %d-st objective %s[%d], Type[%s]"), m + 1, *objective->descStringId.ToString(), objective->id, *GetEnumToString(QuestObjectiveType, objective->type));
					}
					GSALOG(Log, TEXT("Q------------------------------------------------------------------------------------------"));
					GSALOG(Log, TEXT(""));

				}
				GSALOG(Log, TEXT("S=========================================================================================="));
			}
			GSALOG(Log, TEXT(""));
		}
		GSALOG(Log, TEXT("End Print MainQuest "));
	}
}

void UGsCheatManager::ShowCurrentQuestId()
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
		if (FGsQuestHandler* questHandler = castLocal->GetQuestHandler())
		{
			const TArray<FGsQuestInstance*>& questList = questHandler->GetQuesInstancetList();
			for (const FGsQuestInstance* questInstance : questList)
			{
				FString id = FString::Printf(TEXT("%llu"), questInstance->GetQuestId());
				GSLOG(Log, TEXT("UGsCheatManager::ShowCurrentQuestId - current quest id : %s"), *id);
				UKismetSystemLibrary::PrintString(GetWorld(), id, true, true, FLinearColor::Yellow, 30.0f);
			}
		}
	}
}

void UGsCheatManager::CompleteCurrentQuest()
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
		if (FGsQuestHandler* questHandler = castLocal->GetQuestHandler())
		{
			const TArray<FGsQuestInstance*>& questList = questHandler->GetQuesInstancetList();
			for (const FGsQuestInstance* questInstance : questList)
			{
				QuestId questId = questInstance->GetQuestId();

				GSLOG(Log, TEXT("UGsCheatManager::CompleteCurrentQuest - current quest id : %llu"), questId);
				CompleteQuest(questId);
			}
		}
	}
}

void UGsCheatManager::CompletAllObjective(bool isAll, int64 inQuestId)
{
	if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
	{
		const TArray<FGsQuestInstance*>& questList = questHandler->GetQuesInstancetList();
		if (isAll)
		{
			for (const FGsQuestInstance* questInstance : questList)
			{
				FString command = FString::Printf(TEXT("completeQuestAllObjective %llu"), questInstance->GetQuestId());
				FGsNetSendService::SendWorldAdminCommandQA(command);
			}
		}
		else
		{
			QuestId questId = inQuestId;
			for (const FGsQuestInstance* questInstance : questList)
			{
				if (questInstance->GetQuestId() == questId)
				{
					FString command = FString::Printf(TEXT("completeQuestAllObjective %llu"), questInstance->GetQuestId());
					FGsNetSendService::SendWorldAdminCommandQA(command);
					break;
				}
			}
		}
	}
}

void UGsCheatManager::ShowQuestNpcIgnoreRange(bool inShow)
{
#if QUEST_NPC_ACTIVE
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (AActor* actor = local->GetActor())
		{
			if (AGsCharacterLocalPlayer* localPlayer = Cast<AGsCharacterLocalPlayer>(actor))
			{
				localPlayer->GetQuestNpcSphereComp()->SetHiddenInGame(!inShow);
			}
		}
	}
#endif
}

void UGsCheatManager::DrawTownFence()
{
	DrawFenceInteranl(TerritoryType::TOWN);
	DrawFenceInteranl(TerritoryType::TOWN_CHAOS);	
}

void UGsCheatManager::DrawSafeZoneFence()
{
	DrawFenceInteranl(TerritoryType::SAFE_ZONE);	
}

void UGsCheatManager::DrawEnvFence()
{
	DrawFenceInteranl(TerritoryType::Env);
}

void UGsCheatManager::DrawSoundFence()
{
	DrawFenceInteranl(TerritoryType::SOUND);
}

void UGsCheatManager::DrawNoCampZoneFence()
{
	if (FGsFenceHandler* fenceHandler = GFence())
	{
		TArray<TSharedPtr<FGsNoCampZoneFence>> fenceList;
		if (false == fenceHandler->TryGetNoCampZoneFence(fenceList))
		{
			return;
		}

		UWorld* world = GetWorld();
		for (TSharedPtr<FGsNoCampZoneFence> fence : fenceList)
		{
			if (fence.IsValid()
				&& fence->IsValid())
			{
				fence->Draw(world);
			}
		}
	}
}

void UGsCheatManager::DrawNonPKFence()
{
	DrawFenceInteranl(TerritoryType::NON_PK);	
}

void UGsCheatManager::DrawArenaFence()
{
	DrawFenceInteranl(TerritoryType::ARENA);
	DrawFenceInteranl(TerritoryType::ARENA_READY);	
}

void UGsCheatManager::DrawSafeZone()
{
	DrawFenceInteranl(TerritoryType::SAFE_ZONE);
}

void UGsCheatManager::ShowFenceState()
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return;
	}

	UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(find);
	if (nullptr == player)
	{
		return;
	}

	if (FGsFenceHandler* fenceHandler = player->GetFenceHandler())
	{
		fenceHandler->ShowState();
	}
}

void UGsCheatManager::SetAreaEnvTime(float InHours)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			if (FGsAreaEnvHandler* envHandler = localPlayer->GetAreaEnvHandler())
			{
				envHandler->SetTime(InHours);
			}
		}
	}
}

void UGsCheatManager::SetAreaEnvTimeCycleDuration(float InMinutes)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			if (FGsAreaEnvHandler* envHandler = localPlayer->GetAreaEnvHandler())
			{
				envHandler->SetTimeCycleDuration(InMinutes);
			}
		}
	}
}

void UGsCheatManager::SetEnvWeather(int32 InWeather)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			if (FGsAreaEnvHandler* envHandler = localPlayer->GetAreaEnvHandler())
			{
				envHandler->SetWeather(static_cast<EGsEnvWeather>(InWeather));
			}
		}
	}
}

void UGsCheatManager::SetEnvEvent(int32 InEnvEvent, bool bIsStart)
{
	FGsPrimitiveInt32 param(InEnvEvent);

	if (bIsStart)
	{	
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
	}
	else
	{		
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);
	}
}

void UGsCheatManager::PlaySequence(int inSequence)
{
	FGsSequenceMessageParam param;
	param._sequenceId = inSequence;

	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);
}

void UGsCheatManager::StopSequence()
{
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
}

void UGsCheatManager::UseItem(int32 itemid)
{
	const FGsSchemaItemCommon* itemtbl = FGsItem::GetTableDataByID(itemid);
	if (nullptr == itemtbl)
	{
		return;
	}
	
	FString command = FString::Printf(TEXT("useitem %d"), itemid);
	
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::ReconnectTest()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			netBase->Disconnect(true);
		}
	}
}


void UGsCheatManager::ShowFPS()
{
	GScope()->ShowFPS();
}

void UGsCheatManager::ResetFPS()
{
	GScope()->ResetFPS();
}

// 돌아오기 타입 세팅
void UGsCheatManager::SetStartPosType(int In_type)
{
	FGsPrimitiveInt32 param(In_type);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_RETURN_START_POS_TYPE, &param);
}

void UGsCheatManager::SetAIAttackCheckRadius(bool In_show)
{
	GSAI()->SetIsShowAIAttackCheckRadius(In_show);
}

/*
void UGsCheatManager::SetAutoPotionRate(float In_rate)
{
	GSAI()->GetContentsConditionDefault()->SetAutoUseHPPotionRate(In_rate);
}
*/

void UGsCheatManager::SetAutoLootType(int In_type)
{
	GSAI()->GetContentsConditionDefault()->SetAutoLootDropItemGrade(
		(EGsAIAutoLootDropItemGradeType)In_type);
}

void UGsCheatManager::ShowMovetoPosRemainDist(bool In_val)
{
	GSAI()->SetIsShowMovetoPosRemainDist(In_val);
}

void UGsCheatManager::ShowDebugPath(bool In_isDraw)
{
	GSAI()->SetIsDrawDebugPath(In_isDraw);
}

void UGsCheatManager::SetIsDrawPosSphere(bool In_isDraw)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			FGsMovementHandlerLocalPlayer* movement = localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			if (movement != nullptr)
			{
				movement->SetIsDrawPosSphere(In_isDraw);
			}
		}
	}
}

void UGsCheatManager::AllowPartialPath(bool In_isAllow)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			FGsMovementHandlerLocalPlayer* movement = localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			if (movement != nullptr)
			{
				movement->SetIsAllowPartialPath(In_isAllow);
			}
		}
	}
}

void UGsCheatManager::ChangeBodyLocalPlayer(int inShapeId)
{
	bool bResult = false;
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			if (FGsGameObjectDataPlayer* data = localPlayer->GetCastData<FGsGameObjectDataPlayer>())
			{
				auto customize = localPlayer->GetCustomizeHandler();
				if (customize->SetBodyChange(inShapeId))
				{
					customize->SetSkinColorChange(data->GetCustomizeBodyData()->mSkinColorIndex);
					bResult = true;
				}
			}
		}
	}
	if (false == bResult)
	{
		GSLOG(Error, TEXT("Check FGsSchemaPartsShapeInfo DataTable (ex GenderType) or Aready Attached  [%d]"), inShapeId);
	}
}

void UGsCheatManager::ChangeHitEffectLocalPlayer(FString inName)
{

}

void UGsCheatManager::TestHitLocalPlayer()
{

}

void UGsCheatManager::SetSoundEnvTimeline(int inTimeline)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->SetAudioParameter(EGsAudioParameterType::EnvTimeline, (float)inTimeline);
	}
}

void UGsCheatManager::SetSoundEnvWeather(int inWeather)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		float ratio = (float)(inWeather) / ((float)(EGsEnvWeather::Max) - 1.0f);
		soundPlayer->SetAudioParameter(EGsAudioParameterType::EnvWeather, ratio);
	}
}

void UGsCheatManager::IsSoundSpeedScale(bool inValue)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->SetUseSpeedScale(inValue);
	}
}

void UGsCheatManager::CancelInteraction()
{
	if (UGsGameObjectManager* manager = GSGameObject())
	{		
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();
			if (interationHandler)
			{
				FVector sourceLocation;
				FVector destLocation;

				UGsGameObjectBase* target = interationHandler->GetInteractionTarget();
				if (target)
				{
					if (AActor* actor = target->GetActor())
					{
						destLocation = actor->GetActorLocation();
						sourceLocation = UGameplayStatics::GetPlayerCharacter(actor->GetWorld(), 0)->GetMovementComponent()->GetActorFeetLocation();
						float size = (destLocation - sourceLocation).Size();

						GSLOG(Error, TEXT("UGsCheatManager - CancelInteraction - source : %s\t dest : %s\tsize : %f"), *sourceLocation.ToString(), *destLocation.ToString(), size);
					}
				}

				interationHandler->CancelInteration(target);
			}
		}
	}

	if (GSAI()->IsAIOn())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("HUDMain"));
		if (widget.IsValid())
		{
			UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
			if (hudWidget)
			{
				if (UGsUIHUDAuto* autoUI = hudWidget->GetAuto())
				{
					hudWidget->GetAuto()->OnClickAutoButton();
				}				
			}
		}
	}	
}

void UGsCheatManager::CheckContinuityWarp(int32 inSouceMapId, int32 inDestMapId)
{
	UGsLevelUtil::CheckContinuityWarp(inSouceMapId, inDestMapId);
}

void UGsCheatManager::PrintUI()
{
	GUI()->PrintLog();
}

void UGsCheatManager::ClearUI()
{
	GUI()->RemoveWhenFlowChanged();
}

void UGsCheatManager::ForceGC()
{
	GEngine->ForceGarbageCollection(true);
}

void UGsCheatManager::ForceResourceUnLoad()
{
	GResource()->Finalize();
}

void UGsCheatManager::ShowGUObjectArry(bool IsShow)
{
	if (UGsGameObjectManager* manager = GSGameObject())
	{
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* local = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			AGsCharacterLocalPlayer* localActor = local->GetLocalCharacter();
			localActor->_debugLocalPlayer._isGlobalObjectsDisplay = IsShow;
		}
	}
}

void UGsCheatManager::PrintBlockUI()
{
	if (UGsBlockUIController* blockController = GUI()->GetBlockUIController())
	{
		blockController->PrintLog();
	}
}

void UGsCheatManager::HideBlockUI()
{
	if (UGsBlockUIController* blockController = GUI()->GetBlockUIController())
	{
		blockController->Hide();
	}
}

void UGsCheatManager::SetFairyZoomArg(float zoomMaxDistance, float zoomSecond)
{
	if (auto fairyActor = Cast<AGsActorFairyObject>(UGameplayStatics::GetActorOfClass(GetWorld(), AGsActorFairyObject::StaticClass())))
	{
		fairyActor->SetFairyZoomValue(zoomSecond, zoomMaxDistance, 0);
	}
}

void UGsCheatManager::SetFairyTouchZoomArg(float touchDistanceForce, float zoomSecond)
{
	if (auto fairyActor = Cast<AGsActorFairyObject>(UGameplayStatics::GetActorOfClass(GetWorld(), AGsActorFairyObject::StaticClass())))
	{
		fairyActor->SetFairyZoomValue(zoomSecond, 0, touchDistanceForce);
	}
}

void UGsCheatManager::SetFairyRotateArg(float touchDistanceForce, float touchMaxLimit)
{
	if (auto fairyActor = Cast<AGsActorFairyObject>(UGameplayStatics::GetActorOfClass(GetWorld(), AGsActorFairyObject::StaticClass())))
	{
		fairyActor->SetFairyRotateValue(touchDistanceForce, touchMaxLimit);
	}
}

void UGsCheatManager::SetMoveSpeed(int32 inSpeed)
{
	//MOVE_SPEED_RATE = 87
	FString command = FString::Printf(TEXT("MoveSpeed %d"), inSpeed);
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::SetAttackSpeed(int32 inSpeed)
{
	//MOVE_SPEED_RATE = 88
	FString command = FString::Printf(TEXT("setQAStat 88 %d"), inSpeed);
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::SetCastSpeed(int32 inSpeed /*= 10000*/)
{
	FString command = FString::Printf(TEXT("setQAStat 89 %d"), inSpeed);
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::SetFastTest(bool inFast)
{
	if (inFast)
	{
		SetMoveSpeed(100000);
		SetAttackSpeed(100000);
		SetCastSpeed(100000);
	}
	else
	{
		SetMoveSpeed();
		SetAttackSpeed();
		SetCastSpeed();
	}	
}

void UGsCheatManager::TestMoveLocationNotify(int inNotifySetId)
{
	if (const FGsSchemaSkillNotifySet* notifySet = 
		UGsTableUtil::FindRowById< UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(inNotifySetId))
	{
		const FGsSchemaSkillNotifyElement* element = notifySet->notifyElementList[0].GetRow();
		if (const FGsSchemaSkillNotifyMoveLocation* moveLocationNotifySet =
			element->typeInfo.GetRowByType<FGsSchemaSkillNotifyMoveLocation>())
		{
			if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
			{
				// 일단 타겟은 가장 가까이 보이는 친구로 처리
				UGsGameObjectBase* target = GSGameObject()->FindObjectNearest(findLocal->GetActor(),
					EGsGameObjectType::NonPlayerBase);
				FVector destPos = target->GetLocation();

				UGsGameObjectCreature* castGameObject = target->CastGameObject<UGsGameObjectCreature>();
				castGameObject->StartMoveLocation(castGameObject->GetLocation(), destPos, 0.5f);
			}
		}
	}
}

// input
// 터치 이동 관련 그릴지
void UGsCheatManager::SetDrawTouchMove(bool In_val)
{
	if (UGsGameObjectManager* manager = GSGameObject())
	{
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* local = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			{
				inputBinder->SetIsShowDebugTouchMove(In_val);
			}
		}
	}
}
void UGsCheatManager::SetDrawDebugArrow(bool In_val)
{
	if (UGsGameObjectManager* manager = GSGameObject())
	{
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* local = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			{
				inputBinder->SetIsDrawDebugArrow(In_val);
			}
		}
	}
}

// 친구
void UGsCheatManager::SetBuddyAdd(bool In_isAdd, int In_tapIndex, uint64 In_userDBId, FString In_userName)
{
	FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(In_userDBId, In_userName);
	FDateTime nowTime = FDateTime::Now();

	if (In_tapIndex == 0)
	{
		if (In_isAdd)
			GSBuddy()->AddBuddy(In_userDBId, In_userName, false, time_t(nowTime.GetTicks()));
		else
			GSBuddy()->DeleteBuddy(In_userDBId);
	}
	else if (In_tapIndex == 1)
	{
		if (In_isAdd)
			GSBuddy()->AddApplyBuddy(In_userDBId, In_userName);
		else
			GSBuddy()->DeleteApplyBuddy(In_userDBId);
	}
	else if (In_tapIndex == 2)
	{
		if(In_isAdd)
			GSBuddy()->AddAcceptBuddy(In_userDBId, In_userName);
		else
			GSBuddy()->DeleteAcceptBuddy(In_userDBId);
	}
}

// 모든 플레이어 정탄 active
void UGsCheatManager::SetSpiritShotAllActive(bool In_active)
{
	if (UGsGameObjectManager* manager = GSGameObject())
	{
		TArray<UGsGameObjectBase*> arrPlayer = manager->FindObjectArray(EGsGameObjectType::RemotePlayer);
		if (arrPlayer.Num() != 0)
		{
			for (auto iter: arrPlayer)
			{
				if (iter == nullptr)
				{
					continue;
				}

				if (FGsGameObjectDataRemotePlayer* data = iter->GetCastData<FGsGameObjectDataRemotePlayer>())
				{
					data->SetIsActiveSpiritShot(In_active);
				}
			}
		}
	}
}

void UGsCheatManager::ResetTimeContents(int32 InResetTimeTicket)
{
	GGuild()->ResetTimeContents(static_cast<ResetTimeTicket>(InResetTimeTicket));

	// 리셋 메시지 발송. 각 컨텐츠에서 받아서 리셋할 경우
	FGsPrimitiveInt32 param(InResetTimeTicket);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::RESET_TIME_CONTENTS, &param);
}

// 뽑기 창 열기
void UGsCheatManager::OpenSummon(int In_type, int In_countType)
{
	EGsSummonType summonType = (EGsSummonType)In_type;
	if (GMode() != nullptr)
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{		
			if(FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{							
				if (summonType == EGsSummonType::SUMMON_FAIRY)
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsFairy)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowFairy");
					}
				}
				else
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsCostume)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowCostume");
					}
				}
			}
		}
	}
	
	// 재시도 정보 갱신
	// 테스트 용으로 글로벌 값 참조
	bool isRetry = GData()->GetGlobalData()->_isSummonRetry;
	CurrencyType summonCurrencyType = GData()->GetGlobalData()->_summonCurrencyType;
	int summonCurrencyCost = GData()->GetGlobalData()->_summonCurrencyCost;
	if (isRetry == true)
	{
		GSSummon()->SetRetryDataBMShop(summonCurrencyType, summonCurrencyCost);
	}
	else
	{
		GSSummon()->SetNotUseRetry();
	}
	// 어디서부턴 소환인지 컨텐츠 타입 저장
	GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_ITEM);

	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}
	TArray<uint32> testArrayVal;

	if (summonType == EGsSummonType::SUMMON_FAIRY)
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetFairyMinMaxGrade(FairyGrade::RARE, FairyGrade::LEGEND);

		if (In_countType == 0)
		{
			testArrayVal.Add(GData()->GetGlobalData()->_testArraySummonFairyId_Single);
		}
		else
		{
			testArrayVal =
				GData()->GetGlobalData()->_testArraySummonFairyIds;
		}
		
		summonHandler->SetSummonFairyInfo(testArrayVal);
	}
	else
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetCostumeMinMaxGrade(CostumeGrade::RARE, CostumeGrade::UNIQUE);

		if (In_countType == 0)
		{
			testArrayVal.Add(GData()->GetGlobalData()->_testArraySummonCostumeId_Single);
		}
		else
		{ 
			testArrayVal =
				GData()->GetGlobalData()->_testArraySummonCostumeIds;
		}
		summonHandler->SetSummonCostumeInfo(testArrayVal);
	}	
	summonHandler->StartSummon(summonType);

}

// 뽑기 창 열기
void UGsCheatManager::OpenSummonCompose(int In_type, int In_resultType)
{
	EGsSummonType summonType = (EGsSummonType)In_type;
	if (GMode() != nullptr)
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (summonType == EGsSummonType::SUMMON_FAIRY)
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsFairy)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowFairy");
					}
				}
				else
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsCostume)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowCostume");
					}
				}
			}
		}
	}

	// 재시도 정보 갱신
	// 테스트 용으로 글로벌 값 참조
	bool isRetry = GData()->GetGlobalData()->_isSummonRetry;
	CurrencyType summonCurrencyType = GData()->GetGlobalData()->_summonCurrencyType;
	int summonCurrencyCost = GData()->GetGlobalData()->_summonCurrencyCost;
	if (isRetry == true)
	{
		GSSummon()->SetRetryDataBMShop(summonCurrencyType, summonCurrencyCost);
	}
	else
	{
		GSSummon()->SetNotUseRetry();
	}
	// 어디서부턴 소환인지 컨텐츠 타입 저장
	GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_COMPOSE);

	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}
	TArray<uint32> testArrayVal;

	if (summonType == EGsSummonType::SUMMON_FAIRY)
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetFairyMinMaxGrade(FairyGrade::RARE, FairyGrade::LEGEND);
		
		testArrayVal =
			GData()->GetGlobalData()->_testArraySummonFairyIds;
		

		summonHandler->SetSummonFairyInfo(testArrayVal);
	}
	else
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetCostumeMinMaxGrade(CostumeGrade::RARE, CostumeGrade::UNIQUE);

		testArrayVal =
			GData()->GetGlobalData()->_testArraySummonCostumeIds;
		
		summonHandler->SetSummonCostumeInfo(testArrayVal);
	}

	GSSummon()->SetTestComposeResult(In_resultType);

	summonHandler->StartSummon(summonType);

}

void UGsCheatManager::OpenSummonWaitChange(int In_type )
{
	EGsSummonType summonType = (EGsSummonType)In_type;
	if (GMode() != nullptr)
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (summonType == EGsSummonType::SUMMON_FAIRY)
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsFairy)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowFairy");
					}
				}
				else
				{
					if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsCostume)
					{
						// 페어리 ui 닫기
						GUI()->CloseByKeyName("WindowCostume");
					}
				}
			}
		}
	}

	// 재시도 정보 갱신
	// 테스트 용으로 글로벌 값 참조
	bool isRetry = GData()->GetGlobalData()->_isSummonRetry;
	CurrencyType summonCurrencyType = GData()->GetGlobalData()->_summonCurrencyType;
	int summonCurrencyCost = GData()->GetGlobalData()->_summonCurrencyCost;
	if (isRetry == true)
	{
		GSSummon()->SetRetryDataBMShop(summonCurrencyType, summonCurrencyCost);
	}
	else
	{
		GSSummon()->SetNotUseRetry();
	}
	// 어디서부턴 소환인지 컨텐츠 타입 저장
	GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_WAIT_CHANGE);

	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}
	TArray<uint32> testArrayVal;

	if (summonType == EGsSummonType::SUMMON_FAIRY)
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetFairyMinMaxGrade(FairyGrade::RARE, FairyGrade::LEGEND);
		
		testArrayVal.Add(GData()->GetGlobalData()->_testArraySummonFairyId_Single);
		
		summonHandler->SetSummonFairyInfo(testArrayVal);
	}
	else
	{
		// 이펙트 연출 등급 최소, 최대 세팅
		GSSummon()->SetCostumeMinMaxGrade(CostumeGrade::RARE, CostumeGrade::LEGEND);
		
		testArrayVal.Add(GData()->GetGlobalData()->_testArraySummonCostumeId_Single);
		
		summonHandler->SetSummonCostumeInfo(testArrayVal);
	}
	summonHandler->StartSummon(summonType);
}
// 뽑기 창 닫기
void UGsCheatManager::CloseSummon()
{
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}

	if (summonHandler->GetIsSummonPlay() == true)
	{
		summonHandler->EndSummon();
		GUI()->CloseByKeyName("TraySummon", true);
	}
}
void UGsCheatManager::StartTestAutoRetry(bool In_active)
{
	GSSummon()->SetIsTestAutoRetrySummon(In_active);
}

void UGsCheatManager::EffectTextSkill(int32 InSkillId, int32 InSkillLevel)
{
#if WITH_EDITOR
	// 로그에는 줄바꿈표기가 안되어 콤마표기
	int32 option = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::COMMA, option);

	FString strEffect;
	if (GSEffectText()->GetEffectTextSkill(InSkillId, InSkillLevel, strEffect, option))
	{
		GSLOG(Log, TEXT("[EffectText] %s"), *strEffect);
	}
	else
	{
		GSLOG(Log, TEXT("[EffectText] Fail. skillId: %d, level: %d"), InSkillId, InSkillLevel);
	}
#endif
}

void UGsCheatManager::EffectTextSkillNotify(int32 InSkillNotifyId, int32 InLevel)
{
#if WITH_EDITOR
	// 로그에는 줄바꿈표기가 안되어 콤마표기
	int32 option = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::COMMA, option);

	FString strEffect;
	if (GSEffectText()->TestGetEffectTextSkillNotify(InSkillNotifyId, strEffect, InLevel, option))
	{
		GSLOG(Log, TEXT("[EffectText] %s"), *strEffect);
	}
	else
	{
		GSLOG(Log, TEXT("[EffectText] Fail. notifyId: %d, level: %d"), InSkillNotifyId, InLevel);
	}
#endif
}

void UGsCheatManager::EffectTextPassivity(int32 InPassivityId)
{
#if WITH_EDITOR
	// 로그에는 줄바꿈표기가 안되어 콤마표기
	int32 option = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::COMMA, option);

	FString strEffect;
	if (GSEffectText()->GetEffectTextPassivity(InPassivityId, strEffect, option))
	{
		GSLOG(Log, TEXT("[EffectText] %s"), *strEffect);
	}
	else
	{
		GSLOG(Log, TEXT("[EffectText] Fail. passivityId: %d"), InPassivityId);
	}
#endif
}

void UGsCheatManager::EffectTextAbnormality(int32 InAbnormalityId, int32 InLevel)
{
#if WITH_EDITOR
	// 로그에는 줄바꿈표기가 안되어 콤마표기
	int32 option = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::COMMA, option);

	FString strEffect;
	if (GSEffectText()->GetEffectTextAbnormality(InAbnormalityId, strEffect, InLevel, option))
	{
		GSLOG(Log, TEXT("[EffectText] %s"), *strEffect);
	}
	else
	{
		GSLOG(Log, TEXT("[EffectText] Fail. abnormalityId: %d, level: %d"), InAbnormalityId, InLevel);
	}
#endif
}

void UGsCheatManager::GatherEffectTextSkill(int32 InSkillId, int32 InSkillLevel)
{
#if WITH_EDITOR
	GSEffectText()->TestGatherSkill(InSkillId, InSkillLevel);
#endif
}

// Cron식 표현 확인치트
void UGsCheatManager::PrintCron(const FString& in_value)
{
	FString strCron;
	FGsCronUtil::GetCronString(in_value, strCron);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Cron] cronExpression : %s, cronDescriptor: %s"), *in_value, *strCron);
#endif
}

void UGsCheatManager::CrashTest()
{
}

void UGsCheatManager::CommandSet(const FString& inCommand)
{
	if (const FGsSchemaCheatSet* ComandSet = UGsTableUtil::FindRow<FGsSchemaCheatSet>(inCommand))
	{
		FString Command, SendQA;
		for (int cnt = 0; cnt < ComandSet->CommandSet.Num(); ++cnt)
		{
			Command.Empty();
			const FGsSchemaCheatSetData* currentCommand = ComandSet->CommandSet[cnt].GetRow();
			if (nullptr == currentCommand)
			{
				continue;
			}

			Command.Append(currentCommand->Command);

			//make Format;
			TArray< FStringFormatArg > args;
			for (int arg = 0; arg < currentCommand->Argument.Num(); ++arg)
			{
				Command.Append(FString::Printf(TEXT(" {%d}"), arg));

				switch (currentCommand->Argument[arg].Type)
				{
				case EGsCheatArguType::int32:
					args.Add(FStringFormatArg(currentCommand->Argument[arg].TypeInt32Value));
					break;
				case EGsCheatArguType::float32:
					args.Add(FStringFormatArg(currentCommand->Argument[arg].TypefloatValue));
					break;
				case EGsCheatArguType::String:
					args.Add(FStringFormatArg(currentCommand->Argument[arg].TypeStringValue));
					break;
				default:
					break;
				}
			}
			SendQA = FString::Format(*Command, args);

			GSLOG(Log, TEXT("Process Command : %s"), *SendQA);
			FGsNetSendService::SendWorldAdminCommandQA(SendQA);
		}
	}
	else
	{
		GSLOG(Error, TEXT("Make Sure RowName %s In UGsTableCheatSet"), *inCommand);
	}
}

void UGsCheatManager::ShowVersion()
{
	FString gameIni = GetDefaultConfigFilename();
	
#if WITH_EDITOR
	FString versionDisplayName;
	GConfig->GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"),
		TEXT("VersionDisplayName"),
		versionDisplayName
		, gameIni);

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("apk version : ") + versionDisplayName, true, textScale);

	FString versionInfo;
	GConfig->GetString(TEXT("/Script/IOSRuntimeSettings.IOSRuntimeSettings"),
		TEXT("VersionInfo"),
		versionInfo,
		gameIni
	);
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ios version : ") + versionInfo, true, textScale);

	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/GeneralProjectSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		gameIni
	);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("windows version : ") + ProjectVersion, true, textScale);

#elif PLATFORM_ANDROID
	FString versionDisplayName;
	GConfig->GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"),
		TEXT("VersionDisplayName"),
		versionDisplayName,
		gameIni
	);	

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("apk version : ") + versionDisplayName, true, textScale);

#elif PLATFORM_IOS
	FString versionInfo;
	GConfig->GetString(TEXT("/Script/IOSRuntimeSettings.IOSRuntimeSettings"),
		TEXT("VersionInfo"),
		versionInfo,
		gameIni
	);

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ios version : ") + versionInfo, true, textScale);

#elif PLATFORM_WINDOWS
	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/GeneralProjectSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		gameIni
	);

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("windows version : ") + ProjectVersion, true, textScale);
#endif
}

// 시공의 틈새 지도
void UGsCheatManager::AddSpaceCrackIcon(int In_id)
{
	const FGsTestSpaceCrackPortalInfo* findInfo =
	GData()->GetGlobalData()->_arrayTestSpaceCrackPortalInfo.FindByPredicate([In_id](FGsTestSpaceCrackPortalInfo info)
	{
		return info._id == In_id;
	});

	if (findInfo == nullptr)
	{
		return;
	}

	GSSpaceCrack()->AddSpaceCrackAlarmInfo(findInfo->_mapId, 0, true, findInfo->GetPos());
}
void UGsCheatManager::DelSpaceCrackIcon(int In_id)
{
	const FGsTestSpaceCrackPortalInfo* findInfo =
		GData()->GetGlobalData()->_arrayTestSpaceCrackPortalInfo.FindByPredicate([In_id](FGsTestSpaceCrackPortalInfo info)
	{
		return info._id == In_id;
	});

	if (findInfo == nullptr)
	{
		return;
	}

	GSSpaceCrack()->DelSpaceCrackPortalInfo(findInfo->_id);
}

void UGsCheatManager::OnDimmed(bool isOn)
{
	FGsUIHelper::TrayDimmed(isOn);
}

void UGsCheatManager::OpenMarket(uint32 InItemId, int32 InOption)
{
	FGsItemDetailOpenMarketParam::EGsMarketSearchOption option = static_cast<FGsItemDetailOpenMarketParam::EGsMarketSearchOption>(InOption);

	FGsItemDetailOpenMarketParam param(InItemId, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_OPEN_MARKET, &param);
}

void UGsCheatManager::OnLobbyEnterGameTest()
{
	GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::GAME_ENTER_TEST);
}

void UGsCheatManager::OnLobbyEnterGameClear()
{
	GMessage()->GetContentsLobby().SendMessage(MessageContentsLobby::GAME_ENTER_CLEAR);
}

void UGsCheatManager::LobbyCamRotator()
{
	if (GEngine)
	{
		//if (AGsCharacterLobbyPlayer* lobbyPlayer = Cast<AGsCharacterLobbyPlayer>(LSLobbyCharacter()->GetCharacter()))
		//{
		//	UCameraComponent* camComponent = lobbyPlayer->GetFollowCamera();
		//	if (nullptr == camComponent)
		//		return;

		//	FVector camPos = camComponent->GetComponentLocation();
		//	FRotator camRot = lobbyPlayer->Controller->GetControlRotation();
		//
		//	const FString tString = FString::Printf(
		//		TEXT("camLocation [X:%.2f / Y:%.2f / Z:%.2f], camRotator [X:%.2f / Y:%.2f / Z:%.2f]"),
		//		camPos.X, camPos.Y, camPos.Z, camRot.Pitch, camRot.Yaw, camRot.Roll);

		//	FVector2D textScale{ 2.f, 2.f };
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, tString, true, textScale);
		//}
	}
}
// lobby zoom power setting
void UGsCheatManager::SetLobbyZoomMobilePower(float In_value)
{
// 	if (GEngine)
// 	{
// 		if (AGsCharacterLobbyPlayer* lobbyPlayer = Cast<AGsCharacterLobbyPlayer>(LSLobbyCharacter()->GetCharacter()))
// 		{
// 			lobbyPlayer->SetTouchZoomFactorMobile(In_value);
// 		}
// 	}
}
void UGsCheatManager::OnStiffen(float x, float y, float z)
{
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* player = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsMovementHandlerBase* moveHandler = player->GetMovementHandler())
			{
				FVector netDirection = FGsDir::angleToDir(0);
				/*moveHandler->Stiffen(netDirection, FVector(x, y, z));*/
			}

// 			if (FGsGameObjectStateManager* fsm = player->GetBaseFSM())
// 			{
// 				fsm->ProcessEvent(EGsStateBase::Stiffen);
// 			}
		}
	}
}

void UGsCheatManager::OnVehicleStart()
{
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* player = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();
			local->SetFairyVehicleCondition(EGsFairyVehicleCondition::SUCCESS);
		}
	}
}

void UGsCheatManager::OnReqVehicleCancel()
{
	FGsNetSendServiceWorld::SendUnmountVehicle();
}

void UGsCheatManager::ShowQuestId(bool isShow)
{
	GSQuest()->SetIsShowQuestId(isShow);
}

void UGsCheatManager::ShowDownloadSize(float isValue)
{
	FString fileSize;
	FGsUIStringHelper::GetPatchSizeText(isValue, fileSize);

	const FString tString = FString::Printf(
		TEXT("ShowDownloadSize %s"), *fileSize);

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, tString, true, textScale);
}

void UGsCheatManager::OnClickEvent()
{
	GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_Click, nullptr);
}

void UGsCheatManager::LocalTestLoopMontagePlay()
{
	// 애니메이션 리소스 저장용 테이블 로드
	UDataTable* rootmotionTable = LoadObject<UDataTable>(
		nullptr, TEXT("/Game/ArtTest/RootMotionTest/BP_AnimMontage_Test_Table.BP_AnimMontage_Test_Table"));
	if (rootmotionTable)
	{
		const FGsLocalTest_RootMotion* testRootmotion = rootmotionTable->FindRow<FGsLocalTest_RootMotion>(TEXT("PlayMontage"), TEXT(""));
		if (testRootmotion)
		{
			if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
			{
				FVector currentPos = player->GetActor()->GetActorLocation();

				UGsAnimInstanceState* anim = player->GetAnimInstance();
				UAnimMontage* resource = testRootmotion->_resource;
				anim->Montage_Play(resource);

				// 루프 구간 찾음
				int sectionIdx = resource->GetSectionIndex(TEXT("ReactionEnd"));
				float aniLen = resource->SequenceLength;
				float endLenght = resource->GetSectionLength(sectionIdx);
				float endPostion = aniLen - endLenght;
				float variableLength = testRootmotion->_length - endLenght;

				// Loop 탈출 타이머 실행
				UWorld* world = anim->GetWorld();
				FTimerHandle timehandleLoopStop;
				world->GetTimerManager().SetTimer(timehandleLoopStop, [&timehandleLoopStop, anim]()
					{
						anim->Montage_JumpToSection(TEXT("ReactionEnd"));
						anim->GetWorld()->GetTimerManager().ClearTimer(timehandleLoopStop);
					}, variableLength, false);

				// 종료 타이머 실행
				FTimerHandle timehandleStopMontage;
				world->GetTimerManager().SetTimer(timehandleStopMontage, [&timehandleStopMontage, anim, currentPos]()
					{
						anim->StopMontagePlay();
						//anim->GetWorld()->GetTimerManager().ClearTimer(timehandleStopMontage);

						FVector destPos = anim->GetSkelMeshComponent()->GetOwner()->GetActorLocation();
						FVector dir = destPos - currentPos;
						FString message = FString::Printf(TEXT("이동 거리 %f"), dir.Size());
						
						GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, message);
						DrawDebugDirectionalArrow(anim->GetWorld(), currentPos, destPos, 20.f, FColor::Red, false, 5.f);

					}, testRootmotion->_length, false);
			}
		}
	}
}

void UGsCheatManager::OnBlockOtherPlayer(bool isBlockOther)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localGameObj = player->CastGameObject<UGsGameObjectLocalPlayer>();
		check(localGameObj);

		localGameObj->BlockOtherPlayer(isBlockOther);
	}
}

void UGsCheatManager::SetLocalPlayerNetSenderInterval(float inValue)
{
	if (UGsGameObjectBase* base = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* local = Cast<UGsGameObjectLocalPlayer>(base);
		if (nullptr != local)
		{
			FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			if (movement != nullptr)
			{
				movement->SetNetSenderInterval(inValue);
			}
		}
	}
}

void UGsCheatManager::TestStartSkillPreview(bool Start)
{
	if (Start)
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::SKILL_PREVIEW_ROOM, GSSkillPreview());
	}
	else
	{
		GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::SKILL_PREVIEW_ROOM);
		GSSkillPreview()->FinishEvent(EGsEventProgressType::SKILL_PREVIEW_ROOM, EGsEventProgressType::NONE);
	}
}

void UGsCheatManager::TestUseSkillPreview(int SkillId, int SkillLevel)
{
	GSSkillPreview()->PlaySkillPreview(SkillId, SkillLevel);
}

void UGsCheatManager::TestCognitionTargetEffect()
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectBase* findNpc = GSGameObject()->FindObjectNearest(local->GetLocation2D(),
			EGsGameObjectType::NonPlayerBase))
		{
			UGsGameObjectNonPlayerBase* npc = findNpc->CastGameObject<UGsGameObjectNonPlayerBase>();
			npc->SetCognitionTargetEffect();
		}
	}
}

void UGsCheatManager::OnIgnoreSkillAutoTarget(bool IsIgnore)
{
#if WITH_EDITOR
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		FGsTargetHandlerLocalPlayer* localTargetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		localTargetHandler->_checkAutoTargetFlag = IsIgnore;
	}
#endif
}

void UGsCheatManager::OnRootmotionNotSync(bool IsNotsync)
{
	GSGameObject()->_isNotSyncRootmotion = IsNotsync;
}

void UGsCheatManager::StartSaveBattery()
{
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_SAVE_BATTERY);
}

void UGsCheatManager::EndSaveBattery()
{
	GMessage()->GetSaveBattery().SendMessage(MessageContentSaveBattery::CLOSE, nullptr);
}

void UGsCheatManager::SetBrightness(float InBrightness)
{
#if PLATFORM_ANDROID
	FAndroidMisc::SetBrightness(InBrightness);
#elif PLATFORM_IOS
	FIOSPlatformMisc::SetBrightness(InBrightness);
#endif
}

void UGsCheatManager::ShowRegionMapAndMovePopup(int In_mapId)
{
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->ShowRegionMapAndMovePopup(In_mapId);
				}
			}
		}
	}
}


void UGsCheatManager::ShowRegionMapTouchColor(int In_isShow)
{
	bool isShow = (In_isShow == 0) ? false : true;
	GSWorldMap()->SetTestShowRegionMapTouchColor(isShow);
}
// 지도 땅찍고 이동 텍스쳐로 할지
void UGsCheatManager::TestRegionMapPickMoveTexture(int In_val)
{
	bool isOn = (In_val == 0) ? false : true;
	GSWorldMap()->SetTestPickMoveTexture(isOn);
}
// 몬스터 정보 팝업과 지도 열기
void UGsCheatManager::ShowMonsterInfoPopupAndMap(int InMapId, int InCreatureId)
{
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->ShowMonsterInfoPopupAndMap(InMapId, InCreatureId);
				}
			}
		}
	}
}
void UGsCheatManager::DrawSpot()
{
	int32 levelKey = GLevel()->GetCurrentLevelId();
	const TArray<FGsSchemaSpotInfo>& spotArray = GLevel()->GetMapSpot(levelKey);
	
	FlushPersistentDebugLines(GetWorld());	

	for (const FGsSchemaSpotInfo& spotInfo : spotArray)
	{
		FString text = FString::Printf(TEXT("Spot index : %lld"), spotInfo.id);
		DrawDebugCircle(GetWorld(), spotInfo.pos, spotInfo.radius, 100, FColor::Yellow, true, -1, 0, 10, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
		DrawDebugString(GetWorld(), spotInfo.pos, text, NULL, FColor::White, 100);
	}
}

void UGsCheatManager::DrawZone()
{
	int32 levelKey = GLevel()->GetCurrentLevelId();	
	const TArray<FGsSchemaSpotInfo>& zoneArray = GLevel()->GetZone(levelKey);

	FlushPersistentDebugLines(GetWorld());

	for (const FGsSchemaSpotInfo& spotInfo : zoneArray)
	{
		FString text = FString::Printf(TEXT("Zone id : %lld"), spotInfo.id);
		DrawDebugCircle(GetWorld(), spotInfo.pos, spotInfo.radius, 100, FColor::Yellow, true, -1, 0, 10, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
		DrawDebugString(GetWorld(), spotInfo.pos, text, NULL, FColor::White, 100);
	}
}

void UGsCheatManager::ClearSpot()
{
	FlushPersistentDebugLines(GetWorld());
}

void UGsCheatManager::ShowZoneRange()
{
	//현재 해당 존을 얻어 올 것
	//존을 그린다
	//존을 정보를 그린다

	_isShowZoneRange = !_isShowZoneRange;
}

void UGsCheatManager::PrintIFFInfo()
{
	// 현재 IFF 타입 출력
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}
	
	if (FGsGameObjectDataCreature* data = local->GetCastData<FGsGameObjectDataCreature>())
	{
		IffFactionActor& iff = data->GetIff();
		GSLOG(Error, TEXT("[%s] local Player Current IFFType = %d"), *FGsDateTimeUTIL::GetCurrentTime(),
			iff.mIffFactionType);
	}

	// 대상이 있다면 대상 IFF 타입도 출력
	FGsTargetHandlerLocalPlayer* targetHandler = local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (UGsGameObjectBase* target = targetHandler->GetLastTarget())
	{
		if (FGsGameObjectDataCreature* data = target->GetCastData<FGsGameObjectDataCreature>())
		{
			IffFactionActor& iff = data->GetIff();
			GSLOG(Error, TEXT("[%s] Target Player Current IFFType = %d"), *FGsDateTimeUTIL::GetCurrentTime(),
				iff.mIffFactionType);
		}
	}
}

void UGsCheatManager::ShowNpcInfo()
{
	//타겟팅 npc를 가져온다
	//npc 머리 위에 정보를 그린다	
	_isShowNpcTargetInfo = !_isShowNpcTargetInfo;
	FGsTargetHandlerLocalPlayer* target = UGsGameObjectUtil::FindTargetHandlerLocalPlayer();
	if (target)
	{
		FGsGameObjectMessageParamTarget messageParam;
		messageParam._paramTarget = target->GetTarget();
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_TARGET_CHANGED, &messageParam);
	}
}

void UGsCheatManager::PrintNpc()
{
	GSLOG(Log, TEXT("=========print npc info ================="));

	TArray<UGsGameObjectBase*> dropObjectArray = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayerBase);
	for (UGsGameObjectBase* dropObject : dropObjectArray)
	{
		UGsGameObjectNonPlayerBase* castNpc = Cast<UGsGameObjectNonPlayerBase>(dropObject);
		if (castNpc)
		{
			castNpc->Print(GetWorld());
		}
	}

	GSLOG(Log, TEXT("======================================="));
}

void UGsCheatManager::SetNpcTargetSightRadius(float inRange)
{
	FGsTargetHandlerLocalPlayer* targetHandle = UGsGameObjectUtil::FindTargetHandlerLocalPlayer();
	if (nullptr == targetHandle)
	{
		GSLOG(Error, TEXT("%s"), TEXT("no local player target handler"));
		return;
	}

	UGsGameObjectNonPlayerBase* target = Cast<UGsGameObjectNonPlayerBase>(targetHandle->GetTarget());
	if (nullptr == target)
	{
		GSLOG(Error, TEXT("%s"), TEXT("target missing"));
		return;
	}

	if (target->GetData())
	{
		SetNpcSightRadius(target->GetData()->GetSpawnId(), inRange);
	}	
}

void UGsCheatManager::SetNpcSightRadius(int64 inUniqueId, float inRange)
{
	//uniuqe id를 활용해서 npc를 가져온다
	//시야 거리를 변경해준다
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == playerController)
	{
		GSLOG(Error, TEXT("%s"), TEXT("player controller missing"));
		return;
	}

	//find npc 
	TArray<class UGsGameObjectBase*> npcArray = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);
	UGsGameObjectBase** findNpc = npcArray.FindByPredicate([&](UGsGameObjectBase* gameObject)->bool{
		UGsGameObjectNonPlayerBase* npc = Cast<UGsGameObjectNonPlayerBase>(gameObject);
		if(nullptr == npc)
		{
			return false;
		}

		if(nullptr == npc->GetData())
		{
			return false;
		}

		return npc->GetData()->GetSpawnId() == inUniqueId;
	});

	if(nullptr == findNpc || nullptr == (*findNpc))
	{
		GSLOG(Error, TEXT("%s"), TEXT("no npc : %lld"), inUniqueId);
		return;
	}

	Cast<UGsGameObjectNonPlayerBase>(*findNpc)->SetSightRadius(inRange);

	//send command
	FString command = FString::Printf(TEXT("ChangeNpcSightRadius %lld %f"), inUniqueId, inRange);
	playerController->ConsoleCommand(command, true);
}

void UGsCheatManager::ShowTargetNpcSightRadius()
{
	//타겟팅한 npc를 가져온다
	//시야 거리 거려준다
	_isShowNpcTargetSightRadius = !_isShowNpcTargetSightRadius;
	FGsTargetHandlerLocalPlayer* target = UGsGameObjectUtil::FindTargetHandlerLocalPlayer();
	if (target)
	{
		FGsGameObjectMessageParamTarget messageParam;
		messageParam._paramTarget = target->GetTarget();
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_TARGET_CHANGED, &messageParam);
	}	
}

void UGsCheatManager::ShowQuestIndex()
{
	//현재 미작동하고 있는 ShowQuestIndex의 기능 정상화 처리 
}

void UGsCheatManager::CheckPlayerWeaponInfo()
{
	if (UGsGameObjectBase* remote = GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer))
	{
		if (AGsCharacterPlayer* characterActor = Cast<AGsCharacterPlayer>(remote->GetCharacter()))
		{
			if (AGsWeaponActor* weaponActor = characterActor->GetWeapon())
			{
				if (UStaticMeshComponent* staticMesh = weaponActor->GetStaticMeshComponent())
				{
					GSLOG(Error, TEXT("[%s] Player Weapon Info  name : %s"), *FGsDateTimeUTIL::GetCurrentTime(),
						*staticMesh->GetStaticMesh()->GetName());
					GSLOG(Error, TEXT("[%s] Player Weapon Info  show : %s"), *FGsDateTimeUTIL::GetCurrentTime(),
						staticMesh->IsVisible() ? TEXT("true") : TEXT("false"));

					FTransform tm = staticMesh->GetComponentToWorld();
					GSLOG(Error, TEXT("[%s] Player Weapon Info  Scale : %f"), *FGsDateTimeUTIL::GetCurrentTime(),
						tm.GetScale3D().X);
					GSLOG(Error, TEXT("[%s] Player Weapon Info  Pos : %f %f %f"), *FGsDateTimeUTIL::GetCurrentTime(),
						tm.GetLocation().X, tm.GetLocation().Y, tm.GetLocation().Z);
					GSLOG(Error, TEXT("[%s] Player Weapon Info  Scale : %f"), *FGsDateTimeUTIL::GetCurrentTime(),
						tm.GetScale3D().X);

					UMaterialInterface* material = staticMesh->GetMaterial(0);
					if (UMaterialInstanceDynamic* dynamicMaterial = Cast<UMaterialInstanceDynamic>(material))
					{
						float outValue;
						dynamicMaterial->GetScalarParameterValue(TEXT("Dissolve Intensity"), outValue);
						GSLOG(Error, TEXT("[%s] Player Weapon Info : GetScalarParameterValue %f"),
							*FGsDateTimeUTIL::GetCurrentTime(), outValue);
					}
					else
					{
						GSLOG(Error, TEXT("[%s] Player Weapon Info : can not casting UMaterialInstanceDynamic"),
							*FGsDateTimeUTIL::GetCurrentTime());
					}
				}
				else
				{
					GSLOG(Error, TEXT("[%s] Player Weapon Info : not found StaticMesh"),
						*FGsDateTimeUTIL::GetCurrentTime());
				}
			}
			else
			{
				GSLOG(Error, TEXT("[%s] Player Weapon Info : not found WeaponActor"), *FGsDateTimeUTIL::GetCurrentTime());
			}
		}
		else
		{
			GSLOG(Error, TEXT("[%s] Player Weapon Info : not found Character Actor"), *FGsDateTimeUTIL::GetCurrentTime());
		}
	}
}

void UGsCheatManager::DrawFenceInteranl(TerritoryType inType)
{
	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == fenceHandler)
	{
		return;
	}

	TArray<TSharedPtr<FGsFence>> fenceList;
	UWorld* world = GetWorld();
	if (false == fenceHandler->TryGetAllFence(fenceList, inType))
	{
		return;
	}
	
	for (TSharedPtr<FGsFence> fence : fenceList)
	{
		if (fence.IsValid()
			&& fence->IsValid())
		{
			fence->Draw(world);
		}
	}
}

void UGsCheatManager::CheckNpcInfo()
{
	// 내 반경에 있는 NPC정보를 조사한다.
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		TArray<UGsGameObjectBase*> listNpc = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayerBase);

		listNpc.RemoveAll([local](UGsGameObjectBase* el) {
			return local->GetTargetDistSquared(el) > 500.f * 500.f;
			});

		UWorld* world = local->GetWorld();
		FlushDebugStrings(world);
		for (UGsGameObjectBase* el : listNpc)
		{
			FString actorInfo(TEXT("nullptr"));
			bool isHidden = false;
			bool isRenderStateDirty = true;
			AActor* actor = el->GetActor();
			if (actor)
			{
				actorInfo = TEXT("Actor valid");
				isHidden = actor->IsHidden();
				for (UActorComponent* actorComp : actor->GetComponents())
				{
					if (false == actorComp->IsRenderStateDirty())
					{
						isRenderStateDirty = false;
						break;
					}
				}

				FString printMsg = FString::Printf(TEXT("NpcInfo : %s  isRenderStateDirty : %s "),
					isHidden ? TEXT("True") : TEXT("false"),
					isRenderStateDirty ? TEXT("True") : TEXT("false"));

				DrawDebugString(world, el->GetLocation(), *printMsg, nullptr, FColor::Cyan);
			}
		}
	}
}

void UGsCheatManager::DrawBoxCollisionFloor(const FString& inName)
{
#if WITH_EDITOR
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actorList);

	TArray<UBoxComponent*> boxComponentArray;
	UActorComponent* boxComponent = nullptr;
	TArray<int32> indexArray = { 0, 1, 2, 0, 2, 3 };
	for (AActor* actor : actorList)
	{
		if (actor->IsA(ALevelBounds::StaticClass()))
		{
			continue;
		}

		if (actor->GetActorLabel().Equals(inName))
		{
			boxComponent = actor->GetComponentByClass(UBoxComponent::StaticClass());
			if (boxComponent)
			{
				TArray<FVector> pointArray = UGsLevelManager::GetFloorPointList(Cast<UBoxComponent>(boxComponent));
				int32 num = pointArray.Num();
				DrawDebugMesh(GetWorld(), pointArray, indexArray, FColor::Yellow, false, 300);
			}
		}		
	}
#endif
}

// AUTO_OPTION_TEST
// 매너 모드 세팅
void UGsCheatManager::SetMannerMode(int In_mode)
{
	GSAI()->SetMannerMode((EGsAIBattleOptionMannerMode)In_mode);
}
// 수동 이동시 auto 유지 할지
void UGsCheatManager::SetNonAutoMoveKeepAuto(bool In_value)
{
	GSAI()->SetNonAutoMoveKeepAuto(In_value);
}
// AUTO_OPTION_TEST

void UGsCheatManager::PrintHiveProductInfo()
{
	if (nullptr != GBMShop())
	{
		GBMShop()->PrintHiveProductInfo();
	}
}

void UGsCheatManager::HitCampTest()
{
	if (UGsGameObjectBase* findObj = GSGameObject()->FindObject(EGsGameObjectType::Camp))
	{
		UGsGameObjectCamp* campObject = findObj->CastGameObject<UGsGameObjectCamp>();
		campObject->SetHitAnim();
	}
}

void UGsCheatManager::DestroyCamp()
{
	if (UGsGameObjectBase* findObj = GSGameObject()->FindObject(EGsGameObjectType::Camp))
	{
		UGsGameObjectCamp* campObject = findObj->CastGameObject<UGsGameObjectCamp>();
		GSCamp()->DestroyCamp_Test(campObject->GetCampID());
	}
}

void UGsCheatManager::PrintCampAngle(bool InVal)
{
	if (GSCamp())
	{
		GSCamp()->_testAngleFlag = InVal;
	}
}

void UGsCheatManager::OpenBMShop()
{
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
}

void UGsCheatManager::EditorZpay(bool Inval)
{
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr == BMShopManager)
	{
		GSLOG(Error, TEXT("nullptr == BMShopManager"));
	}
	else
	{
		BMShopManager->_editorZpay = Inval;
	}
}

void UGsCheatManager::SendProductTrans(FString InPid)
{
	if (InPid.IsEmpty())
		return;

	MakeShared<FGsBMShopSendTransactionFinishSingle>(InPid)->Send();
}

void UGsCheatManager::SetPurchaseCashCheatFlag(bool Inval)
{
	GBMShop()->_usecheatFlag = Inval;
}

void UGsCheatManager::OnClickUserPgComplete()
{
	GBMShop()->OnClickUserPgComplete(true);
}

static IAPV4Product MakeIAPV4Product(const FGsSchemaBMShopProductData& InBMShopProductData)
{
	IAPV4Product CurrentIAPV4Product;

	CurrentIAPV4Product.productType = TCHAR_TO_UTF8(TEXT("consumable"));
	CurrentIAPV4Product.marketPid = TCHAR_TO_UTF8(*InBMShopProductData.pcPlatformPID);
	CurrentIAPV4Product.currency = TCHAR_TO_UTF8(TEXT("KRW"));
	CurrentIAPV4Product.price = InBMShopProductData.purchaseCurrencyPrice;

	CurrentIAPV4Product.displayPrice = TCHAR_TO_UTF8(*FString::Printf(TEXT("\\%s"), *FText::AsNumber(InBMShopProductData.purchaseCurrencyPrice).ToString()));
	CurrentIAPV4Product.title = TCHAR_TO_UTF8(*InBMShopProductData.titleText.ToString());
	CurrentIAPV4Product.productDescription = TCHAR_TO_UTF8(*InBMShopProductData.desc.ToString());
	// CurrentIAPV4Product.originalMarketJson

	CurrentIAPV4Product.displayOriginalPrice = TCHAR_TO_UTF8(*FString::Printf(TEXT("\\%s"), *FText::AsNumber(InBMShopProductData.purchaseCurrencyPrice).ToString()));
	CurrentIAPV4Product.originalPrice = InBMShopProductData.purchaseCurrencyPrice;

	//CurrentIAPV4Product.iconURL
	//CurrentIAPV4Product.coinsReward

	return MoveTemp(CurrentIAPV4Product);
}

void UGsCheatManager::FillHiveProductInfos(bool InFill)
{
#if WITH_EDITOR
	// 기존 데이터 삭제
	GBMShop()->EditorClearHiveProductInfos();

	if (InFill)
	{
		const UGsTable* Table = FGsSchemaBMShopProductData::GetStaticTable();
		if (nullptr == Table)
		{
			GSLOG(Error, TEXT("nullptr == Table"));
			return;
		}

		TArray<const FGsSchemaBMShopProductData*> BMShopProductDatas;
		if (!Table->GetAllRows<FGsSchemaBMShopProductData>(BMShopProductDatas))
		{
			GSLOG(Error, TEXT("!Table->GetAllRows<const UGsTableBMShopProductData*>(BMShopProductDatas)"));
			return;
		}

		TArray<hive::IAPV4Product> IAPV4Products;
		for (const FGsSchemaBMShopProductData* BMShopProductData : BMShopProductDatas)
		{
			if (nullptr == BMShopProductData)
			{
				GSLOG(Error, TEXT("nullptr == BMShopProductData"));
				continue;
			}

			if (CurrencyType::CASH != BMShopProductData->purchaseCurrencyType || BMShopProductData->pcPlatformPID.IsEmpty())
			{
				continue;
			}

			IAPV4Products.Emplace(MakeIAPV4Product(*BMShopProductData));
		}

		FGsBMShopHiveGetProductInfo::StaticSendAsync(MakeShared<FGsBMShopHiveGetProductInfo>(true, hive::ResultAPI::Code::Success, false, 0, MoveTemp(IAPV4Products)));

		FGsBMShopHiveProudctListInfo::StaticSendAsync(MakeShared<FGsBMShopHiveProudctListInfo>());
	}
#endif // WITH_EDITOR
}

void UGsCheatManager::SendPackectWarp(int32 warpId, int inNumSend)
{
	for (int i = 0; i < inNumSend; ++i)
	{
		FGsNetSendServiceWorld::SendWarp(warpId);
	}
}

// exp test
void UGsCheatManager::TestExp(int In_isOn)
{
	bool isOn = (In_isOn == 0) ? false : true;
	GSBookmark()->TestExp(isOn);
}

void UGsCheatManager::ShowRevive(ResurrectType inResurrectType)
{
	TWeakObjectPtr<UGsUIWidgetBase> reviveWidget = GUI()->OpenAndGetWidget(POPUP_REVIVE_NAME);
	if (false == reviveWidget.IsValid())
	{
		return;
	}

	UGsUIPopupRevive* revive = Cast<UGsUIPopupRevive>(reviveWidget.Get());
	TArray<ResurrectType> resurrectTypeList;
	resurrectTypeList.Add(inResurrectType);
	revive->ShowPopupByExternalResurrectTypes(resurrectTypeList);
}

void UGsCheatManager::UnlockAllContents()
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->UnlockAllContentsCheat();
	}

	FString command = FString::Printf(TEXT("unlockCheatAddAll"));
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::UnlockAllContentsOnlyClient()
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->UnlockAllContentsCheat();
	}
}

void UGsCheatManager::UnlockReset()
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->UnlockResetCheat();
	}

	FString command = FString::Printf(TEXT("unlockCheatDeleteAll"));
	FGsNetSendService::SendWorldAdminCommandQA(command);
}

void UGsCheatManager::UnlockResetOnlyClient()
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->UnlockResetCheat();
	}
}

void UGsCheatManager::PrinttHitMotionTarget()
{
	if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();		
#if WITH_EDITOR
		localPlayer->showDebug = !localPlayer->showDebug;
#endif
	}
}


// 근처 remote에 카메라 이동
void UGsCheatManager::ObserveNearRemote()
{
	UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (findLocal == nullptr || nullptr == findLocal->GetActor())
	{
		return;
	}
	UGsGameObjectBase* findRemote =
		GSGameObject()->FindObjectNearest(findLocal->GetActor(), EGsGameObjectType::RemotePlayer);

	if (findRemote == nullptr)
	{
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsObserverHandler* observerHandler = hud->GetObserverHandler();
	if (observerHandler == nullptr)
	{
		return;
	}
	observerHandler->ChangeTarget(findRemote);
}

void UGsCheatManager::WriteComment(FString InComment)
{
	FGsNetSendService::SendReqUserProfileCommentInput(GSUserProfile()->GetTargetUserDBID(), InComment);
}
// playable event
// clear cheat
void UGsCheatManager::ClearPlayableEvent()
{
	if (GMode())
	{
		if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
		{
			if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
			{
				FGsStateLobbyHud* hud =
					contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

				if (nullptr == hud)
				{
					return;
				}

				if (FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler())
				{
					handler->ClearPlayableEvent();
				}
			}
		}
	}
}

void UGsCheatManager::DrawDebugTalkBalloonRange(bool InShow)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGsCharacterNonPlayer::StaticClass(), actors);

	for (AActor* temp : actors)
	{
		if (AGsCharacterNonPlayer* character = Cast<AGsCharacterNonPlayer>(temp))
		{
			if (UGsUIBillboardTalkBalloon* talkBalloon = character->GetTalkBalloon())
			{
				talkBalloon->SetDrawDebugTalkBalloonRange(InShow);
			}
		}
	}
}

void UGsCheatManager::DrawDebugGhost(int index)
{
	UGsGameObjectNonPlayer::catchGhost = index;
}

void UGsCheatManager::TestDPIScale(float InValue)
{
	UGsUIScaleRule::SetUIScaleValue(InValue);

	// static 변수값이 안바뀐건가 체크해봄
	float uiScaleValue = UGsUIScaleRule::GetUIScaleValue();
	GSLOG(Warning, TEXT("[TEST_DPI] Test UIScaleValue: %f"), uiScaleValue);
}

// 서버에서 보내준 네비 메시 좌표 정보
void UGsCheatManager::ShowNaviMeshLog(int32 InShow)
{
	_IsShowNaviMeshInfo = (0 == InShow) ? false : true;
}

void UGsCheatManager::StartTutorial(int32 InTutorialId)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedStartTutorial(InTutorialId);
	}
}

void UGsCheatManager::EndTutorial()
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void UGsCheatManager::NotSaveTutorial(bool bNotSave)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->NotSaveTutorial(bNotSave);
	}
}

void UGsCheatManager::ChangeFixedQuarterYawMode()
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(local);
	if (nullptr == localObj)
	{
		return;
	}
	FGsCameraModeHandler* cameraModehandler = localObj->GetCameraModeHandler();
	if (nullptr == cameraModehandler)
	{
		return;
	}

	cameraModehandler->ChangeFixedQuarterYawMode();
}

void UGsCheatManager::TestBenchmark()
{
	Scalability::BenchmarkQualityLevels();
}

void UGsCheatManager::LoadArtLevel()
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(GetWorld());
	AGsGameModeWorld* castGameMode = Cast<AGsGameModeWorld>(gameMode);
	if (nullptr == castGameMode)
	{
		return;
	}
	
	castGameMode->TryLoadArtStreamingLevel(true);
}

void UGsCheatManager::UnloadArtLevel()
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(GetWorld());
	AGsGameModeWorld* castGameMode = Cast<AGsGameModeWorld>(gameMode);
	if (nullptr == castGameMode)
	{
		return;
	}

	castGameMode->UnloadArtStreamingLevel();
}


void UGsCheatManager::TestReward(int64 InMainRewardId, int64 InSubRewardId, int32 InQuestType)
{
	QuestType questType = static_cast<QuestType>(InQuestType);
	FText titleText = FText::FromString(TEXT("Test Title"));
	FText descText = FText::FromString(TEXT("Test Desc"));
	FGsQuestPopUpParam param(0, 0, 0, titleText, descText, InMainRewardId, InSubRewardId, 0, questType);

	if (FGsQuestHandler* handler = GSQuest()->GetQuestHandler())
	{
		RewardType mainRewardType = RewardType::MAX;
		if (const FGsSchemaRewardData* mainReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(InMainRewardId))
		{
			mainRewardType = mainReward->rewardType;
		}			

		RewardType subRewardType = RewardType::MAX;
		if (const FGsSchemaRewardData* subReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(InSubRewardId))
		{
			subRewardType = subReward->rewardType;
		}

		handler->OpenPopupReward(&param, mainRewardType, subRewardType, true);
	}
	
}

void UGsCheatManager::TestRandomReward(int64 InMainRewardId, int64 InSubRewardId, int32 InBagIndex, bool bIsSub, int32 InQuestType)
{
	TestReward(InMainRewardId, InSubRewardId, InQuestType);

	int32 mainBagIndex = bIsSub ? 0 : InBagIndex;
	int32 subBagIndex = bIsSub ? InBagIndex : 0;

	FGsUIMsgParamRewardRandomItemBag bagParam(0, mainBagIndex, subBagIndex);
	GMessage()->GetUI().SendMessage(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX, &bagParam);
}


void UGsCheatManager::ShowServerRevision(bool InShow)
{
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->SetHudSetting(EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION, InShow ? 1 : 0);
	}
}

// scan button

void UGsCheatManager::SetScanDragDiff(float In_val)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("HUDMain"));
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDScanButton* autoUI = hudWidget->GetScanButton())
			{
				autoUI->SetTestDragDiff(In_val);
			}
		}
	}
}

void UGsCheatManager::SpawnParticleTest(float InInterval)
{
	const float Delay = InInterval;
	if (_handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_handle);
	}
	if (Delay > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(_handle, this, &UGsCheatManager::OnTestTimer, Delay, true); 
	}
}

void UGsCheatManager::OnTestTimer()
{
	FString particleBlueprintClassPath = TEXT("/Game/VFX/Particle/PC/Staff/PS_ThunderStorm_a02");
	UParticleSystem* PSTemplate = LoadObject<UParticleSystem>(this, *particleBlueprintClassPath);

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}
	FRotator rot = local->GetRotation();
	FVector dest = (rot.Quaternion() * (FVector::ForwardVector * 100.0f)) + local->GetLocation();
	//static UParticleSystemComponent* SpawnEmitterAtLocation(UWorld * World, 
	//UParticleSystem * EmitterTemplate, 
	//const FTransform & SpawnTransform, 
	//bool bAutoDestroy = true, 
	//EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None, 
	//bool bAutoActivate = true);

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(dest);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSTemplate, SpawnTransform, true, EPSCPoolMethod::AutoRelease, true);
}

void UGsCheatManager::ShowHPBar(bool In_isShow)
{
	GOption()->SetIsShowHpbar(In_isShow);
}

void UGsCheatManager::OptionDevelopmentMode(bool In_isOn)
{
	GOption()->SetIsDevelopmentMode(In_isOn);
}

void UGsCheatManager::ClearEnterCommands()
{
	_listCheatEnterCommands.Empty();
}

TArray<FString> UGsCheatManager::GetEnterCommands()
{
	TArray<FString> commands;
	commands.Append(_listCheatEnterCommands);

	return commands;
}

void UGsCheatManager::SetVoiceGender(bool IsFemail)
{
	GLocalization()->SetVoiceGenderEditor(IsFemail);
}

void UGsCheatManager::OptionAlrtTest(int index)
{
	GServerOption()->GetOptionNotificationAlert(EGsOptionNotificationAlert(index));
}

void UGsCheatManager::OpenUserContext(FString InUserName)
{
	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
		return;

	// 2023/9/12 PKT - 안쓰는 것 같아 우선 주석 처리( 사용하라면 변경 된 코드에 맞게 ㅌ테스트 진행 해야 함.
	/*if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIPopupChatUserInfo> weakUserUI = Cast<UGsUIPopupChatUserInfo>(uiManager->OpenAndGetWidget("PopupChatUserInfo"));
		if (weakUserUI.IsValid())
		{
			WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;

			UGsUIPopupChatUserInfo::PopupInitData initData{ 0, InUserName, TEXT(""),
				true, EGsUserContentsOwner::USER_CHEAT, myHomeWorldId };
			weakUserUI->SetData(&initData, true);
			weakUserUI->SetType(UserInfoContentType::NONE, true);
		}
	}*/
}

void UGsCheatManager::SetHiddenLocalPlayer(bool show)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}
	local->SetGameObjectActorVisible(show);

}

void UGsCheatManager::TestTicker(FString In_text)
{
	FText convertText = FText::FromString(In_text);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, convertText);
}

void UGsCheatManager::StartEventDialog(int In_targetNpcTblId, int In_npcDialogId)
{
	if (FGsQuestActionManagement* actionManagement = GSQuest()->GetQuestActionManagement())
	{
		actionManagement->OpenEventDialog(In_targetNpcTblId, In_npcDialogId);
	}
}
void UGsCheatManager::GreetingOffDirect()
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(local);
	if (nullptr == localObj)
	{
		return;
	}
	FGsCameraModeHandler* cameraModehandler = localObj->GetCameraModeHandler();
	if (nullptr == cameraModehandler)
	{
		return;
	}

	cameraModehandler->GreetingOffDirect();
}

void UGsCheatManager::ShowLocalizeText(FString InNameSpace, FString InTextKey)
{
	FText localizeText;

	if (InTextKey.IsEmpty())
	{
		FText::FindText(TEXT(""), InNameSpace, localizeText);
	}
	else
	{
		FText::FindText(InNameSpace, InTextKey, localizeText);
	}
	
	FString cultureName = GLocalization()->GetCurrentCultureName();
	SCREEN_MESSAGE(10010, 20.0f, FColor::Cyan, "Current Language : %s, Text : %s", *cultureName, *localizeText.ToString());
}

void UGsCheatManager::ShowArenaResult(int index)
{
	if (index >= 3)
		return;

	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (UGsUIManager* uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayTickerWin"), true);
			uiManager->CloseByKeyName(TEXT("TrayTickerDraw"), true);
			uiManager->CloseByKeyName(TEXT("TrayTickerLose"), true);
		}

		// UI 오픈
		DuelResult result = static_cast<DuelResult>(index);
		switch (result)
		{
		case DuelResult::WIN:
			GUI()->Open(TEXT("TrayTickerWin"));
			break;
		case DuelResult::NONE:
			GUI()->Open(TEXT("TrayTickerDraw"));
			break;
		case DuelResult::LOSE:
			GUI()->Open(TEXT("TrayTickerLose"));
			break;
		}

		// 사운드 출력
		if (const FGsSchemaArenaData* arenaData = arenaManager->GetEnteredArenaData())
		{
			const FGsSchemaSoundResData* soundRes = nullptr;
			switch (result)
			{
			case DuelResult::WIN:
			case DuelResult::NONE:
				soundRes = arenaData->arenaWinSound.GetRow();
				break;
			case DuelResult::LOSE:
				soundRes = arenaData->arenaLoseSound.GetRow();
				break;
			}

			if (soundRes)
			{
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					soundPlayer->PlaySound2D(soundRes);
				}
			}
		}
	}
}

void UGsCheatManager::TestErrorMessage(int inError)
{
	FGsUIHelper::PopupNetError((PD::Result)inError);
}

void UGsCheatManager::DrawDebugProp()
{
	TArray<class UGsGameObjectBase*> propObjectArray = GSGameObject()->FindObjectArray(EGsGameObjectType::PropInteraction);
	for (UGsGameObjectBase* prop : propObjectArray)
	{
		UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(prop);
		if (castProp)
		{
			castProp->DrawDebugProp();
		}
	}
}

void UGsCheatManager::SetIsShowCurrentWarpPos(bool In_val)
{
	GSClientSpawn()->SetIsShowCurrentWarpPos(In_val);
}

void UGsCheatManager::SetDrawKillCenterMove(bool In_val)
{
	if (FGsAIQuestHandler* questAIHandler = GSAI()->GetAIQuestHandler())
	{
		questAIHandler->SetDrawKillCenterMove(In_val);
	}
}

void UGsCheatManager::DeleteAccountTest()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);

	GHive()->SetIsDeleteAccountSuccess(true);
}

void UGsCheatManager::GameFocusedTest(bool In_val)
{

}

void UGsCheatManager::SetActiveDivideDisplayStat(bool InIsOnActive)
{
	GItem()->SetIsUseRealDisplayStatValue(InIsOnActive);
}

void UGsCheatManager::SetDivideDisplayValue(float InVal)
{
	GItem()->SetRealDisplayDivideValue(InVal);
}

void UGsCheatManager::WindowSizeTest()
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->TestGameWindowMoved();
	}
#endif
}

void UGsCheatManager::PacketLaytency()
{
	FString timeStamp = FGsTimeSyncUtil::GetServerNowDateTimeUTC().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	FGsNetSendServiceWorld::SendReqLatency(timeStamp);
}

// test enter play
void UGsCheatManager::TestPlayableEventEnter()
{
	if (UGsLobbyPlayableEventManager* playableEventManager = LSLobbyPlayableEvent())
	{
		playableEventManager->SetIsTestPlayableEventEnter(true);
	}
}
// 테스트로 체크 포인트 바꾸기
void UGsCheatManager::TestCheckPointChange(int In_type)
{
	if (UGsLobbyPlayableEventManager* playableEventManager = LSLobbyPlayableEvent())
	{
		playableEventManager->SetTestCheckPoint(In_type);
	}
}

void UGsCheatManager::SequencePause(bool In_IsPause)
{
	if (In_IsPause)
	{
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_PAUSE, nullptr);
	}
	else
	{
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_PLAY, nullptr);
	}
}

void UGsCheatManager::MoviePause(bool In_IsPause)
{
	UGsUITrayMovie::MoviePause(In_IsPause);
}


void UGsCheatManager::UpdateCustomizeHeightData(int gender)
{
#ifdef WITH_EDITOR
	InternalCustomizeHeightData((CreatureGenderType)gender);
#endif
}

#ifdef WITH_EDITOR
void UGsCheatManager::InternalCustomizeHeightData(CreatureGenderType gender)
{
	/*스폰*/
	auto SpawnActor = [](UWorld* world, CreatureGenderType gender) -> AGsCharacterPlayer*
	{
		static FString FeMailPath = TEXT("/Game/ArtTest/Character/Client/BP_PC_F_Test.BP_PC_F_Test_C");
		static FString MailPath = TEXT("/Game/ArtTest/Character/Client/BP_PC_M_Test.BP_PC_M_Test_C");

		FString classPath = gender == CreatureGenderType::FEMALE ? FeMailPath : MailPath;

		UClass* bpClass = LoadObject<UClass>(nullptr, *classPath);
		if (nullptr == bpClass)
		{
			return nullptr;
		}

		UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (local == nullptr)
		{
			return nullptr;
		}

		FVector Pos = local->GetLocation();
		Pos.X += FMath::FRandRange(-200, 200);
		Pos.Y += FMath::FRandRange(-200, 200);

		FTransform InSpawnTm(Pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		spawnParam.bDeferConstruction = false;
		AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(world->SpawnActor(bpClass, &InSpawnTm, spawnParam));
		if (UCharacterMovementComponent* MoveCompoent = Cast<UCharacterMovementComponent>(actor->GetMovementComponent()))
		{
			MoveCompoent->GravityScale = 0.0f;
		}

		actor->SetCollisionProfileName("Pawn");


		return actor;
	};

	_testActor = SpawnActor(GetWorld(), gender);

	if (_handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_handle);
	}

	_processData.CurrCount = 0;
	_processData.CurrProc = 0;
	_processData.TestGenderType = gender;
	_valueTest.Low = _valueTest.High = 0.0f;
	_valueSpine2.Low = _valueSpine2.High = 0.0f;
	GSLOG(Error, TEXT("Gender[%d]-----------------------------------------------------------------------------------"), _processData.TestGenderType);
	GetWorld()->GetTimerManager().SetTimer(_handle, this, &UGsCheatManager::OnAnimWaitTimer, 0.03f, true, 1.0f);
}

void UGsCheatManager::OnAnimWaitTimer()
{
	if (nullptr == _testActor)
	{
		GSLOG(Warning, TEXT("Error Char null"));
		GetWorld()->GetTimerManager().ClearTimer(_handle);
		return;
	}

	if (nullptr == _customizeHandler)
	{
		_customizeHandler = new FGsCustomizeHandler();
	}
	if (_processData.CurrCount > 20)
	{
		switch (_processData.CurrProc)
		{
		case 0: //최소 측정을 완료했기에 최대 측정으로 넘어감
		{
			_processData.CurrProc = 1;
			_processData.CurrCount = 0;
		}
		break;
		case 1: // 모든 측정을 완료했으니 정리한다.
		{
			SAFE_DELETE(_customizeHandler);
			if (_testActor)
			{
				_testActor->Destroy();
				_testActor = nullptr;
			}
			//결과 출력
			GSLOG(Error, TEXT("------------------------------------------------------------------------------------------"));
			GSLOG(Log, TEXT("Toe Result  RESULT Min[%.2f] : Max[%.2f]"), _valueTest.Low, _valueTest.High);
			GSLOG(Log, TEXT("Spine2 Result  RESULT Min[%.2f] : Max[%.2f]"), _valueSpine2.Low, _valueSpine2.High);
			GSLOG(Error, TEXT("------------------------------------------------------------------------------------------"));

			GetWorld()->GetTimerManager().ClearTimer(_handle);

			/*데이터 저장*/
			if (UGsGlobalConstant* Data = GData()->GetGlobalData())
			{
				if (_processData.TestGenderType == CreatureGenderType::FEMALE)
				{
					Data->_FEMAILMinOffset = _valueTest.Low;
					Data->_FEMAILMaxOffset = _valueTest.High;

					Data->_FEMAILMinBip001Spine = _valueSpine2.Low;
					Data->_FEMAILMaxBip001Spine = _valueSpine2.High;
				}
				else
				{
					Data->_MAILMinOffset = _valueTest.Low;
					Data->_MAILMaxOffset = _valueTest.High;

					Data->_MAILMinBip001Spine = _valueSpine2.Low;
					Data->_MAILMaxBip001Spine = _valueSpine2.High;
				}
				Data->MarkPackageDirty();
			}
			return;
		}
		}
	}
	_processData.CurrCount++;

	//MAIL----------------------------------------------------------------------------------------------------------

	/*커스터마이즈 핸들링*/
	int TargetHeightValue = _processData.CurrProc == 0 ? 0 : 100;

	_customizeHandler->SetActor(_testActor);
	_customizeHandler->InitializeScaleDisplacementAllBone(_processData.TestGenderType);
	_customizeHandler->SetBodyBoneScaleChange(EGsUICustomizeType::HEIGHT, TargetHeightValue);

	/*높이 측정*/
	auto CalcOffset = [](AGsCharacterPlayer* Target, HeightCalcProc Mode, CustomizeHeightValue& value, CustomizeHeightValue& value2)->void
	{
		if (USkeletalMeshComponent* skeletalMeshComp = Target->GetPartsSkeletalComponentBODY())
		{
			if (skeletalMeshComp->ModifyBoneSpaceTransforms.Num() > 0)
			{
				const TArray<FTransform>& refEditableBoneTm = skeletalMeshComp->GetEditableComponentSpaceTransforms();
				int32 BoneIndex = skeletalMeshComp->GetBoneIndex(FName("Bip001-L-Toe0"));
				if (BoneIndex == INDEX_NONE)
				{
					return;
				}
				FVector vecSpaceBot = refEditableBoneTm[BoneIndex].GetLocation();

				if (Mode.CurrProc == 0)
				{
					GSLOG(Warning, TEXT("[Min] [Count : %d] [Value : %.2f]"), Mode.CurrCount, vecSpaceBot.Z);
					//bak1210 : 0값에 맞추기위한 Offset 기록용이다
					// 키를 낮췄을때는 Z값이 0보다 클떄는 기록해둬서 낮추는 용처로 활용
					if (value.Low < vecSpaceBot.Z)
					{
						value.Low = vecSpaceBot.Z;
					}

				}
				else
				{
					//bak1210 : 0값에 맞추기위한 Offset 기록용이다
					//키를 키웠을때 0점보다 낮은 부분에 있을때 기록한다.
					GSLOG(Log, TEXT("[High] [Count : %d] [Value : %.2f]"), Mode.CurrCount, vecSpaceBot.Z);
					if (value.High > vecSpaceBot.Z)
					{
						value.High = vecSpaceBot.Z;
					}
				}

				// 가슴
				int32 Spine2BoneIndex = skeletalMeshComp->GetBoneIndex(FName("Bip001-Spine2"));
				if (BoneIndex == INDEX_NONE)
				{
					return;
				}
				float fHeight = Target->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
				FVector vecSpaceSpine2 = refEditableBoneTm[Spine2BoneIndex].GetLocation();
				vecSpaceSpine2.Z -= fHeight;
				
				if (Mode.CurrProc == 0)
				{
					// 키값을 기록
					GSLOG(Warning, TEXT("[Min] [Count : %d] [Value : %.2f]"), Mode.CurrCount, vecSpaceSpine2.Z);
					if (value2.Low < vecSpaceSpine2.Z)
					{
						value2.Low = vecSpaceSpine2.Z;
					}

				}
				else
				{
					GSLOG(Log, TEXT("[High] [Count : %d] [Value : %.2f]"), Mode.CurrCount, vecSpaceSpine2.Z);
					if (value2.High < vecSpaceSpine2.Z)
					{
						value2.High = vecSpaceSpine2.Z;
					}
				}
			}
		}
	};
	CalcOffset(_testActor, _processData, _valueTest, _valueSpine2);

}

void UGsCheatManager::PrintQuestBoard()
{
	FGsGameStateQuestBoard* questBoard = FGsGameStateQuestBoard::GetInstance();
	if (nullptr == questBoard)
	{
		return;
	}

	TArray<TSharedPtr<FGsQuestBoardData>> questArray = questBoard->GetQuestDataArray();
	int arraySize = questArray.Num();

	GSLOG(Log, TEXT("=== print quest board ==="));
	for (const TSharedPtr<FGsQuestBoardData>& questPair : questArray)
	{		
		GSLOG(Log, TEXT("%s"), *questPair->ToString());
	}
}

#endif


void UGsCheatManager::RemoveMyFace()
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
	if (auto cutomizeHandler = castLocal->GetCustomizeHandler())
	{
		cutomizeHandler->RemoveFaceMeshBySystem();
	}
}

void UGsCheatManager::PrintDropobject()
{
	GSLOG(Log, TEXT("=========drop itme info ================="));

	TArray<UGsGameObjectBase*> dropObjectArray = GSGameObject()->FindObjectArray(EGsGameObjectType::DropObject);
	for (UGsGameObjectBase* dropObject : dropObjectArray)
	{
		UGsGameObjectDropObject* castDropObject = Cast<UGsGameObjectDropObject>(dropObject);
		if (castDropObject)
		{
			castDropObject->Print(GetWorld());
		}
	}

	GSLOG(Log, TEXT("======================================="));
}

void UGsCheatManager::WindowScreenMode()
{
	/*if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->TestWindowScreenMode();
	}*/
}


void UGsCheatManager::ShowDeviceManagement()
{
	GHive()->ShowDeviceManagement();
}
// 대륙맵 테스트
void UGsCheatManager::ShowContinentalDungeonEnter(int In_mapId)
{
	GSWorldMap()->SetDungeonSideShow(In_mapId);
}

void UGsCheatManager::SetUnlockRegisterDevice(bool InIsRegist)
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		if (InIsRegist)
		{
			unlockManager->UnlockContentsByClient(UnlockCategory::REGISTER_DEVICE);
		}
		else
		{
			unlockManager->LockContentsByClient(UnlockCategory::REGISTER_DEVICE);
		}
		
		GMessage()->GetUnlock().SendMessage(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, nullptr);
	}
}

void UGsCheatManager::SetSoundMix(EGsSoundMixType inMixType)
{
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			soundMixerController->OnMixChanged(inMixType);
		}			
	}
}

void UGsCheatManager::PrintPushedSoundMix()
{
	GSLOG(Log, TEXT("=============current pushed sound print start=============="));
	TMap<USoundMix*, FSoundMixState> soudMixMap = GetWorld()->GetAudioDevice()->GetSoundMixModifiers();
	for (auto soundPair : soudMixMap)
	{
		FString name;
		soundPair.Key->GetFullName(nullptr, name);
		GSLOG(Log, TEXT("pushed sound : %s"), *name);
	}
	GSLOG(Log, TEXT("=============current pushed sound print finish=============="));
}

void UGsCheatManager::PopSoundMix(EGsSoundMixType inMixType)
{
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			USoundMix* soundMix = soundMixerController->GetSoundMix(inMixType);
			if (soundMix)
			{
				UGameplayStatics::PopSoundMixModifier(GetWorld(), soundMix);
				PrintPushedSoundMix();
			}			
		}
	}
}

void UGsCheatManager::AllCollectionComplete()
{

	GSBookmark()->TestAllCollectionComplete();
}

void UGsCheatManager::ApplicationWillEnterBackground()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Broadcast();

	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetRestMode(true);
	}
}

void UGsCheatManager::ApplicationHasEnteredForeground()
{
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Broadcast();

	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetRestMode(false);
	}
}


void UGsCheatManager::Background(bool inValue)
{
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		if (true == inValue)
		{
			sequencePlayer->OnWillEnterBackground();
		}
		else
		{
			sequencePlayer->OnHasEnteredForeground();
		}
	}	
}

void UGsCheatManager::ReturnToLobby()
{
	FGsNetSendService::SendGatewayReturnToLobby();
}

void UGsCheatManager::GameReStart()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
}

void UGsCheatManager::AccountDelete()
{
	FText msg;
	FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Desc"), msg);

	FGsUIHelper::PopupSystemYesNo(msg, [](bool bYes)
	{
		if (bYes)
		{
			TPair<FName, FSoftObjectPath> pathList = GUI()->GetPathIPopupAccountDelete();
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(pathList.Key, pathList.Value);
			if (widget.IsValid())
			{
				UGsUIPopupAccountDelete* popup = Cast<UGsUIPopupAccountDelete>(widget.Get());
				if (popup)
				{
					popup->SetData([](bool bPopupCharacterDeleteYes)
						{
							if (bPopupCharacterDeleteYes)
							{
								FText findText;
								FText::FindText(TEXT("OptionText"), TEXT("AccountDeletePopup_Text2"), findText);

								FGsUIHelper::TrayMessageTicker(findText);
							}
						});
				}
			}
		}
	});
}

void UGsCheatManager::SetForceDisconnectTime(float Time)
{
	if (UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
	{
		globalConstData->ForceDiconnetTime = Time;
	}
}

// 필드 보스 디스폰 티커 테스트

void UGsCheatManager::StartFieldBossDespawnTicker(uint32 In_fieldBossId)
{	
	FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
	if (bossHandler)
	{
		FGsFieldBossHandler* handler = bossHandler->GetFieldBossHandler();
		if (handler)
		{
			handler->FieldBossDespawn(In_fieldBossId);
		}
	}
}


void UGsCheatManager::ShowAIReserveDebugList(bool In_show)
{
	GSAI()->ShowDebugReserveList(In_show);
}

void UGsCheatManager::ShowEffectEmptyPlayer(bool in_show)
{
	IsEffectEmptyPlayer = in_show;

	if (GMode() && false == in_show)
	{
		if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
		{
			if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
			{
				FGsContentsMode::InLobby contentsMode = contents->GetCurrentContentsMode();
				if (contentsMode == FGsContentsMode::InLobby::PRE_PLAYABLE_MOVIE)
				{
					StopMovie();
				}
				else if (contentsMode == FGsContentsMode::InLobby::POST_PLAYABLE_MOVIE)
				{
					StopMovie();
				}
				else if (contentsMode == FGsContentsMode::InLobby::CHARACTER_CREATE_EMPTY_STATE)
				{
					StopSequence();
				}
				else
				{
					FGsStateLobbyHud* hud =
						contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

					if (nullptr == hud)
					{
						return;
					}

					if (FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler())
					{
						handler->ClearPlayableEvent();
					}
				}
			}
		}
	}
}
// 테스트 데이터 만들지	
void UGsCheatManager::SetTestStarLegacy(bool In_val)
{
	GSStarLegacy()->SetIsMakeTestStarLegacyData(In_val);
}