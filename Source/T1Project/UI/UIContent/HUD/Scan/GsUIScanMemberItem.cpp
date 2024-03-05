#include "GsUIScanMemberItem.h"

#include "../ContentsServer/GsContentsServerHelper.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"

#include "Scan/GsScanMemberInfo.h"
#include "Scan/GsScanMemberInfoPlayer.h"
#include "Scan/GsScanMemberInfoMonster.h"
#include "Scan/GsScanMemberInfoCamp.h"
#include "Scan/GsScanMemberInfoSanctum.h"
#include "Scan/GsScanDefine.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"

#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildPlayerData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Icon/GsSchemaScanIconData.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"

#include "UTIL/GsUIUtil.h"
#include "UTIL/GsTableUtil.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "Scan/GsScanIconType.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "T1Project.h"

void UGsUIScanMemberItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnScanMemberTarget->OnClicked.AddDynamic(this, &UGsUIScanMemberItem::OnClickScanTarget);
}

// 스캔 타겟 클릭
void UGsUIScanMemberItem::OnClickScanTarget()
{
	//GSLOG(Log,TEXT("click target"));

	FGsScanMemberInfo* scanMemberInfo =
		GSGameObject()->FindScanMemberInfo(_scanMemberGameId);

	if (scanMemberInfo == nullptr)
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

	FGsLockOnHandler* lockOnHandler = hud->GetLockOnHandler();
	if (lockOnHandler == nullptr)
	{
		return;
	}

	lockOnHandler->SetLockOnTarget(scanMemberInfo->GetTargetObject().Get(), true);
}

// 데이터 세팅
void UGsUIScanMemberItem::SetData(FGsScanMemberInfo* In_data)
{
	if (In_data == nullptr)
	{
		return;
	}

	_scanMemberGameId = In_data->GetGameID();

	_textTargetName = FText::FromString(In_data->GetName());

	TWeakObjectPtr<UGsGameObjectBase> targetObject = In_data->GetTargetObject();
				
	// 딤드 상태이면서 상대 거리 멀어서 디스폰이면 object null 일수도 있따
	if (targetObject.IsValid() == true)
	{
		UpdateIsMonster(In_data);


		// 아이콘과 이름 색상 갱신
		UpdateIconAndNameColor(In_data);
	}
	// 타겟 object null 인상태(거리가 멀어서 디스폰)
	else
	{
		// 일단 몬스터라 하자
		_isMonster = true;

		// 아이콘 hide
		SetSymbolShow(false);
		// 이름 흰색
		_textTargetNameColor->SetColorAndOpacity(FLinearColor::White);
	}

	// 순서 세팅
	_textOrder = FText::AsNumber(In_data->GetOrderNumber());

	// 타게팅 세팅
	SetTargetBG(In_data->GetIsTargeting());

	// 공격자 세팅
	SetAttackerBG(In_data->GetIsAttacker());

	// 딤드 상태 가져오기
	SetDimmed(In_data->GetIsDimmed());


	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);
}
// 몬스터인지 갱신
void UGsUIScanMemberItem::UpdateIsMonster(FGsScanMemberInfo* In_info)
{
	if (In_info == nullptr)
	{
		return;
	}

	TWeakObjectPtr<UGsGameObjectBase> targetObject = In_info->GetTargetObject();

	// 딤드 상태이면서 상대 거리 멀어서 디스폰이면 object null 일수도 있따
	if (targetObject.IsValid() == true)
	{
		if (targetObject->GetObjectType() == EGsGameObjectType::NonPlayer)
		{
			_isMonster = true;
		}
		else
		{
			_isMonster = false;
		}
	}
}
// 아이콘 갱신 , 이름 색상 갱신
void UGsUIScanMemberItem::UpdateIconAndNameColor(FGsScanMemberInfo* In_info)
{
	if (In_info == nullptr)
	{
		return;
	}

	TWeakObjectPtr<UGsGameObjectBase> targetObject = In_info->GetTargetObject();

	// 딤드 상태이면서 상대 거리 멀어서 디스폰이면 object null 일수도 있따
	if (targetObject.IsValid() == true)
	{
		if (targetObject->GetObjectType() == EGsGameObjectType::NonPlayer)
		{
			FGsScanMemberInfoMonster* monsterData = static_cast<FGsScanMemberInfoMonster*>(In_info);
			if (monsterData == nullptr)
			{
				return;
			}

			// 몬스터 아이콘 처리
			// 1. 보스, 네임드인가
			// 2. 퀘스트인가
			// 3. 일반 몬스터인가(hide)

			EGsScanIconType iconType = EGsScanIconType::None;
			CreatureRankType monsterRank = monsterData->GetNPCRank();

			bool isTreasureMonster = monsterData->IsTreasureMonster();

			if (true == isTreasureMonster)
			{
				iconType = EGsScanIconType::TreasureMonster;
			}
			else
			{
				// 랭크 체크
				if (monsterRank == CreatureRankType::RAID)
				{
					iconType = EGsScanIconType::Raid;
				}
				else if (monsterRank == CreatureRankType::BOSS)
				{
					iconType = EGsScanIconType::Boss;
				}
				else if (monsterRank == CreatureRankType::NAMED)
				{
					iconType = EGsScanIconType::Named;
				}
				else
				{
					// 퀘스트인지 체크
					QuestType questType = QuestType::MAX;
					if (true == monsterData->GetQuestType(questType))
					{
						if (questType == QuestType::MAIN)
						{
							iconType = EGsScanIconType::MainQuest;
						}
						else if (questType == QuestType::SUB)
						{
							iconType = EGsScanIconType::SubQuest;
						}
						else if (questType == QuestType::REPEAT)
						{
							iconType = EGsScanIconType::RepeatQuest;
						}
					}
				}
			}

			// 아이콘 세팅
			if (iconType == EGsScanIconType::None)
			{
				SetSymbolShow(false);
			}
			else
			{
				SetMonsterIcon(iconType);
			}

			// 몬스터 이름은 무조건 흰색
			_textTargetNameColor->SetColorAndOpacity(FLinearColor::White);

			_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
		}
		else if(targetObject->GetObjectType() == EGsGameObjectType::RemotePlayer)
		{
			FGsScanMemberInfoPlayer* playerData = static_cast<FGsScanMemberInfoPlayer*>(In_info);

			if (playerData == nullptr)
			{
				return;
			}

			// 이름 색상 처리
			FLinearColor nameColor = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY);
			// https://jira.com2us.com/jira/browse/CHR-15810
			// 타 월드 플레이어 무조건 적대 처리
			// 인터서버 컨텐츠 생겨서 현재 침공맵인지 체크도 필요할듯
			if (true == playerData->IsInvadeWorldOtherServerPlayer())
			{
				nameColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
			}
			else
			{
				nameColor = UGsUIUtil::GetPVPAlignmentGradeColor(playerData->GetPVPGrade());
			}

			// GM일 경우 GM 색상 출력 (최상위 조건)
			if (FGsGameObjectDataPlayer* remotePlayerData = targetObject->GetCastData<FGsGameObjectDataPlayer>())
			{
				if (remotePlayerData->GetAccountGrade() == AccountGrade::GM)
				{
					nameColor = FGsUIColorHelper::GetColor(EGsUIColorType::GM_PLAYER_DEFAULT);
				}
			}
			_textTargetNameColor->SetColorAndOpacity(nameColor);

			// 플레이어 아이콘 처리
			// 1. invasion enemy
			// 1. 길드인가
			// 2. 길드 아님
			if (true == playerData->IsInvadeWorldOtherServerPlayer())
			{
				SetInvasionEnemyIcon();
				// 길드 iff 이미지 처리(침공전 적도 동맹, 적대 길드 표시)
				// https://jira.com2us.com/jira/browse/CHR-25690
				SetGuildIffColorByPlayerData(playerData);
			}
			else if (FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar())
			{
				if (playerData->GetPlayerData())
				{
					SetBattleArenaTeamMark(playerData->GetPlayerData()->GetIff().mIffFactionType);
				}				
			}
			else if (true == playerData->GetIsInGuild())
			{
				SetGuildEmblemIcon(playerData->GetGuildEmblemId());
				// 길드 iff 이미지 처리
				SetGuildIffColorByPlayerData(playerData);
			}
			else
			{
				SetSymbolShow(false);
				_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else if (targetObject->GetObjectType() == EGsGameObjectType::Camp)
		{
			FGsScanMemberInfoCamp* campData = static_cast<FGsScanMemberInfoCamp*>(In_info);

			if (campData == nullptr)
			{
				return;
			}
			if (false == campData->GetIsInGuild())
			{
				SetSymbolShow(false);
				
				_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{ 
				SetGuildEmblemIcon(campData->GetGuildEmblemId());

				// 빌보드와 같이가게 처리
				// 동맹, 적대 아니면 iff color 이미지 hide 처리
				FGsGuildPlayerData::EGsGuildIffType guildIff = FGsGuildPlayerData::GetGuildIff(campData->GetGuildDbId());
				if(guildIff == FGsGuildPlayerData::EGsGuildIffType::NONE)
				{
					_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
				}
				else
				{ 
					FLinearColor guildIffColor = UGsUIUtil::GetCampGuildColor(campData->GetGuildDbId());
					_imgGuildIffColor->SetColorAndOpacity(guildIffColor);
					_imgGuildIffColor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}

			// temp
			FLinearColor nameColor = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DEFAULT_COLOR);
			_textTargetNameColor->SetColorAndOpacity(nameColor);
		}
		else if (targetObject->GetObjectType() == EGsGameObjectType::Sanctum)
		{
			FGsScanMemberInfoSanctum* sanctumData = static_cast<FGsScanMemberInfoSanctum*>(In_info);
			if (nullptr == sanctumData)
			{
				return;
			}

			if (false == sanctumData->GetIsInGuild())
			{
				SetSymbolShow(false);

				_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				SetGuildEmblemIcon(sanctumData->GetGuildEmblemId());

				// 빌보드와 같이가게 처리
				// 동맹, 적대 아니면 iff color 이미지 hide 처리
				FGsGuildPlayerData::EGsGuildIffType guildIff = FGsGuildPlayerData::GetGuildIff(sanctumData->GetGuildDbId());
				if (guildIff == FGsGuildPlayerData::EGsGuildIffType::NONE)
				{
					_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
				}
				else
				{
					FLinearColor guildIffColor = UGsUIUtil::GetCampGuildColor(sanctumData->GetGuildDbId());
					_imgGuildIffColor->SetColorAndOpacity(guildIffColor);
					_imgGuildIffColor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}

			FLinearColor nameColor = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DEFAULT_COLOR);
			_textTargetNameColor->SetColorAndOpacity(nameColor);
		}
	}
}


// 타겟 세팅
void UGsUIScanMemberItem::SetTargetBG(bool In_isTarget)
{
	ESlateVisibility visibility = (In_isTarget == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_panelTargetBG->SetVisibility(visibility);
}
// 공격자 세팅
void UGsUIScanMemberItem::SetAttackerBG(bool In_isAttacker)
{
	ESlateVisibility visibility = (In_isAttacker == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_panelAttackerBG->SetVisibility(visibility);
}

// 몬스터 아이콘 처리
void UGsUIScanMemberItem::SetMonsterIcon(EGsScanIconType In_monsterIconType)
{
	const FGsSchemaScanIconData* iconData = UGsTableUtil::FindRowById<UGsTableScanIconData, FGsSchemaScanIconData>(In_monsterIconType);
	if (nullptr != iconData)
	{
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(iconData->_iconImagePath);
		_imgIcon->SetBrushFromAtlasInterface(spriteImg);
	}

	SetSymbolShow(true);
}
// 아이콘 hide
//void UGsUIScanMemberItem::SetIconShow(bool In_isShow)
//{
//	ESlateVisibility visibility = (In_isShow == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
//	_imgIcon->SetVisibility(visibility);
//}

void UGsUIScanMemberItem::SetSymbolShow(bool InIsShow, int32 InIndex /*= 0*/)
{
	ESlateVisibility visibility = (InIsShow == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_symbolRoot->SetVisibility(visibility);

	if (InIsShow)
	{
		_switcherSymbol->SetActiveWidgetIndex(InIndex);
	}
}

// 길드 엠블럼 아이콘 처리
void UGsUIScanMemberItem::SetGuildEmblemIcon(GuildEmblemId In_emblemId)
{
	const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(In_emblemId);
	if (nullptr != emblemData)
	{
		_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall));
	}
	SetSymbolShow(true);
}

// 딤드 세팅
void UGsUIScanMemberItem::SetDimmed(bool In_isDimmed)
{
	_isDimmed = In_isDimmed;
	ESlateVisibility visibility = (In_isDimmed == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_panelDimmedBG->SetVisibility(visibility);
}

void UGsUIScanMemberItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateChangedInfo();
}

// 변경 된거 갱신
void UGsUIScanMemberItem::UpdateChangedInfo()
{
	FGsScanMemberInfo* scanMemberInfo =
		GSGameObject()->FindScanMemberInfo(_scanMemberGameId);

	if (scanMemberInfo == nullptr ||
	scanMemberInfo->GetIsChangedInfo() == false)
	{
		return;
	}

	// 딤드가 아닐때만 갱신
	if (_isDimmed == false)
	{
		// 로컬 타겟 변경
		if (scanMemberInfo->GetIsChangedInfo(EGsScanMemberInfoType::LocalTarget) == true)
		{
			SetTargetBG(scanMemberInfo->GetIsTargeting());
		}
		// 공격자 변경
		if (scanMemberInfo->GetIsChangedInfo(EGsScanMemberInfoType::Attacker) == true)
		{
			SetAttackerBG(scanMemberInfo->GetIsAttacker());
		}
	}
	// 딤드 변경
	if (scanMemberInfo->GetIsChangedInfo(EGsScanMemberInfoType::Dimmed) == true)
	{
		SetDimmed(scanMemberInfo->GetIsDimmed());
		// 딤드 상태 풀리면 새로 갱신
		if (_isDimmed == false)
		{
			UpdateIsMonster(scanMemberInfo);

			// 로컬 타겟 변경
			SetTargetBG(scanMemberInfo->GetIsTargeting());

			// 공격자 변경
			SetAttackerBG(scanMemberInfo->GetIsAttacker());

			// 아이콘과 이름 색상 갱신
			UpdateIconAndNameColor(scanMemberInfo);
		}
		// 딤드 상태로 전환이면 
		// 1. 타게팅 표시도 해제
		// 2. 공격자 bg 해제
		else
		{
			SetTargetBG(false);
			SetAttackerBG(false);
		}
	}


	// dirty 클리어
	scanMemberInfo->ClearDirtyInfo();
}

void UGsUIScanMemberItem::NativeDestruct()
{

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}

// invasion enemy icon setting
void UGsUIScanMemberItem::SetInvasionEnemyIcon()
{	
	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(GData()->GetGlobalData()->_invasionEnemyIcon);
	_imgIcon->SetBrushFromAtlasInterface(spriteImg);
	
	SetSymbolShow(true);
}

void UGsUIScanMemberItem::SetBattleArenaTeamMark(IffFactionType InIffFactionType)
{
	_imgIconDiplomacy->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_ENEMY_COLOR));

	const static int32 SWITCHER_SHOW_ICON_INDEX = 1;
	
	SetSymbolShow(true, SWITCHER_SHOW_ICON_INDEX);
}

// 길드 적대인지 동맹인지 표시 처리
void UGsUIScanMemberItem::SetGuildIffColorByPlayerData(FGsScanMemberInfoPlayer* In_playerData)
{
	if (In_playerData == nullptr)
	{
		// 길드에 안들어있으면 hide
		_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (true == In_playerData->GetIsInGuild())
	{
		// 빌보드와 같이가게 처리
		// 동맹, 적대 아니면 iff color 이미지 hide 처리
		if (In_playerData->GetPlayerData()->IsAllyGuild() == false &&
			In_playerData->GetPlayerData()->IsEnemyGuild() == false)
		{
			_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			FLinearColor guildIffColor = UGsUIUtil::GetPlayerGuildColor(In_playerData->GetPlayerData());
			_imgGuildIffColor->SetColorAndOpacity(guildIffColor);
			_imgGuildIffColor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		// 길드에 안들어있으면 hide
		_imgGuildIffColor->SetVisibility(ESlateVisibility::Hidden);
	}
}