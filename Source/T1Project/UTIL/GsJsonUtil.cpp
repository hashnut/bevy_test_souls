// Fill out your copyright notice in the Description page of Project Settings.


#include "GsJsonUtil.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

bool UGsJsonUtil::TryGetVectorField(FVector& out_value, TSharedPtr<FJsonValue> in_jsonValue)
{
	if (in_jsonValue.IsValid())
	{
		FString vectorString;

		if (in_jsonValue->TryGetString(vectorString))
		{
			return out_value.InitFromString(vectorString);
		}	
	}

	return false;
}