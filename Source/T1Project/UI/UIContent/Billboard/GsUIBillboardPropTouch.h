// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardPropTouch.generated.h"

class UProgressBar;
/**
 * 크리쳐 기본 빌보드. 위젯 컴포넌트에서 사용되는 객체.
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardPropTouch : public UGsUIBillboard
{
	GENERATED_BODY()

public:
	// UIHideFlags가 변경 될 경우
	// 변경 시점에 Visibility 상태를 backup 해 뒀다가
	// 복구 될때 backup 상태로 돌아가는데
	// 변경 -> 복구 사이에 Visibility가 변경 되어도
	// 복구 시 backup 상태로 세팅되서
	// 사이에 변경 값을 갱신하도록 적용
	// https://jira.com2us.com/jira/browse/C2URWQ-5491
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
