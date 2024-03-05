// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/TargetInfo/GsUITargetInfoHUDBase.h"
#include "GsUITargetInfoHUDNormal.generated.h"

class UPanelWidget;
class UTextBlock;
class UGsUIGuildIcon;
class UGsUIUserInfoHPBar;
class UGsGameObjectCreature;
class FGsGameObjectDataPlayer;

/**
 * HUD 상단 타겟의 정보. 일반 몹, 타 유저.
 */
UCLASS()
class T1PROJECT_API UGsUITargetInfoHUDNormal : public UGsUITargetInfoHUDBase
{
	GENERATED_BODY()

protected:
	// 길드정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGuildRoot;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildIcon* _iconGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;

	// 유저
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIUserInfoHPBar* _userContext;

public:
	virtual void SetTarget(UGsGameObjectCreature* InTarget, const FString& InName) override;

protected:
	void SetGuildInfo(const FGsGameObjectDataPlayer* InPlayerData);

public:
	//https://jira.com2us.com/jira/browse/C2URWQ-4275
	// 몬스터는 hp/ shield 텍스트 출력 안함
	void SetIsMonster(bool In_isMonster);
};
