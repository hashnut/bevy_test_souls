// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Engine/EngineTypes.h"
#include "GsUIBillboardNonPlayer.generated.h"

class FGsStatInfo;
class UProgressBar;
class UPanelWidget;
class UTextBlock;
class UWidget;
/**
 * 플레이어가 아닌 크리쳐의 네임플레이트
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardNonPlayer : public UGsUIBillboardCreature
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _showTime = 2.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _hideTime = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBarRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldBack;

	// 스캔 순서 텍스트 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelTextScanOrder;
	// 스캔 순서 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textScanOrder;

	//살아있을 수 있는 시간
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textLifeTime;
	
protected:
	float _showRemainTime = 0.0f;
	float _hideRemainTime = 0.0f;
	bool _bWasTargetRendered = false;

	int32 _currHp = 0;
	int32 _maxHp = 0;
	int32 _currShield = 0;
	// 스캔 멤버인지
	bool _isScanMember = false;
	// 빌보드 불투명도
	float _billboardOpacity = 0.0f;

	//남은 생존시간
	//time_t _endLifeTime = 0; //Invalidate
	//소유 플레이어 이름
	//FString _ownPlayerName;

	float _backShowTime = 0.f;
	
	//bool _isLocalPlayerOwn = false;
	//bool _isMyTeamPlayerOwn = false;

	float _fdelayInvalidateTime = 0.0f;

protected:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// UGsUIWidgetBase override
	virtual void InvalidateAllInternal() override;

	// UGsUIBillboard override
protected:
	virtual EGsGameObjectType GetOwnerObjetType() const override { return EGsGameObjectType::NonPlayerBase; }

public:
	virtual void Reset() override;

	virtual void OnChangeRenderState(bool bIsRendered) override;
	virtual void OnHit(class UGsGameObjectBase* InAttacker) override;
	virtual void OnTargeting(bool InIsTarget, class UGsGameObjectBase* InOwner, bool InIsFirstTarget = true, bool InIsEnemy = true) override;
	virtual void OnChangedStat(const FGsStatInfo& InStatInfo) override;	

	// 스캔 순서 텍스트 입력
	virtual void SetScanOrderText(const FText& In_name) override;
	// 스캔 멤버인지 변경
	virtual void SetScanMember(bool In_isMember) override;
	virtual bool IsScanNumber() const override { return _isScanMember; }

	// 소유자 이름 옵션 갱신, local player 소유에 대한 갱신
	//virtual void SetOwnMonsterOption(FString ownerName, int32 remainTimeSec = 0, int32 fullTimeSec = 0, bool isLocalPlayerOwn = false) override;
	//virtual void SetRemainLifeTime(time_t endTimeTick) override;
	//virtual void ChangeOwnMonsterOption() override;

	//virtual void UpdateServer() override;

protected:
	void UpdateStat();
	void SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpAndShieldBar();
	// 살아 있을 수 있는 시간 갱신
	void UpdateLifeTime();
	void OnEndShowTime();
	void OnEndHideTime();

	bool IsAlwaysShowHPBar() const { return (0 >= _showTime) ? true : false; }
	bool HasHideAnimation() const { return (0 < _hideTime) ? true : false; }
	void ResetTime(bool In_isForced = false);
	void StartShow();
	bool IsMonsterLike() const;
	// 불투명도 조절
	void SetBillboardOpacity(float In_opacity, bool In_isForced = false);
	// 불투명도 갱신
	void UpdateBillboardOpacity(bool In_isForced = false);

	//void SetLifeTimeColor(const FLinearColor& color);
	virtual bool IsRenderShowStateUpdateMonster() { return false; }

	// hp bar cheat show/hide
	void UpdateShowHPBar();
	void ShowHPBar(bool In_isShow);
	bool IsShowHPBar();

public:
	// 이름 출력 옵션 갱신
	void UpdateShowNameOption();
};
