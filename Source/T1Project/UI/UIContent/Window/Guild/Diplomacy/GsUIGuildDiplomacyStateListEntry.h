// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyListEntryBase.h"
#include "GsUIGuildDiplomacyStateListEntry.generated.h"

class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class FGsGuildDiplomacyDataState;

/**
 * 길드 외교 탭의 동맹 현황 페이지의 리스트 아이템
 */
UCLASS()
class T1PROJECT_API UGsUIGuildDiplomacyStateListEntry : public UGsUIGuildDiplomacyListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReject;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	const FGsGuildDiplomacyDataState* _stateData = nullptr;

	// 시간 많이 남았으면 1분마다, 1분 이하이면 1초마다 갱신
	int32 _checkTermSec = 0;
	float _remainCheckTime = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	

public:
	void SetData(const FGsGuildDiplomacyDataState* InData);

protected:
	UFUNCTION()
	void OnClickAccept();
	UFUNCTION()
	void OnClickReject();
	UFUNCTION()
	void OnClickCancel();

protected:
	void UpdateRemainTime();


	bool CheckMaxAllyCount(bool bInShowMessage);
};