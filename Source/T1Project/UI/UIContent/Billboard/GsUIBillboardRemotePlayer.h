// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Billboard/GsUIBillboardPlayer.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Option/GsOptionValueDefine.h"
#include "GsUIBillboardRemotePlayer.generated.h"


class FGsStatInfo;
class UProgressBar;
class UPanelWidget;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UGsGameObjectBase;

struct FGsGuildPlayerData;

/**
 * 타 플레이어의 네임플레이트
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardRemotePlayer : public UGsUIBillboardPlayer
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _opacityAnimSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBarRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconDiplomacy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textScanOrder;

	// 외교 아이콘 컬러
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	FLinearColor _colorIconAlly;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	FLinearColor _colorIconEnemy;

protected:
	FDelegateHandle _changeStatHandle;
	bool _bWasTargetRendered = false;
	int32 _currHp = 0;
	int32 _maxHp = 0;
	int32 _currShield = 0;
	
	// show name 갱신 dirty
	bool _isDirtyUpdateShowName = false;

protected:
	// UUserWidget override
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	

	// UGsUIWidgetBase override
	virtual void InvalidateAllInternal() override;

	// UGsUIBillboardCreature override.
public:
	virtual void Reset() override;

protected:
	virtual EGsGameObjectType GetOwnerObjetType() const override { return EGsGameObjectType::RemotePlayer; }
	virtual void SetHPPercent(float InPercent) override;	

public:
	virtual void OnChangeRenderState(bool bIsRendered) override;
	virtual void OnTargeting(bool InIsTarget, UGsGameObjectBase* InOwner, bool InIsFirstTarget = true, bool InIsEnemy = true) override;
	virtual void OnChangedStat(const FGsStatInfo& InStatInfo) override;	

	virtual void OnSecondTarget() override;
	
	virtual void SetScanOrderText(const FText& In_name) override;
	virtual void SetScanMember(bool In_isMember) override;
	virtual bool IsScanNumber() const override;

	// UGsUIBillboardPlayer override
protected:
	virtual bool IsShowNameOption() const override;
	virtual bool CheckNameColor(OUT FLinearColor& OutColor) override;
	virtual bool HasServerPrefix() const override;

	virtual PVPAlignmentGrade GetPVPAlignmentGrade() override;

	virtual bool SetUIGuild(const FGsGuildPlayerData* InData, bool bIsShowOption) override;
	virtual void SetUITitle(const class FGsTitlePlayerData* InTitleData, int32 InLevel, bool bIsShowOption) override;
	
protected:
	void UpdateStat();
	void SetHp(int32 InCurrent, int32 InMax, bool bInUpdateBar);	
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpAndShieldBar();

	void UpdateBillboardOpacity();
	void SetOpacityAnimationValue(float InValue);

	// hp bar cheat show/hide
	void UpdateShowHPBar();
	void ShowHPBar(bool In_isShow);
	bool IsShowHPBar();

	void SetBattleArenaTeamMark();

	// 길드 적대, 동맹 표시 처리
	void SetGuildIconDiplomacyByPlayerData(const FGsGuildPlayerData* In_playerData);

private:
	bool IsParty(int64 isGameId);
	bool IsIffNeutral();
	FLinearColor GetIffColor();
};
