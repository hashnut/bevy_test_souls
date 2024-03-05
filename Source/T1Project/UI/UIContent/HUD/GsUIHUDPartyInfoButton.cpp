#include "GsUIHUDPartyInfoButton.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsPartyManager.h"

#include "Message/GsMessageGameObject.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

void UGsUIHUDPartyInfoButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnInfo->OnClicked.AddDynamic(this, &UGsUIHUDPartyInfoButton::OnClickPartyInfoButton);
}

// 파티 정보 클릭 
void UGsUIHUDPartyInfoButton::OnClickPartyInfoButton()
{
	if (GSParty()->GetIsInParty())
	{	
		// GSParty()->OpenPartySettingUI();
	}
	else
	{
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PARTY_CREATE_OR_JOIN, true))
		{
			GSParty()->OpenPartySettingUI();
		}
	}


	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDPartyInfoButton::NativeConstruct()
{
	Super::NativeConstruct();


	if (FGsMessageHolder* msgM = GMessage())
	{
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_HUD_SIDE_TAB_UPDATE, this, &UGsUIHUDPartyInfoButton::OnPartyHUDSideTabUpdate)
		);
	}
}

void UGsUIHUDPartyInfoButton::OnPartyHUDSideTabUpdate(const IGsMessageParam*)
{
	// 내가 파티에 속해있나
	bool isInParty = GSParty()->GetIsInParty();

	int switchIndex = (isInParty == true) ? 1 : 0;

	_switcherPartyState->SetActiveWidgetIndex(switchIndex);
	
	if (isInParty == true)
	{
		int memberCount = GSParty()->GetPartyMemberCount();
		PartyDropOwnershipType dropOwnershipType = GSParty()->GetDropOwnershipType();

		FText findText;

		switch (dropOwnershipType)
		{
		case PartyDropOwnershipType::RANDOM:
			// 랜덤
			FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton1"), findText);
			break;
		case PartyDropOwnershipType::SEQUENCE:
			// 순차
			FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton3"), findText);
			break;
		case PartyDropOwnershipType::FREE:
			// 자유
			FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton2"), findText);
			break;
		}

		const FString partyInfoString =
			FString::Printf(TEXT("%s %d"), *findText.ToString(), memberCount);

		_textPartyInfo = FText::FromString(partyInfoString);
	}
}

void UGsUIHUDPartyInfoButton::NativeDestruct()
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