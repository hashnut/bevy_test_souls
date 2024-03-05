// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/TargetInfo/GsUITargetInfoHUDBase.h"
#include "GsUITargetInfoHUDBoss.generated.h"

class UPanelWidget;
class UTextBlock;
class UGsUIGuildIcon;
class UGsGameObjectCreature;
class FGsGameObjectDataPlayer;

/**
 * HUD 상단 타겟의 정보. 보스.
 */
UCLASS()
class T1PROJECT_API UGsUITargetInfoHUDBoss : public UGsUITargetInfoHUDBase
{
	GENERATED_BODY()

protected:
	// 보스 어그로 대상 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAggro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserNameAggro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGuildRootAggro;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildIcon* _iconGuildAggro;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildNameAggro;	

protected:
	int64 _targetGameId = 0;
	bool _isShowAggroTarget = false;
	bool _isExistAggroTarget = false;

public:
	virtual void SetTarget(UGsGameObjectCreature* InTarget, const FString& InName) override;
	virtual void ShowAggroTarget(bool InIsBoss) override;

protected:
	void InvalidateAggroTarget();
	void SetAggroTargetGuild(const FGsGameObjectDataPlayer* InPlayerData);
};
