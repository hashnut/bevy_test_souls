// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageContents.h"
#include "GsUIWindowGuildStart.generated.h"

class UGsButton;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowGuildStart : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCreate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnJoin;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

protected:
	MsgGuildHandle _msgGuildHandle;
	bool _bIsReservedCreate = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;

protected:
	UFUNCTION()
	void OnClickCreate();

	UFUNCTION()
	void OnClickJoin();

protected:
	// 길드 리스트를 받음
	void OnReceiveGuildList(const struct IGsMessageParam*);

protected:
	void CheckAndOpenCreateWindow();
	void CheckAndOpenJoinWindow();

public:
	void StartOpenCreate();
	void StartOpenJoin();
};