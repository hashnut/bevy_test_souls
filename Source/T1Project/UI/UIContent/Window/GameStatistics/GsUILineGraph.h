// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GameStatistics/GsLineGraphData.h"
#include "GsUILineGraph.generated.h"

class UGsUILineGraphLines;
class UGsUILineGraphPoints;
class FGsGameStatisticsData;

/**
 * 꺾은선 그래프 
 */
UCLASS()
class T1PROJECT_API UGsUILineGraph : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 그래프 사이즈 및 열너비 등을 정의
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUILineGraph")
	FVector2D _graphSize;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUILineGraphLines* _uiLines;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUILineGraphPoints* _uiPoints;

protected:
	FGsLineGraphData _data;
	
public:
	// 주의: WBP에서 코드 테스트 위해 만든 함수. 사용하지 말 것.
	UFUNCTION(BlueprintCallable)
	void SetData(TArray<int32> InValueList, EGsStatisticsValueType InValueType);

	void SetData(const FGsGameStatisticsData* InData);
};
