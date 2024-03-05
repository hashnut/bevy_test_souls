// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsSampleListItem.generated.h"

/**
 * ListView, TileView의 데이터 역할 예제. UObject를 상속해야 한다.
 */
UCLASS()
class T1PROJECT_API UGsSampleListItem : public UObject
{
	GENERATED_BODY()

protected:
	int32 _id = 0;

public:
	void SetItemId(int32 InId) { _id = InId; }
	int32 GetItemId() const { return _id; }
};
