// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIGuildAuthorityEntry.generated.h"

class UTextBlock;
class UGsCheckBox;
class UWidgetSwitcher;

/**
 * 길드 권한 창 리스트 Entry
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuthorityEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	struct FGsAuthoritySlot
	{
		UWidgetSwitcher* _switcher;
		UGsCheckBox* _checkBoxUneditable;
		UGsCheckBox* _checkBoxEditable;
		uint32 _authorityFlag;

		FGsAuthoritySlot() = default;
		FGsAuthoritySlot(UWidgetSwitcher* InSwitcher, uint32 InAuthorityFlag)
		{
			_switcher = InSwitcher;
			_authorityFlag = InAuthorityFlag;
			InitCheckBox();
		}

		void InitCheckBox();
		bool IsChecked() const;
		void SetIsEditable(bool bInIsEditable);
		void SetIsChecked(bool bInIsChecked);
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher0;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher1;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher2;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher3;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher4;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher5;

protected:
	TArray<FGsAuthoritySlot> _slotList;
	GuildMemberGradeType _memberGrade;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(GuildMemberGradeType InType, GuildRightsLevel InAuthority);
	void SetEditableMode(bool bInEditable);

	GuildRightsLevel GetAuthority();

protected:
	void SetCheckBox(int32 InIndex, bool bIsChecked);
};
