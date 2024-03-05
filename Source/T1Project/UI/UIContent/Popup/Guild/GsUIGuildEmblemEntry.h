// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIGuildEmblemEntry.generated.h"

class UImage;
/**
 * 길드 문장 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIGuildEmblemEntry : 
	public UUserWidget, 
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	// 레벨 제한 아이콘 딤드로 처리할 경우. 버튼 자체를 막는 처리일 경우 제거
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelDimmed;

	// 제한 레벨 텍스트 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockLevel;	

protected:
	bool _bIsSelected = false;	
	int32 _guildLevel = 0;
	int32 _requireGuildLevel = 0;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsEntryInterface override
	virtual void OnHide() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override { return _bIsSelected; }

public:
	void SetData(const struct FGsSchemaGuildEmblem* InData);
	void SetIconImage(class UTexture2D* InTexture);

protected:
	UFUNCTION()
	void OnClickSlot();
};
