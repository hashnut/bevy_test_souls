// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "GsRankingListItem.generated.h"

struct FGsRankingData;
/**
 * 랭킹 정보 리스트뷰의 데이터
 */
UCLASS()
class T1PROJECT_API UGsRankingListItem : public UObject
{
	GENERATED_BODY()

public:
	const FGsRankingData* _data = nullptr;

	TFunction<void(UWidget*)> OnClickListItem;

public:
	void Clear();
	void SetData(const FGsRankingData* InData);
	void SetClickListItemCallback(TFunction<void(UWidget*)> InCallback) { OnClickListItem = InCallback; }
};
