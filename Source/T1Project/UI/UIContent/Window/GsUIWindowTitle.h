// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageUI.h"
#include "MediaPlayer.h"
#include "GsUIWindowTitle.generated.h"

class UTextBlock;
class UGsButton;
class UMediaPlayer;
class UMediaTexture;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowTitle : public UGsUIWindow
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClickEvent);
	DECLARE_DELEGATE_OneParam(FOnClickStartEvent, uint16);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOption;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnServerSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAccountChange;

	// BSAM_PATCH_DATA_REPAIR
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* btnPatchCheck;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _serverNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaPlayer* _mediaPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaTexture* _mediaTexture;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherRegisterIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSecurityRegister;

protected:
	uint16	_planetWorldId = 0;
	uint64	_audioId;		// audio ID
	FString _fileName;

public:
	FOnClickEvent OnClickServerSelect;
	FOnClickEvent OnClickOption;
	FOnClickStartEvent OnClickStart;
	FOnClickEvent OnClickAccountChange;	
	FOnClickEvent OnClickPatchCheck; // BSAM_PATCH_DATA_REPAIR
	FOnClickEvent OnClickDeviceRegister;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool IsNotDestroyedDuringLoading() const { return true; }
	virtual bool SyncContentState() override;

	virtual bool OnBack() override;
	virtual void CloseInternal() override;
	virtual UObject* GetMediaSource(const FString& inMediaSource);

protected:
	UFUNCTION()
	void OnClickStartFunction();
	UFUNCTION()
	void OnClickOptionFunction();
	UFUNCTION()
	void OnClickServerSelectFunction();
	UFUNCTION()
	void OnClickAccountChangeFunction();
	UFUNCTION()
	void OnClickPatchCheckFunction(); // BSAM_PATCH_DATA_REPAIR
	UFUNCTION()
	void OnClickDeviceRegisterFunction();

public:
	void SetClickStartEnabled(bool inEnabled);
	void SetServerName(uint16 In_PlanetWorldId);
	void UpdateServerName();
	void SetVisibleOptionBtn(bool In_Visible);
	void SetVisibleAccountChangeBtn(bool In_Visible);
	void SetVisiblePatchCheckBtn(bool In_Visible); // BSAM_PATCH_DATA_REPAIR
	void SetRegisterIcon(bool In_Visible);
	void SetVisibleDeviceRegisterBtn(bool In_Visible);

	void PlayMovie();
	void StopMovie();
};