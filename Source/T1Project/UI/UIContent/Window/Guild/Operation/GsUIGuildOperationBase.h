// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIGuildOperationBase.generated.h"

/**
 * 기사단 운영 하위탭 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildOperationBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) {}
	virtual void Hide() {}
	
	virtual void OnReceiveBuildingList() {}
	virtual void OnChangeGuildLevel() {}
	virtual void OnChangeCurrency(CurrencyType InType) {}
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) {}
	virtual void OnChangeMyAuthority() {}
};
