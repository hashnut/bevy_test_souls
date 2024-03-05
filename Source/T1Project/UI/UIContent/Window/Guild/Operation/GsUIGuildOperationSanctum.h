// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GsUIGuildOperationSanctum.generated.h"

class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsUISanctumBuffInfoGuild;
class UGsUIGuildSanctumNexusStateText;

/**
 * 길드 성소탭 페이지 내용
 */
UCLASS()
class T1PROJECT_API UGsUIGuildOperationSanctum : public UGsUIGuildOperationBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	// 0: 수호탑 없음 1: 수호탑 있음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherNexus;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISanctumBuffInfoGuild* _buffInfoUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNexusName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipNexus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgNexus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildSanctumNexusStateText* _nexusStateUI;	
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemoveNexus;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// IGsInvalidateUIInterface override;
protected:
	virtual void InvalidateAllInternal() override;

	// UGsUIGuildOperationBase override
public:
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;

	virtual void OnReceiveBuildingList() override;
	//virtual void OnChangeGuildLevel() override;
	//virtual void OnChangeMyAuthority() override;

public:
	void OnNexusDestruction();

protected:
	UFUNCTION()
	void OnClickTooltipNexus();
	UFUNCTION()
	void OnClickRemoveNexus();

public:
	void SetSanctumNexusImage(class UTexture2D* InTexture);
};
