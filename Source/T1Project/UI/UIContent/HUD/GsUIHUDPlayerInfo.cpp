// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDPlayerInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"

#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/State/GsStateBase.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsPlayerDetailInfoMessageParam.h"

#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

#include "T1Project.h"


void UGsUIHUDPlayerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
	
	// Text: {0}/{1}
	FText::FindText(TEXT("UICommonText"), TEXT("HUDHPValue"), _textValueFormat);

	_detailInfoBtn->OnClicked.AddDynamic(this, &UGsUIHUDPlayerInfo::OnClickPlayerDetailInfo);

	//reddot
	InvalidStatDistributionRedDot();
}

void UGsUIHUDPlayerInfo::NativeDestruct()
{
	UnbindMessages();

	if (_detailInfoBtn->OnClicked.IsBound())
	{
		_detailInfoBtn->OnClicked.RemoveDynamic(this, &UGsUIHUDPlayerInfo::OnClickPlayerDetailInfo);
	}	

	Super::NativeDestruct();
}

bool UGsUIHUDPlayerInfo::UpdateManagerTick(float InDeltaTime)
{
	// NativeTick처럼 현재 보이지 않을 경우 틱을 돌지 않도록 처리
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (0 < _hpBarAnimRemainTime)
	{
		_hpBarAnimRemainTime -= InDeltaTime * _hpBarSpeed;

		if (0 >= _hpBarAnimRemainTime)
		{
			_hpBarAnimRemainTime = 0.f;
			_barHp->SetPercent(_targetHpRatio);
		}
		else
		{
			_barHp->SetPercent(FMath::Lerp(_barHp->Percent, _targetHpRatio, 1 - _hpBarAnimRemainTime));
		}
	}

	if (0 < _delayInvalidateTime)
	{
		_delayInvalidateTime -= InDeltaTime;
		if (0 >= _delayInvalidateTime)
		{
			_delayInvalidateTime = 0.f;
			UpdateHpShieldBar();
		}
	}

	return true;
}

void UGsUIHUDPlayerInfo::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MHudRedDot& msgHudRedDot = msgMgr->GetHudRedDot();
	_redDotMessageHandlerArray.Emplace(msgHudRedDot.AddUObject(MessageContentHUDRedDot::STAT,
		this, &UGsUIHUDPlayerInfo::OnStatDistributionRedDot));

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	_msgHandlerGameObjectList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_ADD,
		this, &UGsUIHUDPlayerInfo::OnAddAbnormality));
	_msgHandlerGameObjectList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
		this, &UGsUIHUDPlayerInfo::OnRemoveAbnormality));
}

void UGsUIHUDPlayerInfo::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MHudRedDot& msgHudRedDot = msgMgr->GetHudRedDot();
	for (MsgHUDRedDotHandle& handle : _redDotMessageHandlerArray)
	{
		msgHudRedDot.Remove(handle);
	}
	_redDotMessageHandlerArray.Empty();

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgHandlerGameObjectList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandlerGameObjectList.Empty();
}

void UGsUIHUDPlayerInfo::OnStatDistributionRedDot(bool inActive)
{
	InvalidStatDistributionRedDot();
}

void UGsUIHUDPlayerInfo::OnAddAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		if (static_cast<int32>(AbnormalityEffectType::UTIL_CONFUSE) == param->_second)
		{
			SetAbnormalityConfuse(true);
		}
	}
}

void UGsUIHUDPlayerInfo::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		if (static_cast<int32>(AbnormalityEffectType::UTIL_CONFUSE) == param->_second)		
		{
			SetAbnormalityConfuse(false);
		}
	}
}

void UGsUIHUDPlayerInfo::InvalidStatDistributionRedDot()
{
	if (_statDistributionRedDot)
	{
		bool isActive = GGameData()->GetUserData()->GetStatPoint() > 0;
		_statDistributionRedDot->SetRedDot(isActive, EGsIconRedDotType::CHECK);
	}
}

void UGsUIHUDPlayerInfo::SetStat(const FGsStatInfo& InStatInfo)
{
	if (StatType::HP == InStatInfo.GetStatType())
	{
		SetHP(InStatInfo.GetStatValue(), InStatInfo.GetMaxStatValue(), true);
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

void UGsUIHUDPlayerInfo::SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		if (0.f != _delayInvalidateTime)
		{
			UpdateHpShieldBar();
		}
		_delayInvalidateTime = DELAY_HP_DISPLAY_TIME;
	}
	else
	{
		if (false == _textValueFormat.IsEmpty())
		{
			_hpValueText->SetText(FText::Format(_textValueFormat, FText::AsNumber(_currHp), FText::AsNumber(_maxHp)));
		}
	}
}

void UGsUIHUDPlayerInfo::SetMP(int32 InCurrent, int32 InMax)
{
	// MP도 애니메이션 처리가 필요하면 넣을 것
	_barMp->SetPercent(FMath::Clamp(static_cast<float>(InCurrent) / static_cast<float>(InMax), 0.0f, 1.0f));	

	if (false == _textValueFormat.IsEmpty())
	{
		_mpValueText->SetText(FText::Format(_textValueFormat, FText::AsNumber(InCurrent), FText::AsNumber(InMax)));
	}
}

void UGsUIHUDPlayerInfo::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (0 >= _currShield)
	{		
		_shieldValueText->SetText(FText::GetEmpty());
	}
	else
	{
		FString strShield = FString::Format(TEXT("(+{0})"), { _currShield });
		_shieldValueText->SetText(FText::FromString(strShield));
	}

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}
}

void UGsUIHUDPlayerInfo::UpdateHpShieldBar()
{
	if (false == _textValueFormat.IsEmpty())
	{
		_hpValueText->SetText(FText::Format(_textValueFormat, FText::AsNumber(_currHp), FText::AsNumber(_maxHp)));
	}

	// 실드가 없을 경우 
	if (0 >= _currShield)
	{
		// 실드 모두 가림
		_barShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		_barShieldBack->SetVisibility(ESlateVisibility::Collapsed);
		
		float hpRatio = FMath::Clamp(static_cast<float>(_currHp) / static_cast<float>(_maxHp), 0.0f, 1.0f);
		if (_targetHpRatio == hpRatio)
		{
			_barHp->SetPercent(_targetHpRatio);	
		}
		else
		{
			// HP바 애니메이션 시간 세팅
			_hpBarAnimRemainTime = 1.0f;
			_targetHpRatio = hpRatio;
		}

		return;
	}

	// 애니메이션 시간 제거, 실드가 있을 땐 애니메이션 없음
	_targetHpRatio = FMath::Clamp(static_cast<float>(_currHp) / static_cast<float>(_maxHp), 0.0f, 1.0f);
	_barHp->SetPercent(_targetHpRatio);
	_hpBarAnimRemainTime = 0.f;
	
	// maxHp를 초과할 경우
	if (_currShield + _currHp > _maxHp)
	{
		_barShieldFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_barShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		_barShieldFront->SetPercent(FMath::Clamp(
			static_cast<float>(_currShield) / static_cast<float>(_maxHp), 0.0f, 1.0f));
	}
	else
	{
		_barShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		_barShieldBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// HP바 뒤로 나와보이도록 더해서 계산
		_barShieldBack->SetPercent(FMath::Clamp(
			static_cast<float>(_currHp + _currShield) / static_cast<float>(_maxHp), 0.0f, 1.0f));
	}
}

void UGsUIHUDPlayerInfo::SetAbnormalityConfuse(bool bIsConfuse)
{
	_panelRootBar->SetVisibility(bIsConfuse ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIHUDPlayerInfo::InvalidateAllInternal()
{
	// HP 세팅
	UGsGameObjectLocalPlayer* localPlayer = nullptr;
	if (UGsGameObjectManager* spawner = GSGameObject())
	{
		localPlayer = Cast<UGsGameObjectLocalPlayer>(spawner->FindObject(EGsGameObjectType::LocalPlayer));
	}

	if (nullptr == localPlayer)
	{
		return;
	}

	// HP
	if (const FGsStatInfo* hpInfo = localPlayer->GetCreatureStatInfoByType(StatType::HP))
	{
		SetHP(hpInfo->GetStatValue(), hpInfo->GetMaxStatValue(), false);
	}

	// MP
	if (const FGsStatInfo* mpInfo = localPlayer->GetCreatureStatInfoByType(StatType::MP))
	{
		SetMP(mpInfo->GetStatValue(), mpInfo->GetMaxStatValue());
	}

	// Shield
	if (const FGsStatInfo* shieldInfo = localPlayer->GetCreatureStatInfoByType(StatType::HP_SHIELD))
	{
		SetShield(shieldInfo->GetStatValue(), false);
	}
	else
	{
		SetShield(0, false);
	}

	// 두번 호출하지 않기 위해 위에 false 인자 넘긴 후 처리
	UpdateHpShieldBar();

	// 레벨 세팅
	FGsGameDataManager* dataManager = GGameData();
	const FGsNetUserData* userData = dataManager->GetUserData();
	if (nullptr == userData)
	{
		return;
	}

	_textLevel = FText::AsNumber(userData->mLevel);
}

void UGsUIHUDPlayerInfo::OnClickPlayerDetailInfo()
{
	// 뒤로가기 광클시 눌려서 이상해지는 현상 방지를 위해 HUDState인지 체크(C2URWQ-6911)
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (FGsContentsMode::InGame::ContentsHud != contents->GetCurrentContentsMode())
			{
				return;
			}
		}
	}

	GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::OPEN, nullptr);

	GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::CLOSE_ABORT);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDPlayerInfo::SetSaveBatteryMode()
{
	if (_detailInfoBtn->OnClicked.IsBound())
	{
		_detailInfoBtn->OnClicked.RemoveDynamic(this, &UGsUIHUDPlayerInfo::OnClickPlayerDetailInfo);
	}
}
