// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyListEntryBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildDiplomacyAllyListEntry.generated.h"

class UTextBlock;
class UGsButton;
class FGsGuildDiplomacyDataAlly;

/**
 * 길드 외교 탭 동맹 기사단 페이지의 리스트 아이템, 길드 메인탭 동맹 리스트 아이템
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyAllyListEntry : public UGsUIGuildDiplomacyListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbAllyTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemove;

private:
	const FGsGuildDiplomacyDataAlly* _allyData = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const FGsGuildDiplomacyDataAlly* InData);

protected:
	UFUNCTION()
	void OnClickRemove();
};
