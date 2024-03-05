#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupEventPrayer.generated.h"

class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIPopupEventPrayer : public UGsUIPopup
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textBuffName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textBuffDetail;

private:
	CreatureId _targetNpcTblId;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ui event
public:
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickOK();

public:
	// set 
	void SetEventPrayerData(CreatureId In_id, FText In_buffName, FText In_buffDetail);
	void SetTargetNpcTblId(CreatureId In_id)
	{
		_targetNpcTblId = In_id;
	}

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};