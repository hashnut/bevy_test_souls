// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageUI.h"
#include "MediaPlayer.h"
#include "GsUIWindowDownload.generated.h"

class UProgressBar;
class UTextBlock;
class UMediaPlayer;
class UMediaTexture;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowDownload : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _donloadProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _donloadCheckingBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _donloadCoundText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _donloadStateText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaPlayer* _mediaPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaTexture* _mediaTexture;

	UPROPERTY()
	bool _isShowDonloadProgressBar = false;

	UPROPERTY()
	bool _isShowDonloadCheckingBar = false;

protected:
	uint64	_audioId;		// audio ID
	FString _fileName;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;
	virtual UObject* GetMediaSource(const FString& inMediaSource);

public:
	void SetDownloadText(const FText& inText);
	void SetDownloadCoundText(const FText& inText);
	void SetProgressBarRatio(float inRatio);
	void SetCheckingBarRatio(float inRatio);
	void ShowDonloadProgressBar(bool inIsShow);
	void ShowDonloadCheckingBar(bool inIsShow);
	void PlayMovie();
	void StopMovie();
};
