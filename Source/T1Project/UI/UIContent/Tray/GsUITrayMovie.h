// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "Sound/SoundBase.h"
#include "Classes/Engine/EngineTypes.h"
#include "Message/GsMessageSystem.h"
#include "GsUITrayMovie.generated.h"

/**
 * Tray for Movie
 */

class UImage;
class UMediaPlayer;
class UMediaSource;
class UMediaTexture;
class UAudioComponent;
class UGsButton;

struct FGsSchemaMovieData;

UCLASS()
class T1PROJECT_API UGsUITrayMovie : public UGsUITray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* _movieImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaPlayer* _mediaPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _skipButton;
	UPROPERTY()
	UAudioComponent* _audioComp;
	// bg image 색상 변경위해 등록
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMediaTexture* _mediaTexture;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animSkipButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animClickSkipEffect;

	// 반복인가
	bool _isLoop = false;
	// 플레이할 무비 데이터
	const FGsSchemaMovieData* _playMovieData;
	// 로딩 무비인지
	bool _isLoadingMovie = false;
	//무비 종료 콜백
	TFunction<void()> _callbackCloseMovie;
	// 버튼 애니메이션 중인가?
	bool _isBtnAnimPlaying = true;
	// sound res ID
	uint64 _soundId = 0;
	// 화면을 터치해야 스킵버튼이 스르륵 나옴 isSkip이 on 일때만 작동
	bool _isSkipEffect = false;

	// 메세지
	MsgSystemHandleArray	_listSystemDelegate;
	MsgSystemHandleArray	_listSystemParamDelegate;

private:
	FDelegateHandle _movieEndHandle;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	virtual void CloseInternal();
	//virtual bool IsNotDestroyedDuringLoading() const override { return true; }
	//virtual int32 GetManagedDefaultZOrder() const override { return 800; }

protected:
	virtual FColor GetClearColor() { return FColor::Black; }
	virtual UObject* GetMediaSource(const FString& inMediaSource);
	virtual bool IsLoadingMovie() const { return false; }

public:	
	bool Play(const FString& inName, bool inIsShowSkipMovieButton = true,
		TFunction<void()> In_callbackOpenFunc = nullptr);
	// 테이블 데이터로 무비 플레이
	bool PlayMovieByTblData(bool In_isLoop);
	void Stop();
	void ClearCloseMovieCallBack();

	void Pause(bool In_isPause);

private:
	bool PlayMediaSource(const FString& inMediaSource,
		bool In_isLoop);

public:
	void HideSkipMovieButton();
	void ShowSkipMovieButton();

public:
	void PlaySound(const FString& inSound);
	UFUNCTION(BlueprintCallable, Category = "GsUIWindowMovie")
	void OnEnd();
	// 스킵 클릭
	UFUNCTION(BlueprintCallable, Category = "GsUIWindowMovie")
	void OnSkip();
	UFUNCTION(BlueprintCallable, Category = "Movie")
	static void PlayMovie(FString inName);
	UFUNCTION()
	void StopMovie();
	
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnBtnAnimFinished();

	UFUNCTION()
	void OnWillEnterBackground();
	UFUNCTION()
	void OnHasEnteredForeground();

	static bool PlayMovieCallbackFunc(FString inName,
		TFunction<void()> In_callbackCloseFunc = nullptr);

	static void StopMovieWithoutCallbackFunc();

	static void MoviePause(bool In_isPause);

	void NetDisconected();			// 네트워크 연결 해제
	void ReconnectStart();			// 재연결 시작
	void ReconnectEnd();			// 재연결 완료

	void LostFocus(const IGsMessageParam*);
	void ReveiveFocus(const IGsMessageParam*);			// 포커스가 다시 들어왔을떄

public:
	UMediaPlayer* GetMediaPlayer() { return _mediaPlayer; }
};
