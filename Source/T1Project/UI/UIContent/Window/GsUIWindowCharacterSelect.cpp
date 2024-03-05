// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowCharacterSelect.h"

#include "GameObject/Define/EGsPartsType.h"

#include "T1Project.h"
#include "Net/GsNetSendService.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Level/LevelScriptActor/GsLobbyLevelScriptActor.h"

#include "GameFlow/GameContents/ContentsLobby/GsStateLobbyCharacterSelect.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageLobbyCharacterSelect.h"
#include "GameFlow/GsGameFlowLobby.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Map/GsSchemaMapData.h"

#include "Item/GsSchemaItemEquipDetail.h"
#include "Item/GsItemManager.h"

#include "CharacterSelect/GsUICharacterSlot.h"

#include "DataSchema/User/GsSchemaUserData.h"
#include "Util/GsTableUtil.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Components/VerticalBox.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeLobby/GsLobbyDataType.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/CharacterSelect/GsUICharacterSlot.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Message/GsMessageUI.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UMG/Public/Components/SizeBox.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"


#define MAX_CHARACTER_PER_WORLD_LOBBY	10

void UGsUIWindowCharacterSelect::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowCharacterSelect::OnRefreshEntryScrollBox);
	}
	_scrollBoxHelper = nullptr;
	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowCharacterSelect::NativeOnInitialized()
{
	_helpContentsKey = EGsHelpContents::CharacterSelect;
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);

		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _verticalBox);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowCharacterSelect::OnRefreshEntryScrollBox);
	}	
	
	_btnGameStart->OnClicked.AddDynamic(this, &UGsUIWindowCharacterSelect::OnClickSelectUser);
	_btnCustomizing->OnClicked.AddDynamic(this, &UGsUIWindowCharacterSelect::OnClickCustomizingTicketUse);
		
	FText::FindText(TEXT("LobbyText"), TEXT("Delete_Duedate"), _findExpiringTimeText);

	Super::NativeOnInitialized();
}

void UGsUIWindowCharacterSelect::NativeConstruct()
{
	_characterIndex = -1;

	// 레벨 블루프린트 이벤트 호출
	if (AGsLobbyLevelScriptActor* level = Cast<AGsLobbyLevelScriptActor>(GetWorld()->GetLevelScriptActor()))
	{
		level->BeginCharacterSelect();
	}

	_delegateUdateUI = GMessage()->GetContentsLobby().AddUObject(MessageContentsLobby::UPDATE_UI,
		this, &UGsUIWindowCharacterSelect::UpdateUserList);

	Super::NativeConstruct();
}

void UGsUIWindowCharacterSelect::NativeDestruct()
{
	_toggleGroup.Clear();
	GMessage()->GetContentsLobby().Remove(_delegateUdateUI);

	Super::NativeDestruct();
}

void UGsUIWindowCharacterSelect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
		
	int num = _listCharacter.Num();
	if (0 < num)
	{
		for (int i = 0; i < num; ++i)
		{
			LobbyDataType::FGsLobbyUserListData& data = _listCharacter[i];

			if (0 < data._expiringTime)
			{
				FDateTime expiringDateTime = FGsTimeSyncUtil::ConvertToDateTime(_listCharacter[i]._expiringTime);
				FTimespan remainTime = expiringDateTime - FGsTimeSyncUtil::GetServerNowDateTime();

				float remainSeconds = remainTime.GetTotalSeconds();
				if (0 < remainSeconds)
				{
					if (i == _characterIndex)
					{
						FString strRemainTime;
						FGsTimeStringHelper::GetTimeStringCeil(remainTime, strRemainTime);	
						_textExpiringTime->SetText(FText::Format(_findExpiringTimeText, FText::FromString(strRemainTime)));						
					}
				}
				else
				{					
					UpdateUserList();
					break;
				}
			}
		}
	}
}

bool UGsUIWindowCharacterSelect::OnBack()
{
	// 로딩 중에는 처리하지 않기
	if (true == GUI()->IsActiveLoadingUI())
		return true;

	if (FGsNetManager* netMgr = GNet())
	{
		if (true == netMgr->IsBlockingSend())
			return true;
	}

	if (true == FGsUIHelper::IsOpenBlockUI())
		return true;

	FGsUIHelper::TrayFadeIn(1.5f);
	Close();

	return false;
}

void UGsUIWindowCharacterSelect::IsBeingDeveloped()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::SHOW_DEVELOP_MESSAGE);
}

void UGsUIWindowCharacterSelect::OnClickSelectUser()
{
	if (_listCharacter.IsValidIndex(_characterIndex))
	{
		if (_listCharacter[_characterIndex]._isMigrationNameChanged == false)
		{
			FText currNickName;
			if (const FGsNetUserData* userData = GGameData()->GetUserData())
			{
				currNickName = FText::FromString(userData->mUserName);
			}

			FGsMigrationNameChangeMessageParam param(_listCharacter[_characterIndex]._id, currNickName);
			GMessage()->GetMigration().SendMessage(MessageContentsMigration::MIGRATION_NAME_CHANGE, &param);
			return;
		}

		if (_listCharacter[_characterIndex]._isMigrationGuildNameChanged == false)
		{
			FGsMigrationNameChangeMessageParam param( _listCharacter[_characterIndex]._id,
				FText::FromString(_listCharacter[_characterIndex]._guildName),
				FGsMigrationNameChangeMessageParam::Type::GuildName );

			GMessage()->GetMigration().SendMessage(MessageContentsMigration::MIGRATION_GUILD_CHANGE, &param);
			return;
		}
	}

	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("LobbySelectUser")));

	if (_listCharacter.Num() > _characterIndex)
	{
		if (0 < _listCharacter[_characterIndex]._expiringTime)
		{
			FDateTime expiringDateTime = FGsTimeSyncUtil::ConvertToDateTime(_listCharacter[_characterIndex]._expiringTime);
			FTimespan remainTime = expiringDateTime - FGsTimeSyncUtil::GetServerNowDateTime();

			float remainSeconds = remainTime.GetTotalSeconds();
			if (0 < remainSeconds)
			{
				return;
			}
		}

		OnSelectUser.ExecuteIfBound(_listCharacter[_characterIndex]._id);
	}
}

void UGsUIWindowCharacterSelect::OnClickDeleteUser()
{
	if (_listCharacter.Num() > _characterIndex)
	{
		OnDeleteUser.ExecuteIfBound(_listCharacter[_characterIndex]._id);
	}
}

void UGsUIWindowCharacterSelect::OnClickCharacterDeleteCancel()
{
	if (_listCharacter.Num() > _characterIndex)
	{
		OnDeleteCancel.ExecuteIfBound(_listCharacter[_characterIndex]._id);
	}		
}

void UGsUIWindowCharacterSelect::OnRefreshEntryScrollBox(int32 InIndex, UWidget * InEntry)
{
	int8 listNum = _listCharacter.Num();

	UGsUICharacterSlot* entry = Cast<UGsUICharacterSlot>(InEntry);
	if (entry)
	{
		if (listNum > InIndex)
		{
			entry->SetData(InIndex, ECharacterSlotType::SLOT_NORMAL,
				_listCharacter[InIndex]._userName, static_cast<uint16>(_listCharacter[InIndex]._lv),
				_listCharacter[InIndex]._mapId,
				static_cast<int32>(_listCharacter[InIndex]._classType), _listCharacter[InIndex]._expiringTime > 0);
		}
		else
		{
			int characterSlotCount = LSLobbyData()->GetCharacterSlotCount();			
			int characterPaidOpenSlotCount = LSLobbyData()->GetCharacterPaidOpenSlotCount();
			if (characterSlotCount + characterPaidOpenSlotCount > InIndex)
				entry->SetData(InIndex, ECharacterSlotType::SLOT_EMPTY);
			else			
				entry->SetData(InIndex, ECharacterSlotType::SLOT_LOCKED);
		}

		entry->OnClickSlot.BindUObject(this, &UGsUIWindowCharacterSelect::OnClickSlot);
	}	
}

uint8 UGsUIWindowCharacterSelect::GetCurrentSlotIndex()
{
	return _characterIndex;
}

void UGsUIWindowCharacterSelect::OnClickSlot(const ECharacterSlotType InType, const uint8 InIndex)
{
	FText findText;
	switch (InType)
	{
	case ECharacterSlotType::SLOT_NORMAL:
	case ECharacterSlotType::SLOT_SLEECTED:
		{
			if (_listCharacter.Num() > InIndex)
			{
				_characterIndex = InIndex;
				OnSlotClick.ExecuteIfBound(_listCharacter[_characterIndex]._id);				
					
				switch (_listCharacter[InIndex]._classType)
				{
				case CreatureWeaponType::SWORD:
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Sword"), findText);
					break;
				case CreatureWeaponType::DAGGER:
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Dagger"), findText);
					break;
				case CreatureWeaponType::BOW:
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Bow"), findText);
					break;
				case CreatureWeaponType::WAND:
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Wand"), findText);
					break;
				case CreatureWeaponType::ORB:
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Orb"), findText);
					break;
 				case CreatureWeaponType::SHOTGUN:
 					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_WeaponName_Shotgun"), findText);
 					break;
				default:
					FText::FindText(TEXT("LobbyText"), TEXT("NoWeapon"), findText);
					break;
				}

				_weaponName->SetText(findText);				

				const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_listCharacter[_characterIndex]._mapId);
				if (nullptr != mapData)
					_mapName->SetText(mapData->MapStringId);

				if (_listCharacter[InIndex]._guildName.IsEmpty())
				{
					// 길드 없음
					FText::FindText(TEXT("LobbyText"), TEXT("NoGuild"), findText);
					_textGuildName->SetText(findText);
				}
				else
					_textGuildName->SetText(FText::FromString(_listCharacter[InIndex]._guildName));				

				if (_listCharacter[InIndex]._lastLoginTime > 0)
				{
					FString outLastLoginTime;
					FGsTimeStringHelper::GetElapsedTime(_listCharacter[InIndex]._lastLoginTime, outLastLoginTime);
					_textLastLoginTime->SetText(FText::FromString(outLastLoginTime));
				}	
				else
					_textLastLoginTime->SetText(FText::FromString("-"));

				FString outCreatedTime;
				FGsTimeStringHelper::GetTimeStringNYMDHM(_listCharacter[InIndex]._createdTime, outCreatedTime);
				_textCreatedTime->SetText(FText::FromString(outCreatedTime));				
#if WITH_EDITOR
				FDateTime createdDateTime = FGsTimeSyncUtil::ConvertToDateTime(_listCharacter[InIndex]._createdTime);				

				GSLOG(Warning, TEXT("[UGsUIWindowCharacterSelect] CreatedTime: time_t: %d, %s / UtcNow: %s, Now: %s"), _listCharacter[InIndex]._createdTime,
					*createdDateTime.ToString(), *FDateTime::UtcNow().ToString(), *FDateTime::Now().ToString());
#endif
				_switcherBtn->SetActiveWidgetIndex((0 < _listCharacter[InIndex]._expiringTime) ? 1 : 0);
				_btnCharacterDelete->SetVisibility((0 < _listCharacter[InIndex]._expiringTime) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
				_textExpiringTime->SetText(FText::FromString(""));
			}
			_toggleGroup.SetSelectedIndex(InIndex);
		}
		break;
	case ECharacterSlotType::SLOT_EMPTY:
		{
			// 캐릭터 생성
			if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
			{
				if (lobbyDataMgr->GetIsCharacterEmpty())
				{
					int sequenceResId = GData()->GetGlobalData()->LobbyCharacterPresetSequenceId;
					if (0 < sequenceResId)
					{
						FGsSequenceMessageParam param(sequenceResId, true, true);
						GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);
					}
				}
			}

			GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
		}			
		break;
	case ECharacterSlotType::SLOT_LOCKED:
		{
			// 캐릭터 슬롯 확장권을 사용해 슬롯을 추가할 수 있습니다.
			FText::FindText(TEXT("LobbyText"), TEXT("LockSlotTouch"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		break;
	default:
		break;
	}
}

uint32 UGsUIWindowCharacterSelect::GetPartsMeshID(EGsPartsType InPartsType, LobbyDataType::FGsLobbyUserListData InLobbyUserData)
{
	// 파츠 추가되면 case 추가 해서 id 찾기
	uint32 result = 0;
	switch (InPartsType)
	{
	/*case EGsPartsType::BODY:
		result =  LSLobbyCharacter()->GetPartsID(InPartsType, InLobbyUserData._lookInfo.mArmor);
		break;*/
	case EGsPartsType::WEAPON:
		result =  LSLobbyCharacter()->GetPartsID(InPartsType, InLobbyUserData._lookInfo.mWeapon);
		break;
	}

	return result;
}

void UGsUIWindowCharacterSelect::UpdateUserList()
{
	_listCharacter.Empty();
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		TArray<LobbyDataType::FGsLobbyUserListData> listCharacter;
		listCharacter = lobbyDataMgr->GetUserList();
		UserDBId customizingUserDBId = lobbyDataMgr->GetCustomizingUserDBId();
		int customizingUserIndex = 0;
		int characterNum = listCharacter.Num();
		for (int i = 0; i < characterNum; ++i)
		{
			if (0 < listCharacter[i]._expiringTime)
			{
				FDateTime expiringDateTime = FGsTimeSyncUtil::ConvertToDateTime(listCharacter[i]._expiringTime);
				FTimespan remainTime = expiringDateTime - FGsTimeSyncUtil::GetServerNowDateTime();

				float remainSeconds = remainTime.GetTotalSeconds();
				if (0 >= remainSeconds)
				{
					continue;
				}
			}
			
			_listCharacter.Emplace(listCharacter[i]);
			if (customizingUserDBId == listCharacter[i]._id)
			{
				customizingUserIndex = i;
			}
		}
		
		int8 listNum = _listCharacter.Num();	
		if (0 < listNum)
		{
			int characterSlotCount = lobbyDataMgr->GetCharacterSlotCount();
			int characterPaidSlotCount = lobbyDataMgr->GetCharacterPaidSlotCount();

			_scrollBoxHelper->RefreshAll(characterSlotCount + characterPaidSlotCount);
			_toggleGroup.Clear();
			_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);
			_toggleGroup.SetSelectedIndex(0);

			if (0 < _listCharacter.Num())
			{
				// 디폴트로 반드시 호출
				OnClickSlot(ECharacterSlotType::SLOT_SLEECTED, customizingUserIndex);
			}
		}
		else
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		}

		_switcher->SetActiveWidgetIndex((0 < listNum) ? 1 : 0);
		
		if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
		{
			if (FGsStageManagerLobby* stageManagerLobby = lobbyFlow->GetStageManagerLobby())
			{
				if (auto lobbyStage = stageManagerLobby->GetCurrentState().Pin())
				{
					lobbyStage->ShowCharacter((0 < listNum) ? true : false);
				}
			}
		}

		int customizingTicketCount = lobbyDataMgr->GetCustomizingTicketCount();
		bool isChangeCustomizing = (0 < customizingTicketCount) ? true : false;

		if (_sizeBoxCustomize)
		{
			_sizeBoxCustomize->SetVisibility(isChangeCustomizing ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}

		if (isChangeCustomizing)
		{
			FText findText;
			FText::FindText(TEXT("LobbyText"), TEXT("Count_Available_Customize_Text"), findText);
			_textCustomizing->SetText(FText::Format(findText, customizingTicketCount));
		}

		// 커스텀 백업 아이디 삭제
		lobbyDataMgr->ClearCustomizingUserDBId();
	}
}

void UGsUIWindowCharacterSelect::OnClickCustomizingTicketUse()
{
	if (_listCharacter.IsValidIndex(_characterIndex))
	{
		OnCustomizingTicketUse.ExecuteIfBound(_listCharacter[_characterIndex]._id);
	}
}