// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowIntro.generated.h"

class UMediaPlayer;
class UMediaTexture;
class UPanelWidget;
class UGsButton;

/**
 * UGsUIWindowIntro
 * : 컴투스 CI영상 후 등급(UI애니) 표시
 */
UCLASS()
class T1PROJECT_API UGsUIWindowIntro : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIIntro")
	UMediaPlayer* _mediaPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIIntro")
	UMediaTexture* _mediaTexture;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIntro", meta = (BindWidget))
	UGsButton* _btnSkip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIntro", meta = (BindWidget))
	UPanelWidget* _panelRating;

protected:
	FString _fileName;
	uint64 _soundId = 0;
	bool bIsAnimationStop = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void PlayMovie();
	void StopMovie();
	void PlayRatingAnimation();
	void FinishIntro();

protected:
	UFUNCTION()
	void OnClickSkip();

	UFUNCTION()
	void OnMovieEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "GsUIIntro")
	void StartRatingAnimation();

	UFUNCTION(BlueprintCallable, Category = "GsUIIntro")
	void OnRatingAnimationFinished();

	UFUNCTION()
	void OnWillEnterBackground();
	UFUNCTION()
	void OnHasEnteredForeground();
	
protected:
	// UGsUIWidgetBase override
	virtual bool OnBack() override;
};
