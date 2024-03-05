#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "GsUIIconRegionFieldBoss.generated.h"

/*
	아이콘: 필드 보스
*/

class UWidgetSwitcher;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIIconRegionFieldBoss : public UGsUIIconDefault
{
	GENERATED_BODY()

protected:

	// 선택 스위치(0: 스폰대기(시간 표시), 1: 스폰 x, 2: 스폰 o)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBossState;
	// 남은 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIcon;
private:
	TFunction<void()> _callbackClick;

protected:
	virtual void NativeOnInitialized() override;

	// logic function
public:
	UFUNCTION()
	void OnClickButton();

	// set
public:
	// 보스 상태 스위칭
	void SetBossState(EGsRegionFieldBossState In_bossState);
	// 남은시간 세팅
	void SetRemainTime(FText In_remainTime);
	void SetClickCallback(TFunction<void()> In_callback)
	{
		_callbackClick = In_callback;
	}
};