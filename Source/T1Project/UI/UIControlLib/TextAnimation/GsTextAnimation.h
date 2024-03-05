// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsTextAnimation.generated.h"

class UTextBlock;
class UDataTable;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsTextAnimation : public UObject
{
	GENERATED_BODY()

private:
	const FString TAG_CLOSE = TEXT("</>");
	
protected:
	UPROPERTY()
	class UTextBlock* _textBlock;

	UPROPERTY()
	class URichTextBlock* _richTextBlock;

protected:
	UPROPERTY()
	float _velocity = 0.0f;

	UPROPERTY()
	int32 _currPos = 0;

	UPROPERTY()
	int32 _textLen = 0;

	UPROPERTY()
	FString _strScript;

	UPROPERTY()
	bool _isFindTag = false;

protected:
	FTimerHandle _timerHandle;
	TArray<FString> _styleNameList;	

public:
	virtual void BeginDestroy() override;	

public:
	// NativeOnInitialized 타이밍에 호출
	void Initialize(float inVelocity, UDataTable* inRichTextStyle = nullptr);
	// 해제
	void Finalize();
	// 업데이트
	void Update(float InDeltaTime);

public:
	// 연출 시작
	void StartAnimation();
	// 연출 중지
	void StopAnimation();
	// 연출 할 전체 스트링 셋팅
	void SetData(class UTextBlock* inTextBlock, FString inString);
	void SetData(class URichTextBlock* inTextBlock, FString inString);
	// 연출 위치 변경
	void SetPostion(int inPos);
	// 연출 중인가?
	bool IsAnimation();

protected:
	virtual void SetTextAnimation();
	void SetText(FString inValue);
}; 