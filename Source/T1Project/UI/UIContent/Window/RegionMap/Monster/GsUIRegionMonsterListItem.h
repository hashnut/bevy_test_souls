#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GsUIRegionMonsterListItem.generated.h"

class UGsButton;

class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UProgressBar;
/*
	지역맵: 몬스터 리스트 아이템
*/

UCLASS()
class UGsUIRegionMonsterListItem :
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 아이템 버튼(전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 선택 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelTextBlock;

	// 보스 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBoss;

	// long press에서 사용하는 progressbar
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, DisplayName = "maximum 1초 minimum 0초"))
	UProgressBar* _progressBar;

	// 롱프레스 총 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _longPressTotalDurationTime = 1.f;


protected:
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	FText _levelTextFormat;

	bool _bIsSelected = false;
	// 롱프레스 시작했는가
	bool _isStartLongPress = false;
	// 롱프레스 체크 시간(누적)
	float _time = 0.f;

private:
	// 롱프레스 끝나면 상부에 호출할 내용(버튼에서 받아서 전달)
	TFunction<void(int32)> _callbackLongPress;
	// 롱프레스 체크가 끝나면(여기서 계산) 버튼에다가 호출 전달
	TFunction<void(bool)> _callbackFinishLongPress;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override
	{
		return _bIsSelected;
	}

	// 로직 함수
public:
	// 아이템 클릭
	UFUNCTION()
	void OnClickItem();
	// 아이템 릴리즈
	UFUNCTION()
	void OnReleased();
	
	// 보스 세팅
	void SetBoss(EGsReionMapMonsterSwitchType In_switchType);
	// 롱프레스 눌렀다 신호(버튼에서 호출 받음)
	UFUNCTION()
	void OnLongPressIcon();
	// 롱프레스 끝난 시점 호출(리스트 아이템에서 처리)
	void OnLongPressFinished();
	// 롱프레스 관련 초기화
	void InitializeLongPress();
	// 롱프레서 처리 시작
	void StartLongPress();

	// set, get
public:

	void SetText(const FText& In_name);
	void SetRegionMonsterListItem(const FText& In_name, int32 inLevel);

	void SetLongPressCallback(TFunction<void(int32)> In_callback)
	{
		_callbackLongPress = In_callback;
	}
};