// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Billboard/GsUIBillboardPlayer.h"
#include "GsUIBillboardLocalPlayer.generated.h"

class UProgressBar;
class UPanelWidget;
class UImage;

/**
 * 내 캐릭터의 네임플레이트
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardLocalPlayer : public UGsUIBillboardPlayer
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barMp;

protected:
	int32 _currHp = 0;
	int32 _maxHp = 0;
	int32 _currShield = 0;
	float _delayInvalidateTime = 0.f;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// UGsUIWidgetBase override
	virtual void InvalidateAllInternal() override;

protected:
	virtual EGsGameObjectType GetOwnerObjetType() const override { return EGsGameObjectType::LocalPlayer; }

public:
	virtual void OnChangedStat(const FGsStatInfo& InStatInfo) override;
	virtual void SetAbnormalityConfuse(bool bIsConfuse) override;

protected:
	// 이름 출력 옵션 갱신
	virtual bool IsShowNameOption() const override;	
	virtual bool CheckNameColor(OUT FLinearColor& outColor) override;
	virtual bool HasServerPrefix() const override;

	virtual bool SetUIGuild(const FGsGuildPlayerData* InData, bool bIsShowOption) override;

	//PVPAlignment
protected:
	virtual PVPAlignmentGrade GetPVPAlignmentGrade() override;

protected:
	void UpdateStat();
	void SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetMP(int32 InCurrent, int32 InMax);
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpAndShieldBar();
};
