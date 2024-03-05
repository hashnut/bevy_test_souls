// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITooltip.generated.h"

/**
 * 툴팁 기본 클래스. 외부 클릭 시 Hide 처리.
 * - 주의: 포커스로 외부 클릭을 판단하므로 내부에 다른 포커스 갖는 항목 있으면(예: 버튼) 별도 처리가 필요하다.
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUITooltip : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;	
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

public:
	virtual void Open();
	virtual void Close();

public:
	bool IsOpen() const;
};
