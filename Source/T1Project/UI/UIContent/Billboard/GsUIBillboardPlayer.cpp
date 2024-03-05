// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardPlayer.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "UTIL/GsUIUtil.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "UI/UIContent/Common/GsUITalkBubble.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "Guild/GsGuildPlayerData.h"
#include "Guild/GsGuildHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Data/GsDataContainManager.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsInvadeBattleLevelManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Misc/App.h"
#include "Option/GsGameUserSettings.h"
#include "Components/Image.h"
#include "Party/GsPartyMemberInfo.h"
#include "Title/GsTitlePlayerData.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "ContentsServer/GsContentsServerHelper.h"
#include "T1Project.h"


void UGsUIBillboardPlayer::NativeConstruct()
{
	Super::NativeConstruct();

	// 주의: 타겟을 비우면 안됨. (이름없는 캐릭터 버그 생김)
	// UWidgetComponent::TickComponent 에서 RemoveWidgetFromScreen 후에 다시 AddToViewport시키는 경우가있다.	
	// 타겟 정보가 있으면 갱신을 한다. _panelGuild등의 복원을 위함.
	if (_target.IsValid())
	{
		InvalidateAll();
	}

	SetTalkBubbleVisible(false);
}

void UGsUIBillboardPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_talkBubbleTime > 0)
	{
		if (FApp::GetCurrentTime() - _talkBubbleTime > _talkRemainTime)
		{
			_talkBubbleTime = 0;
			SetTalkBubbleVisible(false);
		}
	}
}

void UGsUIBillboardPlayer::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{			
			bool bShowNameOption = IsShowNameOption();

			SetUIName(targetData, bShowNameOption);
			UpdateNameColor();

			SetUIGuild(targetData->GetGuildData(), bShowNameOption);
			SetUIContribution(targetData->IsShowContribution());
			SetUIAggro(targetData->IsShowAggro());

			UpdateParty();
			
			SetUITitle(targetData->GetTitleData(), targetData->GetInvadeBattleLevel(), bShowNameOption);

			return;
		}
	}

	// 빈처리
	SetNameText(FText::GetEmpty());	

	SetUIGuild(nullptr, false);	
	SetUIContribution(false);
	SetUIAggro(false);
	SetUIParty(-1);

	const static int32 HIDE_INVADE_BATTLE = -1;
	SetUITitle(nullptr, HIDE_INVADE_BATTLE, false);
}

void UGsUIBillboardPlayer::InitFunctionMap()
{
	Super::InitFunctionMap();

	_updateFuncMap.Emplace(EGsBillboardUpdateType::NameColor, [this]() { UpdateNameColor(); });
	_updateFuncMap.Emplace(EGsBillboardUpdateType::Guild, [this]() { UpdateGuild(); });
	_updateFuncMap.Emplace(EGsBillboardUpdateType::Party, [this]() { UpdateParty(); });
	_updateFuncMap.Emplace(EGsBillboardUpdateType::Title, [this]() { UpdateTitle(); });
	_updateFuncMap.Emplace(EGsBillboardUpdateType::Contribution, [this]() { UpdateContribution(); });
	_updateFuncMap.Emplace(EGsBillboardUpdateType::Aggro, [this]() { UpdateAggro(); });
}

void UGsUIBillboardPlayer::SetNameColor(const FLinearColor& InColor)
{
	_textBlockServer->SetColorAndOpacity(InColor);

	Super::SetNameColor(InColor);
}

void UGsUIBillboardPlayer::UpdateName()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{
			SetUIName(targetData, IsShowNameOption());
		}
	}
}

void UGsUIBillboardPlayer::UpdateNameColor()
{
	FLinearColor color;
	if (CheckNameColor(color))
	{
		SetNameColor(color);
		return;
	}

	// 에러 상황인 경우 이름이 Gray로 나오도록 처리
	SetNameColor(FLinearColor::Gray);
}

void UGsUIBillboardPlayer::UpdateGuild()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{
			SetUIGuild(targetData->GetGuildData(), IsShowNameOption());
		}
	}
}

void UGsUIBillboardPlayer::UpdateParty()
{
	if (_target.IsValid())
	{
		if (FGsPartyManager* partyMgr = GSParty())
		{
			if (FGsPartyMemberInfo* partyInfo = partyMgr->FindPartyMemberInfoByGameId(_target->GetGameId()))
			{
				SetUIParty(partyInfo->GetPartySlotId());
				return;
			}
		}
	}

	// 비우기 처리
	SetUIParty(-1);
}

void UGsUIBillboardPlayer::UpdateTitle()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{
			SetUITitle(targetData->GetTitleData(), targetData->GetInvadeBattleLevel(), IsShowNameOption());
			return;
		}
	}
	
	SetUITitle(nullptr, -1, IsShowNameOption());
}

void UGsUIBillboardPlayer::UpdateContribution()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{
			SetUIContribution(targetData->IsShowContribution());
			return;
		}
	}

	SetUIContribution(false);
}

void UGsUIBillboardPlayer::UpdateAggro()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>())
		{
			SetUIAggro(targetData->IsShowAggro());
			return;
		}
	}

	SetUIAggro(false);
}

void UGsUIBillboardPlayer::SetUIName(const FGsGameObjectDataPlayer* InData, bool bIsShowOption)
{
	if (nullptr == InData ||
		false == bIsShowOption)
	{
		_textBlockName->SetVisibility(ESlateVisibility::Collapsed);
		SetNameText(FText::GetEmpty());

		_textBlockServer->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_textBlockName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// GM일 경우 이름 앞에 GM 표시
	if (AccountGrade::GM == InData->GetAccountGrade())
	{
		FString userName = FString::Format(TEXT("{0} {1}"), { *(InData->GetGMPrefix()), *(InData->GetName()) });
		SetNameText(FText::FromString(userName));
	}
	else
	{
		SetNameText(FText::FromString(InData->GetName()));
	}	

	if (HasServerPrefix())
	{
		_textBlockServer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(InData->GetHomeWorldId());
		FString strServer = FString::Format(TEXT("[{0}]"), { *(textPrefix.ToString()) });

		_textBlockServer->SetText(FText::FromString(strServer));
	}
	else
	{
		_textBlockServer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIBillboardPlayer::SetUIParty(int32 InPartySlotId)
{
	if (0 > InPartySlotId)
	{
		_textBlockParty->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_textBlockParty->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 0부터 시작하는 번호
	FString strParty = FString::Format(TEXT("{0}P"), { InPartySlotId + 1 });
	_textBlockParty->SetText(FText::FromString(strParty));
}

void UGsUIBillboardPlayer::SetUITitle(const class FGsTitlePlayerData* InTitleData, int32 InLevel, bool bIsShowOption)
{
	_panelTitle->SetVisibility(ESlateVisibility::Collapsed);

	if (false == bIsShowOption)
	{
		return;
	}

	// 2022/08/26 PKT - 현재 Local Player가 위치한 지역
	const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo();

	bool isBattleTitle = false;

	if (nullptr == currentMapData || nullptr == GSInterServer() || nullptr == GGameData())
	{	// 2022/12/20 PKT - 기타 여러가지 이유로 인해 정보를 알수 없다면 기본적으로 칭호 타이틀을 세팅.
		isBattleTitle = false;
	}
	else
	{
		// 2022/12/20 PKT - 침공으로 넘어 갔거나 현재 위치가 침공 가능한 맵일 경우
		isBattleTitle = (GGameData()->IsInvadeWorld() || GSInterServer()->IsValidAirRaidState());
	}

	if (isBattleTitle)
	{	// 2022/08/26 PKT - 침공전 지역 일때..
		const FGsSchemaInvadeBattleLevel* invadeBattleLevel = GsInvadeBattleLevelManager()->FindBattleLevelInfo(InLevel);		
		if (invadeBattleLevel)
		{
			_textBlockTitleFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_textBlockTitleBack->SetVisibility(ESlateVisibility::Collapsed);

			FLinearColor color = FGsUIColorHelper::GetColor(EGsUIColorType::TITLE_INVADE_BATTLE_LEVEL);

			_textBlockTitleFront->SetText(invadeBattleLevel->LevelName);
			_textBlockTitleFront->SetColorAndOpacity(color);

			_panelTitle->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (InTitleData)
		{
			const FGsTitlePlayerData::TitleSet& front = InTitleData->Get(TitleEquipSlot::FRONT);
			const FGsTitlePlayerData::TitleSet& back = InTitleData->Get(TitleEquipSlot::BACK);

			if (false == front.Key.IsEmpty())
			{
				_textBlockTitleFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_textBlockTitleFront->SetText(front.Key);
				_textBlockTitleFront->SetColorAndOpacity(front.Value);
			}
			else
			{
				_textBlockTitleFront->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (false == back.Key.IsEmpty())
			{
				_textBlockTitleBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_textBlockTitleBack->SetText(back.Key);
				_textBlockTitleBack->SetColorAndOpacity(back.Value);
			}
			else
			{
				_textBlockTitleBack->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (false == front.Key.IsEmpty() || false == back.Key.IsEmpty())
			{	// 2022/08/26 PKT - 하나의 칭호라도 있으면..
				_panelTitle->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
}

void UGsUIBillboardPlayer::SetUIContribution(bool bIsShow)
{	
	_imgContribution->SetVisibility(bIsShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

#if CONTRIBUTION_DEBUG
	UE_LOG(LogTemp, Log, TEXT("UGsUIBillboardPlayer::SetUIContribution - %d"), bIsShow);
#endif
}

void UGsUIBillboardPlayer::SetUIAggro(bool bIsShow)
{
	_imgAggro->SetVisibility(bIsShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

#if AGGRO_DEBUG
	if (_target.IsValid())
	{
		UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(_target.Get());
		if (bIsShow)
		{
			GSLOG(Log, TEXT("AGGRO_DEBUG - UGsUIBillboardPlayer::SetUIAggro - show - owner game id : %lld\tname : %s"), _target->GetGameId(), *_target->GetData()->GetName());
		}
		else
		{
			GSLOG(Log, TEXT("AGGRO_DEBUG - UGsUIBillboardPlayer::SetUIAggro - hide - owner game id : %lld\tname : %s"), _target->GetGameId(), *_target->GetData()->GetName());
		}
	}

#endif
}

EGsOptionValuePCNameType UGsUIBillboardPlayer::GetOptionPCNameType() const
{
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const int32 optionValue = gameUserSettings->GetCombatSetting(EGsOptionCombat::PLAYER_NAME);
		if (0 <= optionValue)
		{
			return static_cast<EGsOptionValuePCNameType>(optionValue);
		}
	}

	return EGsOptionValuePCNameType::ALL_ON;
}

bool UGsUIBillboardPlayer::IsInvadeWorld() const //침공이 아님
{
	return GGameData()->IsInvadeWorld();
}

void UGsUIBillboardPlayer::SetTalkBubbleVisible(bool bInIsShow)
{
	_talkBubble->SetVisibility(bInIsShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIBillboardPlayer::SetTalkText(FText text)
{
	if (_talkBubble)
	{
		SetTalkBubbleVisible(true);
		_talkBubble->SetText(text);

		_talkBubbleTime = FApp::GetCurrentTime();
	}
}

void UGsUIBillboardPlayer::SetEmoticon(int32 emojiID)
{
	if (_talkBubble)
	{
		SetTalkBubbleVisible(true);
		_talkBubble->SetEmoticon(emojiID);

		_talkBubbleTime = FApp::GetCurrentTime();
	}
}

bool UGsUIBillboardPlayer::IsPVPAlignmentNeutral()
{
	return (PVPAlignmentGrade::NETURAL == GetPVPAlignmentGrade());
}

FLinearColor UGsUIBillboardPlayer::GetPVPAlignmentGradeColor(PVPAlignmentGrade inPvpAlignmentGrade)
{
	FLinearColor color;
	switch (inPvpAlignmentGrade)
	{
	case PVPAlignmentGrade::NETURAL:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY);
		break;
	case PVPAlignmentGrade::THREAT:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_THREAT);
		break;
	case PVPAlignmentGrade::CHAOTIC:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_CHAOS);
		break;
	}

	return color;
}

