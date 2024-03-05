// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsSampleTreeItem.generated.h"

/**
 * TreeView의 데이터 역할 예제. UObject를 상속해야 한다.
 */
UCLASS()
class T1PROJECT_API UGsSampleTreeItem : public UObject
{
	GENERATED_BODY()

protected:
	int32 _id = 0;

	UPROPERTY()
	TArray<UGsSampleTreeItem*> _childList;

public:
	void SetItemId(int32 InId) { _id = InId; }
	int32 GetItemId() const { return _id; }

	void AddChild(UGsSampleTreeItem* InItem);
	TArray<UGsSampleTreeItem*> GetChildList() const { return _childList; }

	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);
};
