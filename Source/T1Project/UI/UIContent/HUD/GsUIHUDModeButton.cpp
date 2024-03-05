// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDModeButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/WidgetSwitcher.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

void UGsUIHUDModeButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnMode->OnClicked.AddDynamic(this, &UGsUIHUDModeButton::OnClickMode);
}

void UGsUIHUDModeButton::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);
	if (GetVisibility() == ESlateVisibility::Hidden)
		return;

	_switchMode->SetActiveWidgetIndex(static_cast<int32>(InMode));
}

void UGsUIHUDModeButton::OnClickMode()
{	
	int32 index = _switchMode->GetActiveWidgetIndex();
	index = (index + 1) % static_cast<int32>(EGsUIHUDMode::MAX_MODE);

	if (UGsUIManager* uiManager = GUI())
	{
		// UIManager에 의해 발송되는 메시지에 의해 갱신된다.
		uiManager->SetHUDMode(static_cast<EGsUIHUDMode>(index));
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}
