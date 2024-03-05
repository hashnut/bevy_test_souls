// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrainUniqueTableDefine.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "T1Project/UTIL/GsText.h"

const FString FGsTerrainUniqueRange::ToString() const
{
	FString temp = TEXT("Min : ") + FString::FromInt(Min) + TEXT(", Max : ") + FString::FromInt(Max) + TEXT("");

	return temp;
}

bool FGsTerrainUnique::InvalideRange()
{
	return Range.InvalideRange();
}

bool FGsTerrainUnique::InvalideRange(FString& out_log)
{
	bool invalide = Range.InvalideRange();

	if (invalide)
	{
		out_log = "WorngRange - type : " + FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), ObjType) + "\trange : "
			+ Range.ToString() + "\n";
	}


	return invalide;
}

bool FGsTerrainUnique::InvalideCount(FString& out_log)
{
	if (Range.Min + Count > Range.Max)
	{
		out_log = TEXT("Count is over than max - type : ") + FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), ObjType) + Range.ToString() + TEXT("\tcount : ") + FString::FromInt(Count) + TEXT("\n");

		return true;
	}

	if (Range.Min + Count > MAX_TERRAIN_UNIQUE_ID)
	{
		out_log = TEXT("Count is over than uint32 - type : ") + FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), ObjType) + Range.ToString() + TEXT("\tcount : ") + FString::FromInt(Count) + TEXT("\n");

		return true;
	}

	return false;
}

bool FGsTerrainUnique::TryCheckOverlapRange(const FGsTerrainUnique* in_range)
{
	return (Range.Min < in_range->Range.Min
		&& in_range->Range.Min < Range.Max)
		&& (Range.Min < in_range->Range.Max && in_range->Range.Max < Range.Max);
}

bool FGsTerrainUnique::TryCheckOverlapRange(const FGsTerrainUnique& in_range, FString& out_log)
{
	bool overlap = (Range.Min < in_range.Range.Min
		&& in_range.Range.Min < Range.Max)
		|| (Range.Min < in_range.Range.Max && in_range.Range.Max < Range.Max);

	if (overlap)
	{
		out_log = "Oevelap source type : " + FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), ObjType)
			+ TEXT("\trange : ") + Range.ToString()
			+ TEXT("\tdest type : ") + FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), in_range.ObjType)
			+ TEXT("\trange : ") + in_range.Range.ToString() + TEXT("\n");
	}

	return overlap;
}


bool FGsTerrainUnique::TryGetUniqueID(int64& out_uniuqe)
{
	out_uniuqe = 0;

	if (Count == TNumericLimits<uint32>::Max())
	{
		return false;
	}

	out_uniuqe = ++Count;

	return true;
}

void FGsTerrainUnique::Set(ETerrainObjType in_obj, FGsTerrainUniqueRange in_range, int64 in_count)
{
	ObjType = in_obj;
	Range.Min = in_range.Min;
	Range.Max = in_range.Max;
	Count = in_count;
}

FString FGsTerrainUnique::ToString()
{
	FString temp;

	temp = FGsTextUtil::GetStringFromEnum(TEXT("ETerrainObjType"), ObjType)
		+ "," + Range.ToString()
		+ "," + FString::FromInt(Count) + "\n";

	return temp;
}

