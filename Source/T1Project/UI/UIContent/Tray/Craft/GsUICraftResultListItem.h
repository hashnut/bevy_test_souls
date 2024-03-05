#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUICraftResultListItem.generated.h"

/**
 * 제작 결과를 담는 구조체
 */
struct FCraftResultParameters
{
	FText _title;
	ItemAmount _amount;
	FSoftObjectPath _icon;
	CraftResult _craftResult;
	ItemGrade _itemGrade;
	bool _isEquipment;
	int32 _enchantLevel;
};

class UImage;
class UWidgetSwitcher;
class UGsUIVFX;
class UGsButton;
class UWidget;
class UTextBlock;


UCLASS()
class T1PROJECT_API UGsCraftResultListItemData : public UObject
{
	GENERATED_BODY()

private:
	/** 제작 결과 리스트 아이템 연출 상태 */
	enum class ECraftResultListItemSceneState
	{
		Ready,
		Playing,
		Completed,
	};

private:
	ECraftResultListItemSceneState _sceneState = ECraftResultListItemSceneState::Ready;

	bool _isOpened;

	/** 실패가 없는 경우, 잠깐 카드를 가려놨다가 한꺼번에 오픈할 것이다 */
	bool _isSuccess = false;

protected:
	FCraftResultParameters _parameters;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	/** 결과 입력 */
	void SetParameters(FCraftResultParameters InParamerters) { _parameters = MoveTemp(InParamerters); }
	void SetIsOpened(bool InIsOpened) { _isOpened = InIsOpened; }
	void SetIsSuccess(bool InIsSuccess) { _isSuccess = InIsSuccess; }

	/************************************************************************/
	/* getter                                                               */
	/************************************************************************/

public:
	/** 전체 데이터 조회 */
	FCraftResultParameters GetParameters() const { return _parameters; }

	/** 오픈을 위해 결과를 반환함. */
	CraftResult GetCraftResult() const { return _parameters._craftResult; }

	/** 오픈을 위해 오픈 여부를 반환함. */
	bool IsOpened() const { return _isOpened; }

	/** 실패가 없는 경우, 애니메이션 재생 후 오픈 상태로 전환해야 한다 */
	bool IsSuccess() const { return _isSuccess; }
};

UCLASS()
class UGsUICraftResultListItem final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/** 오프닝 연출의 총 시간(초). 연출이 종료되면 이벤트를 발송하기 위해 사용함. */
	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	float _openingDurationSecs;

	/** 결과를 가려야 할 경우, 이 위젯을 출력하여 가림 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _containerCover;

	///** 메인 버튼. 가려진 상태에서 개별 오픈하기 위해 사용 */
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsButton* _btnMain;
	
	/** 제목 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;
	
	/** 갯수 혹은 강화단계 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtAmount;
	
	/** 아이콘 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

	/** 제작 결과가 "실패" 일 때만 이 위젯을 출력함. 이 위젯은 연출 후에도 계속 유지함 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _failMarker;

	/** None/성공/대성공/실패 순서로 위젯 스위칭 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherResult;
	
	/** 오프닝 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxOpening;
	
	/** 실패 시 출력할 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxFail;

	/** 성공 시 출력할 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxSuccess;
	
	/** 대성공 시 출력할 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxGreatSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _rankImgWidgetSwicher;

protected:
	void NativeOnInitialized() final;

	/** 실제 엔트리의 데이터를 세팅해 주어야 한다 */
	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

public:
	/** 가려진 결과를 오픈함. 매개변수로 오프닝 연출 여부를 입력함 */
	bool OpenCraftResultEntry(const bool InPlayOpening, const CraftResult InCraftResult);

	/** 오픈 관련 */
	void SetOpened(const bool InIsOpened);

	/** 텍스트나 아이콘 관련 데이터를 매칭시켜줌 */
	void SetParameters(const FCraftResultParameters& InParamerters);

	/** 강화 수치 표기 */
	void SetEnchantLevelText(Level InLevel, ItemGrade InItemGrade);

	/************************************************************************/
	/* event																*/
	/************************************************************************/

public:
	/** 결과창을 처음 열었을 때 나오는 오프닝 연출 애니메이션 재생하는 함수 */ 
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation();

	/** 애니메이션 시작 초반, Entry 위젯의 Visibility 및 아이템의 IsSuccess 세팅 (애니메이션 튀는 현상 수정) */
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnEntryWidgetVisibleEvent();
};