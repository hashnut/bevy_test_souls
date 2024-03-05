// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUICustomizeImageListEntry.generated.h"

class UImage;
class UGsButton;

/**
 * 커스터마이징 이미지 미리보기 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeImageListEntry : 
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

protected:
	bool _bIsSelected = false;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	// 이미지를 바로 넣을까 경로를 넣을까...
	void SetData(const FSoftObjectPath& InImagePath);

protected:
	UFUNCTION()
	void OnClickSlot();
};
