#pragma once
#include "CoreMinimal.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUISummonMileageItem.generated.h"

/* 
*  소환 마일리지 리스트 아이템
*/

class UGsUIProgressBar;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UGsUIIconItem;

class UWidget;

UCLASS()
class UGsUISummonMileageItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 마일리지 제목(페어리, 코스튬)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMileageTitle;
	// 마일리지 보상 정보(획득 불가, 획득 가능 스텝)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMileageRewardInfo;

	// 마일리지 이펙트 위젯(받을 보상이 있을때 표시)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _mileageRewardEffectWidget;
	// 뽑기 마일리지 프로그래스 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _summonMileageProgressBar;

	// 뽑기 마일리지 아이템 셀렉터(보상)
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetSummonMileageItemIconSelector;
	// 뽑기 보상 아이콘 생성하는거
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetSummonMileageIconSlotHelper;

private:
	// 보상 아이템 tbl id
	int _rewardItemTblId = 0;
	// 보상 아이템 갯수
	int _rewardItemCount = 0;
	// 뽑기 타입(페어리, 코스튬)
	EGsSummonType _summonType;
	// 유니크 보상인가
	// https://jira.com2us.com/jira/browse/CHR-24962
	bool _isUniqueReward;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// bp call function
public:
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateTargetIcon(UGsUIIconBase* InIcon);

	// 이벤트
public:
	// 보상 아이콘 클릭
	void OnClickMileageReward(UGsUIIconItem& InIcon);
	// 보상 아이콘 롱프레스
	void OnLongPressMileageReward(UGsUIIconItem& InIcon);

	// set
public:
	// 마일리지 데이터 세팅
	void SetMileageData(bool In_isUniqueReward, EGsSummonType In_summonType, Currency In_mileage, int In_itemCount);
	// 보상 데이터 세팅
	void SetRewardData(int In_rewardItemId, int In_rewardItemCount);
	// 마일리지 제목 텍스트 세팅(코스튬, 페어리)
	void SetMileageTitleText(bool In_isUniqueReward, EGsSummonType In_summonType);
	// 마일리지 보상 텍스트 세팅(획득 가능, 불가)
	void SetMileageRewardInfoText(bool In_isUniqueReward, bool In_isRewardOn);
};