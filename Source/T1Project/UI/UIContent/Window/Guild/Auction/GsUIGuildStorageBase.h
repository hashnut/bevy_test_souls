// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIContent/Window/Guild/GsUIGuildContentStorage.h"

#include "GsUIGuildStorageBase.generated.h"

/**
 * 기사단 정산 - 페이지 베이스
 */
UCLASS()
class T1PROJECT_API UGsUIGuildStorageBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UGsUIGuildContentStorage* _uiContent;

	bool _isStackRecoverFlag = false;

	int32 _realDataCount = 0;

public:
	virtual void Show(bool InIsStackRecoverFlag) {}
	virtual void Hide() {}

	virtual void InvalidateTick() {}

	virtual void SetContentStorage(UGsUIGuildContentStorage* InContent) { _uiContent = InContent; }
};
