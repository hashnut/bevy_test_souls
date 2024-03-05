// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIInvadeMapEntry.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIInvadeMapEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnReqGoMap, MapId);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGoMap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _levelTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _mapIcon;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _mapName;
	FText _levelFormat;

protected:
	TWeakPtr<struct FGsInvadeMap> _mapData;

public:
	FOnReqGoMap OnGoWorldMap;

protected:
	UFUNCTION()
	void OnClickGoWorldMap();

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetMapInfo(TWeakPtr<FGsInvadeMap> mapInfo);
	bool IsValidInvasionMap();
};
