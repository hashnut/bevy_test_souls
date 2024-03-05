// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardRemotePlayer.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsUIUtil.h"
#include "Guild/GsGuildPlayerData.h"
#include "Title/GsTitlePlayerData.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "T1Project.h"

//#define  USE_BILLBOARD_IFF

void UGsUIBillboardRemotePlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_isDirtyUpdateShowName)
	{
		_isDirtyUpdateShowName = false;
		UpdateBillboardOpacity();

		// 아래 Super::NativeTick에 의해 InvalidateAllInternal 호출될 것
		InvalidateAll();
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBillboardRemotePlayer::Reset()
{
	Super::Reset();

	_target = nullptr;
	_bWasTargetRendered = false;
	_currHp = 0;
	_maxHp = 0;
	_currShield = 0;
	
	SetOpacityAnimationValue(0.0f);
	SetScanMember(false);
}

void UGsUIBillboardRemotePlayer::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	UpdateStat();

	if (false == _target.IsValid())
	{
		SetTargetingName(false);
	}

	UpdateShowHPBar();
}

void UGsUIBillboardRemotePlayer::SetHPPercent(float InPercent)
{
	// 꺼져있을 경우 확인을 위해 체크
	if (0.0f < InPercent)
	{
		if (0 == GetRenderOpacity())
		{
			UpdateShowHPBar();
		}
	}

	bool isUpdateHP = IsShowHPBar();
	if (isUpdateHP == true)
	{
		Super::SetHPPercent(InPercent);
	}

	// 주의: Super::SetHPPercent 이후에 수행해야 _hpBarValue 가 갱신된값으로 검사됨
	if (0 >= InPercent)
	{
		UpdateBillboardOpacity();
	}
}

void UGsUIBillboardRemotePlayer::OnChangedStat(const FGsStatInfo& InStatInfo)
{
	Super::OnChangedStat(InStatInfo);

	if (StatType::HP == InStatInfo.GetStatType())
	{
		SetHp(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue(), true);
	}
	else if (StatType::HP_SHIELD == InStatInfo.GetStatType())
	{
		SetShield(InStatInfo.GetStatValue(), true);
	}
}

void UGsUIBillboardRemotePlayer::UpdateStat()
{
	if (false == _target.IsValid())
	{
		return;
	}

	if (UGsGameObjectCreature* creature = _target->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsStatInfo* hpStat = creature->GetCreatureStatInfoByType(StatType::HP))
		{
			SetHp(hpStat->GetStatValue(), hpStat->GetMaxStatValue(), false);
		}

		if (const FGsStatInfo* shieldStat = creature->GetCreatureStatInfoByType(StatType::HP_SHIELD))
		{
			SetShield(shieldStat->GetStatValue(), false);
		}
		else
		{
			SetShield(0, false);
		}

		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardRemotePlayer::SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardRemotePlayer::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (bInUpdateBar)
	{
		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardRemotePlayer::UpdateHpAndShieldBar()
{
	SetHPShieldProgressBar(_barShieldFront, _barShieldBack, _currHp, _maxHp, _currShield);

	if (false == IsShowHPBar())
	{
		_barShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		_barShieldBack->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIBillboardRemotePlayer::UpdateBillboardOpacity()
{
	if (0.0f >= _hpBarValue)
	{
		SetOpacityAnimationValue(0.0f);
	}

	if (_bWasTargetRendered || IsTargeting() || IsScanNumber())
	{
		SetOpacityAnimationValue(1.0f);
		return;
	}

	// 모델이 바로꺼져서 이상하다. 애니 없이 세팅.
	SetOpacityAnimationValue(0.0f);
}

void UGsUIBillboardRemotePlayer::SetOpacityAnimationValue(float InValue)
{
	float clampVal = FMath::Clamp(InValue, 0.0f, 1.0f);
	SetRenderOpacity(clampVal);
}

void UGsUIBillboardRemotePlayer::OnChangeRenderState(bool bIsRendered)
{
	_bWasTargetRendered = bIsRendered;

	UpdateBillboardOpacity();
}

bool UGsUIBillboardRemotePlayer::CheckNameColor(OUT FLinearColor& OutColor)
{
	// 기본색상
	OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ALLY);
	
	if (false == _target.IsValid())
	{
		GSLOG(Warning, TEXT("[BillboardRemote] Target is invalid."));
		return false;
	}

	FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == targetData)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] targetData is null."));
		return false;
	}

	// GM 캐릭터 전용 색상
	if (targetData->GetAccountGrade() == AccountGrade::GM)
	{
		OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::GM_PLAYER_DEFAULT);
		return true;
	}

	// 침공전 전용색상
	// 	https://jira.com2us.com/wiki/pages/viewpage.action?pageId=297698559
	// https://jira.com2us.com/jira/browse/C2URWQ-2321
	// 1. other server user: red
	// 2. not PVPAlignmentNeutral
	// 3. party
	// 4. my guild
	// 5. else white
	bool isInvationEnemy = UGsUIUtil::IsInvasionEnemy(targetData);
	if (isInvationEnemy == true)
	{
		// 타 월드 플레이어 무조건 적대 처리		
		OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
		return true;
	}

	if (false == IsPVPAlignmentNeutral())
	{
		OutColor = GetPVPAlignmentGradeColor(GetPVPAlignmentGrade());
		return true;
	}

	if (IsParty(_target->GetGameId()))
	{
		OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_PARTY);
		return true;
	}

	if (targetData->IsMyGuild())
	{
		OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_MY_GUILD);
		return true;
	}

#ifdef USE_BILLBOARD_IFF
	// 둘중 하나라도 NEUTRAL이면 적대관계 아님
	// 둘중 하나라도 FFA이면 적대관계
	// 둘이 서로 팀이 다르면 적대관계 
	if (false == IsIffNeutral())
	{
		OutColor = GetIffColor();
		return true;
	}
#endif

	return true;
}

bool UGsUIBillboardRemotePlayer::HasServerPrefix() const
{
	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
	{
		return false;
	}

	// 인터서버일 경우 무조건 출력
	if (gameDataMgr->IsInInterWorld())
	{
		return true;
	}

	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* data = _target.Get()->GetCastData<FGsGameObjectDataPlayer>())
		{
			WorldId myHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;
			WorldId nowMapWorldId = GGameData()->GetLoadWorldId();

			return SharedFunc::HasOtherUserNamePrefix(GGameData()->GetLoadWorldType(), nowMapWorldId, myHomeWorldId, data->GetHomeWorldId());
		}
	}

	return false;
}

bool UGsUIBillboardRemotePlayer::IsParty(int64 isGameId)
{
	FGsPartyManager* partyMgr = GSParty();
	return partyMgr->GetIsInParty() &&
		partyMgr->IsInPartyGameId(isGameId);
}

bool UGsUIBillboardRemotePlayer::IsIffNeutral()
{
#ifdef USE_BILLBOARD_IFF
	FGsGameObjectData* targetData = _target->GetData();
	UGsGameObjectLocalPlayer* localPlayer = GetLocalPlayer();
	if (targetData && localPlayer)
	{
		FGsGameObjectData* localPlayerData = localPlayer->GetData();
		if (localPlayerData)
		{
			if (targetData->GetIffTeam() == IffTeamType::NEUTRAL)
			{
				return true;
			}

			if (targetData->GetIffTeam() == localPlayer->GetData()->GetIffTeam())
			{
				return true;
			}
		}
	}
#endif
	return false;
}

FLinearColor UGsUIBillboardRemotePlayer::GetIffColor()
{
#ifdef USE_BILLBOARD_IFF
	FLinearColor color = FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ALLY);

	FGsGameObjectData* targetData = _target->GetData();
	if (nullptr == targetData)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] Target data is null."));
		return color;
	}

	IffTeamType targetTeam = targetData->GetIffTeam();
	if (targetTeam == IffTeamType::FFA)
	{
		return FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
	}

	// 로컬 플레이어의 정보 확인
	UGsGameObjectLocalPlayer* localPlayer = GetLocalPlayer();
	if (nullptr == localPlayer)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] LocalPlayer is null"));
		return color;
	}

	FGsGameObjectData* localPlayerData = localPlayer->GetData();
	if (nullptr == localPlayerData)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] LocalPlayer data is null"));
		return color;
	}

	IffTeamType localPlayerTeam = localPlayerData->GetIffTeam();
	if (IffTeamType::MAX == localPlayerTeam)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] LocalPlayer teamType is MAX"));
		return color;
	}

	if (IffTeamType::FFA == localPlayerTeam)
	{
		//GSLOG(Warning, TEXT("[BillboardRemote] LocalPlayer teamType is FFA"));
		return FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
	}

	// 같은 팀인지 확인
	if (targetTeam != localPlayerTeam)
	{
		return FGsUIColorHelper::GetColor(EGsUIColorType::REMOTE_PLAYER_ENEMY);
	}

	return color;
#else
	return FLinearColor::White;
#endif
}

PVPAlignmentGrade UGsUIBillboardRemotePlayer::GetPVPAlignmentGrade()
{
	if (_target.IsValid())
	{
		if (FGsGameObjectDataPlayer* data = _target.Get()->GetCastData<FGsGameObjectDataPlayer>())
		{
			return data->GetUserLookInfo()->mPVPAlignmentGrade;
		}
	}

	return PVPAlignmentGrade::NETURAL;
}

void UGsUIBillboardRemotePlayer::SetScanOrderText(const FText& In_name)
{
	_textScanOrder->SetText(In_name);
}

void UGsUIBillboardRemotePlayer::SetScanMember(bool In_isMember)
{
	_textScanOrder->SetVisibility(In_isMember ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	UpdateBillboardOpacity();
}

bool UGsUIBillboardRemotePlayer::IsScanNumber() const
{
	return (ESlateVisibility::SelfHitTestInvisible == _textScanOrder->GetVisibility()) ? true : false;
}

// 타게팅
void UGsUIBillboardRemotePlayer::OnTargeting(bool InIsTarget, UGsGameObjectBase* InOwner, bool InIsFirstTarget, bool InIsEnemy)
{
	Super::OnTargeting(InIsTarget, InOwner, InIsFirstTarget, InIsEnemy);

	// 이시점엔 아직 타겟이 클리어 안되어서 다음틱에 한다
	_isDirtyUpdateShowName = true;
}

void UGsUIBillboardRemotePlayer::OnSecondTarget()
{
	SetTargetBlockColor(FLinearColor::Green);
}

bool UGsUIBillboardRemotePlayer::IsShowNameOption() const
{
	// 예외사항에선 그냥 보여주도록 true 리턴 처리함
	if (false == _target.IsValid())
	{
		return true;
	}

	// 타겟이라면 이름 표시
	if (UGsGameObjectUtil::IsLocalTarget(_target.Get()))
	{
		return true;
	}

	EGsOptionValuePCNameType pcNameType = GetOptionPCNameType();
	switch (pcNameType)
	{
	case EGsOptionValuePCNameType::ALL_ON:
	case EGsOptionValuePCNameType::OTHER_NAME_ON:
		return true;
	}

	return false;
}

bool UGsUIBillboardRemotePlayer::SetUIGuild(const FGsGuildPlayerData* InData, bool bIsShowOption)
{
	// https://jira.com2us.com/jira/browse/CHR-10277
	if (false == _target.IsValid())
	{
		GSLOG(Warning, TEXT("[BillboardRemote] Target is invalid."));
		return false;
	}

	FGsGameObjectDataPlayer* targetData = _target->GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == targetData)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] targetData is null."));
		return false;
	}

	if (InData == nullptr)
	{
		GSLOG(Warning, TEXT("[BillboardRemote] FGsGuildPlayerData is null."));
		return false;
	}

	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
	{
		return false;
	}	
	
	// U1 공통 룰: 침공 넘어온 적일 경우 무조건 빨간 칼 아이콘 출력(출력 옵션 상관 없이)
	// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=297698559
	if (UGsUIUtil::IsInvasionEnemy(targetData))
	{
		_panelGuild->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconGuild->SetInvasionEnemyIcon();
		//_imgIconDiplomacy->SetVisibility(ESlateVisibility::Collapsed);
		// 외교 마크(동맹/적대) 표시(침공전 적도 표시)
		// https://jira.com2us.com/jira/browse/CHR-25690
		SetGuildIconDiplomacyByPlayerData(InData);

		return true;
	}

	bool bShowGuild = false;
	if (INVALID_GUILD_DB_ID != InData->_guildDBId)
	{
		// 내 타겟으로 잡힌 유저면 무조건 출력
		if (UGsGameObjectUtil::IsLocalTarget(_target.Get()))
		{
			bShowGuild = true;
		}
		else
		{
			// U1 공통 룰: 나와 같은 길드, 적대, 동맹일 경우 무조건 출력(출력 옵션 상관 없이)
			if (InData->IsMyGuild() || InData->IsEnemy() || InData->IsAlly() || FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar())
			{
				bShowGuild = true;
			}
		}		
	}

	// 길드 UI 출력
	if (bShowGuild)
	{
		_panelGuild->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconGuild->SetEmblemId(InData->_emblemId, false);

		if (FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar())
		{	// 2024/1/4 PKT - 그림자 전장에서는 내 길드가 아니면 무조건 적대 표시
			if (false == InData->IsMyGuild())
			{
				_imgIconDiplomacy->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_imgIconDiplomacy->SetColorAndOpacity(_colorIconEnemy);
			}
			else
			{
				_imgIconDiplomacy->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			// 외교 마크(동맹/적대) 표시
			SetGuildIconDiplomacyByPlayerData(InData);
		}
	}
	else
	{
		_panelGuild->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

void UGsUIBillboardRemotePlayer::SetUITitle(const class FGsTitlePlayerData* InTitleData, int32 InLevel, bool bIsShowOption)
{
	Super::SetUITitle(InTitleData, InLevel, bIsShowOption);
}

void UGsUIBillboardRemotePlayer::UpdateShowHPBar()
{
	bool activeHPBarRoot = IsShowHPBar();
	ShowHPBar(activeHPBarRoot);
}

void UGsUIBillboardRemotePlayer::ShowHPBar(bool In_isShow)
{
	ESlateVisibility hpBarVisibility = 
		(In_isShow == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_panelBarRoot->SetVisibility(hpBarVisibility);
}

bool UGsUIBillboardRemotePlayer::IsShowHPBar()
{
	// 1. cheat option on check
	bool isShowHPBarOption = GOption()->GetIsShowHPbar();
	// 2. check party(party always show)
	bool isParty = false;
	if (true == _target.IsValid())
	{
		isParty = IsParty(_target->GetGameId());
	}

	bool isShowHPBar = (isParty == true) ? true : isShowHPBarOption;
	return isShowHPBar;
}
// 길드 적대, 동맹 표시 처리
void UGsUIBillboardRemotePlayer::SetGuildIconDiplomacyByPlayerData(const FGsGuildPlayerData* In_playerData)
{
	if (In_playerData == nullptr)
	{
		_imgIconDiplomacy->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 외교 마크(동맹/적대) 표시
	if (In_playerData->IsEnemy())
	{
		_imgIconDiplomacy->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_imgIconDiplomacy->SetColorAndOpacity(_colorIconEnemy);
	}
	else if (In_playerData->IsAlly())
	{
		_imgIconDiplomacy->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_imgIconDiplomacy->SetColorAndOpacity(_colorIconAlly);
	}
	else
	{
		_imgIconDiplomacy->SetVisibility(ESlateVisibility::Collapsed);
	}
}