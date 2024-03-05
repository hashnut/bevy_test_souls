// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildMemberListEntry.generated.h"

class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
struct FGsGuildMemberData;
class UImage;

/**
 * 기사단원 리스트의 entry
 */
UCLASS()
class T1PROJECT_API UGsUIGuildMemberListEntry : public UUserWidget, 
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;

	// 접속 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockState;

	// 금주 기여도
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockWeeklyContribution;

	// 누적 기여도
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAccumContribution;
	
	// 출석
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgAttendance;

	// 권한있는 사람만 보는 메뉴 스왑
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherManage;

	// 임명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAppointment;

	//추방
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBan;

protected:
	bool _bIsSelected = false;
	const FGsGuildMemberData* _memberData = nullptr;

	TFunction<void(UserDBId, UWidget*)> OnClickUserName;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsEntryInterface override
	virtual void OnHide() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override { return _bIsSelected; }

public:
	void SetData(const struct FGsGuildMemberData* InData);
	void SetClickUserNameCallback(TFunction<void(UserDBId, UWidget*)> InCallback) { OnClickUserName = InCallback; }

protected:
	UFUNCTION()
	void OnClickSlot();

	UFUNCTION()
	void OnClickName();
	
	UFUNCTION()
	void OnClickAppointment();
	
	UFUNCTION()
	void OnClickBan();

protected:
	void SetUIManageMode(bool bManageMode);
	bool IsMyData() const;
};