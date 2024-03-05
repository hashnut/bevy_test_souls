#include "GsUIPartyDrawMenu.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsPartyManager.h"

#include "Party/GsPartyDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIPartyDrawMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnChangeLeader->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickChangeLeader);
	_btnKick->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickKick);

	_btnChannelMove->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickChannelMove);

	_btnUserInfoLeader->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickUserInfo);
	_btnUserInfoImLeader->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickUserInfo);
	_btnUserInfoMemberImNotReader->OnClicked.AddDynamic(this, &UGsUIPartyDrawMenu::OnClickUserInfo);
}

void UGsUIPartyDrawMenu::OnClickChangeLeader()
{
	if (_callbackChangeLeader != nullptr)
	{
		_callbackChangeLeader();
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

// kick
void UGsUIPartyDrawMenu::OnClickKick()
{
	if (_callbackKick != nullptr)
	{
		_callbackKick();
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

// channel move
void UGsUIPartyDrawMenu::OnClickChannelMove()
{
	if (_callbackChannelMove != nullptr)
	{
		_callbackChannelMove();
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}
// user info
void UGsUIPartyDrawMenu::OnClickUserInfo()
{
	if (_callbackUserInfo != nullptr)
	{
		_callbackUserInfo();
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIPartyDrawMenu::SetType(EGsUIPartyHUDDrawMenuSwitchType In_type)
{
	_switcherDrawMenu->SetActiveWidgetIndex((int)In_type);

	if (EGsUIPartyHUDDrawMenuSwitchType::Open_LeaderImNotLeader == In_type)
	{
		bool isDiffMap = false;		
		// 0: moveBtn visible
		// 1: moveBtn not visible, only text show
		EGsPartyWarpToLeaderResult result = GSParty()->IsPossibleWarpToLeader(isDiffMap);
		int switcherChannelMoveIndex = (result == EGsPartyWarpToLeaderResult::WarpPossible) ? 0 : 1;
		_switcherChannelMove->SetActiveWidgetIndex(switcherChannelMoveIndex);		
	}
}