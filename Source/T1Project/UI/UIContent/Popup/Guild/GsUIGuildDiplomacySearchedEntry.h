// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildDiplomacySearchedEntry.generated.h"

class UTextBlock;
class UGsButton;
class FGsGuildData;

/**
 * 기사단 외교 길드 검색 리스트
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacySearchedEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMemberNum;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMaster;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRequest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockButton;

protected:
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	bool _bIsAlly = false;
	bool _bIsOtherServer = false;
	const FGsGuildData* _guildData = nullptr;

protected:
	virtual void NativeOnInitialized() override;

	// IGsEntryInterface override
public:
	virtual void OnHide() override;

public:
	void SetData(const FGsGuildData* InData, bool bInIsAlly);
	
protected:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickRequest();
};