#include "GsUIHUDSkillTargetSelect.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Skill/GsSkill.h"

#include "Party/GsPartyMemberInfo.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/HUD/Skill/GSUISkillButtonActive.h"
#include "UI/UIContent/HUD/SkillTargetSelect/GsUISkillTargetButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Components/SlateWrapperTypes.h"
#include "UMG/Public/Components/Widget.h"

void UGsUIHUDSkillTargetSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIHUDSkillTargetSelect::OnClickClose);
	_btnNowTarget->OnClicked.AddDynamic(this, &UGsUIHUDSkillTargetSelect::OnClickNowTarget);
}

void UGsUIHUDSkillTargetSelect::NativeConstruct()
{
	Super::NativeConstruct();


	if (FGsMessageHolder* msgM = GMessage())
	{
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_SKILL_TARGET_ALL_LIST_CHANGE, this, &UGsUIHUDSkillTargetSelect::OnAllListChange)
		);

		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_SKILL_TARGET_ONE_LIST_CHANGE, this, &UGsUIHUDSkillTargetSelect::OnOneListChange)
		);

		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::LOCAL_TARGET_CHANGED, this, &UGsUIHUDSkillTargetSelect::OnLocalTargetChanged)
		);

		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::LOCAL_TARGET_CLEAR, this, &UGsUIHUDSkillTargetSelect::OnLocalTargetClear)
		);
	}
}

void UGsUIHUDSkillTargetSelect::NativeDestruct()
{
	if (FGsMessageHolder* msgM = GMessage())
	{
		if (_arrDelegateGameObjectMsg.Num() != 0)
		{
			for (auto iter : _arrDelegateGameObjectMsg)
			{
				msgM->GetGameObject().Remove(iter);
			}
			_arrDelegateGameObjectMsg.Empty();
		}
	}
	Super::NativeDestruct();
}

void UGsUIHUDSkillTargetSelect::OnClickClose()
{
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDSkillTargetSelect::OnClickNowTarget()
{
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

	// send skill now target(same original skill slot click)
	// check original skill slot

	int currentSlotId = GSkill()->GetCurrentSkillTargetSlotId();
	const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(currentSlotId);

	if (slotSkillData == nullptr)
	{
		return;
	}

	if (slotSkillData->IsEnable() == false)
	{
		return;
	}

	if (slotSkillData->IsSelectTarget() == false)
	{
		return;
	}

	// check now target exist
	if (false == targetHandler->SetLastTargetSelectTarget())
	{
		FText findText;
		//“해당 대상을 찾을 수 없습니다.”
		FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_OverVisibleRange"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (UGsGameObjectBase* lastTarget = targetHandler->GetLastTarget())
	{
		// 몬스터 적대 확인
		// https://jira.com2us.com/jira/browse/C2URWQ-5149
		if (lastTarget->IsObjectType(EGsGameObjectType::NonPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
			if (slotSkillData->GetIffType() == IffApplyType::ALLY && false == castLocal->IsEnemy(lastTarget))
			{
				FText findText;
				//"대상에게 해당 스킬을 사용할 수 없습니다."
				FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_UnsuitableSkill"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
				return;
			}
		}
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

void UGsUIHUDSkillTargetSelect::InitSkillTargetSelectData()
{
	// party check	
	FGsPartyManager* partyManager = GSParty();
	if (nullptr == partyManager)
	{
		return;
	}
	int currentPartyButtonIndex = 0;
	// is in party
	if (partyManager->GetIsInParty())
	{
		// member count check
		const TArray<FGsPartyMemberInfo* > arrayPartyMembers = partyManager->GetAllPartyMemberInfo();
		for (FGsPartyMemberInfo* info: arrayPartyMembers)
		{
			if (info == nullptr)
			{
				continue;
			}
			int slotId = info->GetPartySlotId();
			// my info skip
			if (true == partyManager->IsMySlotId(slotId))
			{
				continue;
			}
			// set party slot info
			UGsUISkillTargetButton* findButton = GetPartyMemberTargetButton(currentPartyButtonIndex);

			if (findButton == nullptr)
			{
				continue;
			}
			findButton->SetPartyTargetButtonData(slotId, info->GetIsSpawned(),
				info->GetNowHp(),
				info->GetMaxHp(),
				info->GetNowShield()
			);

			++currentPartyButtonIndex;
		}
	}

	// remain party button hide
	for (int i= currentPartyButtonIndex; _arrayPartyMemberTargetButton.Num() > i ; ++i)
	{
		UGsUISkillTargetButton* iterButton = _arrayPartyMemberTargetButton[i];
		if (iterButton == nullptr)
		{
			continue;
		}

		iterButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	// target exist check
	bool isLastTargetExist = GetLastTarget();
	SetIsTargetExist(isLastTargetExist);
}
void UGsUIHUDSkillTargetSelect::InitOneSkillTargetButton(int In_slotId)
{
	FGsPartyManager* partyManager = GSParty();
	if (nullptr == partyManager)
	{
		return;
	}

	FGsPartyMemberInfo* memberInfo = partyManager->FindPartyMemberInfo(In_slotId);
	if (memberInfo == nullptr)
	{
		return;
	}

	for (UGsUISkillTargetButton* iterButton : _arrayPartyMemberTargetButton)
	{
		if (iterButton == nullptr)
		{
			continue;
		}

		if (iterButton->GetPartySlotId() == In_slotId)
		{
			iterButton->SetPartyTargetButtonData(In_slotId, memberInfo->GetIsSpawned(),
				memberInfo->GetNowHp(),
				memberInfo->GetMaxHp(),
				memberInfo->GetNowShield());

			return;
		}
	}
}
UGsUISkillTargetButton* UGsUIHUDSkillTargetSelect::GetPartyMemberTargetButton(int In_index)
{
	if (_arrayPartyMemberTargetButton.Num() <= In_index)
	{
		return nullptr;
	}
	return _arrayPartyMemberTargetButton[In_index];
}

void UGsUIHUDSkillTargetSelect::SetHUDMode(EGsUIHUDMode InMode)
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	
	if (IsHideMode(InMode) ||
		isShowSkillTargetSelect == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InitSkillTargetSelectData();
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIHUDSkillTargetSelect::UpdateVisible()
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	if (isShowSkillTargetSelect == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InitSkillTargetSelectData();
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

}

// 모든 리스트 변경
void UGsUIHUDSkillTargetSelect::OnAllListChange(const IGsMessageParam*)
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	if (isShowSkillTargetSelect == false)
	{
		return;
	}

	InitSkillTargetSelectData();
}

void UGsUIHUDSkillTargetSelect::OnOneListChange(const IGsMessageParam* In_param)
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	if (isShowSkillTargetSelect == false)
	{
		return;
	}

	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	int slotIndex = param->_data;

	InitOneSkillTargetButton(slotIndex);
}

void UGsUIHUDSkillTargetSelect::SetIsTargetExist(bool In_isExist)
{
	// target not exist image
	// exist hide
	// not exist show
	ESlateVisibility notExistImgVisibility =
		(In_isExist) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible;

	// same state return
	if (_widgetTargetNotExist->GetVisibility() == notExistImgVisibility)
	{
		return;
	}
	// image on, off
	_widgetTargetNotExist->SetVisibility(notExistImgVisibility);
}

void UGsUIHUDSkillTargetSelect::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	if (isShowSkillTargetSelect == false)
	{
		return;
	}

	bool isLastTargetExist = GetLastTarget();
	SetIsTargetExist(isLastTargetExist);
}

void UGsUIHUDSkillTargetSelect::OnLocalTargetClear(const IGsMessageParam*)
{
	bool isShowSkillTargetSelect = GSkill()->GetIsShowSkillTargetSelect();
	if (isShowSkillTargetSelect == false)
	{
		return;
	}

	bool isLastTargetExist = GetLastTarget();
	if (isLastTargetExist == false)
	{
		SetIsTargetExist(false);
	}
}

bool UGsUIHUDSkillTargetSelect::GetLastTarget()
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return false;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return false;
	}

	return (targetHandler->GetLastTarget() != nullptr)? true : false ;
}