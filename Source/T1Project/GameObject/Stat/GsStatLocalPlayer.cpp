// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatLocalPlayer.h"
#include "T1Project.h"



void FGsStatLocalPlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);

}

void FGsStatLocalPlayer::Finalize()
{
	Super::Finalize();
}

void FGsStatLocalPlayer::Update()
{
	Super::Update();
}


void FGsStatLocalPlayer::UpdateStatInfo(const TArray<TPair<StatType, int32>>& InStatInfoMap)
{
	Super::UpdateStatInfo(InStatInfoMap);
}

void FGsStatLocalPlayer::UpdateLocalPlayerStatData(int32 InLv)
{	
	/*
	const FGsStatLocalPlayerData* statData = GetTableData(InLv);

	if (statData)
	{
		SetCurStatInfo(ECharStatType::TypeLevel, InLv);

		if (FApp::GetBuildConfiguration() != EBuildConfigurations::Shipping)
		{
			if (UGsCheatManager::IsPlayerMighty)
			{	
				return;
			}			
		}
		
		SetCurStatInfo(ECharStatType::TypeLevel, InLv);
		SetCurStatInfo(ECharStatType::TypeCriChance, statData->CriticalChance);
		SetCurStatInfo(ECharStatType::TypeHp, statData->Hp);

		// 최저,최고 공격력 Value 수정
		SetCurStatInfo(ECharStatType::TypeAtkDmg, statData->MinimumAtkDmg);
		SetMaxStatInfo(ECharStatType::TypeAtkDmg, statData->MaximumAtkDmg);

		// 경험치 및 hp 최대 경험치량 수정
		SetMaxStatInfo(ECharStatType::TypeExp, statData->NeedLvUpExp);
		SetMaxStatInfo(ECharStatType::TypeHp, statData->Hp);
	}
	*/

	
}

/*
void FGsStatLocalPlayer::LevelupForCheatCode(int32& InLv)
{
	Super::LevelupForCheatCode(InLv);

	const FGsStatLocalPlayerData* statData = GetTableData(InLv);
	if (nullptr == statData)
	{
		GSLOG(Log, TEXT("Data is null !!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		return;
	}
	
	UpdateLocalPlayerStatData(InLv);
	OnChangeLevelDelegate.Broadcast(InLv);
	SetCurStatInfo(ECharStatType::TypeExp, 0);
}
*/

