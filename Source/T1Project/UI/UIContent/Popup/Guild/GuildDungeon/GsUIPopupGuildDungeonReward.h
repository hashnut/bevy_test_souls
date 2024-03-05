#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Reward/GsRewardHelper.h"
#include "GsUIPopupGuildDungeonReward.generated.h"

/*
	popup 위치 즐겨찾기
*/

class UTextBlock;
class UListView;
class UGsButton;
struct FGsSchemaGuildDungeon;
struct FGsSchemaRewardItemBagData;

UCLASS()
class  T1PROJECT_API UGsUIPopupGuildDungeonReward : public UGsUIPopup
{
	GENERATED_BODY()

		// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _titleTextBlock;
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
		UListView* _rewardListView;
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _closeButton;

private:
	TArray<const FGsSchemaRewardItemBagData*> _rewardItemBagDataList;
	FGsRewardUnboxingData _unboxingData;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void InitRewardList();	
	void InitUnboxingRewardData();

public:
	void SetData(const FText& inTitle, const TArray<const FGsSchemaRewardItemBagData*>& inRewardBagList);
	void SetData(const FText& inTitle, const TArray<const FGsSchemaRewardItemBagData*>& inRewardBagList, const FGsRewardUnboxingData& inRewardUnboxingData);
	
	UFUNCTION()
	void OnClickClose();

	virtual void OnInputCancel() override;
};
