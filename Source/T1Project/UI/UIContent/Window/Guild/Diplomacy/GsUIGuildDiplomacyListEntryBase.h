// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildDiplomacyListEntryBase.generated.h"

class UImage;
class UTextBlock;
class FGsGuildDiplomacyDataBase;

/**
 * 길드 외교 리스트 아이템의 부모 클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIGuildDiplomacyListEntryBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbGuildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMasterName;	

protected:
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;

protected:
	void SetBaseData(const FGsGuildDiplomacyDataBase* InData);
};
