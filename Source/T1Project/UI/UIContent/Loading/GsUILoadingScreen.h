// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUILoadingScreen.generated.h"

class UImage;
class UTextBlock;

/**
 * 로딩화면 위젯. 씬전환 시 못지우도록 막아놨음(_bEnableAutoDestroy)
 * MoviePlayer 에 넘긴 후 틱이 돌면서 게임 스레드 관련 문제가 생기는 듯 해서 DisableNativeTick 로 막아봄 
 */
UCLASS(meta=(DisableNativeTick))
class T1PROJECT_API UGsUILoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBG;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTooltip;

protected:
	bool _bNotDestroy = false;
	bool _bIsForUI = false;

public:
	virtual void RemoveFromParent() override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Remove();
	void SetData(const struct FGsSchemaLoadingScreen* InTable);
	void SetTextTooltip(const FText& InText, const FColor& InColor);
	void SetIsForUI(bool bInIsForUI) { _bIsForUI = bInIsForUI; }

	// 주의: 초기화, 파괴시에만 사용할 것. 초기화 시 bNotDestroy 에 의해 설정. 씬전환 시 파괴처리여부.
	void SetNotDestroy(bool bInEnable) { _bNotDestroy = bInEnable; }

	void OnStartLoadingModule();
	void OnEndLoadingModule();
};
