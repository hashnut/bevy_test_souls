// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryUniqueTable.h"

//T1 Project
#include "UTIL/GsText.h"
/**
 *
 */

const FString FGsTerritoryUniqueIDRange::ToString() const
{
	FString temp = TEXT("Min : ") + FString::FromInt(Min) + TEXT(", Max : ") + FString::FromInt(Max) + TEXT("");

	return temp;
}

FString FGsTerritoryUniqueIDRange::ToCSV()
{
	FString temp = TEXT("\"(Min=") + FString::FromInt(Min) + TEXT(",Max=") + FString::FromInt(Max) + TEXT(")\"");

	return temp;
}

bool FGsTerritoryUniqueIDRow::InvalideRange(FString& out_log)
{
	bool invalide = Range.InvalideRange();

	if (invalide)
	{
		out_log = "WorngRange - type : " + FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), ObjType) + "\trange : "
			+ Range.ToString() + "\n";
	}

	return invalide;
}

void FGsTerritoryUniqueIDRow::Set(EGsTerritoryObjType in_obj, FGsTerritoryUniqueIDRange in_range, int64 in_count)
{
	ObjType = in_obj;
	Range.Min = in_range.Min;
	Range.Max = in_range.Max;
	Count = in_count;
}

void FGsTerritoryUniqueIDRow::Set(EGsTerritoryObjType inObj, int64 inMin, int64 inMax, int64 inCount)
{
	ObjType = inObj;
	Range.Min = inMin;
	Range.Max = inMax;
	Count = inCount;
}

FString FGsTerritoryUniqueIDRow::ToString()
{
	FString temp;

	temp = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), ObjType)
		+ "," + Range.ToString()
		+ "," + FString::FromInt(Count) + "\n";

	return temp;
}

FString FGsTerritoryUniqueIDRow::ToCSV()
{
	FString temp = "\"" + FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), ObjType) + "\"" +
		+"," + Range.ToCSV()
		+ ",\"" + FString::FromInt(Count) + "\"\n";

	return temp;
}

FString FGsTerritoryUniqueIDRow::GetCSVHeader()
{
	return ("---,Obj Type,Range,Count\n");
}

bool FGsTerritoryUniqueIDRow::TryCheckOverlapRange(const FGsTerritoryUniqueIDRow& in_range, FString& out_log)
{
	bool isOverlap = (Range.Min < in_range.Range.Min
		&& in_range.Range.Min < Range.Max)
		|| (Range.Min < in_range.Range.Max && in_range.Range.Max < Range.Max);

	if (isOverlap)
	{
		out_log = "Overlap source type : " + FGsTextUtil::GetStringFromEnum(TEXT("ETerritoryObjType"), ObjType)
			+ TEXT("\trange : ") + Range.ToString()
			+ TEXT("\tdest type : ") + FGsTextUtil::GetStringFromEnum(TEXT("ETerritoryObjType"), in_range.ObjType)
			+ TEXT("\trange : ") + in_range.Range.ToString() + TEXT("\n");
	}

	return isOverlap;
}

bool FGsTerritoryUniqueIDRow::TryGetUniqueID(int64& out_uniuqe)
{
	out_uniuqe = 0;

	if (Count == TNumericLimits<uint32>::Max())
	{
		return false;
	}

	out_uniuqe = ++Count;

	return true;
}

bool FGsTerritoryUniqueIDRow::InvalideCount(FString& out_log)
{
	if (Range.Min + Count > Range.Max)
	{
		FString countText = FString::Printf(TEXT("%lld"), Count);

		out_log = TEXT("Count is over than max - type : ")
		+ FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), ObjType)
		+ Range.ToString() + TEXT("\tcount : ")
		+ countText + TEXT("\n");

		return true;
	}

	if (Range.Min + Count > TNumericLimits<uint32>::Max())
	{
		FString countText = FString::Printf(TEXT("%lld"), Count);

		out_log = TEXT("Count is over than uint32 - type : ")
		+ FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), ObjType)
		+ Range.ToString() + TEXT("\tcount : ")
		+ countText + TEXT("\n");

		return true;
	}

	return false;
}