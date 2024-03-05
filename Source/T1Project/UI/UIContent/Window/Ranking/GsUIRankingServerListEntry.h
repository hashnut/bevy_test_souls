// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "GsUIRankingServerListEntry.generated.h"

class UTextBlock;
class UGsButton;
struct FGsSchemaWorldData;
/**
 * 랭킹 서버 목록 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRankingServerListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_OneParam(FOnClickedServer, const FGsSchemaWorldData*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockServer;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnServer;

public:
	FOnClickedServer OnClickedServer;

protected:
	const FGsSchemaWorldData* _worldData = nullptr;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClickServer();

public:
	void SetData(const FGsSchemaWorldData* InWorldData);

};
