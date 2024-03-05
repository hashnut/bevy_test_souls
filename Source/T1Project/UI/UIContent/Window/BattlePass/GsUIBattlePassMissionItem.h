// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBattlePassMissionItem.generated.h"

class UGsUIProgressBar;
class UPanelWidget;
class UGsUIIconBattlePassExp;

struct FGsBattlePassMissionData;
struct IconBattlePassExpData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassMissionItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _titleText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _missionProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIIconBattlePassExp* _iconBattlePassExp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _complete;

protected:
	TWeakPtr<FGsBattlePassMissionData> _data;
	TSharedPtr<IconBattlePassExpData> _iconData;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(TWeakPtr<FGsBattlePassMissionData> data);
	void RefreshUI();
	
};
