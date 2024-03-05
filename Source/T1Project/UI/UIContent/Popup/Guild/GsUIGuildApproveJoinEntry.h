// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildApproveJoinEntry.generated.h"

class UGsButton;
class UTextBlock;

/**
 * 길드 가입 승인 창 리스트 Entry
 */
UCLASS()
class T1PROJECT_API UGsUIGuildApproveJoinEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApprove;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReject;

protected:
	UserDBId _userDBId = 0;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void OnHide() override;

public:
	void SetData(const struct FGsGuildJoinWaitingUser& InData);

protected:
	bool CheckAuthority();

protected:
	UFUNCTION()
	void OnClickApprove();

	UFUNCTION()
	void OnClickReject();
};
