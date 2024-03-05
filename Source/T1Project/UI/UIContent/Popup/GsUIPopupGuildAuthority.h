// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupGuildAuthority.generated.h"

class UGsButton;

/**
 * 길드 권한 창. 권한 변경창이 이 클래스를 상속함.
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuthority : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelListRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChange;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	bool _bIsEditableMode = false;

	// index: 0:단장, 1: 부단장...
	TArray<class UGsUIGuildAuthorityEntry*> _entryList;
	MsgGuildHandle _msgHandleGuild;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	// 변경 패킷을 받음
	void OnReceiveChangeAuthority(const struct IGsMessageParam*);

	// 변경
	UFUNCTION()
	void OnClickChange();
	// 확정
	UFUNCTION()
	void OnClickOk();
	// 취소
	UFUNCTION()
	void OnClickCancel();	

protected:
	void InvalidateData();
	void SetEditableMode(bool bInEditable);
};
