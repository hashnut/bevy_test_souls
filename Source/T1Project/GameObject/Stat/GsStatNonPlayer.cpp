// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatNonPlayer.h"
#include "T1Project.h"



void FGsStatNonPlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);

}

void FGsStatNonPlayer::Finalize()
{
	Super::Finalize();
}

void FGsStatNonPlayer::Update()
{
	Super::Update();

}

void FGsStatNonPlayer::UpdateStatInfo(const TArray<TPair<StatType, int32>>& InStatInfoMap)
{
	Super::UpdateStatInfo(InStatInfoMap);
	// 정적 스텟 데이터도 생각 필요
	// 자식에서 할지 , 부모에서 Add 될때 처리할지..
}

