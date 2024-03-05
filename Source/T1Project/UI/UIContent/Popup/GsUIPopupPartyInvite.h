#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "GsUIPopupPartyInvite.generated.h"

/*
	popup 파티 초대 보내기
*/

class UEditableTextBox;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIPopupPartyInvite : public UGsUIPopup 
{
	GENERATED_BODY()

protected:
	// 수정 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editInviteeName;
	// ok 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	// cancel 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

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
	UFUNCTION()
	void SetInitInvitee(FString inviteeName);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel();
};