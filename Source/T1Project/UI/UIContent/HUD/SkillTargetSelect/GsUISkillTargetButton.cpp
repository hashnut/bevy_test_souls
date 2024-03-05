#include "GsUISkillTargetButton.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"

#include "Message/GsMessageInput.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Skill/GsSkill.h"

#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartyDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/HUD/Skill/GSUISkillButtonActive.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UTIL/GsUIUtil.h"

#include "UMG/Public/Components/SlateWrapperTypes.h"
#include "UMG/Public/Components/Widget.h"

#include "GameObject/Skill/Checker/GsSkillCheckAbnormality.h"
#include "GameObject/Skill/Checker/GsSkillCheckCoolTime.h"
#include "GameObject/Skill/Checker/GsSkillCheckRequireCost.h"
#include "GameObject/Skill/Checker/GsSkillCheckStageMode.h"

void UGsUISkillTargetButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClick->OnClicked.AddDynamic(this, &UGsUISkillTargetButton::OnClickButton);
}

bool UGsUISkillTargetButton::Initialize()
{
	_skillChecker.RemoveAll();
	_skillChecker.Add(new FGsSkillCheckAbnormality());
	_skillChecker.Add(new FGsSkillCheckCoolTime());
	_skillChecker.Add(new FGsSkillCheckRequireCost());
	_skillChecker.Add(new FGsSkillCheckStageMode());
	_skillChecker.Initialize();

	return Super::Initialize();
}

void UGsUISkillTargetButton::BeginDestroy()
{
	_skillChecker.RemoveAll();
	Super::BeginDestroy();
}

void UGsUISkillTargetButton::OnClickButton()
{
	// check original skill slot
	int currentSlotId = GSkill()->GetCurrentSkillTargetSlotId();
	const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(currentSlotId);

	if (slotSkillData == nullptr)
	{
		return;
	}

	if (false == slotSkillData->IsSelectTarget())
	{
		return;
	}

	// set skill select target
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return;
	}

	// 스킬 사용 유효성 검사
	EGsSkillCheckResultType errType = _skillChecker.ProcessCheck(slotSkillData);
	if (errType != EGsSkillCheckResultType::Pass)
	{
		FText errText = _skillChecker.GetErrMessage();
		if (false == errText.IsEmpty())
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errText);
			return;
		}
	}

	if (false == targetHandler->SetPartySkillSelectTarget(_partySlotId))
	{
		FText findText;
		//“해당 대상을 찾을 수 없습니다.”
		FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_OverVisibleRange"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	_inputMsg.Id = currentSlotId;
	GMessage()->GetInput().SendMessage(MessageInput::ClickSkill, _inputMsg);

	if (_clickSelectTargetUseSkillVFXPath.IsValid())
	{
		UClass* vfxClass = GPath()->GetClassByObjectPath(_clickSelectTargetUseSkillVFXPath);
		if (vfxClass)
		{
			UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, _clickSelectTargetUseSkillVFXZOrder);
		}
	}
}

void UGsUISkillTargetButton::SetPartyTargetButtonData(int In_slotId, bool In_isSpawned, int In_currHp, int In_maxHp, int In_currShield)
{
	// slot id
	SetPartySlotId(In_slotId);
	// show widget
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// spawn/ despawn state
	SetIsSpawned(In_isSpawned);

	SetHP(In_currHp, In_maxHp, false);
	SetShield(In_currShield, false);
	UpdateHpShieldBar();
}
void UGsUISkillTargetButton::SetPartySlotId(int In_partySlotId)
{
	if (_partySlotId == In_partySlotId)
	{
		return;
	}

	_partySlotId = In_partySlotId;

	int addId = In_partySlotId + 1;
	const FString slotText = FString::Printf(TEXT("%dP"), addId);
	_textPartySlotNumber = FText::FromString(slotText);
}

void UGsUISkillTargetButton::SetIsSpawned(bool In_isSpawned)
{
	// 디스폰 이미지 처리
	// 스폰 되어있으면 hide
	// 디스폰 되어있으면 show
	ESlateVisibility despawnVisibility =
		(In_isSpawned) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible;

	// 이미 같으면 리턴
	if (_widgetDespawn->GetVisibility() == despawnVisibility)
	{
		return;
	}
	// 디스폰 이미지 on, off
	_widgetDespawn->SetVisibility(despawnVisibility);
}

void UGsUISkillTargetButton::SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}
}

void UGsUISkillTargetButton::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}
}

void UGsUISkillTargetButton::UpdateHpShieldBar()
{
	UGsUIUtil::SetHPShieldProgressBar(_progressBarHp, _progressBarShieldFront, _progressBarShieldBack,
		_currHp, _maxHp, _currShield);

}

void UGsUISkillTargetButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateChangedInfo();	
}

void UGsUISkillTargetButton::UpdateChangedInfo()
{
	FGsPartyMemberInfo* info =
		GSParty()->FindPartyMemberInfo(_partySlotId);
	if (info == nullptr ||
		info->IsChangedSkillTargetSelectInfo() == false)
	{
		return;
	}

	// hp 변경
	if (info->IsChangedSkillTargetSelectInfo(EGsPartyMemberInfoType::HP) == true)
	{
		SetHP(info->GetNowHp(), info->GetMaxHp(), true);
	}
	if (info->IsChangedSkillTargetSelectInfo(EGsPartyMemberInfoType::HP_Shield) == true)
	{
		SetShield(info->GetNowShield(), true);
	}
	// 스폰 변경
	if (info->IsChangedSkillTargetSelectInfo(EGsPartyMemberInfoType::Spawn) == true)
	{
		SetIsSpawned(info->GetIsSpawned());
	}

	info->ClearSkillTargetSelectDirtyInfo();
}