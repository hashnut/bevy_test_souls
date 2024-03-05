// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Billboard/GsUIBillboardNonPlayer.h"
#include "GsUIBillboardNonPlayerTreasure.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardNonPlayerTreasure : public UGsUIBillboardNonPlayer
{
	GENERATED_BODY()
	
protected:
	//살아있을 수 있는 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textLifeTime;

protected:
	//남은 생존시간
	time_t _endLifeTime = 0; //Invalidate
	//소유 플레이어 이름
	FString _ownPlayerName;
	bool _isLocalPlayerOwn = false;
	bool _isMyTeamPlayerOwn = false;

	bool _isInitialize = false;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void InvalidateAllInternal() override;

public:
	virtual void Reset() override;
public:
	// 소유자 이름 옵션 갱신, local player 소유에 대한 갱신
	virtual void SetOwnMonsterOption(FString ownerName, int32 remainTimeSec = 0, int32 fullTimeSec = 0, bool isLocalPlayerOwn = false) override;
	virtual void SetRemainLifeTime(time_t endTimeTick) override;
	virtual void ChangeOwnMonsterOption() override;

protected:
	// 살아 있을 수 있는 시간 갱신
	void UpdateLifeTime(bool forced = false);
	void SetLifeTimeColor(const FLinearColor& color);

	virtual void SetNameColor(const FLinearColor& InColor) override;
	virtual bool IsRenderShowStateUpdateMonster() override;
};
