// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITargetInfoHUDNormal.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "UI/UIContent/HUD/TargetInfo/GsUIUserInfoHPBar.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "Guild/GsGuildPlayerData.h"
#include "UTIL/GsText.h"
#include "UTIL/GsUIUtil.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

void UGsUITargetInfoHUDNormal::SetTarget(UGsGameObjectCreature* InTarget, const FString& InName)
{
	/**
	 * User 이름에 Prefix를 붙인다.
	 */		
	Super::SetTarget(InTarget, InName);

	if (InTarget->GetObjectType() == EGsGameObjectType::RemotePlayer)
	{	// 2023/9/11 PKT - 유저
		_userContext->SetData(InTarget, InName);
		_userContext->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (MapType::GUILD_WAR == GLevel()->GetCurrentMapType())
		{
			// 2023/12/28 PKT - 그림자 전장 일 경우 상대팀 UserContext 정보는 표기 하지 않는다.
			const UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
			if (localPlayer && localPlayer->GetData() && InTarget->GetData())
			{
				if (localPlayer->GetData()->GetIff().mIffFactionType != InTarget->GetData()->GetIff().mIffFactionType)
				{	// 2023/12/28 PKT - 상대팀이라면..아무런 Context 메뉴를 활성화 하지 않겠다.
					_userContext->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
		
		if (InTarget->GetData())
		{
			// 2023/9/13 PKT - 이름에 Prefix를 붙인다.
			auto userNameAndPrefix = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InName), InTarget->GetData()->GetHomeWorldId());
			_textBlockName->SetText(FText::FromString(userNameAndPrefix));
		}

		SetGuildInfo(InTarget->GetCastData<FGsGameObjectDataPlayer>());
		SetIsMonster(false);
	}
	else
	{
		_userContext->SetVisibility(ESlateVisibility::Collapsed);

		SetGuildInfo(nullptr);
		SetIsMonster(InTarget->GetObjectType() == EGsGameObjectType::NonPlayer);
	}
}

void UGsUITargetInfoHUDNormal::SetGuildInfo(const FGsGameObjectDataPlayer* InPlayerData)
{
	ESlateVisibility guildVisibility = ESlateVisibility::Collapsed;
	if (InPlayerData)
	{
		if (const FGsGuildPlayerData* guildData = InPlayerData->GetGuildData())
		{
			if (INVALID_GUILD_DB_ID != guildData->_guildDBId)
			{
				guildVisibility = ESlateVisibility::SelfHitTestInvisible;

				_iconGuild->SetEmblemId(guildData->_emblemId, false);

				FGsGuildPlayerData* guildDataNoConst = const_cast<FGsGuildPlayerData*>(guildData);
				_textBlockGuildName->SetText(FText::FromString(guildDataNoConst->GetGuildName()));
			}
		}
	}

	_panelGuildRoot->SetVisibility(guildVisibility);
}

//https://jira.com2us.com/jira/browse/C2URWQ-4275
// 몬스터는 hp/ shield 텍스트 출력 안함
void UGsUITargetInfoHUDNormal::SetIsMonster(bool In_isMonster)
{
	ESlateVisibility textVisibility = (In_isMonster == true)? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	_textBlockHpValue->SetVisibility(textVisibility);
	_textBlockShieldValue->SetVisibility(textVisibility);
}