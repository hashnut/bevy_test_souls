// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUICustomizePresetEntry.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICustomizePresetEntry : 
	public UUserWidget, 
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelected;

	bool _bIsSelected = false;

	// 슬롯에 번호를 출력하기 위한 임시 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textTempId;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	// 번호를 출력하기 위한 임시 처리. 필요 시 변경
	void SetData(int32 InTempId);

protected:
	UFUNCTION()
	void OnClickNormal();
	
	UFUNCTION()
	void OnClickSelected();
};
