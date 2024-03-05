// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUIGuildListEntry.generated.h"

class UTextBlock;
class FGsGuildData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

public:
	enum EGsJoinButtonState
	{
		Immediate = 0, // 즉시가입
		Approval = 1, // 가입 신청
		WaitApproval = 2, // 신청중
		Unable = 3, // 가입불가
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMemberNum;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMaster;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textBlockIntroduction;
	
	// EGsJoinButtonState 인덱스로 사용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _switcherJoin;

protected:
	FGsIntervalChecker _checkerJoin;
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	GuildJoinType _joinType = GuildJoinType::MAX;
	Level _limitLevel = 0;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void OnHide() override;

public:
	void SetData(const class FGsGuildData* InData);
	// 가입요청 가능한 개수인가 (true:가능/false:불가)
	bool CheckJoinRequestCount();
	
protected:
	UFUNCTION()
	void OnClickJoin();

	UFUNCTION()
	void OnClickSlot();
};