#include "GsUIPresetSelect.h"

#include "Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Item/GsItemManager.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsPresetManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Management/GsMessageHolder.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIPresetSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnPresetInfo->OnClicked.AddDynamic(this, &UGsUIPresetSelect::OnClickPresetInfo);
	_btnPresetFirst->OnClicked.AddDynamic(this, &UGsUIPresetSelect::OnClickPresetFirst);
	_btnPresetSecond->OnClicked.AddDynamic(this, &UGsUIPresetSelect::OnClickPresetSecond);
	_btnPresetThird->OnClicked.AddDynamic(this, &UGsUIPresetSelect::OnClickPresetThird);
}

void UGsUIPresetSelect::NativeConstruct()
{
	Super::NativeConstruct();

	Hide();

	FGsMessageHolder* msg = GMessage();
	_uiMsgHandlerList.Empty();
	_uiMsgHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::INVALIDATE_PRESET,
		this, &UGsUIPresetSelect::InvalidatePreset));
	_uiMsgHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::INVALIDATE_PRESET_NAME,
		this, &UGsUIPresetSelect::InvalidatePresetName));
}

void UGsUIPresetSelect::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	for (MsgUIHandle& elemItem : _uiMsgHandlerList)
	{
		msg->GetUI().Remove(elemItem);
	}
	_uiMsgHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIPresetSelect::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_isOpened = true;

	InvalidateSelectUI();
}

void UGsUIPresetSelect::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);

	_isOpened = false;
}

void UGsUIPresetSelect::SetHUDMode(bool InIsHideMode)
{
	if (InIsHideMode || false == _isOpened)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIPresetSelect::OnClickPresetInfo()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PRESET), true)
	{
		// 프리셋 정보 팝업 열기
		GUI()->Open(TEXT("PopupPreset"));

		Hide();

		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PRESET);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIPresetSelect::OnClickPresetFirst()
{
	OnClickPresetButton(EGsPresetId::FIRST);
}

void UGsUIPresetSelect::OnClickPresetSecond()
{
	OnClickPresetButton(EGsPresetId::SECOND);
}

void UGsUIPresetSelect::OnClickPresetThird()
{
	OnClickPresetButton(EGsPresetId::THIRD);
}

void UGsUIPresetSelect::OnClickPresetButton(PresetId InPresetId)
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	EGsPresetDimmedState dimmedState = presetManager->GetDimmedState(InPresetId);
	switch (dimmedState)
	{
	case EGsPresetDimmedState::NONE:
	{
		if (InPresetId == presetManager->GetCurrentPresetId())
		{
			
		}
		else
		{
			//GItem()->OnRollbackLoginCreatureWeaponType();
			presetManager->ReqUserPresetChange(InPresetId);
			
			Hide();

			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PRESET);
		}
		break;
	}
	case EGsPresetDimmedState::COOLTIME:
		// 클릭할 수 없는 사유
		break;
	case EGsPresetDimmedState::CROWD_CONTROL:
		// 클릭할 수 없는 사유
		break;
	case EGsPresetDimmedState::CONTENTS_LOCKED:
		// 클릭할 수 없는 사유
		break;
	case EGsPresetDimmedState::COOLTIME_WEAPON:
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		break;
	}
	default:
		break;
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIPresetSelect::InvalidateSelectUI()
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
	{
		return;
	}

	// 버튼 딤드 및 선택 여부 확인
	PresetId currentPrsetId = presetManager->GetCurrentPresetId();
	auto GetSwitcherIndex = [presetManager, currentPrsetId](EGsPresetId InPresetId) -> PresetButtonIndex
	{
		EGsPresetDimmedState dimmedState = presetManager->GetDimmedState(InPresetId);
		return (dimmedState == EGsPresetDimmedState::NONE) ? ((currentPrsetId == InPresetId) ? PresetButtonIndex::SELECTED : PresetButtonIndex::NORMAL) : PresetButtonIndex::DIMMED;
	};

	_btnPresetFirst->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::FIRST));
	_btnPresetSecond->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::SECOND));
	_btnPresetThird->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::THIRD));

	InvalidatePresetName();
}

void UGsUIPresetSelect::InvalidatePreset(const IGsMessageParam* InParam /*= nullptr*/)
{
	InvalidateSelectUI();
}

void UGsUIPresetSelect::InvalidatePresetName(const IGsMessageParam* InParam /*= nullptr*/)
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
	{
		return;
	}

	_textFirst = FText::FromString(presetManager->GetPresetName(EGsPresetId::FIRST));
	_textSecond = FText::FromString(presetManager->GetPresetName(EGsPresetId::SECOND));
	_textThird = FText::FromString(presetManager->GetPresetName(EGsPresetId::THIRD));
}
