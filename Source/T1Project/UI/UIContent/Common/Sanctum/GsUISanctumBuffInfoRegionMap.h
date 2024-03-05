// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Sanctum/GsUISanctumBuffInfoBase.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISanctumBuffInfoRegionMap.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
class UGsUITooltipDesc;
class UGsUIGuildIcon;

/**
 * 성소 버프 효과 정보 표시(지도)
 */
UCLASS()
class T1PROJECT_API UGsUISanctumBuffInfoRegionMap : public UGsUISanctumBuffInfoBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildIcon* _iconGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRequiredNexusLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTipRequiredNexusLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipRequiredNexusLevel;
	// 스위처
	// 0: 선택된게 있음
	// 1: 선택된게 없음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfoType;

private:
	// ui 메시지 처리용
	MsgUIHandleArray _msgUIHandlerList;
	// 성소 id
	SanctumAreaId _sanctumAreaId;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetData(SanctumAreaId InSanctumId) override;

public:
	UFUNCTION()
	void OnPressTipRequiredNexusLevel();

private:
	void SetUIRequiredNexusLevel(int32 InLevel);
	void InitializeMessage();
	void FinalizeMessage();

private:
	// 성소 정보 갱신됨
	void OnUpdatedSanctumInfoUpdated(const IGsMessageParam*);

public:
	// 선택 된게 있는지
	void SetSelectInfo(bool In_isSelected);
};
