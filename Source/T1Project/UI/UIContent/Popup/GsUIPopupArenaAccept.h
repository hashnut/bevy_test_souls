#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupArenaAccept.generated.h"


class UTextBlock;
class UProgressBar;
class UGsButton;
struct IGsMessageParam;

/**
 * 대결 수락 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupArenaAccept : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarWait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;


protected:
	TArray<TPair<MessageContentArena, FDelegateHandle>> _msgArenaHandlerList;
	TArray<TPair<MessageStage, FDelegateHandle>> _msgStageHandlerList;
	MsgUIHandleArray _msgUIHandlerList;

	bool _isTimeOver = false;
	double _startTime = 0;
	double _maxRequestTime = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool OnBack() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_ITEM_DETAIL; }

protected:
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickAccept();

	double GetWaitTimeRate();

private:
	void OnCloseArenaUI(const IGsMessageParam* InParam);
	void OnOpenMiddleSmallPopup(const IGsMessageParam* param);
	
public:
	void InitializeUI(Level InLevel, FString InUserName, bool InIsVisible);

};