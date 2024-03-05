// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupGuildDiplomacyConfirm.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class FGsGuildDiplomacyDataBase;
struct IGsMessageParam;

/**
 * 길드 외교 관련 확인창. (동맹 요청/취소, 동맹 수락/거절, 적대 등록, 동맹 해제 등)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildDiplomacyConfirm : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbGuildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMasterName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMessage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	TFunction<void(bool)> _resultFunc;
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	TArray<MsgHandleGuild> _msgHandleGuildList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

private:
	void BindMessages();
	void UnbindMessages();

public:
	void SetData(const FGsGuildDiplomacyDataBase* InData, const FText& InMessage, bool bIsAlly, 
		TFunction<void(bool)> InFunc = nullptr);

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

	void OnEnemyStateNotify(const IGsMessageParam* InParam);
	void OnAllyStateNotify(const IGsMessageParam* InParam);

	void ProcessClose(bool bIsOk);
};
