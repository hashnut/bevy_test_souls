// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageContentDungeon.h"

#include "GsUIHUDDungeonTime.generated.h"

struct FTimerHandle;
struct FTimespan;
class FDelegateHandle;
struct IGsMessageParam;
/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDDungeonTime : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _updateSec = 0.5f;

private:
	bool _isShow = false;
	bool _isTimeLimit = true;		// 2022/10/24 PKT - true : 제한 시간 있음. false = 제한 시간 없음.
	//월드 보스 경우 30초 안에 초단위까지 보여줘야함
	bool _isShowSecond = false;

	FDateTime _endDateTime;	

private:
	FText _timeFormat;
	FText _offlineTimeFormat;

private:
	// 메시지 핸들러
	MsgDungeonHUDTimeHandleArray  _listDungeonHUDTimeDelegate;
	MsgDungeonHandleArray _listDungeonDelegate;
	MsgGuildDungeonHandleArray _listGuildDungeonDelegate;
	FTimerHandle _updateTimerHandle;

protected:
	// UUserWidget override.
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	void UpdateTime();
	void PrintTime(const FTimespan& InTimespan, bool InShowSecond);

public:
	void SetData(const IGsMessageParam* inParam);
	void Play(const IGsMessageParam*);
	void Pause(const IGsMessageParam*);
	void Stop(const IGsMessageParam*);
};
