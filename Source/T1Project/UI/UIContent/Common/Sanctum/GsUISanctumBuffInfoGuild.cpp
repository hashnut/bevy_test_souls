// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISanctumBuffInfoGuild.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Sanctum/GsSanctumData.h"


void UGsUISanctumBuffInfoGuild::SetData(SanctumAreaId InSanctumId)
{
	Super::SetData(InSanctumId);

	TWeakPtr<FGsSanctumData> sanctumDataPtr = GSSanctum()->GetSanctumData(InSanctumId);
	if (false == sanctumDataPtr.IsValid())
	{
		return;
	}

	const FGsSanctumData* sanctumData = sanctumDataPtr.Pin().Get();
	if (nullptr == sanctumData)
	{
		return;
	}

	FText textMapName = sanctumData->GetMapName();
	if (textMapName.IsEmpty())
	{
		_textBlockSanctumName->SetText(sanctumData->GetSanctumName());
	}
	else
	{
		FText textSanctumName = sanctumData->GetSanctumName();
		FString strName = FString::Format(TEXT("[{0}] {1}"), 
			{ *(textMapName.ToString()), *(textSanctumName.ToString()) });

		_textBlockSanctumName->SetText(FText::FromString(strName));
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	PassivityId nexusPassivityId = FGsSanctumData::GetNexusPassivityIdStatic(InSanctumId, guildMgr->GetCampLevel());
	SetNexusBuff(nexusPassivityId);	
}
