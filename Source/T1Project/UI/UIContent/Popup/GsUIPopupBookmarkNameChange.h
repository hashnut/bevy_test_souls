#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "GsUIPopupBookmarkNameChange.generated.h"

/*
	popup 위치 즐겨 찾기 이름 변경
*/

class UEditableTextBox;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIPopupBookmarkNameChange : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 수정 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editUpdateName;
	// ok 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	// cancel 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	// 선택 인덱스
	int _selectIndex = -1;
	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// 이벤트
protected:
	// ok 클릭
	UFUNCTION()
	void OnClickOk();
	// cancel 클릭
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnTextChanged(const FText& InText);
public:
	void SetData(int In_selIndex, const FString& In_oldText);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};