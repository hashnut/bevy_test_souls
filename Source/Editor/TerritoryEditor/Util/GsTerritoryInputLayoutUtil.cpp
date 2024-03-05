// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryInputLayoutUtil.h"

FString UGsTerritoryInputLayoutUtil::GetKey(const FString& inKeyInfo)
{
	FString key;
	FString temp = inKeyInfo;

	TArray<FString> parses;	

	if (temp.ParseIntoArray(parses, TEXT("-")) > 0)
	{
		key = parses[0];
	}

	return key;
}
