#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupChatReport.generated.h"

class UMultiLineEditableText;
class UEditableTextBox;
class UGsSwitcherButton;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIPopupChatReport : public UGsUIPopup
{
	GENERATED_BODY()

	const int MAX_NICKNAME{ 12 };
	const int MAX_CONTENTS{ 40 };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editEnterText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UMultiLineEditableText* _multiEditContents;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkObscene;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkRepeat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkTradeAdv;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _checkInsult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReport;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickClose();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReport();

	//UFUNCTION(BlueprintCallable, Category = "GsUI")
	//void OnChangedTitleText(const FText& OriginalText);

	//UFUNCTION(BlueprintCallable, Category = "GsUI")
	//void OnChangedContentText(const FText& OriginalText);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCheckButtonChanged(int32 index);

protected:
	virtual void NativeOnInitialized() override;

public:
	struct PopupInitData
	{
		FString _nickName = TEXT("Geena");
		FString _chatText = TEXT("");
	};

	virtual void InitializeData(PopupInitData* initParam = nullptr);

protected:
	FGsToggleGroup _toggleGroup;
	int _selectedCheckIndex{ -1 };

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};

