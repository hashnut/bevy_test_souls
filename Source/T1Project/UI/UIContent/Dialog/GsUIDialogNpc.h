// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIDialog.h"
#include "NpcDialog/GsSchemaNpcDialogCutData.h"
#include "T1Project/UTIL/GsTimeUtil.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Option/GsOptionEnumType.h"
#include "Quest/Dialog/GsDialogType.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIDialogNpc.generated.h"

struct FGsSchemaNpcDialogCutData;
class UTextBlock;
class URichTextBlock;
class UWidgetSwitcher;
class UImage;
class UGsTextAnimation;
class UGsButton;
class UScrollBox;
class UCurveFloat;
class UGsWidgetPoolManager;
class USpacer;
class UImage;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIDialogNpc : public UGsUIDialog
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnDelegate);

protected:
	FTimerHandle	_effectDelayTimerHandle;
	bool			_isDelayed = false;
	MsgInputHandle _inputParamDelegate;	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _addStayTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _mainCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textActorName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextMy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _panelImgLeft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _panelImgCenter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _panelImgRight;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLeft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgCenter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgRight;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UImage* _imgItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UUserWidget* _nextEffectWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSkip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidgetSwitcher* _swicherType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextNarration;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnTouchNarration;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _panelNarration;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USpacer* _spacerTopNarration;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USpacer* _spacerBottomNarration;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgNarration;

	// TextAnimation
	UPROPERTY()
	class UGsTextAnimation* _textAnimation;

	UPROPERTY()
	class UUserWidget* _effectBpNarration;

	// BP 풀매니저
	TWeakObjectPtr<UGsWidgetPoolManager> _continentalBPPoolManager;

private:
	FString _strScript;
	bool _isAutoNextCut = true;
	uint64 _voiceAudioId = 0;
	EGsNpcDialogType _dialogType = EGsNpcDialogType::DIALOG_TYPE_DIALOG;
	float _currDist = 0.f;
	float _totalDist = 0.f;
	float _currTime = 0.f;
	float _speedNarration = 10.f;
	bool _isPressed = false;
	float _updateSec = 0.25f;
	bool _isNarrationPlaying = false;

public:
	FOnDelegate _delegateRemainTimeOver;
	FOnDelegate _delegateSkip;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) override;

public:
	void StartAction(const FGsSchemaNpcDialogCutData* inCutData, bool inIsAutoNextCut);	
	void PlayEffect(const FGsSchemaNpcDialogCutData* inCutData);
	void SetTextEmpty();
	void HideAll(bool inIsHide);

private:
	void SetImg(const FSoftObjectPath& iconPath);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNext();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSkip();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRemainTimeEnd();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnPressedTouchNarration();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnReleasedTouchNarration();

protected:
	void SetTalkText();
	void SetVisibleNextBtn(bool inValue);
		void PlayEffectNarration(const FGsSchemaNpcDialogCutData* inCutData);

	// BP 만들기
	UUserWidget* MakeContinentalBP(const FSoftObjectPath& In_path);
	UUserWidget* Claim(UClass* In_class);
	// BP 해제
	void ReleaseContinentalBP(UUserWidget* In_BP);

public:
	void OnStopVoice();	
};
