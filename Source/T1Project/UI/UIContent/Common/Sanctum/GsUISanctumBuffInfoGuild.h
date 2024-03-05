// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Sanctum/GsUISanctumBuffInfoBase.h"
#include "GsUISanctumBuffInfoGuild.generated.h"


/**
 * 성소 버프 효과 정보 표시(기사단 운영탭)
 */
UCLASS()
class T1PROJECT_API UGsUISanctumBuffInfoGuild : public UGsUISanctumBuffInfoBase
{
	GENERATED_BODY()

public:
	virtual void SetData(SanctumAreaId InSanctumId) override;
};
