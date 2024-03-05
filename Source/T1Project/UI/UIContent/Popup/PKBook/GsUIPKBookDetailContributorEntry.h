#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPKBookDetailContributorEntry.generated.h"


class UImage;
class UGsButton;
class UTextBlock;
class UWidgetSwitcher;


UCLASS()
class T1PROJECT_API UGsUIPKBookDetailContributorEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	const static int32 SWITCHER_STATE_DAMEGE_INDEX = 0;
	const static int32 SWITCHER_STATE_DETECTION_CLEAR = 1;
	const static int32 SWITCHER_STATE_DETECTION_ADD = 2;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRecordReason;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherContributorType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;

	// 2022/09/26 PKT - 0 : 대미지, 1: 경게 해제, 2 : 경계 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDamege;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetectionTargetClear;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetectionTargetAdd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSetDetectionTarget;	

	// 2022/09/26 PKT - 기여자 정보
	const struct FGsPKRecordingContributorData* _data = nullptr;

	// 2022/09/28 PKT - 기여자가 어떤 일에 대한 기여자인지에 대한 값(Kill / Die)
	PKBookResult _recordReason;

protected:
	virtual void NativeOnInitialized() override;
public:
	virtual void BeginDestroy() override;

protected:
	bool IsSetDetectionTarget() const;

	void SetSwitcherState(int32 InSwiterIndex);

	UFUNCTION()
	void OnClickedDetectionClear();

	UFUNCTION()
	void OnClickedDetectionAdd();

	UFUNCTION()
	void OnClickedSetDetectionTarget();

public:
	void SetData(PKBookResult InRecordReason, const struct FGsPKRecordingContributorData* InData);

	void Invalidate();
};
