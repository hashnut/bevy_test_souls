#include "GsGameStateBattlePass.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "DataSchema/GsSchemaEnums.h"
#include "Message/MessageParam/GsBattlePassMessageParam.h"

#include "../GsContentsMode.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

FGsGameStateBattlePass::FGsGameStateBattlePass() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsBattlePass)
{
}

FGsGameStateBattlePass::~FGsGameStateBattlePass()
{
}

void FGsGameStateBattlePass::Enter()
{
	FGsContentsGameBase::Enter();
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowBattlePass"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowBattlePass>(widget);
		}
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);
	RegisterMessages();
}

void FGsGameStateBattlePass::Exit()
{
	UnregisterMessages();

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BATTLEPASS, false);

	FGsContentsGameBase::Exit();
}

void FGsGameStateBattlePass::RegisterMessages()
{
	auto& battlePassMsg = GMessage()->GetBattlePass();

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_ACTIVE_ADDREWARD,
		this, &FGsGameStateBattlePass::UpdateBattlePass));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_LEVEL_REWARD,
		this, &FGsGameStateBattlePass::AckBattlePassLevelReward));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_LEVEL_EXP,
		this, &FGsGameStateBattlePass::UpdateBattlePassLevel));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_MISSIONTAB_MISSION_EXP,
		this, &FGsGameStateBattlePass::UpdateBattlePassMission));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_RESET_MISSIONTAB,
		this, &FGsGameStateBattlePass::UpdateBattlePassMissionTab));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_LEVEL_REWARDALL,
		this, &FGsGameStateBattlePass::UpdateBattlePassLevelRewardAll));

	_battlePassDelegates.Emplace(battlePassMsg.AddRaw(MessageContentBattlePass::UPDATE_BATTLEPASS,
		this, &FGsGameStateBattlePass::ModifyBattlePassInfo));

	_msgRedDotHandle = GMessage()->GetHudRedDot().AddRaw(MessageContentHUDRedDot::BATTLEPASS,
		this, &FGsGameStateBattlePass::OnUpdateRedDot);
}

void FGsGameStateBattlePass::UnregisterMessages()
{
	auto& battlePassMsg = GMessage()->GetBattlePass();

	for (auto handler : _battlePassDelegates)
	{
		battlePassMsg.Remove(handler);
	}
	_battlePassDelegates.Empty();

	auto& hudMsg = GMessage()->GetHudRedDot();
	hudMsg.Remove(_msgRedDotHandle);
}

// 추가 리워드 오픈
void FGsGameStateBattlePass::UpdateBattlePass(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const auto param = InParam->Cast<const FGsBattlePassMsgParamPass>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->UpdateBattlePass(param);
			}
		}
	}
}
//리워드 받음
void FGsGameStateBattlePass::AckBattlePassLevelReward(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const auto param = InParam->Cast<const FGsBattlePassMsgParamLevelReward>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->AckBattlePassLevelReward(param);
			}
		}
	}
}

//레벨 구입 또는 레벨 경험치 증가
void FGsGameStateBattlePass::UpdateBattlePassLevel(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (auto param = InParam->Cast<const FGsBattlePassMsgParamLevel>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->UpdateBattlePassLevel(param);
			}
		}
	}
}
//mission 경험치 증가
void FGsGameStateBattlePass::UpdateBattlePassMission(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (auto param = InParam->Cast<const FGsBattlePassMsgParamMission>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->UpdateBattlePassMission(param);
			}
		}
	}
}
//mission tab reset
void FGsGameStateBattlePass::UpdateBattlePassMissionTab(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (auto param = InParam->Cast<const FGsBattlePassMsgParamMissionTab>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->UpdateBattlePassMissionTab(param);
			}
		}
	}
}

void FGsGameStateBattlePass::UpdateBattlePassLevelRewardAll(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (auto param = InParam->Cast<const FGsBattlePassMsgParamLevelRewardAll>())
		{
			if (_windowUI.IsValid())
			{
				_windowUI->AckBattlePassLevelRewardAll(param);
			}
		}
	}
}

void FGsGameStateBattlePass::ModifyBattlePassInfo(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (auto param = InParam->Cast<const FGsBattlePassMsgParamPassModify>())
		{
			if (_windowUI.IsValid() && (param->_isAddedPass || param->_isClosedPass))
			{
				_windowUI->OnForcedClose();
			}
		}
	}
}

void FGsGameStateBattlePass::OnUpdateRedDot(bool bInActive)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->OnUpdateRedDot();
	}
}
