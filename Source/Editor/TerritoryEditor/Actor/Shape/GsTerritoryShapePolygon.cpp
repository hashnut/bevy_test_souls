// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryShapePolygon.h"

bool AGsTerritoryShapePolygon::IsInvalidePointNum(int32 in_num)
{
	return in_num < DEFAULT_TERRITORY_POLYGON_POINT_NUM;
}