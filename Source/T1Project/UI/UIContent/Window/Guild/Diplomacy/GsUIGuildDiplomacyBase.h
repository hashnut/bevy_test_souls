// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildDiplomacyBase.generated.h"


/**
 * 길드 외교 탭의 각 페이지
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIGuildDiplomacyBase : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsOnReqSearchGuild, const FString& /*InGuildName*/);

public:
	//FGsOnReqSearchGuild _onRequestSearchGuild;

public:
	virtual void Show() {}
	virtual void Hide() {}
	// 리스트 요청. bInResetCoolTime: T: 무조건 요청, F: 쿨타임 있으면 요청하지 않음
	virtual void SendRequestList(bool bInResetCoolTime) {}
	virtual bool IsRedDot() { return false; }

	// 권한 변경
	virtual void OnChangeMyAuthority() {}

	// 외교 길드 검색 결과 받음
	//virtual void OnDiplomacySearchGuild(const class FGsGuildDiplomacyDataBase& InData) {}
	// 외교 리스트 갱신
	virtual void OnDiplomacyInvalidateList() {}
	// 리스트 재요청
	virtual void OnDiplomacyResendList() {}
	// 외교 적대길드 상태 바뀜
	virtual void OnDiplomacyEnemyStateNotify(const struct FGsGuildMsgParamDiplomacyNotify* InParam) {}
	// 외교 동맹 요청 현황 상태 바뀜
	virtual void OnDiplomacyAllyStateNotify(const struct FGsGuildMsgParamDiplomacyNotify* InParam) {}
};
