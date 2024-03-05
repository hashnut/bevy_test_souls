// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyListEntryBase.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsUIGuildDiplomacyEnemyListEntry.generated.h"


class UImage;
class UTextBlock;
class UGsButton;
class FGsGuildDiplomacyDataEnemy;

/**
 * 길드 외교 탭의 적대 기사단 페이지의 리스트 아이템
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyEnemyListEntry : public UGsUIGuildDiplomacyListEntryBase,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbEnemyTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	const FGsGuildDiplomacyDataEnemy* _enemyData = nullptr;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const FGsGuildDiplomacyDataEnemy* InData);

protected:
	UFUNCTION()
	void OnClickCancel();
};
