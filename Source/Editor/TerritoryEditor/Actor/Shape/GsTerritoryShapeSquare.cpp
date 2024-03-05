// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryShapeSquare.h"

bool AGsTerritoryShapeSquare::IsInvalidePointNum(int32 in_num)
{
	return in_num < DEFAULT_TERRITORY_SQUARE_POINT_NUM;
}