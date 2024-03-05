// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIUserProfileCommentListEntry.generated.h"


class UTextBlock;
class UGsButton;
struct UserComment;
/**
 * 유저 프로필 한마디 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIUserProfileCommentListEntry : 
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockWriteTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockComment;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDelete;

protected:
	UserDBId _userDBId;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const UserComment& InUserComment);

protected:
	void SetTimeText(time_t InTime);

protected:
	UFUNCTION()
	void OnClickDelete();
	UFUNCTION()
	void OnClickUserName();
};