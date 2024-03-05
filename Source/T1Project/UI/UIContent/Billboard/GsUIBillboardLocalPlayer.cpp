// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardLocalPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Option/GsOptionValueDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UMG/Public/Components/Image.h"
#include "T1Project.h"



void UGsUIBillboardLocalPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (0 < _delayInvalidateTime)
	{
		_delayInvalidateTime -= InDeltaTime;
		if (0 >= _delayInvalidateTime)
		{
			_delayInvalidateTime = 0.f;
			UpdateHpAndShieldBar();
		}
	}
}

void UGsUIBillboardLocalPlayer::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	UpdateStat();
}

void UGsUIBillboardLocalPlayer::OnChangedStat(const FGsStatInfo& InStatInfo)
{
	Super::OnChangedStat(InStatInfo);

	if (StatType::HP == InStatInfo.GetStatType())
	{
		SetHp(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue(), true);
	}
	else if (StatType::MP == InStatInfo.GetStatType())
	{
		SetMP(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue());
	}
	else if (StatType::HP_SHIELD == InStatInfo.GetStatType())
	{
		SetShield(InStatInfo.GetStatValue(), true);
	}
}

void UGsUIBillboardLocalPlayer::SetAbnormalityConfuse(bool bIsConfuse)
{
	_panelRootBar->SetVisibility(bIsConfuse ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
}

bool UGsUIBillboardLocalPlayer::IsShowNameOption() const
{
	EGsOptionValuePCNameType pcNameType = GetOptionPCNameType();
	switch (pcNameType)
	{
	case EGsOptionValuePCNameType::ALL_ON:
	case EGsOptionValuePCNameType::MY_NAME_ON:
		return true;
	}

	return false;
}

bool UGsUIBillboardLocalPlayer::CheckNameColor(OUT FLinearColor& outColor)
{
	FGsGameObjectDataPlayer* myData = _target->GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == myData)
	{
		GSLOG(Warning, TEXT("[BillboardLocalPlayer] myData is null."));
		return false;
	}

	// GM 캐릭터 전용 색상
	if (myData->GetAccountGrade() == AccountGrade::GM)
	{
		outColor = FGsUIColorHelper::GetColor(EGsUIColorType::GM_PLAYER_DEFAULT);
		return true;
	}

	// 카오틱 수치
	outColor = FGsUIColorHelper::GetColor(EGsUIColorType::LOCAL_PLAYER_DEFAULT);
	if (false == IsPVPAlignmentNeutral())
	{
		outColor = GetPVPAlignmentGradeColor(GetPVPAlignmentGrade());
	}

	return true;
}

bool UGsUIBillboardLocalPlayer::HasServerPrefix() const
{
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;
		WorldId nowMapWorldId = gameDataMgr->GetLoadWorldId();

		return SharedFunc::HasMyUserNamePrefix(gameDataMgr->GetLoadWorldType(), nowMapWorldId, myHomeWorldId);
	}

	return false;
}

bool UGsUIBillboardLocalPlayer::SetUIGuild(const FGsGuildPlayerData* InData, bool bIsShowOption)
{
	bool bShowGuild = false;

	// 길드 정보, 가입한 길드가 있을 경우에만 표시
	if (InData &&
		INVALID_GUILD_DB_ID != InData->_guildDBId)
	{
		if (bIsShowOption)
		{
			bShowGuild = true;
		}
	}

	if (bShowGuild)
	{
		_panelGuild->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconGuild->SetEmblemId(InData->_emblemId, false);
	}
	else
	{
		_panelGuild->SetVisibility(ESlateVisibility::Collapsed);
	}

	return bShowGuild;
}

PVPAlignmentGrade UGsUIBillboardLocalPlayer::GetPVPAlignmentGrade()
{
	return GGameData()->GetUserData()->mLookInfo.mPVPAlignmentGrade;
}

void UGsUIBillboardLocalPlayer::UpdateStat()
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

		if (const FGsStatInfo* mpStat = creature->GetCreatureStatInfoByType(StatType::MP))
		{
			SetMP(mpStat->GetStatValue(), mpStat->GetMaxStatValue());
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

void UGsUIBillboardLocalPlayer::SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		if (0.f != _delayInvalidateTime)
		{
			UpdateHpAndShieldBar();
		}
		_delayInvalidateTime = DELAY_HP_DISPLAY_TIME;
	}
}

void UGsUIBillboardLocalPlayer::SetMP(int32 InCurrent, int32 InMax)
{
	// MP도 애니메이션 처리가 필요하면 넣을 것
	_barMp->SetPercent(FMath::Clamp(static_cast<float>(InCurrent) / static_cast<float>(InMax), 0.0f, 1.0f));
}

void UGsUIBillboardLocalPlayer::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (bInUpdateBar)
	{
		UpdateHpAndShieldBar();
	}
}

void UGsUIBillboardLocalPlayer::UpdateHpAndShieldBar()
{
	SetHPShieldProgressBar(_barShieldFront, _barShieldBack, _currHp, _maxHp, _currShield);
}