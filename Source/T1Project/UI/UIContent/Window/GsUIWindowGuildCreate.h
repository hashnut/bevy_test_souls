// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageUserInfo.h"
#include "GsUIWindowGuildCreate.generated.h"

class UGsButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowGuildCreate : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgUserInfo = TPair<MessageUserInfo, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnChangeEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _inputBoxName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelJoinType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _inputBoxIntroduction;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnCreateGuild;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	FGsToggleGroup _toggleGroupJoinType;
	int32 _guildEmblemId = 1;
	int32 _createGoldAmount = 0;

	MsgUserInfo _msgUserInfoHandle;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool SyncContentState() override;

private:
	void RegisterMessages();
	void DeregisterMessages();
	
protected:
	UFUNCTION()
	void OnClickChangeEmblem();
	
	UFUNCTION()
	void OnTextChangedGuildName(const FText& InText);

	UFUNCTION()
	void OnTextCommittedGuildName(const FText& InText, ETextCommit::Type InCommitType);
	
	UFUNCTION()
	void OnTextChangedIntroduction(const FText& InText);

	void OnClickTabJoinType(int32 InIndex);
	
	void OnClickCreateGuild();

	void OnChangeCurrency(uint64 InParam);

protected:
	void SetUIGuildEmblem(int32 InGuildEmblemId);

	void InvalidateCreateButton();
};