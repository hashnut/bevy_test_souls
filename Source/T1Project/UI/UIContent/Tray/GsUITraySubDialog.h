// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "SubDialog/GsSchemaSubDialogCutData.h"
#include "GsUITraySubDialog.generated.h"

class URichTextBlock;
class AActor;
/**
 * UGsUITraySubDialog
 */
UCLASS()
class T1PROJECT_API UGsUITraySubDialog : public UGsUITray
{
	GENERATED_BODY()	

public:
	DECLARE_DELEGATE(FOnRemainTimeOver);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (MultiLine = true))
	FText _scriptText;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSlateBrush _acterFaceImg;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock*  _richTextScript;

	UPROPERTY()
	float _remainTime = 0.0f;

	UPROPERTY()
	class AActor* _actor = nullptr;

public:
	FOnRemainTimeOver _delegateRemainTimeOver;

private:
	int64 _voiceAudioId = 0;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SUB_DIALOG; }

public:
	// 대화 연출 시작
	void StartAction(const FGsSchemaSubDialogCutData* inCutData, bool inIsVoicePlayable = true);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation();

private:
	// 대화 연출이 끝나면 노티를 준다.
	void OnEndPlayTime();
	// 대화 연출 중인가?
	bool IsPlaying() const;
	// 액터 삭제
	void DestroyActor();

public:
	// 성우 목소리 멈춤
	void OnStopVoice();
};
