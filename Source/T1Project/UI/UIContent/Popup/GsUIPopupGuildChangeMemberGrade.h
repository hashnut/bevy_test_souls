// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsStaticPanelSlotHelper.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupGuildChangeMemberGrade.generated.h"

class UWidget;
class UGsButton;
class UImage;
class UTextBlock;

/**
 * 길드원 등급 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildChangeMemberGrade : public UGsUIPopup
{
	GENERATED_BODY()

public:
	struct FGsGuildMemberGradeData
	{
		GuildMemberGradeType _gradeType;
		int32 _maxCount;
		int32 _currCount;		

		explicit FGsGuildMemberGradeData(GuildMemberGradeType InGradeType, int32 InMaxCount, int32 InCurrCount)
			: _gradeType(InGradeType), _maxCount(InMaxCount), _currCount(InCurrCount)
		{

		}

		void Reset()
		{
			_currCount = 0;
		}

		void SetCount(int32 InMaxCount, int32 InCurrCount)
		{
			_maxCount = InMaxCount;
			_currCount = InCurrCount;
		}

		bool IsMaxCount() const
		{
			return (_maxCount == _currCount) ? true : false;
		}
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

protected:
	FGsStaticPanelSlotHelper _slotHelper;
	FGsToggleGroup _toggleGroup;
	UserDBId _userDBId = 0;
	FString _userName;
	GuildMemberGradeType _prevGrade = GuildMemberGradeType::MAX;
	TArray<FGsGuildMemberGradeData> _dataList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(const struct FGsGuildMemberData* InData);

protected:
	void OnRefreshSlotEntry(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	virtual void OnClickCancel();

	UFUNCTION()
	virtual void OnClickOk();

protected:	
	int32 GetDataIndex(GuildMemberGradeType InGrade);
	int32 GetMemberCountByGrade(GuildMemberGradeType InGrade);
};
