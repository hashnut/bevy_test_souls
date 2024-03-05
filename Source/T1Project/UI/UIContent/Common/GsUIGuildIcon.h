// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildIcon.generated.h"

/**
 * 길드 문장
 */
UCLASS()
class T1PROJECT_API UGsUIGuildIcon : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgIcon;

public:
	void SetEmblemId(GuildEmblemId InEmblemId, bool bInBigSize);
	void SetInvasionEnemyIcon();
};
