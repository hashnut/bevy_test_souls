// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryShapeCircle.h"

bool AGsTerritoryShapeCircle::IsInvalidePointNum(int32 in_num)
{
	return in_num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM;
}
