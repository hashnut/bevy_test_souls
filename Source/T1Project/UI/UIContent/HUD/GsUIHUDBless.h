//#pragma once
//
//#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
//#include "Message/GsMessageGameObject.h"
//#include "UI/UILib/Base/GsUIHUD.h"
//#include "UI/UILib/Define/GsUIDefine.h"
//#include "GsUIHUDBless.generated.h"
//
//class UImage;
//class UGsSwitcherButton;
//class UTextBlock;
//class UWidgetSwitcher;
//struct FGsBlessData;
//struct IGsMessageParam;
//
//UENUM(BlueprintType)
//enum class BlessHUDState : uint8
//{
//	NONE = 0,
//	FAIRY_BLESS = 1,
//	ARCASS_BLESS = 2,
//	MEDITAION = 3,
//	MAX = MEDITAION,
//};
//
//UCLASS()
//class T1PROJECT_API UGsUIHUDBless : public UGsUIHUD
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UGsSwitcherButton* _btnBless;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UWidgetSwitcher* _blessSwitcher;
//	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (BindWidget))
//		TMap<BlessHUDState, uint8> _switchIndex;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UTextBlock* _textPoint;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UTextBlock* _textPointDisable;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UTextBlock* _textPointFairy;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UTextBlock* _remainMeditation;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UImage* _iconBlessState;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//		UImage* _iconNoneBlessState;
//
//private:
//	bool _initialized{ false };
//	//FLinearColor _pointMaxColor;
//	//FLinearColor _pointMinColor;
//
//	UFUNCTION(BlueprintCallable, Category = "GsUI")
//		void OnClickBlesState();
//
//	TArray<UTextBlock*> _points;
//
//	bool _isProtection{ false };
//	MsgGameObjHandle _msgHandlerObject;
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
//	virtual void SetHUDMode(EGsUIHUDMode InMode) override;
//	virtual void InvalidateAllInternal() override;
//
//	void RegisterMessage();
//	void UnRegisterMessage();
//
//public:
//	void OnChangedProtection(const IGsMessageParam* InParam);
//	void OnChangedBlessPoint(uint32 point);
//};
