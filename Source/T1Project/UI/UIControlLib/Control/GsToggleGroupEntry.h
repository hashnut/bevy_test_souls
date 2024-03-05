// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GsToggleGroupEntry.generated.h"

/**
 * IGsToggleGroupEntry
 * : FGsToggleGroup과 연계하여 사용되는 토글 버튼의 인터페이스. 
 */
UINTERFACE(meta=( CannotImplementInterfaceInBlueprint ))
class T1PROJECT_API UGsToggleGroupEntry : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class T1PROJECT_API IGsToggleGroupEntry
{
	GENERATED_IINTERFACE_BODY()

protected:
	int32 _indexInGroup = -1; // 그룹 내에서의 인덱스. -1: 그룹없음
	TFunction<void(int32, bool)> _toggleCallback;

public:
	// 토글 등록 타이밍에 초기화가 필요할 경우 정의
	virtual void InitializeToggle() {}	

	// 체크되었을때 일어날 UI변화 내용을 정의
	virtual void SetIsSelected(bool bInIsSelected) = 0;
	virtual bool GetIsSelected() const = 0;

public:
	void SetIndexInGroup(int InIndex) { _indexInGroup = InIndex; }
	int32 GetIndexInGroup() const { return _indexInGroup; }
	void SetToggleCallback(TFunction<void(int32, bool)> InToggleCallback) { _toggleCallback = InToggleCallback; }
};
