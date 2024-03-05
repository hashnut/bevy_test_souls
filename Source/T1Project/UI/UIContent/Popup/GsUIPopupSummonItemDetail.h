// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Popup/SummonDetailInfo/GsUISummonDetailGradeContents.h"
#include "GsUIPopupSummonItemDetail.generated.h"

class UScrollBox;
class UGsUIBattlePassLevelHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSummonItemDetail : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	struct FGsSummonChangeGroup
	{
		TArray<FGsSummonChangeData> dataList;

		int64 groupProbability{ 0 };
		float groupPerProbability{ 0.f };
	};

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _contentsScroll;
	
	UPROPERTY()
	UGsUIBattlePassLevelHelper* _scrollHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "contentsWidget"))
	TSubclassOf<UUserWidget> _entryContent;

protected:
	TArray<FGsSummonChangeData> _data;

public:
	UFUNCTION(BlueprintCallable)
	void OnClickClose();

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:
	// 뽑기 상태에서 UI_HIDE_DEFAULT 로 안보여 별도로 만듬
	// https://jira.com2us.com/jira/browse/C2URWQ-8110
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SUMMON_ITEM_DETAIL_POPUP; }
public:
	struct PopupInitData
	{
		int32 detailId;
	};

	void InitializeData(PopupInitData* initParam = nullptr);

protected:
	void SetItemDetailId(int32 detailId);
	void RefreshUI();
	virtual void OnInputCancel() override;

private:
	int32 MakeFairyData(int32 effectId, OUT TArray<FGsSummonChangeGroup>& detailElements);
	int32 MakeCostumeData(int32 effectId, OUT TArray<FGsSummonChangeGroup>& detailElements);
};
