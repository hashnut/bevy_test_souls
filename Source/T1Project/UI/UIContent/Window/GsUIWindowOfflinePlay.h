// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowOfflinePlay.generated.h"


class UGsButton;
class UTextBlock;
class UGsCheckBox;

/**
 * 
 */

UCLASS()
class T1PROJECT_API UGsUIWindowOfflinePlay : public UGsUIWindow
{
	GENERATED_BODY()
	
	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	/**
	 * 2021/06/25 PKT - Common Button( Back / Close )
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	/**
	 * 2021/06/25 PKT - 남은 시간
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFreeRemainTimeNum;

	/**
	 * 2021/06/25 PKT - 돌발적 종료에 대한 대응 옵션
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxUsePlay;

	/**
	 * 2021/06/25 PKT - 오프라인 플레이 시작
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOffLinePlayStart;



	
	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
protected:
	void NativeOnInitialized() override;
	void BeginDestroy() override;

	void NativeConstruct() override;
	void NativeDestruct() override;

	virtual bool SyncContentState() override;

	/************************************************************************/
	/* Clicked                                                              */
	/************************************************************************/
	UFUNCTION()
	void OnStartButtonClicked();

	/************************************************************************/
	/* Refresh                                                              */
	/************************************************************************/
	void OnRefreshTime();
	void OnRefreshButtonEnable(bool isEnable);


	/************************************************************************/
	/* Invalidate                                                           */
	/************************************************************************/
public:
	void InvalidateTime();
};
