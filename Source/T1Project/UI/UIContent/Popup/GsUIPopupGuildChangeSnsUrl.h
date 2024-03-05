#pragma once

#include "UI/UILib/Base/GsUIPopup.h"

#include "GsUIPopupGuildChangeSnsUrl.generated.h"

class UEditableTextBox;
class UGsButton;

UCLASS()
class UGsUIPopupGuildChangeSnsUrl final : public UGsUIPopup
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		int32 _index = 0;
		FString _oldSnsLink;
		FString _newSnsLink;
		bool _result = false;
	};

	DECLARE_DELEGATE_OneParam(FOnClosePopup, const UGsUIPopupGuildChangeSnsUrl::Parameters* /*InParameters*/);

	static const FName WIDGET_KEY;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _inputTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

private:
	int32 _guildSnsUrlMaxLength = 40;
	TUniquePtr<Parameters> _parameters;
	FOnClosePopup _onClosePopup;

protected:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	void NativeDestruct() final;

public:
	void SetParameters(TUniquePtr<Parameters> InParameters, FOnClosePopup InOnClosePopup);

private:
	void OnInputOk() final;
	void OnInputCancel() final;
	
protected:
	UFUNCTION()
	void OnTextChanged(const FText& InText);

	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();
};
