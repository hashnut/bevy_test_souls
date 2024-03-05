// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialGuildJoin.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


bool FGsTutorialGuildJoin::OnPrevStart()
{	
	// 가입된 길드가 있으면 튜토리얼 바로 종료	
	// 조건체크 단계에서 하지 않는 이유는, 길드 탈퇴시 튜토리얼이 발동하는 것을 막기 위함
	if (GGuild()->IsGuild())
	{	
		return false;
	}

	return Super::OnPrevStart();
}
