// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIAgitDungeonPageBase.generated.h"


/**
 * 기사단 영지 던전들 부모 클래스
 */
UCLASS(Abstract)
class UGsUIAgitDungeonPageBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Show() {}
	//virtual void Hide() {}

public:
	// 시간제 컨텐츠 리셋
	virtual void OnResetTimeContents(ResetTimeTicket InType) {}
	// 재화 변경
	virtual void OnChangeCurrency(CurrencyType InType) {}
	// 멤버 등급 변경
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) {}
};
