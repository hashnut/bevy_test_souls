#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPKTargetDetectionDataEntry.generated.h"


class UGsButton;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIPKTargetDetectionDataEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPeriod;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetectionClear;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSwitchingState;

	UserDBId _userDBId = INVALID_USER_DB_ID;

	time_t _lastAccess = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void BeginDestroy() override;

protected:
	void SetLogInState(const time_t InLastAccess);	

protected:
	UFUNCTION()
	void OnClickedDetectionClear();

	UFUNCTION()
	void OnClickedStateSwitch();

public:
	void SetData(const struct FGsPKDetectionTargetData* InData);
	
	void Invalidate(const time_t InLastAccess);
};
