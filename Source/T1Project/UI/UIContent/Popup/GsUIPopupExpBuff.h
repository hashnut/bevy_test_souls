//#ifdef R3_ARCAS_NOT_USE
//#pragma once
//#include "CoreMinimal.h"
//#include "Message/GsMessageGameObject.h"
//#include "Message/GsMessageContents.h"
//#include "Message/GsMessageUI.h"
//#include "UI/UILib/Base/GsUIPopup.h"
//#include "GsUIPopupExpBuff.generated.h"
//
//class UProgressBar;
//class UWidgetSwitcher;
//class UTextBlock;
//class UGsButton;
//class UGsSwitcherButton;
//struct IGsMessageParam;
//struct FGsBlessData;
//
//UCLASS()
//class T1PROJECT_API  UGsUIPopupExpBuff : public UGsUIPopup
//{
//	GENERATED_BODY()
//
//protected:
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UProgressBar* _blessProgress;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UGsSwitcherButton* _btnUseFairyBless;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UWidgetSwitcher* _useProtectArcas;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UTextBlock* _textProtectionTime;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UWidgetSwitcher* _useArcasBless;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UGsButton* _btnUseArcasBless;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UTextBlock* _textMeditationTime;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	class UGsButton* _btnClose;
//
//	MsgUIHandle _msgUIHandle;
//	MsgGameObjHandle _msgObjectHandler;
//
//	float _endTime = 0.f;
//	float _checkRemainTime = 0.f;
//
//	FText _meditationTimeFormat;
//	FText _protectionTimeFormat;
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
//
//	UFUNCTION(BlueprintCallable, Category = "GsUI")
//	void OnUseFairyBless(bool select);
//
//	UFUNCTION(BlueprintCallable, Category = "GsUI")
//	void OnUseArcasBless(); //켜는 순간 즉시
//
//	UFUNCTION(BlueprintCallable, Category = "GsUI")
//	void OnClosePopup();
//
//	void UpdateBlessInfo(const IGsMessageParam* InParam);
//	void UpdateAbnormalityInfo(const IGsMessageParam* InParam);
//	void OnForcedClose();
//
//private:
//	void UpdateRemainText();
//
//};
//#endif
