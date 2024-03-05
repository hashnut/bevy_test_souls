// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryLine.h"
#include "DataSchema/Map/Line/GsSchemaLineElement.h"
#include "GsTerritoryLinePatrol.generated.h"

/**
 *
 */

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryLinePatrol : public AGsTerritoryLine
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory Line Patrol")
		bool _isReturn;
	UPROPERTY(EditInstanceOnly, Category = "Territory Line Patrol")
		bool _isRepeat;
	UPROPERTY(EditInstanceOnly, Category = "Territory Line Patrol")
		float _repeatDelay;
	UPROPERTY(EditInstanceOnly, Category = "Territory Line Patrol")
		bool _isSuicide;
	UPROPERTY(EditInstanceOnly, Category = "Territory Line Patrol")
		bool _isReverse;

public:
	AGsTerritoryLinePatrol();
	virtual void InvalidDestription() override;
	void Export(FGsSchemaLineElement& outLineElement);	
	void Import(const FGsSchemaLineElement& inLineElement);

public:
	virtual void OptimizePath() override;

public:
	void SetReturn(bool inReturn){_isReturn = inReturn;}
	void SetRepeat(bool inRepeat) { _isRepeat = inRepeat; }
	void SetRepeatDelay(float inDelay) { _repeatDelay = inDelay; }
	void SetSuicide(bool inSuicide) { _isSuicide = inSuicide; }
	void SetReverse(bool inReverse) { _isReverse = inReverse; }
	virtual EGsTerritoryLineType GetLineType() override { return EGsTerritoryLineType::Patrol; }
};
